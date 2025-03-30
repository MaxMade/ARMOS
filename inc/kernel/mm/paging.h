#ifndef _INC_KERNEL_MM_PAGING_H_
#define _INC_KERNEL_MM_PAGING_H_

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
		private:
			/**
			 * @var level0TT
			 * @brief Pointer to Translation Table Level 0
			 */
			void* level0TT;

			/**
			 * @var kernelLevel0TT
			 * @brief Poiner to translation table 0 created by createEarlyKernelMapping
			 */
			static void* kernelLevel0TT;

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
			 * @fn int genTTEntries(void* vaddr)
			 * @brief Generate Translation Table Level 1, 2, 3 if necessary and initialize with default values
			 * @tparm earlyBoot Use TTAllocator::earlyAlloc instead of TTAllocator::alloc
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure
			 */
			template<bool earlyBoot = false>
			int genTTEntries(void* vaddr);

			/**
			 * @fn lib::tuple<bool, void*, void*, void*, void*> getTTEntries(void *vaddr) const
			 * @brief Lookup Translation Tables for given virtual address
			 * @return
			 *
			 *	- (true, Translation Table Level 0, Translation Table Level 1, Translation Table Level 2, Translation Table Level 3) - Success
			 *	- (false, Invalid Pointer,  Invalid Pointer, Invalid Pointer, Invalid Pointer)                                       - Failure
			 */
			lib::tuple<bool, void*, void*, void*, void*> getTTEntries(void *vaddr) const;

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
			 * @fn explicit Paging(void* level0TT)
			 * @brief Constructor Paging from given pointer
			 */
			explicit Paging(void* level0TT);

			Paging(const Paging& other) = delete;

			Paging(Paging&& other) = delete;

			Paging& operator=(const Paging& other) = delete;

			Paging& operator=(Paging&& other) = delete;

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
			 * @fn void* earlyMap(void* vaddr, void* paddr, priv_lvl_t priv, prot_t prot, mem_attr_t attr)
			 * @brief Create mapping (in early initialization code)
			 * @param vaddr Desired virtual address
			 * @param paddr Desired physical adress
			 * @param priv Privileged Level
			 * @param prot Protection
			 * @param attr Memory Attribute
			 * @tparam earlyBoot Prevent lock and use Translation::allocEarly()
			 */
			void* earlyMap(void* vaddr, void* paddr, priv_lvl_t priv, prot_t prot, mem_attr_t attr);
	};
};

#endif /* ifndef _INC_KERNEL_MM_PAGING_H_ */
