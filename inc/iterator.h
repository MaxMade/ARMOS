#ifndef _INC_ITERATOR_H_
#define _INC_ITERATOR_H_

#include <cstdint.h>
#include <type_traits.h>

/**
 * @file iterator.h
 * @brief Common iterator operations
 */

namespace lib {

	/**
	 * @fn constexpr void advance(InputIt& it, Distance n)
	 * @brief Advance iterator
	 */
	template<class InputIt, class Distance>
	constexpr void advance(InputIt& it, Distance n) {
		while (n > 0) {
			--n;
			++it;
		}
		while (n < 0) {
			++n;
			--it;
		}
	}

	/**
	 * @fn constexpr InputIt next(InputIt it, long n = 1)
	 * @brief Get next entry
	 */
	template<class InputIt>
	constexpr InputIt next(InputIt it, long n = 1) {
		advance(it, n);
		return it;
	}

	/**
	 * @fn constexpr BidirIt prev(BidirIt it, long n = 1)
	 * @brief Get previous entry
	 */
	template<class BidirIt>
	constexpr BidirIt prev(BidirIt it, long n = 1) {
		advance(it, -n);
		return it;
	}

	/**
	 * @fn long distance(InputIt first, InputIt last)
	 * @brief Get distance between iterators
	 */
	template<class InputIt>
	long distance(InputIt first, InputIt last) {
		long ret = 0;
		while(first != last) {
			ret++;
			++first;
		}

		return ret;
	}

	/**
	 * @class reverse_iterator
	 * @brief Reverse Iterator
	 */
	template<class Iter>
	class reverse_iterator {
		private:
			Iter it;

		public:
			using iterator_type = Iter;

			reverse_iterator() {}

			reverse_iterator(iterator_type x) : it(x) {}

			iterator_type base() const {
				return it;
			}

			auto operator*() const {
				auto tmp = it;
				return *(--tmp);
			}

			reverse_iterator& operator++() {
				--it;
				return *this;
			}

			reverse_iterator& operator--() {
				++it;
				return *this;
			}

			reverse_iterator& operator++(int) {
				auto tmp = it;
				it--;
				return tmp;
			}

			reverse_iterator& operator--(int) {
				auto tmp = it;
				it++;
				return tmp;
			}

			reverse_iterator operator+(int n) const {
				return reverse_iterator(base() - n);
			}

			reverse_iterator operator-(int n) const {
				return reverse_iterator(base() + n);
			}

			reverse_iterator& operator+=(int n) {
				it -= n;
				return *this;
			}

			reverse_iterator& operator-=(int n) {
				it += n;
				return *this;
			}
	};
	template< class Iterator1, class Iterator2 >
	bool operator==(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) {
		return lhs.base() == rhs.base();
	}

	template< class Iterator1, class Iterator2 >
	bool operator!=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) {
		return lhs.base() != rhs.base();
	}

	template< class Iterator1, class Iterator2 >
	bool operator<(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) {
		return lhs.base() > rhs.base();
	}

	template< class Iterator1, class Iterator2 >
	bool operator>(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) {
		return lhs.base() < rhs.base();
	}

	template< class Iterator1, class Iterator2 >
	bool operator<=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) {
		return lhs.base() >= rhs.base();
	}

	template< class Iterator1, class Iterator2 >
	bool operator>=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) {
		return lhs.base() <= rhs.base();
	}

} /* namespace lib */

#endif /* ifndef _INC_ITERATOR_H_ */
