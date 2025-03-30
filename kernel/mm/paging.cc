#include <kernel/cpu.h>
#include <kernel/math.h>
#include <kernel/error.h>
#include <kernel/linker.h>
#include <kernel/symbols.h>
#include <kernel/mm/paging.h>
#include <kernel/mm/translation_table.h>
#include <kernel/mm/translation_table_allocator.h>

using namespace mm;

lock::spinlock Paging::lock;

void* Paging::tables = nullptr;

Paging::Paging() {
	tables = CPU::getTranslationTable();
}

template<size_t level>
size_t Paging::getOffset(void* addr) {
	static_assert(level < NUM_TABLES, "level should be less than number of NUM_TABLES");

	if constexpr (level == 3) {
		return (reinterpret_cast<uintptr_t>(addr) >> 12) % TranslationTable::NUM_ENTRIES;

	} else if constexpr(level == 2) {
		return (reinterpret_cast<uintptr_t>(addr) >> 21) % TranslationTable::NUM_ENTRIES;

	} else if constexpr (level == 1) {
		return (reinterpret_cast<uintptr_t>(addr) >> 30) % TranslationTable::NUM_ENTRIES;
	}

	return (reinterpret_cast<uintptr_t>(addr) >> 39) % TranslationTable::NUM_ENTRIES;
}

void Paging::getOffsets(void* addr, size_t offsets[4]) {
	offsets[0] = getOffset<0>(addr);
	offsets[1] = getOffset<1>(addr);
	offsets[2] = getOffset<2>(addr);
	offsets[3] = getOffset<3>(addr);
}

template<bool earlyBoot>
int Paging::genTTs(void* vaddr) {
	size_t offsets[NUM_TABLES];
	getOffsets(vaddr, offsets);

	void* drag = tables;
	for (size_t i = 0; i < NUM_TABLES - 1; i++) {
		TranslationTable tt(drag);

		/* Check if next level is accessable */
		if (!tt.getPresentBit(offsets[i])) {
			void* page = nullptr;
			/* Allocate new page */
			if constexpr (earlyBoot) {
				page = ttAlloc.earlyAlloc();
			} else {
				page = ttAlloc.alloc();
			}

			if (page == nullptr)
				return -ENOMEM;

			/* Prepare next level */
			TranslationTable ttNext(page);
			ttNext.setDefault();

			/* Link next level */
			tt.setAddress(offsets[i], page);
			tt.setPresentBit(offsets[i], true);
		}

		drag = tt.getAddress(offsets[i]);
	}

	return 0;
}

int Paging::getTTs(void *vaddr, TranslationTable tt[4]) const {
	size_t offsets[NUM_TABLES];
	getOffsets(vaddr, offsets);

	void* drag = tables;
	void* tts[NUM_TABLES] = {drag, nullptr, nullptr, nullptr};
	for (size_t i = 0; i < NUM_TABLES - 1; i++) {
		TranslationTable tt(drag);

		/* Check if next level is accessable */
		if (tt.getPresentBit(offsets[i]) == 0)
			return -EINVAL;

		drag = tt.getAddress(offsets[i]);
		tts[i + 1] = tt.getAddress(offsets[i]);
	}

	tt[0] = TranslationTable(tts[0]);
	tt[1] = TranslationTable(tts[1]);
	tt[2] = TranslationTable(tts[2]);
	tt[3] = TranslationTable(tts[3]);

	return 0;
}

int Paging::internalProtect(void *vaddr, priv_lvl_t priv, prot_t prot, mem_attr_t attr) {
	/* Get translation tables */
	TranslationTable tts[NUM_TABLES];
	if (getTTs(vaddr, tts))
		return -EINVAL;

	/* Prepare offsets */
	size_t offs[NUM_TABLES];
	getOffsets(vaddr, offs);

	if (tts[3].getPresentBit(offs[3]) == 0)
		return -ENXIO;

	/* Update protection */
	TranslationTable::access_t access;
	if (priv == KERNEL_MAPPING && prot == READONLY) {
		access = TranslationTable::ELX_RO_EL0_NONE;
		tts[3].setExecuteNeverBitUser(offs[3], true);
		tts[3].setExecuteNeverBitKernel(offs[3], prot != EXECUTABLE);

	} else if (priv == KERNEL_MAPPING && prot == WRITABLE) {
		access = TranslationTable::ELX_RW_EL0_NONE;
		tts[3].setExecuteNeverBitUser(offs[3], true);
		tts[3].setExecuteNeverBitKernel(offs[3], prot != EXECUTABLE);

	} else if (priv == KERNEL_MAPPING && prot == EXECUTABLE) {
		access = TranslationTable::ELX_RO_EL0_NONE;
		tts[3].setExecuteNeverBitUser(offs[3], true);
		tts[3].setExecuteNeverBitKernel(offs[3], prot != EXECUTABLE);

	} else if (priv == USER_MAPPING && prot == READONLY) {
		access = TranslationTable::ELX_RO_EL0_RO;
		tts[3].setExecuteNeverBitUser(offs[3], prot != EXECUTABLE);
		tts[3].setExecuteNeverBitKernel(offs[3], true);

	} else if (priv == USER_MAPPING && prot == WRITABLE) {
		access = TranslationTable::ELX_RW_EL0_RW;
		tts[3].setExecuteNeverBitUser(offs[3], prot != EXECUTABLE);
		tts[3].setExecuteNeverBitKernel(offs[3], true);

	} else if (priv == USER_MAPPING && prot == EXECUTABLE) {
		access = TranslationTable::ELX_RO_EL0_RO;
		tts[3].setExecuteNeverBitUser(offs[3], prot != EXECUTABLE);
		tts[3].setExecuteNeverBitKernel(offs[3], true);

	} else {
		lock.unlock();
		return -EINVAL;
	}
	tts[3].setAccessControl(offs[3], access);

	/* Set memory attribute */
	tts[3].setAttrIndex(offs[3], attr);

	return 0;
}

