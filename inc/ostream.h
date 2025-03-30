#ifndef _INC_OSTREAM_H_
#define _INC_OSTREAM_H_

#include <atomic.h>
#include <cstddef.h>
#include <cstring.h>
#include <cstdint.h>
#include <kernel/cpu.h>
#include <driver/drivers.h>

/**
 * @file ostream.h
 * @brief Output stream
 * @details
 * The ostream class abstracts the outstream of the kernel. To write messages
 * to the underlying console, a new ostream object must be created on the
 * stack.  Afterwards, it can be used just like the normal std::cout object
 * with two differences major differences: Firstly, the underlying locking
 * mechanism behaves significantly different. While the write(...) and put(...)
 * function will lock and then unlock the console, all remaining operation<<
 * function overloads will only lock the console and perform its output. This
 * is the desired behavior as several operator<< calls to the same ostream
 * object should be always observed without any interferences of concurrent
 * write operations. To unlock, either the flush() function must be called or
 * the ostream object must be destructed (in case of a non-panic ostream). The
 * second difference concerns the existing of a panic outstream. A
 * panic object will be a panic outstream and can be used at any moment
 * (even in interrupt handlers) after virtual memory is active. Hereby, the
 * flush method must be called explicitly for all output functions and the
 * constructor will not call flush implicitly.
 */

namespace lib {

	class ostreamBase {
		public:
			/**
			 * @var cpu_lock
			 * @brief Synchronization Lock (containing active CPU number)
			 */
			static lib::atomic<int> cpu_lock;
	};

	/**
	 * @class ostreamHelper
	 * @brief Output stream
	 */
	template<bool panic = false>
	class ostreamHelper : public ostreamBase {
		public:
		/**
		 * @var dec
		 * @brief Decimal base for integer
		 */
		static const uint16_t dec = (1 << 0);

		/**
		 * @var oct
		 * @brief Octal base for integer
		 */
		static const uint16_t oct = (1 << 1);

		/**
		 * @var hex
		 * @brief Hexadecimal base for integer
		 */
		static const uint16_t hex = (1 << 2);

		/**
		 * @var bin
		 * @brief Hexadecimal base for integer
		 */
		static const uint16_t bin = (1 << 3);

		/**
		 * @var left
		 * @brief Left adjustment (adds fill characters to the right)
		 */
		static const uint16_t left = (1 << 4);

		/**
		 * @var right
		 * @brief Right adjustment (adds fill characters to the left)
		 */
		static const uint16_t right = (1 << 5);

		/**
		 * @var boolalpha
		 * @brief Insert and Extract bool type in alphanumeric format
		 */
		static const uint16_t boolalpha = (1 << 6);

		/**
		 * @var showbase
		 * @brief Prefix indicating the numeric base for integer output
		 */
		static const uint16_t showbase = (1 << 7);

		/**
		 * @var uppercase
		 * @brief Replace certain lowercase letters with their uppercase
		 */
		static const uint16_t uppercase = (1 << 8);

		/**
		 * @var basefield
		 * @brief Mask of base field
		 */
		static const uint16_t basefield = bin | oct | dec | hex;

		/**
		 * @var adjustfield
		 * @brief Mask of alignment
		 */
		static const uint16_t adjustfield = left | right;

		/**
		 * @typedef fmtflags
		 * @brief Format flags
		 */
		using fmtflags = uint16_t;

		private:
			/**
			 * @var format
			 * @brief Format flags
			 */
			fmtflags format;

			/**
			 * @var fillChar
			 * @brief Fill Character
			 */
			char fillChar;

			/**
			 * @var minWidth
			 * @brief Minimum output width
			 */
			size_t minWidth;

			/**
			 * @fn void lock()
			 * @brief Lock stream
			 */
			void lock() {
				int cpuID = CPU::getProcessorID();
				int activeCPU = cpu_lock.load();
				if (activeCPU != cpuID) {
					int unlocked = -1;
					while (!cpu_lock.compare_exchange_strong(unlocked, cpuID));
				}

			}

			/**
			 * @fn void unlock()
			 * @brief Unlock stream
			 */
			void unlock() {
				int cpuID = CPU::getProcessorID();
				int unlocking = -1;
				this->cpu_lock.compare_exchange_strong(cpuID, unlocking);
			}

			/**
			 * @fn void submitWithWidth(const char *str)
			 * @brief Print string width width correction
			 */
			void submitWithWidth(const char *str) {
				size_t len = strlen(str);

				/* Check if write correction is necessary */
				size_t correction = 0;
				if (minWidth > len)
					correction = minWidth - len;

				/* Use right alignment */
				bool rightAligned = ((format & right) != 0);
				if (rightAligned) {
					for (size_t i = 0; i < correction; i++) {
						driver::console.write(&fillChar, 1);
					}
				}

				/* Output string */
				driver::console.write(str, len);

				/* Use left alignment */
				if (!rightAligned) {
					for (size_t i = 0; i < correction; i++) {
						driver::console.write(&fillChar, 1);
					}
				}
			}

