#ifndef _INC_KERNEL_DEVICE_TREE_DEFINITION_H_
#define _INC_KERNEL_DEVICE_TREE_DEFINITION_H_

#include <cstdint.h>

/**
 * @file kernel/device_tree/definition.h
 * @brief Definitions of Device Tree Parser
 */

namespace DeviceTree {

		/**
		 * @struct fdt_header
		 * @brief Flat device tree header
		 */
		struct fdt_header {
			uint32_t magic;             /**< Magic value */
			uint32_t totalsize;         /**< Total size of header in bytes */
			uint32_t off_dt_struct;     /**< Structure block offset (relative to beginning of fdt_header) */
			uint32_t off_dt_strings;    /**< String block offset (relative to beginning of fdt_header) */
			uint32_t off_mem_rsvmap;    /**< Reserved memory block offset (relative to beginning of fdt_header) */
			uint32_t version;           /**< Version */
			uint32_t last_comp_version; /**< Lowest compatible version */
			uint32_t boot_cpuid_phys;   /**< Physical ID of boot processor */
			uint32_t size_dt_strings;   /**< String block size (in bytes) */
			uint32_t size_dt_struct;    /**< Structure block size (in bytes) */
		} __attribute__((packed));

		/**
		 * @typedef FDTHeader
		 * @brief Flat device tree header
		 */
		using FDTHeader = struct fdt_header;

		/**
		 * @struct fdt_reserve_entry
		 * @brief Reserved memory entry
		 */
		struct fdt_reserve_entry {
			uint64_t address;      /**< Address */
			uint64_t size;         /**< Size */
		} __attribute__((packed));

		/**
		 * @typedef FDTResEnt
		 * @brief Reserved memory entry
		 */
		using FDTResEnt = struct fdt_reserve_entry;

		/**
		 * @struct fdt_prop
		 * @brief Property
		 */
		struct fdt_prop {
			uint32_t len;     /**< Length */
			uint32_t nameoff; /**< Offset in string block */
			uint32_t vals[0]; /**< Values */
		} __attribute__((packed));

		/**
		 * @typedef FDTProp
		 * @brief Property
		 */
		using FDTProp = struct fdt_prop;

		/**
		 * @var static constexpr size_t FDT_MAGIC
		 * @brief Magic Number of FDT
		 */
		enum : uint32_t {
			FDT_MAGIC      = 0xd00dfeed, /**< Magic Number */
			FDT_BEGIN_NODE = 0x00000001, /**< Marker for beginning node */
			FDT_END_NODE   = 0x00000002, /**< Marker for ending node */
			FDT_PROP       = 0x00000003, /**< Marker for beginning property */
			FDT_NOP        = 0x00000004, /**< No operation */
			FDT_END        = 0x00000009, /**< Marker for end of FDT */
		};

		const uint32_t FDT_ALIGNMENT = 4;

} // namespace DeviceTree

#endif // _INC_KERNEL_DEVICE_TREE_H_
