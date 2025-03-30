#ifndef _INC_KERNEL_MM_PAGING_H_
#define _INC_KERNEL_MM_PAGING_H_

#include "kernel/mm/translation_table.h"
#include <tuple.h>
#include <cerrno.h>
#include <climits.h>
#include <cstdint.h>
#include <kernel/lock/spinlock.h>

/**
 * @file kernel/mm/paging.h
 * @brief Paging operations
 * @see
 * D5.2.6 Overview of the VMSAv8-64 address translation stages
 */

namespace mm {

	class Paging {
		public:
			/**
			 * @typedef priv_lvl_t
			 * @brief Privileged Level
			 */
			typedef	enum {
				KERNEL_MAPPING = (1 << 0), /**< Kernel Space Mapping */
				USER_MAPPING   = (1 << 1), /**< User Space Mapping */
			} priv_lvl_t;

			/**
			 * @typedef prot_t
			 * @brief Protection
			 */
			typedef enum {
				READONLY       = (1 << 0), /**< Read-Only Page */
				WRITABLE       = (1 << 1), /**< Readable and Writable Page */
				EXECUTABLE     = (1 << 2), /**< Readable and Executable Page */
			} prot_t;

			/**
			 * @typedef mem_attr_t
			 * @brief Memory attribute
			 */
			typedef enum {
				NORMAL_ATTR    = (1 << 0), /**< Normal Memory */
				DEVICE_ATTR    = (1 << 1), /**< Device Memory */
			} mem_attr_t;

		private:
			/**
			 * @var tt0
			 * @brief Pointer to Translation Table Level 0
			 */
			void* tt0;

			/**
			 * @var kernel_tt0
			 * @brief Poiner to translation table 0 created by createEarlyKernelMapping
			 */
			static void* kernel_tt0;

			/**
			 * @var lock
			 * @brief Synchronization lock
			 */
			static lock::spinlock lock;

			/**
			 * @fn static size_t getOffset(void* addr)
			 * @brief Get offset in translation table for for given level
			 * @tparam level Target Translation Table Level (0-3)
			 */
			template<size_t level>
			static size_t getOffset(void* addr);

			/**
			 * @fn static void getOffsets(void* addr, size_t offsets[4])
			 * @brief Save all offsets in target array
			 */
			static void getOffsets(void* addr, size_t offsets[4]);

			/**
			 * @fn int genTTs(void* vaddr)
			 * @brief Generate Translation Table Level 1, 2, 3 if necessary and initialize with default values
			 * @tparm earlyBoot Use TTAllocator::earlyAlloc instead of TTAllocator::alloc
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure
			 */
			template<bool earlyBoot = false>
			int genTTs(void* vaddr);

			/**
			 * @fn int getTTs(void *vaddr, TranslationTable tt[4]) const
			 * @brief Lookup Translation Tables for given virtual address
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Error (-errno)
			 */
			int getTTs(void *vaddr, TranslationTable tt[4]) const;

			/**
			 * @fn int internalProtect(void *vaddr, priv_lvl_t priv, prot_t prot, mem_attr_t attr)
			 * @brief Update protection of page
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int internalProtect(void *vaddr, priv_lvl_t priv, prot_t prot, mem_attr_t attr);

			/**
			 * @fn void* internalMap();
			 * @brief Create mapping
			 * @param vaddr Desired virtual address
			 * @param paddr Desired physical adress
			 * @param priv Privileged Level
			 * @param prot Protection
			 * @param attr Memory Attribute
			 * @return
			 *
			 *	-  0 - Success
			 *	- -1 - Failure (-errno)
			 */
			template<bool earlyBoot = false>
			int internalMap(void* vaddr, void* paddr, priv_lvl_t priv, prot_t prot, mem_attr_t attr);

		public:
			/**
			 * @var NUM_TABLES
			 * @brief Number of Translation Tables
			 */
			const static size_t NUM_TABLES = 4;

			/**
			 * @fn Paging()
			 * @brief Constructor Paging from current TTBR0
			 */
			Paging();

			/**
			 * @fn explicit Paging(void* tt0)
			 * @brief Constructor Paging from given pointer
			 */
			explicit Paging(void* tt0);

			Paging(const Paging& other) = delete;

			Paging(Paging&& other) = delete;

			Paging& operator=(const Paging& other) = delete;

			Paging& operator=(Paging&& other) = delete;

			/**
			 * @fn static int createEarlyKernelMapping()
			 * @brief Create initial mapping
			 * @details
			 * Create mapping for text, data, rodata, bss (of kernel and app) and symbol map
			 */
			static int createEarlyKernelMapping();

			/**
			 * @fn static int loadKernelMapping()
			 * @brief Load kernel mapping
			 */
			static int loadKernelMapping();

			/**
			 * @fn int earlyMap(void* vaddr, void* paddr, priv_lvl_t priv, prot_t prot, mem_attr_t attr)
			 * @brief Create mapping (in early initialization code)
			 * @param vaddr Desired virtual address
			 * @param paddr Desired physical adress
			 * @param priv Privileged Level
			 * @param prot Protection
			 * @param attr Memory Attribute
			 * @return
			 *
			 *	-  0 - Success
			 *	- -1 - Failure (-errno)
			 */
			int earlyMap(void* vaddr, void* paddr, priv_lvl_t priv, prot_t prot, mem_attr_t attr);

			/**
			 * @fn int map(void* vaddr, void* paddr, priv_lvl_t priv, prot_t prot, mem_attr_t attr)
			 * @brief Create mapping
			 * @param vaddr Desired virtual address
			 * @param paddr Desired physical adress
			 * @param priv Privileged Level
			 * @param prot Protection
			 * @param attr Memory Attribute
			 * @return
			 *
			 *	-  0 - Success
			 *	- -1 - Failure (-errno)
			 */
			int map(void* vaddr, void* paddr, priv_lvl_t priv, prot_t prot, mem_attr_t attr);

			/**
			 * @fn int unmap(void* vaddr)
			 * @brief Unmap page
			 * @return
			 *
			 *	- Pointer to page frame   - Success
			 *	- makeError<void*>(errno) - Failure
			 */
			void* unmap(void* vaddr);

			/**
			 * @fn int protect(void* vaddr, priv_lvl_t priv, prot_t prot, mem_attr_t attr)
			 * @brief Update protection of page
			 * @return
			 *
			 *	-  0 - Success
			 *	- -1 - Failure (-errno)
			 */
			int protect(void* vaddr, priv_lvl_t priv, prot_t prot, mem_attr_t attr);
	};
};

#endif /* ifndef _INC_KERNEL_MM_PAGING_H_ */