template<bool earlyBoot>
int Paging::internalMap(void* vaddr, void* paddr, priv_lvl_t priv, prot_t prot, mem_attr_t attr) {
	/* Generate translation tables */
	int err = genTTs<earlyBoot>(vaddr);
	if (err < 0)
		return err;

	/* Get translation tables */
	TranslationTable tts[NUM_TABLES];
	err = getTTs(vaddr, tts);
	if (err)
		return -EINVAL;

	/* Prepare offsets */
	size_t offs[NUM_TABLES];
	getOffsets(vaddr, offs);

	/* Set address */
	tts[3].setAddress(offs[3], paddr);
	tts[3].setPresentBit(offs[3], true);

	/* Update protection */
	err = internalProtect(vaddr, priv, prot, attr);

	return err;
}

int Paging::earlyMap(void* vaddr, void* paddr, priv_lvl_t priv, prot_t prot, mem_attr_t attr) {
	return internalMap<true>(vaddr, paddr, priv, prot, attr);
}

int Paging::createEarlyKernelMapping() {
	/* Create translation table level 0 */
	void* frame = ttAlloc.earlyAlloc();
	TranslationTable tt(frame);
	tt.setDefault();

	/* Create translation tables */
	Paging paging;
	paging.tables = frame;
	auto text = linker::getTextSegment();
	for (uintptr_t addr = reinterpret_cast<uintptr_t>(text.first);
			addr < math::roundUp(reinterpret_cast<uintptr_t>(text.first) + text.second, PAGESIZE);
			addr += PAGESIZE) {

		auto ret = paging.earlyMap(reinterpret_cast<void*>(addr), reinterpret_cast<void*>(addr), KERNEL_MAPPING, EXECUTABLE, NORMAL_ATTR);
		if (isError(ret))
			return castError<int, decltype(ret)>(ret);
	}

	auto data = linker::getDataSegment();
	for (uintptr_t addr = reinterpret_cast<uintptr_t>(data.first);
			addr < math::roundUp(reinterpret_cast<uintptr_t>(data.first) + data.second, PAGESIZE);
			addr += PAGESIZE) {

		auto ret = paging.earlyMap(reinterpret_cast<void*>(addr), reinterpret_cast<void*>(addr), KERNEL_MAPPING, WRITABLE, NORMAL_ATTR);
		if (isError(ret))
			return castError<int, decltype(ret)>(ret);
	}

	auto rodata = linker::getRODataSegment();
	for (uintptr_t addr = reinterpret_cast<uintptr_t>(rodata.first);
			addr < math::roundUp(reinterpret_cast<uintptr_t>(rodata.first) + rodata.second, PAGESIZE);
			addr += PAGESIZE) {

		auto ret = paging.earlyMap(reinterpret_cast<void*>(addr), reinterpret_cast<void*>(addr), KERNEL_MAPPING, READONLY, NORMAL_ATTR);
		if (isError(ret))
			return castError<int, decltype(ret)>(ret);
	}

	auto bss = linker::getBSSSegment();
	for (uintptr_t addr = reinterpret_cast<uintptr_t>(bss.first);
			addr < math::roundUp(reinterpret_cast<uintptr_t>(bss.first) + bss.second, PAGESIZE);
			addr += PAGESIZE) {

		auto ret = paging.earlyMap(reinterpret_cast<void*>(addr), reinterpret_cast<void*>(addr), KERNEL_MAPPING, WRITABLE, NORMAL_ATTR);
		if (isError(ret))
			return castError<int, decltype(ret)>(ret);
	}

    auto sym_map = symbols.getRange();
	for (uintptr_t addr = reinterpret_cast<uintptr_t>(sym_map.first);
			addr < math::roundUp(reinterpret_cast<uintptr_t>(sym_map.first) + sym_map.second, PAGESIZE);
			addr += PAGESIZE) {

		auto ret = paging.earlyMap(reinterpret_cast<void*>(addr), reinterpret_cast<void*>(addr), KERNEL_MAPPING, WRITABLE, NORMAL_ATTR);
		if (isError(ret))
			return castError<int, decltype(ret)>(ret);
	}

	auto textApp = linker::getAppTextSegment();
	for (uintptr_t addr = reinterpret_cast<uintptr_t>(textApp.first);
			addr < math::roundUp(reinterpret_cast<uintptr_t>(textApp.first) + textApp.second, PAGESIZE);
			addr += PAGESIZE) {

		auto ret = paging.earlyMap(reinterpret_cast<void*>(addr), reinterpret_cast<void*>(addr), USER_MAPPING, EXECUTABLE, NORMAL_ATTR);
		if (isError(ret))
			return castError<int, decltype(ret)>(ret);
	}

	auto dataApp = linker::getAppDataSegment();
	for (uintptr_t addr = reinterpret_cast<uintptr_t>(dataApp.first);
			addr < math::roundUp(reinterpret_cast<uintptr_t>(dataApp.first) + dataApp.second, PAGESIZE);
			addr += PAGESIZE) {

		auto ret = paging.earlyMap(reinterpret_cast<void*>(addr), reinterpret_cast<void*>(addr), USER_MAPPING, WRITABLE, NORMAL_ATTR);
		if (isError(ret))
			return castError<int, decltype(ret)>(ret);
	}

	auto rodataApp = linker::getAppRODataSegment();
	for (uintptr_t addr = reinterpret_cast<uintptr_t>(rodataApp.first);
			addr < math::roundUp(reinterpret_cast<uintptr_t>(rodataApp.first) + rodataApp.second, PAGESIZE);
			addr += PAGESIZE) {

		auto ret = paging.earlyMap(reinterpret_cast<void*>(addr), reinterpret_cast<void*>(addr), USER_MAPPING, READONLY, NORMAL_ATTR);
		if (isError(ret))
			return castError<int, decltype(ret)>(ret);
	}

	auto bssApp = linker::getAppBSSSegment();
	for (uintptr_t addr = reinterpret_cast<uintptr_t>(bssApp.first);
			addr < math::roundUp(reinterpret_cast<uintptr_t>(bssApp.first) + bssApp.second, PAGESIZE);
			addr += PAGESIZE) {

		auto ret = paging.earlyMap(reinterpret_cast<void*>(addr), reinterpret_cast<void*>(addr), USER_MAPPING, WRITABLE, NORMAL_ATTR);
		if (isError(ret))
			return castError<int, decltype(ret)>(ret);
	}

	/* Update TTBR0 */
	tt.updateTTBR0();
	return 0;
}