			/**
			 * @fn char* convertStrToInt(T val, char buf[128])
			 * @brief Convert string to integer
			 */
			template<typename T>
			char* convertStrToInt(T val, char buf[128]) {
				size_t len = 128;

				int base = 10;
				if ((format & bin) != 0) {
					base = 2;
				} else if ((format & oct) != 0) {
					base = 8;
				} else if ((format & hex) != 0) {
					base = 16;
				}

				/* Upper Case character */
				bool upperCase = (format & uppercase) != 0;

				/* Save sign and make val positive */
				bool sign = false;
				if (lib::is_signed<T>()) {
					if (val < 0) {
						sign = true;
						val = -val;
					}
				}

				/* Prefix */
				bool prefix = (format & showbase) != 0;

				/* Computer set of used characters */
				const char* character = nullptr;
				if (base == 2) {
					character = "01";
				} else if (base == 8) {
					character = "01234567";
				} else if (base == 10) {
					character = "0123456789";
				} else if (base == 16 && upperCase) {
					character = "0123456789ABCDEF";
				} else if (base == 16 && !upperCase) {
					character = "0123456789abcdef";
				}

				/* Prepare buffer (in case val is zero) */
				memset(buf, 0, len);
				buf[len - 1] = '\0';
				buf[len - 2] = '0';

				char* drag = &buf[len - 2];
				while (val > 0) {
					int off = val % base;
					*drag = character[off];
					drag--;
					val = val / base;

				}
				if (*drag == '\0')
					drag++;

				/* Check for sign */
				if (sign && base == 10) {
					drag--;
					*drag = '-';
				}

				/* Check prefix */
				if (prefix && base == 2) {
					*(--drag) = 'b';
					*(--drag) = '0';
				} else if (prefix && base == 8) {
					*(--drag) = '0';
				} else if (prefix && base == 16) {
					*(--drag) = 'x';
					*(--drag) = '0';
				}

				return drag;
			}

		public:
			/**
			 * @fn ostreamHelper()
			 * @brief Construct ostreamHelper
			 */
			ostreamHelper() : format(right), fillChar(' '), minWidth(0) {}

			/**
			 * @fn ~ostreamHelper()
			 * @brief Flush stream in case of non-panic object
			 */
			~ostreamHelper() {
				if constexpr (!panic)
					flush();
			}

			/**
			 * @fn ostreamHelper& put(char value)
			 * @brief Put single character and flush
			 */
			ostreamHelper& put(char value) {
				lock();
				driver::console.write(&value, 1);
				if constexpr (!panic)
					unlock();
				return *this;
			}

			/**
			 * @fn ostreamHelper& write(const char *value, size_t size)
			 * @brief Write string to console and flush
			 */
			ostreamHelper& write(const char *value, size_t size) {
				lock();
				driver::console.write(value, size);
				if constexpr (!panic)
					unlock();
				return *this;
			}

			/**
			 * @fn ostreamHelper& flush()
			 * @brief Flush and unlock stream
			 */
			ostreamHelper& flush() {
				unlock();
				return *this;
			}

			/**
			 * @fn char fill() const
			 * @brief Get current fill character
			 */
			char fill() const {
				return fillChar;
			}

			/**
			 * @fn char fill(char value)
			 * @brief Set curent fill character and return previous
			 */
			char fill(char value) {
				auto ret = fillChar;
				fillChar = value;
				return ret;
			}

			/**
			 * @fn fmtflags flags() const
			 * @brief Get format flags
			 */
			fmtflags flags() const {
				return format;
			}

			/**
			 * @fn fmtflags flags(fmtflags flags)
			 * @brief Set format flags and return previous
			 */
			fmtflags flags(fmtflags flags) {
				auto ret = format;
				format = flags;
				return ret;
			}

			/**
			 * @fn fmtflags setf(fmtflags flags)
			 * @brief Perform fl = fl | flags where fl is internal flag state
			 */
			fmtflags setf(fmtflags flags) {
				auto ret = format;
				format = format | flags;
				return ret;
			}

			/**
			 * @fn fmtflags setf(fmtflags flags, fmtflags mask)
			 * @brief Perform fl = (fl & ~mask) | (flags & mask) where fl is internal flag state
			 */
			fmtflags setf(fmtflags flags, fmtflags mask) {
				auto ret = format;
				format = (format & ~mask) | (flags & mask);
				return ret;
			}

