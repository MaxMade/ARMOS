#include <cerrno.h>
#include <kernel/error.h>
#include <kernel/utility.h>
#include <driver/bcm_intc.h>

using namespace driver;

#define NUM_SETS    3
#define NUM_ENTRIES 32
#define FIXUP_RANGE 0x1000000

generic_driver* bcm_intc::handlers[NUM_SETS * NUM_ENTRIES] = {nullptr};

bcm_intc::bcm_intc() {
	name = "brcm,bcm2836-armctrl-ic";
}

int bcm_intc::init(const config& conf) {
	base = reinterpret_cast<void*>(conf.getRange().first);

	return 0;
}

int bcm_intc::registerHandler(void* data, size_t size, generic_driver* driver) {
	/* The bcm2837 accepts a configuration array of uint32_t values with a even
	 * length. Each pair consists of a set identifier (Basic, IRQ 1, IRQ 2) and
	 * an entry (0 - 31) with the set.
	 */
	if ((size / sizeof(uint32_t)) % 2 != 0)
		return -EINVAL;

	for (size_t i = 0; i < size / sizeof(uint32_t); i += 2) {
		uint32_t set = util::bigEndianToHost(static_cast<uint32_t*>(data)[i]);
		uint32_t entry = util::bigEndianToHost(static_cast<uint32_t*>(data)[i + 1]);
		if (set >=  NUM_SETS || entry >= NUM_ENTRIES)
			return -EINVAL;

		/* Enable entry in basic set */
		if (set == 0) {
			auto enable = readRegister<enable_basic_irqs>();
			enable |= (1 << entry);
			writeRegister<enable_basic_irqs>(enable);

		/* Enable entry in set 1 */
		} else if (set == 1) {
			auto enable = readRegister<enable_irqs_1>();
			enable |= (1 << entry);
			writeRegister<enable_irqs_1>(enable);

		/* Enable entry in set 2 */
		} else {
			auto enable = readRegister<enable_irqs_2>();
			enable |= (1 << entry);
			writeRegister<enable_irqs_2>(enable);

		}

		handlers[set * NUM_ENTRIES + entry] = driver;
	}

	return 0;
}

generic_driver* bcm_intc::getHandler() {
	for (size_t i = 0; i < NUM_SETS * NUM_ENTRIES; i++) {
		/* Skip unused handlers */
		if (handlers[i] == nullptr)
			continue;

		/* Calculate set and entry */
		uint32_t set = i / NUM_ENTRIES;
		uint32_t entry = i % NUM_ENTRIES;

		/* Check basic pending */
		if (set == 0) {
			auto pending = readRegister<irq_basic_pending>();
			/* Check if pending */
			if (pending & (1 << entry)) {
				/* Clear pending bit and branch to handler */
				pending &= ~(1 << entry);
				writeRegister<irq_basic_pending>(pending);
				return handlers[i];
			}

		/* Check pending 1 */
		} else if (set == 1) {
			auto pending = readRegister<irq_pending_1>();
			/* Check if pending */
			if (pending & (1 << entry)) {
				/* Clear pending bit and branch to handler */
				pending &= ~(1 << entry);
				writeRegister<irq_pending_1>(pending);
				return handlers[i];
			}

		/* Check pending 2 */
		} else {
			auto pending = readRegister<irq_pending_2>();
			/* Check if pending */
			if (pending & (1 << entry)) {
				/* Clear pending bit and branch to handler */
				pending &= ~(1 << entry);
				writeRegister<irq_pending_2>(pending);
				return handlers[i];
			}

		}
	}

	return makeError<generic_driver*>(-EINVAL);
}

int bcm_intc::prologue() {
	return -EINVAL;
}

int bcm_intc::epilogue() {
	return -EINVAL;
}