int Paging::loadKernelMapping() {
	if (!tables)
		return -EINVAL;

	TranslationTable tt(tables);
	tt.updateTTBR0();

	return 0;
}

int Paging::map(void* vaddr, void* paddr, priv_lvl_t priv, prot_t prot, mem_attr_t attr) {
	lock.lock();
	auto ret = internalMap<false>(vaddr, paddr, priv, prot, attr);
	lock.unlock();
	return ret;
}

void* Paging::unmap(void* vaddr) {
	lock.lock();

	/* Get translation table wrapper */
	TranslationTable tts[NUM_TABLES];
	if (getTTs(vaddr, tts)) {
		lock.unlock();
		return makeError<void*>(EINVAL);
	}

	/* Get offsets in translation tables */
	size_t offs[NUM_TABLES];
	getOffsets(vaddr, offs);

	/* Save address of page frame */
	void* ret = makeError<void*>(ENXIO);
	if (tts[3].getPresentBit(offs[3])) {
		ret = tts[3].getAddress(offs[3]);
	}

	for (size_t i = 3; i >= 1; i--) {
		/* Set corrosponding entry as non-present */
		tts[i].setPresentBit(offs[i], false);

		/* Check if translation table is empty */
		for (size_t entry = 0; entry < TranslationTable::NUM_ENTRIES; entry++) {
			/* Translation Table contains other entries -> Exit */
			if (tts[i].getPresentBit(entry) == 1) {
				lock.unlock();
				return ret;
			}
		}

		/* Mark entry in translation table above as non present */
		tts[i - 1].setPresentBit(offs[i - 1], false);

		/* Free frame */
		ttAlloc.free(tts[i].getFrame());
	}

	lock.unlock();
	return ret;
}

int Paging::protect(void* vaddr, priv_lvl_t priv, prot_t prot, mem_attr_t attr) {
	lock.lock();

	/* Get translation table wrapper */
	int err = internalProtect(vaddr, priv, prot, attr);

	lock.unlock();
	return err;
}
