#ifndef _INC_HW_REGISTER_FAR_H_
#define _INC_HW_REGISTER_FAR_H_

#include <cstdint.h>

/**
 * @file hw/register/far.h
 * @brief Fault Address Register
 * @details
 * See D13.2.39 FAR_EL1, Fault Address Register (EL1)
 */

namespace hw::reg {

	class FAR {
		private:
			/**
			 * @var val
			 * @brief Value
			 */
			uintptr_t val;

		public:
			/**
			 * @fn FAR()
			 * @brief Read FAR
			 */
			FAR();

			/**
			 * @fn void* getValue() const
			 * @brief Get underlying value
			 */
			void* getValue() const;

			/**
			 * @fn operator uintptr_t() const
			 * @brief Get underlying value as uintptr_t
			 */
			operator uintptr_t() const;

			/**
			 * @fn operator uintptr_t() const
			 * @brief Get underlying value as void*
			 */
			operator void*() const;
	};

} /* namespace hw::reg */

#endif /* ifndef _INC_HW_REGISTER_FAR_H_ */