			/**
			 * @fn void unsetf(fmtflags flags)
			 * @brief Perform fl = fl & ~flags where fl is internal flag state
			 */
			void unsetf(fmtflags flags) {
				format = format & (~flags);
			}

			/**
			 * @fn size_t width() const
			 * @brief Get minimum width of output
			 */
			size_t width() const {
				return minWidth;
			}

			/**
			 * @fn size_t width(size_t value)
			 * @brief Set minimum width of output and return previous
			 */
			size_t width(size_t value) {
				auto ret = minWidth;
				minWidth = value;
				return ret;
			}

			/**
			 * @fn ostreamHelper& operator<<(bool value)
			 * @brief Output boolean value
			 */
			ostreamHelper& operator<<(bool value) {
				/* Check if numeric output or alphanumeric */
				const char* str = nullptr;
				if ((format & boolalpha) != 0) {
					str = value ? "true" : "false";
				} else {
					str = value ? "1" : "0";
				}

				lock();
				submitWithWidth(str);

				return *this;
			}

			/**
			 * @fn ostreamHelper& operator<<(short value)
			 * @brief Output short value
			 */
			ostreamHelper& operator<<(short value) {
				char str[128];
				char* ret = convertStrToInt<short>(value, str);

				lock();
				submitWithWidth(ret);

				return *this;
			}

			/**
			 * @fn ostreamHelper& operator<<(int value)
			 * @brief Output int value
			 */
			ostreamHelper& operator<<(int value) {
				char str[128];
				char* ret = convertStrToInt<int>(value, str);

				lock();
				submitWithWidth(ret);

				return *this;
			}

			/**
			 * @fn ostreamHelper& operator<<(long value)
			 * @brief Output long value
			 */
			ostreamHelper& operator<<(long value) {
				char str[128];
				char* ret = convertStrToInt<long>(value, str);

				lock();
				submitWithWidth(ret);

				return *this;
			}

			/**
			 * @fn ostreamHelper& operator<<(long long value)
			 * @brief Output long long value
			 */
			ostreamHelper& operator<<(long long value) {
				char str[128];
				char* ret = convertStrToInt<long long>(value, str);

				lock();
				submitWithWidth(ret);

				return *this;
			}

			/**
			 * @fn ostreamHelper& operator<<(unsigned short value)
			 * @brief Output unsigned short value
			 */
			ostreamHelper& operator<<(unsigned short value) {
				char str[128];
				char* ret = convertStrToInt<unsigned short>(value, str);

				lock();
				submitWithWidth(ret);

				return *this;
			}

			/**
			 * @fn ostreamHelper& operator<<(unsigned int value)
			 * @brief Output unsigned int balue
			 */
			ostreamHelper& operator<<(unsigned int value) {
				char str[128];
				char* ret = convertStrToInt<unsigned int>(value, str);

				lock();
				submitWithWidth(ret);

				return *this;
			}

			/**
			 * @fn ostreamHelper& operator<<(unsigned long value)
			 * @brief Output unsigned long value
			 */
			ostreamHelper& operator<<(unsigned long value) {
				char str[128];
				char* ret = convertStrToInt<unsigned long>(value, str);

				lock();
				submitWithWidth(ret);

				return *this;
			}

			/**
			 * @fn ostreamHelper& operator<<(unsigned long long value)
			 * @brief Output unsigned long long value
			 */
			ostreamHelper& operator<<(unsigned long long value) {
				char str[128];
				char* ret = convertStrToInt<unsigned long long>(value, str);

				lock();
				submitWithWidth(ret);

				return *this;
			}

			/**
			 * @fn ostreamHelper& operator<<(const char* value)
			 * @brief Output string
			 */
			ostreamHelper& operator<<(const char* value) {
				lock();
				submitWithWidth(value);

				return *this;
			}

			/**
			 * @fn ostreamHelper& operator<<(void* value)
			 * @brief Output pointer
			 */
			ostreamHelper& operator<<(void* value) {
				uintptr_t ptr = reinterpret_cast<uintptr_t>(value);

				lock();
				auto oldFormat = format;
				format &= ~basefield;
				format |= hex;
				format |= showbase;
				*this << ptr;
				format = oldFormat;

				return *this;
			}
	};

	/**
	 * @typedef ostream
	 * @brief Outstream
	 */
	using ostream = ostreamHelper<false>;

	/**
	 * @typedef panic
	 * @brief Panic Outstream
	 */
	using panic = ostreamHelper<true>;

} /* namespace lib */

#endif /* ifndef _INC_OSTREAM_H_ */
