#ifndef _INC_ALGORITHM_H_
#define _INC_ALGORITHM_H_

#include <utility.h>
#include <iterator.h>

/**
 * @file algorithm.h
 * @brief Common algorithms
 */

namespace lib {

	template<typename InputIt, typename UnaryPredicate>
	bool all_of(InputIt first, InputIt last, UnaryPredicate p) {
		while (first != last) {
			if (!p(*first))
				return false;
			++first;
		}

		return true;
	}

	template<typename InputIt, typename UnaryPredicate>
	bool any_of(InputIt first, InputIt last, UnaryPredicate p) {
		while (first != last) {
			if (p(*first))
				return true;
			++first;
		}

		return false;
	}

	template<typename InputIt, typename UnaryPredicate>
	bool none_of(InputIt first, InputIt last, UnaryPredicate p) {
		while (first != last) {
			if (p(*first))
				return false;
			++first;
		}

		return true;
	}

	template<typename InputIt, typename UnaryFunction>
	UnaryFunction for_each(InputIt first, InputIt last, UnaryFunction f) {
		while (first != last) {
			f(*first);
			++first;
		}
		return f;
	}

	template<typename InputIt, typename T>
	InputIt find(InputIt first, InputIt last, const T& value) {
		while (first != last) {
			if (*first == value)
				return first;
			++first;
		}
		return last;
	}

	template<typename InputIt, typename UnaryPredicate>
	InputIt find_if(InputIt first, InputIt last, UnaryPredicate p) {
		while (first != last) {
			if (p(*first))
				return first;
			++first;
		}
		return last;
	}

	template<typename InputIt, typename UnaryPredicate>
	InputIt find_if_not(InputIt first, InputIt last, UnaryPredicate p) {
		while (first != last) {
			if (!p(*first))
				return first;
			++first;
		}
		return last;
	}

	template<typename InputIt, typename OutputIt>
	OutputIt copy(InputIt first, InputIt last, OutputIt d_first) {
		while (first != last) {
			*d_first++ = *first++;
		}
		return d_first;
	}

	template<typename InputIt, typename OutputIt, typename UnaryPredicate>
	OutputIt copy_if(InputIt first, InputIt last, OutputIt d_first, UnaryPredicate pred) {
		while (first != last) {
			if (pred(*first))
				*d_first++ = *first;
			++first;
		}
		return d_first;
	}

	template<typename InputIt, typename OutputIt>
	OutputIt move(InputIt first, InputIt last, OutputIt d_first) {
		while (first != last) {
			*d_first++ = move(*first++);
		}
		return d_first;
	}

	template<typename ForwardIt, typename T>
	void fill(ForwardIt first, ForwardIt last, const T& value) {
		while (first != last) {
			*first = value;
			++first;
		}
	}

	template<typename InputIt, typename OutputIt, typename UnaryOperation>
	OutputIt transform(InputIt first1, InputIt last1, OutputIt d_first, UnaryOperation unary_op) {
		while (first1 != last1) {
			*d_first++ = unary_op(*first1++);
		}
		return d_first;
	}

	template<typename InputIt1, typename InputIt2, typename OutputIt, typename BinaryOperation>
	OutputIt transform(InputIt1 first1, InputIt1 last1, InputIt2 first2, OutputIt d_first, BinaryOperation binary_op) {
		while (first1 != last1) {
			*d_first++ = binary_op(*first1++, *first2++);
		}
		return d_first;
	}

	template<typename ForwardIt, typename Generator>
	void generate(ForwardIt first, ForwardIt last, Generator g) {
		while (first != last) {
			*first++ = g();
		}
	}

	template<typename ForwardIt, typename T>
	ForwardIt remove(ForwardIt first, ForwardIt last, const T& value) {
		first = find(first, last, value);
		if (first != last)
			for (ForwardIt i = first; ++i != last;)
				if (!(*i == value))
					*first++ = move(*i);
		return first;
	}

