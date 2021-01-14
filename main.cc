#include <cstring.h>
#include <kernel/device_tree/parser.h>

int kernelMain(void *fdt) {
	/* Start parsing device tree */
	DeviceTree::Parser dtp(fdt);
	if (!dtp.isValid())
		return -1;

	/* Iterate over nodes */
	for (auto nodeIt = dtp.begin(); nodeIt != dtp.end(); ++nodeIt) {
		DeviceTree::Node node = *nodeIt;
		if (!node.isValid())
			return -1;

		auto compatible = node.findStringlistProperty("compatible");
		if (compatible.first != nullptr && strcmp("arm,pl011", compatible.first) == 0) {
			auto reg = node.findRegisterProperty("reg");
			auto& itBegin = reg.first;
			auto& itEnd = reg.second;

			/* Check if valid iterator */
			if (itBegin == itEnd)
				return -1;

			auto data = *itBegin;
			void* addr = data.first;
			size_t size = data.second;

			(void) addr;
			(void) size;

			return 1;
		}
	}

	return 0;
}
