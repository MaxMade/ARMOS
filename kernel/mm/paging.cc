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

void* Paging::kernelLevel0TT = nullptr;

Paging::Paging() {
	level0TT = CPU::getTranslationTable();
}

Paging::Paging(void* level0TT) : level0TT(level0TT) {}

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

template<bool earlyBoot>
int Paging::genTTEntries(void* vaddr) {
	size_t offsets[NUM_TABLES];
	offsets[0] = getOffset<0>(vaddr);
	offsets[1] = getOffset<1>(vaddr);
	offsets[2] = getOffset<2>(vaddr);
	offsets[3] = getOffset<3>(vaddr);

	void* drag = level0TT;
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

lib::tuple<bool, void*, void*, void*, void*> Paging::getTTEntries(void *vaddr) const {
	size_t offsets[NUM_TABLES];
	offsets[0] = getOffset<0>(vaddr);
	offsets[1] = getOffset<1>(vaddr);
	offsets[2] = getOffset<2>(vaddr);
	offsets[3] = getOffset<3>(vaddr);

	bool valid = true;
	void* tts[NUM_TABLES] = {nullptr};


	void* drag = level0TT;
	tts[0] = drag;
	for (size_t i = 0; i < NUM_TABLES - 1; i++) {
		TranslationTable tt(drag);

		/* Check if next level is accessable */
		if (tt.getPresentBit(offsets[i]) == 1) {
			drag = tt.getAddress(offsets[i]);

		} else {
			valid = false;
			break;
		}

		tts[i + 1] = tt.getAddress(offsets[i]);
	}

	return lib::tuple(valid, tts[0], tts[1], tts[2], tts[3]);
}

int Paging::createEarlyKernelMapping() {
	/* Create translation table level 0 */
	void* frame = ttAlloc.earlyAlloc();
	TranslationTable tt(frame);
	tt.setDefault();

	/* Create translation tables */
	Paging paging(frame);
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

	/* Save translation tabel 0 */
	kernelLevel0TT = frame;

	/* Update TTBR0 */
	tt.updateTTBR0();
	return 0;
}

int Paging::loadKernelMapping() {
	if (!kernelLevel0TT)
		return -EINVAL;

	TranslationTable tt(kernelLevel0TT);
	tt.updateTTBR0();

	return 0;
}

void* Paging::earlyMap(void* vaddr, void* paddr, priv_lvl_t priv, prot_t prot, mem_attr_t attr) {
	/* Generate translation tables */
	int err = genTTEntries(vaddr);
	if (err < 0)
		return makeError<void*>(err);

	/* Get pointer to translation tables */
	auto tts = getTTEntries(vaddr);
	if (!lib::get<0>(tts))
		return makeError<void*>(-EINVAL);

	/* Prepare translation table level 3 */
	size_t tt3Entry = getOffset<3>(vaddr);
	TranslationTable tt3(lib::get<4>(tts));

	/* Set address */
	tt3.setAddress(tt3Entry, paddr);

	/* Set address control */
	TranslationTable::access_t access;
	if (priv == KERNEL_MAPPING && prot == READONLY) {
		access = TranslationTable::ELX_RO_EL0_NONE;

	} else if (priv == KERNEL_MAPPING && prot == WRITABLE) {
		access = TranslationTable::ELX_RW_EL0_NONE;

	} else if (priv == KERNEL_MAPPING && prot == EXECUTABLE) {
		access = TranslationTable::ELX_RO_EL0_NONE;

	} else if (priv == USER_MAPPING && prot == READONLY) {
		access = TranslationTable::ELX_RO_EL0_RO;

	} else if (priv == USER_MAPPING && prot == WRITABLE) {
		access = TranslationTable::ELX_RW_EL0_RW;

	} else if (priv == USER_MAPPING && prot == EXECUTABLE) {
		access = TranslationTable::ELX_RO_EL0_RO;

	} else {
		return makeError<void*>(-EINVAL);
	}
	tt3.setAccessControl(tt3Entry, access);

	/* Set nx bit */
	tt3.setExecuteNeverBit(tt3Entry, prot != EXECUTABLE);

	/* Set memory attribute */
	tt3.setAttrIndex(tt3Entry, attr);

	/* Set present */
	tt3.setPresentBit(tt3Entry, true);

	return vaddr;
}