	template<typename ForwardIt, typename UnaryPredicate>
	ForwardIt remove_if(ForwardIt first, ForwardIt last, UnaryPredicate p) {
		first = find_if(first, last, p);
		if (first != last)
			for (ForwardIt i = first; ++i != last;)
				if (!p(*i))
					*first++ = move(*i);
		return first;
	}

	template<typename ForwardIt, typename T>
	void replace(ForwardIt first, ForwardIt last, const T& old_value, const T& new_value) {
		while (first != last) {
			if (*first == old_value)
				*first = new_value;

			++first;
		}
	}

	template<typename ForwardIt, typename UnaryPredicate, typename T>
	void replace_if(ForwardIt first, ForwardIt last, UnaryPredicate p, const T& new_value) {
		while (first != last) {
			if(p(*first))
				*first = new_value;

			++first;
		}
	}

	template<typename ForwardIt>
	ForwardIt unique(ForwardIt first, ForwardIt last) {
		if (first == last)
			return last;

		ForwardIt result = first;
		while (++first != last) {
			if (!(*result == *first) && ++result != first) {
				*result = move(*first);
			}
		}

		return ++result;
	}

	template<typename ForwardIt, typename BinaryPredicate>
	ForwardIt unique(ForwardIt first, ForwardIt last, BinaryPredicate p) {
		if (first == last)
			return last;

		ForwardIt result = first;
		while (++first != last) {
			if (!p(*result, *first) && ++result != first) {
				*result = move(*first);
			}
		}

		return ++result;
	}

	template<typename ForwardIt>
	bool is_sorted(ForwardIt first, ForwardIt last) {
		if (first != last) {
			ForwardIt next = first;
			while (++next != last) {
				if (*next <*first)
					return false;
				first = next;
			}
		}

		return true;
	}

	template<typename ForwardIt, typename Compare>
	bool is_sorted(ForwardIt first, ForwardIt last, Compare comp) {
		if (first != last) {
			ForwardIt next = first;
			while (++next != last) {
				if (comp(*next, *first))
					return false;
				first = next;
			}
		}

		return true;
	}

	template<typename RandomIt>
	void sort(RandomIt first, RandomIt last) {
		if (first != last) {
			for (auto drag = first; drag != prev(last); ++drag) {
				for (auto drag = first; drag != prev(last); ++drag) {
					auto dragNext = next(drag);
					if (*dragNext < *drag)
						swap(*drag, *dragNext);
				}
			}
		}
	}

	template<typename RandomIt, typename Compare>
	void sort(RandomIt first, RandomIt last, Compare comp) {
		if (first != last) {
			for (auto drag = first; drag != prev(last); ++drag) {
				for (auto drag = first; drag != prev(last); ++drag) {
					auto dragNext = next(drag);
					if (comp(*dragNext, *drag))
						swap(*drag, *dragNext);
				}
			}
		}
	}

	template<typename T>
	const T& max(const T& a, const T& b) {
		return (a < b) ? b : a;
	}

	template<typename T, typename Compare>
	const T& max(const T& a, const T& b, Compare comp) {
		return (comp(a, b)) ? b : a;
	}

	template<typename T>
	const T& min(const T& a, const T& b) {
		return (b < a) ? b : a;
	}

	template<typename T, typename Compare>
	const T& min(const T& a, const T& b, Compare comp) {
		return (comp(b, a)) ? b : a;
	}

	template<typename InputIt1, typename InputIt2>
	bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2) {
		while (first1 != last1) {
			if (!(*first1 == *first2))
				return false;

			++first1;
			++first2;
		}

		return true;
	}

	template<typename InputIt1, typename InputIt2, typename BinaryPredicate>
	bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPredicate p) {
		while (first1 != last1) {
			if (!p(*first1, *first2))
				return false;
			++first1;
			++first2;
		}

		return true;
	}

	template<typename InputIt, typename T>
	T accumulate(InputIt first, InputIt last, T init) {
		while (first != last) {
			init = move(init) + *first;
		}

		return init;
	}

} /* namespace lib */

#endif /* ifndef _INC_ALGORITHM_H_ */
