#ifndef _INC_KERNEL_SYMBOLS_H_
#define _INC_KERNEL_SYMBOLS_H_

#include <cstdint.h>
#include <utility.h>


/**
 * @file kernel/symbols.h
 * @brief Kernel Symbols
 */

/**
 * @class Symbols
 * @brief Kernel Symbols
 */
class Symbols {
	private:
		/**
		 * @struct header
		 * @brief Header of symbol map
		 */
		struct header {
			uint64_t magic;
			uint64_t size;
			uint64_t numEntries;
			uint64_t offSymbol;
			uint64_t offString;
		} __attribute__((packed));


		/**
		 * @struct symbol
		 * @brief Symbol entry within symbol map
		 */
		struct symbol {
			uint64_t idx;
			uint64_t value;
			uint64_t size;
		};

		/**
		 * @var MAGIC
		 * @brief Magic number of symbol map
		 */
		static const size_t MAGIC = 0x7553017854221078;

		/**
		 * @var hdr
		 * @brief Pointer to header of symbol map
		 */
		header* hdr;

	public:
		/**
		 * @fn bool init(void* symbol_map)
		 * @brief Initialize Symbols
		 * @return
		 *
		 *	- true  - Success
		 *	- false - Failure
		 */
		bool init(void* symbol_map);

		/**
		 * @fn lib::pair<const char*, size_t> lookup(void* addr) const
		 * @brief Lookup symbol name and offset for given address
		 * @return
		 *
		 *	- <Name of symbol, Offset> - Success
		 *	- <nullptr, 0> - Failure
		 */
		lib::pair<const char*, size_t> lookup(void* addr) const;

		/**
		 * @fn lib::pair<void*, size_t> getRange() const
		 * @brief Get used range
		 */
		lib::pair<void*, size_t> getRange() const;
};

extern Symbols symbols;

#endif /* ifndef _INC_KERNEL_SYMBOLS_H_ */
