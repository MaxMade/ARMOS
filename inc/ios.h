#ifndef _INC_IOS_H_
#define _INC_IOS_H_

#include <ostream.h>

/**
 * @file ios.h
 * @brief I/O Stream manipulation
 */

namespace lib {

	/**
	 * @fn lib::ostream& boolalpha(lib::ostream& str)
	 * @brief Set boolalpha flag
	 */
	lib::ostream& boolalpha(lib::ostream& str);

	/**
	 * @fn lib::ostream& noboolalpha(lib::ostream& str)
	 * @brief Unsst boolalpha flag
	 */
	lib::ostream& noboolalpha(lib::ostream& str);

	/**
	 * @fn lib::ostream& showbase(lib::ostream& str)
	 * @brief Set showbase flag
	 */
	lib::ostream& showbase(lib::ostream& str);

	/**
	 * @fn lib::ostream& noshowbase(lib::ostream& str)
	 * @brief Unset showbase flag
	 */
	lib::ostream& noshowbase(lib::ostream& str);

	/**
	 * @fn lib::ostream& uppercase(lib::ostream& str)
	 * @brief Set uppercase flag
	 */
	lib::ostream& uppercase(lib::ostream& str);

	/**
	 * @fn lib::ostream& nouppercase(lib::ostream& str)
	 * @brief Unset uppercase flag
	 */
	lib::ostream& nouppercase(lib::ostream& str);

	/**
	 * @fn lib::ostream& nouppercase(lib::ostream& str)
	 * @brief Set alignment to left
	 */
	lib::ostream& left(lib::ostream& str);

	/**
	 * @fn lib::ostream& right(lib::ostream& str)
	 * @brief Set alignmnet to right
	 */
	lib::ostream& right(lib::ostream& str);

	/**
	 * @fn lib::ostream& dec(lib::ostream& str)
	 * @brief Use decimal output
	 */
	lib::ostream& dec(lib::ostream& str);

	/**
	 * @fn lib::ostream& hex(lib::ostream& str)
	 * @brief Use hexadecimal output
	 */
	lib::ostream& hex(lib::ostream& str);

	/**
	 * @fn lib::ostream& oct(lib::ostream& str)
	 * @brief Use octal output
	 */
	lib::ostream& oct(lib::ostream& str);

	/**
	 * @fn lib::ostream& bin(lib::ostream& str)
	 * @brief Use binary output
	 */
	lib::ostream& bin(lib::ostream& str);

	struct ostreamFill {
		char c;

		explicit ostreamFill(char c) : c(c) {}

		friend ostream& operator<<(lib::ostream& str, const ostreamFill& fill) {
			str.fill(fill.c);
			return str;
		}

	};

	struct ostreamWidth {
		size_t w;

		explicit ostreamWidth(size_t w) : w(w) {}

		friend ostream& operator<<(lib::ostream& str, const ostreamWidth& width) {
			str.width(width.w);
			return str;
		}
	};

	/**
	 * @fn ostreamFill setfill(char c)
	 * @brief Set fill character
	 */
	ostreamFill setfill(char c);

	/**
	 * @fn ostreamWidth setw(size_t w)
	 * @brief Set width
	 */
	ostreamWidth setw(size_t w);

} /* namespace lib */

#endif /* ifndef _INC_IOS_H_ */
