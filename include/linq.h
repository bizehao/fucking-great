#pragma once

#include <utility>
#include <xutility>
#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <stdexcept>
#include <concepts>
#include <type_traits>
#include "function_traits.hpp"

namespace vczh {

	namespace linq_extract {
		template <typename Container>
		concept is_container = requires(Container && x) {
			std::begin(x);
			std::end(x);
		};

		template <typename Iter>
		concept is_iterator = requires(Iter && x) {
			typename Iter::iterator_category;
			typename Iter::value_type;
			typename Iter::difference_type;
			typename Iter::pointer;
			typename Iter::reference;
		};

	} // namespace linq_extract

	template <typename TIterator>
	using iterator_type = decltype(**(TIterator*)0);

	class linq_exception : std::exception {
	public:
		std::string message;

		linq_exception(const std::string& _message) : message(_message) {}

		char const* what() const override { return message.c_str(); }
	};

	template <typename TMethod>
	struct linq_lambda_retriver {
		using ResultType = void;
	};

	template <typename TClass, typename TResult, typename... TArgs>
	struct linq_lambda_retriver<TResult(TClass::*)(TArgs...)> {
		using ResultType = TResult;
	};

	template <typename TClass, typename TResult, typename... TArgs>
	struct linq_lambda_retriver<TResult(TClass::*)(TArgs...) const> {
		using ResultType = TResult;
	};

	template <typename T, typename U>
	struct zip_pair {
		T first;
		U second;

		zip_pair() {}
		zip_pair(const T& _first, const U& _second) : first(_first), second(_second) {}
		template <typename X, typename Y>
		zip_pair(const zip_pair<X, Y>& p) : first(p.first), second(p.second) {}

		static int compare(const zip_pair<T, U>& a, const zip_pair<T, U>& b) {
			return a.first < a.second ? -1 : a.first > a.second ? 1 : b.first < b.second ? -1 : b.first > b.second ? 1 : 0;
		}

		// auto operator<=>(const zip_pair<T, U>& p) const = default;

		template <typename TT, typename UU>
		bool operator==(const zip_pair<TT, UU>& p) const {
			return first == p.first && second == p.second;
		}
		template <typename TT, typename UU>
		bool operator!=(const zip_pair<TT, UU>& p) const {
			return first != p.first || second != p.second;
		}
		template <typename TT, typename UU>
		bool operator<(const zip_pair<TT, UU>& p) const {
			return compare(*this, p) < 0;
		}

		template <typename TT, typename UU>
		bool operator<=(const zip_pair<TT, UU>& p) const {
			return compare(*this, p) <= 0;
		}

		template <typename TT, typename UU>
		bool operator>(const zip_pair<TT, UU>& p) const {
			return compare(*this, p) > 0;
		}

		template <typename TT, typename UU>
		bool operator>=(const zip_pair<TT, UU>& p) const {
			return compare(*this, p) >= 0;
		}
	};

	template <typename TKey, typename TValue1, typename TValue2>
	using join_pair = zip_pair<TKey, zip_pair<TValue1, TValue2>>;

	namespace iterators {
		//////////////////////////////////////////////////////////////////
		// hide_type
		//////////////////////////////////////////////////////////////////

		template <typename T>
		class hide_type_iterator {
		private:
			class iterator_interface {
				using TSelf = iterator_interface;

			public:
				virtual std::shared_ptr<TSelf> next() = 0;
				virtual T deref() = 0;
				virtual bool equals(const std::shared_ptr<TSelf>& it) = 0;
			};

			template <typename TIterator>
			class iterator_implement : public iterator_interface {
				using TSelf = iterator_implement<TIterator>;

			private:
				TIterator iterator;

			public:
				iterator_implement(const TIterator& _iterator) : iterator(_iterator) {}

				std::shared_ptr<iterator_interface> next() override {
					TIterator t = iterator;
					t++;
					return std::make_shared<TSelf>(t);
				}

				T deref() override { return *iterator; }

				bool equals(const std::shared_ptr<iterator_interface>& it) override {
					auto impl = std::dynamic_pointer_cast<TSelf>(it);
					return impl && (iterator == impl->iterator);
				}
			};

			using TSelf = hide_type_iterator<T>;

			std::shared_ptr<iterator_interface> iterator;

		public:
			template <typename TIterator>
			hide_type_iterator(const TIterator& _iterator) : iterator(std::make_shared<iterator_implement<TIterator>>(_iterator)) {}

			TSelf& operator++() {
				iterator = iterator->next();
				return *this;
			}

			TSelf operator++(int) {
				TSelf t = *this;
				iterator = iterator->next();
				return t;
			}

			T operator*() const { return iterator->deref(); }

			bool operator==(const TSelf& it) const { return iterator->equals(it.iterator); }

			bool operator!=(const TSelf& it) const { return !(iterator->equals(it.iterator)); }
		};

		//////////////////////////////////////////////////////////////////
		// storage
		//////////////////////////////////////////////////////////////////

		template <typename T>
		class storage_iterator {
			using TSelf = storage_iterator<T>;

		private:
			std::shared_ptr<std::vector<T>> values;
			typename std::vector<T>::iterator iterator;

		public:
			storage_iterator(const std::shared_ptr<std::vector<T>>& _values, const typename std::vector<T>::iterator& _iterator)
				: values{ _values }, iterator{ _iterator } {}

			TSelf& operator++() {
				iterator++;
				return *this;
			}

			TSelf operator++(int) {
				TSelf t = *this;
				iterator++;
				return t;
			}

			T operator*() const { return *iterator; }

			bool operator==(const TSelf& it) const { return iterator == it.iterator; }

			bool operator!=(const TSelf& it) const { return iterator != it.iterator; }
		};

		//////////////////////////////////////////////////////////////////
		// empty
		//////////////////////////////////////////////////////////////////

		template <typename T>
		class empty_iterator {
			using TSelf = empty_iterator<T>;

		private:
		public:
			empty_iterator() {}

			TSelf& operator++() { return *this; }

			TSelf operator++(int) { return *this; }

			T operator*() const { throw linq_exception("Failed to get a value from an empty collection."); }

			bool operator==(const TSelf& it) const { return true; }

			bool operator!=(const TSelf& it) const { return false; }
		};

		//////////////////////////////////////////////////////////////////
		// select
		//////////////////////////////////////////////////////////////////

		template <typename TIterator, typename TFunction>
		class select_iterator {
			using TSelf = select_iterator<TIterator, TFunction>;

		private:
			TIterator iterator;
			TFunction f;

		public:
			select_iterator(const TIterator& _iterator, const TFunction& _f) : iterator{ _iterator }, f{ _f } {}

			TSelf& operator++() {
				iterator++;
				return *this;
			}

			TSelf operator++(int) {
				TSelf t = *this;
				iterator++;
				return t;
			}

			auto operator*() const -> decltype(f(*iterator)) { return f(*iterator); }

			bool operator==(const TSelf& it) const { return iterator == it.iterator; }

			bool operator!=(const TSelf& it) const { return iterator != it.iterator; }
		};

		//////////////////////////////////////////////////////////////////
		// where
		//////////////////////////////////////////////////////////////////

		template <typename TIterator, typename TFunction>
		class where_iterator {
			using TSelf = where_iterator<TIterator, TFunction>;

		private:
			TIterator iterator;
			TIterator end;
			TFunction f;

			void move_iterator(bool next) {
				if (iterator == end)
					return;
				if (next)
					iterator++;
				while (iterator != end && !f(*iterator)) {
					iterator++;
				}
			}

		public:
			where_iterator(const TIterator& _iterator, const TIterator& _end, const TFunction& _f) : iterator{ _iterator }, end{ _end }, f{ _f } {
				move_iterator(false);
			}

			TSelf& operator++() {
				move_iterator(true);
				return *this;
			}

			TSelf operator++(int) {
				TSelf t = *this;
				move_iterator(true);
				return t;
			}

			iterator_type<TIterator> operator*() const { return *iterator; }

			bool operator==(const TSelf& it) const { return iterator == it.iterator; }

			bool operator!=(const TSelf& it) const { return iterator != it.iterator; }
		};

		//////////////////////////////////////////////////////////////////
		// skip
		//////////////////////////////////////////////////////////////////

		template <typename TIterator>
		class skip_iterator {
			using TSelf = skip_iterator<TIterator>;

		private:
			TIterator iterator;
			TIterator end;

		public:
			skip_iterator(const TIterator& _iterator, const TIterator& _end, int _count) : iterator{ _iterator }, end{ _end } {
				for (int i = 0; i < _count && iterator != end; i++, iterator++)
					;
			}

			TSelf& operator++() {
				iterator++;
				return *this;
			}

			TSelf operator++(int) {
				TSelf t = *this;
				iterator++;
				return t;
			}

			iterator_type<TIterator> operator*() const { return *iterator; }

			bool operator==(const TSelf& it) const { return iterator == it.iterator; }

			bool operator!=(const TSelf& it) const { return iterator != it.iterator; }
		};

		//////////////////////////////////////////////////////////////////
		// skip_while
		//////////////////////////////////////////////////////////////////

		template <typename TIterator, typename TFunction>
		class skip_while_iterator {
			using TSelf = skip_while_iterator<TIterator, TFunction>;

		private:
			TIterator iterator;
			TIterator end;
			TFunction f;

		public:
			skip_while_iterator(const TIterator& _iterator, const TIterator& _end, const TFunction& _f) : iterator(_iterator), end(_end), f(_f) {
				while (iterator != end && f(*iterator)) {
					iterator++;
				}
			}

			TSelf& operator++() {
				iterator++;
				return *this;
			}

			TSelf operator++(int) {
				TSelf t = *this;
				iterator++;
				return t;
			}

			iterator_type<TIterator> operator*() const { return *iterator; }

			bool operator==(const TSelf& it) const { return iterator == it.iterator; }

			bool operator!=(const TSelf& it) const { return iterator != it.iterator; }
		};

		//////////////////////////////////////////////////////////////////
		// take
		//////////////////////////////////////////////////////////////////

		template <typename TIterator>
		class take_iterator {
			using TSelf = take_iterator<TIterator>;

		private:
			TIterator iterator;
			TIterator end;
			int count;
			int current;

		public:
			take_iterator(const TIterator& _iterator, const TIterator& _end, int _count) : iterator{ _iterator }, end{ _end }, count{ _count }, current{ 0 } {
				if (current == count) {
					iterator = end;
				}
			}

			TSelf& operator++() {
				if (++current == count) {
					iterator = end;
				}
				else {
					iterator++;
				}
				return *this;
			}

			TSelf operator++(int) {
				TSelf t = *this;
				if (++current == count) {
					iterator = end;
				}
				else {
					iterator++;
				}
				return t;
			}

			iterator_type<TIterator> operator*() const { return *iterator; }

			bool operator==(const TSelf& it) const { return iterator == it.iterator; }

			bool operator!=(const TSelf& it) const { return iterator != it.iterator; }
		};

		//////////////////////////////////////////////////////////////////
		// take_while
		//////////////////////////////////////////////////////////////////

		template <typename TIterator, typename TFunction>
		class take_while_iterator {
			using TSelf = take_while_iterator<TIterator, TFunction>;

		private:
			TIterator iterator;
			TIterator end;
			TFunction f;

		public:
			take_while_iterator(const TIterator& _iterator, const TIterator& _end, const TFunction& _f) : iterator{ _iterator }, end{ _end }, f{ _f } {
				if (iterator != end && !f(*iterator)) {
					iterator = end;
				}
			}

			TSelf& operator++() {
				if (!f(*++iterator)) {
					iterator = end;
				}
				return *this;
			}

			TSelf operator++(int) {
				TSelf t = *this;
				if (!f(*++iterator)) {
					iterator = end;
				}
				return t;
			}

			iterator_type<TIterator> operator*() const { return *iterator; }

			bool operator==(const TSelf& it) const { return iterator == it.iterator; }

			bool operator!=(const TSelf& it) const { return iterator != it.iterator; }
		};

		//////////////////////////////////////////////////////////////////
		// concat
		//////////////////////////////////////////////////////////////////

		template <typename TIterator1, typename TIterator2>
		class concat_iterator {
			using TSelf = concat_iterator<TIterator1, TIterator2>;

		private:
			TIterator1 current1;
			TIterator1 end1;
			TIterator2 current2;
			TIterator2 end2;
			bool first;

		public:
			concat_iterator(const TIterator1& _current1, const TIterator1& _end1, const TIterator2& _current2, const TIterator2& _end2)
				: current1{ _current1 }, end1{ _end1 }, current2{ _current2 }, end2{ _end2 }, first{ _current1 != _end1 } {}

			TSelf& operator++() {
				if (first) {
					if (++current1 == end1) {
						first = false;
					}
				}
				else {
					current2++;
				}
				return *this;
			}

			TSelf operator++(int) {
				TSelf t = *this;
				if (first) {
					if (++current1 == end1) {
						first = false;
					}
				}
				else {
					current2++;
				}
				return t;
			}

			iterator_type<TIterator1> operator*() const { return first ? *current1 : *current2; }

			bool operator==(const TSelf& it) const {
				if (first != it.first)
					return false;
				return first ? current1 == it.current1 : current2 == it.current2;
			}

			bool operator!=(const TSelf& it) const {
				if (first != it.first)
					return true;
				return first ? current1 != it.current1 : current2 != it.current2;
			}
		};

		//////////////////////////////////////////////////////////////////
		// zip
		//////////////////////////////////////////////////////////////////

		template <typename TIterator1, typename TIterator2>
		class zip_iterator {
			using TSelf = zip_iterator<TIterator1, TIterator2>;
			using TElement = zip_pair<iterator_type<TIterator1>, iterator_type<TIterator2>>;

		private:
			TIterator1 current1;
			TIterator1 end1;
			TIterator2 current2;
			TIterator2 end2;

		public:
			zip_iterator(const TIterator1& _current1, const TIterator1& _end1, const TIterator2& _current2, const TIterator2& _end2)
				: current1{ _current1 }, end1{ _end1 }, current2{ _current2 }, end2{ _end2 } {}

			TSelf& operator++() {
				if (current1 != end1 && current2 != end2) {
					current1++;
					current2++;
				}
				return *this;
			}

			TSelf operator++(int) {
				TSelf t = *this;
				if (current1 != end1 && current2 != end2) {
					current1++;
					current2++;
				}
				return t;
			}

			TElement operator*() const { return TElement(*current1, *current2); }

			bool operator==(const TSelf& it) const { return current1 == it.current1 && current2 == it.current2; }

			bool operator!=(const TSelf& it) const { return current1 != it.current1 || current2 != it.current2; }
		};
	} // namespace iterators

	namespace types {
		template <typename T>
		using storage_it = iterators::storage_iterator<T>;

		template <typename T>
		using empty_it = iterators::empty_iterator<T>;

		template <typename TIterator, typename TFunction>
		using select_it = iterators::select_iterator<TIterator, TFunction>;

		template <typename TIterator, typename TFunction>
		using where_it = iterators::where_iterator<TIterator, TFunction>;

		template <typename TIterator>
		using skip_it = iterators::skip_iterator<TIterator>;

		template <typename TIterator, typename TFunction>
		using skip_while_it = iterators::skip_while_iterator<TIterator, TFunction>;

		template <typename TIterator>
		using take_it = iterators::take_iterator<TIterator>;

		template <typename TIterator, typename TFunction>
		using take_while_it = iterators::take_while_iterator<TIterator, TFunction>;

		template <typename TIterator1, typename TIterator2>
		using concat_it = iterators::concat_iterator<TIterator1, TIterator2>;

		template <typename TIterator1, typename TIterator2>
		using zip_it = iterators::zip_iterator<TIterator1, TIterator2>;
	} // namespace types

	//////////////////////////////////////////////////////////////////
	// linq
	//////////////////////////////////////////////////////////////////

	template <typename TIterator>
	class linq_enumerable;

	template <typename T>
	class linq;

	template <typename TElement>
	linq<TElement> from_values(std::shared_ptr<std::vector<TElement>> xs) {
		using storage_it = types::storage_it<TElement>;
		return linq_enumerable<storage_it>{storage_it{ xs, xs->begin() }, storage_it{ xs, xs->end() }};
	}

	template <typename TElement>
	linq<TElement> from_values(const std::initializer_list<TElement>& ys) {
		auto xs = std::make_shared<std::vector<TElement>>(ys.begin(), ys.end());
		using storage_it = types::storage_it<TElement>;
		return linq_enumerable<storage_it>{storage_it{ xs, xs->begin() }, storage_it{ xs, xs->end() }};
	}

	template <typename TElement>
	linq<TElement> from_value(const TElement& value) {
		auto xs = std::make_shared<std::vector<TElement>>();
		xs->push_back(value);
		return from_values(xs);
	}

	template <typename TElement>
	linq<TElement> from_empty() {
		return linq_enumerable<types::empty_it<TElement>>{types::empty_it<TElement>{}, types::empty_it<TElement>{}};
	}

	template <typename TIterator>
		requires linq_extract::is_iterator<TIterator> linq_enumerable<TIterator> from(const TIterator& begin, const TIterator& end);

	template <typename TContainer>
		requires linq_extract::is_container<TContainer>
	auto from(const TContainer& container)->linq_enumerable<decltype(std::begin(container))>;

	template <typename TIterator>
	class linq_enumerable {
		using TElement = std::remove_cvref_t<iterator_type<TIterator>>;

	private:
		TIterator _begin;
		TIterator _end;

	public:
		linq_enumerable() {}

		linq_enumerable(const TIterator& _begin_, const TIterator& _end_) : _begin(_begin_), _end(_end_) {}

		TIterator begin() const { return _begin; }

		TIterator end() const { return _end; }

#ifdef _MSC_VER

#define SUPPORT_STL_CONTAINERS(NAME)                                                                                                                 \
    template <typename TIterator2>                                                                                                                   \
    auto NAME(const linq_enumerable<TIterator2>& e) const->decltype(NAME##_(e)) {                                                                    \
        return NAME##_(e);                                                                                                                           \
    }                                                                                                                                                \
    template <typename TContainer>                                                                                                                   \
    auto NAME(const TContainer& e) const->decltype(NAME##_(from(e))) {                                                                               \
        return NAME##_(from(e));                                                                                                                     \
    }                                                                                                                                                \
    template <typename TElement>                                                                                                                     \
    auto NAME(const std::initializer_list<TElement>& e) const->decltype(NAME##_(from(e))) {                                                          \
        return NAME##_(from(e));                                                                                                                     \
    }

#define PROTECT_PARAMETERS(...) __VA_ARGS__

#define SUPPORT_STL_CONTAINERS_EX(NAME, TYPES, PARAMETERS, ARGUMENTS)                                                                                \
    template <typename TIterator2, TYPES>                                                                                                            \
    auto NAME(const linq_enumerable<TIterator2>& e, PARAMETERS) const->decltype(NAME##_(e, ARGUMENTS)) {                                             \
        return NAME##_(e, ARGUMENTS);                                                                                                                \
    }                                                                                                                                                \
    template <typename TContainer, TYPES>                                                                                                            \
    auto NAME(const TContainer& e, PARAMETERS) const->decltype(NAME##_(from(e), ARGUMENTS)) {                                                        \
        return NAME##_(from(e), ARGUMENTS);                                                                                                          \
    }                                                                                                                                                \
    template <typename TElement, TYPES>                                                                                                              \
    auto NAME(const std::initializer_list<TElement>& e, PARAMETERS) const->decltype(NAME##_(from(e), ARGUMENTS)) {                                   \
        return NAME##_(from(e), ARGUMENTS);                                                                                                          \
    }

#else

#define SUPPORT_STL_CONTAINERS(NAME)                                                                                                                 \
    template <typename TIterator2>                                                                                                                   \
    auto NAME(const linq_enumerable<TIterator2>& e) const->decltype(this->NAME##_(e)) {                                                              \
        return NAME##_(e);                                                                                                                           \
    }                                                                                                                                                \
    template <typename TContainer>                                                                                                                   \
    auto NAME(const TContainer& e) const->decltype(this->NAME##_(from(e))) {                                                                         \
        return NAME##_(from(e));                                                                                                                     \
    }                                                                                                                                                \
    template <typename TElement>                                                                                                                     \
    auto NAME(const std::initializer_list<TElement>& e) const->decltype(this->NAME##_(from(e))) {                                                    \
        return NAME##_(from(e));                                                                                                                     \
    }

#define PROTECT_PARAMETERS(...) __VA_ARGS__

#define SUPPORT_STL_CONTAINERS_EX(NAME, TYPES, PARAMETERS, ARGUMENTS)                                                                                \
    template <typename TIterator2, TYPES>                                                                                                            \
    auto NAME(const linq_enumerable<TIterator2>& e, PARAMETERS) const->decltype(this->NAME##_(e, ARGUMENTS)) {                                       \
        return NAME##_(e, ARGUMENTS);                                                                                                                \
    }                                                                                                                                                \
    template <typename TContainer, TYPES>                                                                                                            \
    auto NAME(const TContainer& e, PARAMETERS) const->decltype(this->NAME##_(from(e), ARGUMENTS)) {                                                  \
        return NAME##_(from(e), ARGUMENTS);                                                                                                          \
    }                                                                                                                                                \
    template <typename TElement, TYPES>                                                                                                              \
    auto NAME(const std::initializer_list<TElement>& e, PARAMETERS) const->decltype(this->NAME##_(from(e), ARGUMENTS)) {                             \
        return NAME##_(from(e), ARGUMENTS);                                                                                                          \
    }

#endif

		//////////////////////////////////////////////////////////////////
		// iterating (lazy evaluation) std::same_as<std::invoke_result_t<TFunction, TElement>,TElement>;
		//////////////////////////////////////////////////////////////////
		//, typename std::enable_if_t<std::is_same_v<std::invoke_result_t<TFunction, TElement>, TElement>>* = nullptr
		// requires(TFunction&& fun) { requires std::same_as<std::invoke_result_t<TFunction, TElement>, TElement>; }
		// requires std::same_as<std::invoke_result_t<TFunction, TElement>, TElement>

		template <typename TFunction>
		linq_enumerable<types::select_it<TIterator, TFunction>> select(const TFunction& f) const {
			using select_it = types::select_it<TIterator, TFunction>;
			return linq_enumerable<select_it>{select_it{ _begin, f }, select_it{ _end, f }};
		}

		template <typename TFunction>
		linq_enumerable<types::where_it<TIterator, TFunction>> where(const TFunction& f) const {
			using where_it = types::where_it<TIterator, TFunction>;
			return linq_enumerable<where_it>(where_it{ _begin, _end, f }, where_it{ _end, _end, f });
		}

		linq_enumerable<types::skip_it<TIterator>> skip(int count) const {
			using skip_it = types::skip_it<TIterator>;
			return linq_enumerable<skip_it>{skip_it{ _begin, _end, count }, skip_it{ _end, _end, count }};
		}

		template <typename TFunction>
		linq_enumerable<types::skip_while_it<TIterator, TFunction>> skip_while(const TFunction& f) const {
			using skip_while_it = types::skip_while_it<TIterator, TFunction>;
			return linq_enumerable<skip_while_it>{skip_while_it{ _begin, _end, f },
				skip_while_it{ _end, _end, f }};
		}

		linq_enumerable<types::take_it<TIterator>> take(int count) const {
			using take_it = types::take_it<TIterator>;
			return linq_enumerable<take_it>{take_it{ _begin, _end, count }, take_it{ _end, _end, count }};
		}

		template <typename TFunction>
		linq_enumerable<types::take_while_it<TIterator, TFunction>> take_while(const TFunction& f) const {
			using take_while_it = types::take_while_it<TIterator, TFunction>;
			return linq_enumerable<take_while_it>{take_while_it{ _begin, _end, f }, take_while_it{ _end, _end, f }};
		}
	private:
		template <typename TIterator2>
		linq_enumerable<types::concat_it<TIterator, TIterator2>> concat_(const linq_enumerable<TIterator2>& e) const {
			using concat_it = types::concat_it<TIterator, TIterator2>;
			return linq_enumerable<concat_it>{concat_it{ _begin, _end, e.begin(), e.end() }, concat_it{ _end, _end, e.end(), e.end() }};
		}
	public:
		SUPPORT_STL_CONTAINERS(concat)

			//////////////////////////////////////////////////////////////////
			// counting
			//////////////////////////////////////////////////////////////////

			template <typename T>
		bool contains(const T& t) const {
			for (auto it = _begin; it != _end; it++) {
				if (*it == t)
					return true;
			}
			return false;
		}

		int count() const {
			int counter = 0;
			for (auto it = _begin; it != _end; it++) {
				counter++;
			}
			return counter;
		}

		linq<TElement> default_if_empty(const TElement& value) const {
			if (count() == 0) {
				return from_value(value);
			}
			else {
				return *this;
			}
		}

		TElement element_at(int index) const {
			if (index >= 0) {
				int counter = 0;
				for (auto it = _begin; it != _end; it++) {
					if (counter == index)
						return *it;
					counter++;
				}
			}
			throw linq_exception("Argument out of range: index.");
		}

		bool empty() const { return _begin == _end; }

		TElement first() const {
			if (empty())
				throw linq_exception("Failed to get a value from an empty collection.");
			return *_begin;
		}

		TElement first_or_default(const TElement& value) const { return empty() ? value : *_begin; }

		TElement last() const {
			if (empty())
				throw linq_exception("Failed to get a value from an empty collection.");
			auto it = _begin;
			TElement result = *it;
			while (++it != _end) {
				result = *it;
			}
			return result;
		}

		TElement last_or_default(const TElement& value) const {
			auto result = value;
			for (auto it = _begin; it != _end; it++) {
				result = *it;
			}
			return result;
		}

		linq_enumerable<TIterator> single() const {
			auto it = _begin;
			if (it == _end)
				throw linq_exception("Failed to get a value from an empty collection.");

			it++;
			if (it != _end)
				throw linq_exception("The collection should have exactly one value.");

			return *this;
		}

		linq<TElement> single_or_default(const TElement& value) const {
			auto it = _begin;
			if (it == _end)
				return from_value(value);

			it++;
			if (it != _end)
				throw linq_exception("The collection should have exactly one value.");

			return *this;
		}

	private:
		template <typename TIterator2>
		bool sequence_equal_(const linq_enumerable<TIterator2>& e) const {
			auto x = _begin;
			auto xe = _end;
			auto y = e.begin();
			auto ye = e.end();

			while (x != xe && y != ye) {
				if (*x++ != *y++) {
					return false;
				}
			}
			return x == xe && y == ye;
		}

	public:
		SUPPORT_STL_CONTAINERS(sequence_equal)

			//////////////////////////////////////////////////////////////////
			// set
			//////////////////////////////////////////////////////////////////

			linq<TElement> distinct() const {
			std::set<TElement> set;
			auto xs = std::make_shared<std::vector<TElement>>();
			for (auto it = _begin; it != _end; it++) {
				if (set.insert(*it).second) {
					xs->push_back(*it);
				}
			}
			return from_values(xs);
		}
	private:

		template <typename TIterator2>
		linq<TElement> except_with_(const linq_enumerable<TIterator2>& e) const {
			std::set<TElement> set(e.begin(), e.end());
			auto xs = std::make_shared<std::vector<TElement>>();
			for (auto it = _begin; it != _end; it++) {
				if (set.insert(*it).second) {
					xs->push_back(*it);
				}
			}
			return from_values(xs);
		}
	public:
		SUPPORT_STL_CONTAINERS(except_with)
	private:
		template <typename TIterator2>
		linq<TElement> intersect_with_(const linq_enumerable<TIterator2>& e) const {
			std::set<TElement> seti, set(e.begin(), e.end());
			auto xs = std::make_shared<std::vector<TElement>>();
			for (auto it = _begin; it != _end; it++) {
				if (seti.insert(*it).second && !set.insert(*it).second) {
					xs->push_back(*it);
				}
			}
			return from_values(xs);
		}
	public:
		SUPPORT_STL_CONTAINERS(intersect_with)
	private:
		template <typename TIterator2>
		linq<TElement> union_with_(const linq_enumerable<TIterator2>& e) const {
			return concat(e).distinct();
		}
	public:
		SUPPORT_STL_CONTAINERS(union_with)

			//////////////////////////////////////////////////////////////////
			// aggregating
			//////////////////////////////////////////////////////////////////

			template <typename TFunction>
		TElement aggregate(const TFunction& f) const {
			auto it = _begin;
			if (it == _end)
				throw linq_exception("Failed to get a value from an empty collection.");

			TElement result = *it;
			while (++it != _end) {
				result = f(result, *it);
			}

			return result;
		}

		template <typename TResult, typename TFunction>
		TResult aggregate(const TResult& init, const TFunction& f) const {
			TResult result = init;
			for (auto it = _begin; it != _end; it++) {
				result = f(result, *it);
			}
			return result;
		}

		template <typename TFunction>
		bool all(const TFunction& f) const {
			return select(f).aggregate(true, [](bool a, bool b) { return a && b; });
		}

		template <typename TFunction>
		bool any(const TFunction& f) const {
			return !where(f).empty();
		}

		template <typename TResult>
		TResult average() const {
			if (_begin == _end)
				throw linq_exception("Failed to get a value from an empty collection.");
			TResult sum = 0;
			int counter = 0;
			for (auto it = _begin; it != _end; it++) {
				sum += (TResult)*it;
				counter++;
			}
			return sum / counter;
		}

		TElement max() const {
			return aggregate([](const TElement& a, const TElement& b) { return a > b ? a : b; });
		}

		TElement min() const {
			return aggregate([](const TElement& a, const TElement& b) { return a < b ? a : b; });
		}

		TElement sum() {
			return aggregate(0, [](const TElement& a, const TElement& b) { return a + b; });
		}

		TElement product() {
			return aggregate([](const TElement& a, const TElement& b) { return a * b; });
		}

		//////////////////////////////////////////////////////////////////
		// restructuring
		//////////////////////////////////////////////////////////////////

		template <typename TFunction>
		auto select_many(const TFunction& f) const
#ifdef _MSC_VER
			-> linq<decltype(*f(*(TElement*)0).begin())>
#else
			-> linq<decltype(*((typename linq_lambda_retriver<decltype(&TFunction::operator())>::ResultType*)0)->begin())>
#endif
		{
			using TCollection = decltype(f(*(TElement*)0));
			using TValue = decltype(*f(*(TElement*)0).begin());
			return select(f).aggregate(from_empty<TValue>(), [](const linq<TValue>& a, const TCollection& b) { return a.concat(b); });
		}

		template <typename TFunction>
		auto group_by(const TFunction& keySelector) const -> linq<zip_pair<decltype(keySelector(*(TElement*)0)), linq<TElement>>> {
			using TKey = decltype(keySelector(*(TElement*)0));
			using TValueVector = std::vector<TElement>;
			using TValueVectorPtr = std::shared_ptr<TValueVector>;

			std::map<TKey, TValueVectorPtr> map;
			for (auto it = _begin; it != _end; it++) {
				auto value = *it;
				auto key = keySelector(value);
				auto it2 = map.find(key);
				if (it2 == map.end()) {
					auto xs = std::make_shared<TValueVector>();
					xs->push_back(value);
					map.insert(std::make_pair(key, xs));
				}
				else {
					it2->second->push_back(value);
				}
			}

			auto result = std::make_shared<std::vector<zip_pair<TKey, linq<TElement>>>>();
			for (auto p : map) {
				result->push_back(zip_pair<TKey, linq<TElement>>(p.first, from_values(p.second)));
			}
			return from_values(result);
		}
	private:

		template <typename TIterator2, typename TFunction1, typename TFunction2>
		auto full_join_(const linq_enumerable<TIterator2>& e, const TFunction1& keySelector1, const TFunction2& keySelector2) const
			->linq<join_pair<std::remove_reference_t<decltype(keySelector1(*(TElement*)0))>,
			linq< std::remove_cv_t<std::remove_reference_t<iterator_type<TIterator>>>>,
			linq<std::remove_cv_t<std::remove_reference_t<iterator_type<TIterator2>>>>>> {


			using TKey = std::remove_reference_t<decltype(keySelector1(*(TElement*)0))>;
			using TValue1 = std::remove_cv_t<std::remove_reference_t<iterator_type<TIterator>>>;
			using TValue2 = std::remove_cv_t< std::remove_reference_t<iterator_type<TIterator2>>>;
			using TFullJoinPair = join_pair<TKey, linq<TValue1>, linq<TValue2>>;

			std::multimap<TKey, TValue1> map1;
			std::multimap<TKey, TValue2> map2;

			for (auto it = _begin; it != _end; it++) {
				auto value = *it;
				auto key = keySelector1(value);
				map1.insert(std::make_pair(key, value));
			}
			for (auto it = e.begin(); it != e.end(); it++) {
				auto value = *it;
				auto key = keySelector2(value);
				map2.insert(std::make_pair(key, value));
			}

			auto result = std::make_shared<std::vector<TFullJoinPair>>();


			using TempValue = std::pair< std::shared_ptr<std::vector<TValue1>>, std::shared_ptr<std::vector<TValue2>>>;
			std::map<TKey, TempValue> temp;

			//Fun = std::function<void(TempValue&, std::shared_ptr<std::vector<TValue>>)>
			auto append_map = [&temp]<typename TValue, typename Fun>(std::multimap<TKey, TValue> cnmap, Fun&& callback) {
				auto lower = cnmap.begin();
				while (lower != cnmap.end()) {
					auto key = lower->first;
					auto upper = cnmap.upper_bound(key);

					auto outers = std::make_shared<std::vector<TValue>>();
					for (auto it = lower; it != upper; it++) {
						outers->push_back(it->second);
					}
					if (!temp.contains(key)) {
						temp[key] = std::make_pair(std::make_shared<std::vector<TValue1>>(), std::make_shared<std::vector<TValue2>>());
					}
					auto& zp = temp[key];
					callback(zp, outers);
					lower = upper;
				}
			};

			append_map(map1, [](TempValue& zp, std::shared_ptr<std::vector<TValue1>> linq) {
				zp.first = linq;
				});

			append_map(map2, [](TempValue& zp, std::shared_ptr<std::vector<TValue2>> linq) {
				zp.second = linq;
				});

			for (auto& it : temp) {
				auto value = it.second;

				if (value.first->empty()) {
					result->push_back(TFullJoinPair({ it.first,{from_empty<TValue1>(), from_values<TValue2>(value.second)} }));
				}
				else {
					if (value.second->empty()) {
						result->push_back(TFullJoinPair({ it.first,{from_values(value.first), from_empty<TValue2>()} }));
					}
					else {
						result->push_back(TFullJoinPair({ it.first,{from_values(value.first), from_values(value.second)} }));
					}
				}
			}

			//auto lower1 = map1.begin();
			//auto lower2 = map2.begin();
			/*while (lower1 != map1.end() || lower2 != map2.end()) {
				auto key1 = lower1->first;
				auto key2 = lower2->first;
				auto upper1 = map1.upper_bound(key1);
				auto upper2 = map2.upper_bound(key2);
				if (key1 < key2) {
					auto outers = std::make_shared<std::vector<TValue1>>();
					for (auto it = lower1; it != upper1; it++) {
						outers->push_back(it->second);
					}
					result->push_back(TFullJoinPair({ key1, {from_values(outers), from_empty<TValue2>()} }));
					lower1 = upper1;
				}
				else if (key1 > key2) {
					auto inners = std::make_shared<std::vector<TValue2>>();
					for (auto it = lower2; it != upper2; it++) {
						inners->push_back(it->second);
					}
					result->push_back(TFullJoinPair({ key2, {from_empty<TValue1>(), from_values(inners)} }));
					lower2 = upper2;
				}
				else {
					auto outers = std::make_shared<std::vector<TValue1>>();
					for (auto it = lower1; it != upper1; it++) {
						outers->push_back(it->second);
					}
					auto inners = std::make_shared<std::vector<TValue2>>();
					for (auto it = lower2; it != upper2; it++) {
						inners->push_back(it->second);
					}
					result->push_back(TFullJoinPair({ key1, {from_values(outers), from_values(inners)} }));
					lower2 = upper2;
					lower1 = upper1;
				}
			}*/
			return from_values(result);
		}
	public:
		SUPPORT_STL_CONTAINERS_EX(full_join, PROTECT_PARAMETERS(typename TFunction1, typename TFunction2),
			PROTECT_PARAMETERS(const TFunction1& keySelector1, const TFunction2& keySelector2),
			PROTECT_PARAMETERS(keySelector1, keySelector2))
	private:
		//same as left_join_
		template <typename TIterator2, typename TFunction1, typename TFunction2>
		auto group_join_(const linq_enumerable<TIterator2>& e, const TFunction1& keySelector1, const TFunction2& keySelector2) const
			->linq<join_pair<typename std::remove_reference<decltype(keySelector1(*(TElement*)0))>::type,
			std::remove_cv_t<std::remove_reference_t<iterator_type<TIterator>>>,
			linq< std::remove_cv_t<std::remove_reference_t<iterator_type<TIterator2>>>>>> {
			using TKey = std::remove_reference_t<decltype(keySelector1(*(TElement*)0))>;
			using TValue1 = std::remove_cv_t< std::remove_reference_t<iterator_type<TIterator>>>;
			using TValue2 = std::remove_cv_t< std::remove_reference_t<iterator_type<TIterator2>>>;
			using TFullJoinPair = join_pair<TKey, linq<TValue1>, linq<TValue2>>;
			using TGroupJoinPair = join_pair<TKey, TValue1, linq<TValue2>>;

			auto f = full_join(e, keySelector1, keySelector2);
			auto g = f.select_many([](const TFullJoinPair& item) -> linq<TGroupJoinPair> {
				linq<TValue1> outers = item.second.first;
				return outers.select([item](const TValue1& outer) -> TGroupJoinPair {
					return TGroupJoinPair({ item.first, {outer, item.second.second} });
					});
				});
			return g;
		}
	public:
		SUPPORT_STL_CONTAINERS_EX(group_join, PROTECT_PARAMETERS(typename TFunction1, typename TFunction2),
			PROTECT_PARAMETERS(const TFunction1& keySelector1, const TFunction2& keySelector2),
			PROTECT_PARAMETERS(keySelector1, keySelector2))
	private:
		//same as group_join_
		template <typename TIterator2, typename TFunction1, typename TFunction2>
		auto left_join_(const linq_enumerable<TIterator2>& e, const TFunction1& keySelector1, const TFunction2& keySelector2) const
			->linq<join_pair<typename std::remove_reference<decltype(keySelector1(*(TElement*)0))>::type,
			std::remove_cv_t<std::remove_reference_t<iterator_type<TIterator>>>,
			linq< std::remove_cv_t<std::remove_reference_t<iterator_type<TIterator2>>>>>> {
			using TKey = std::remove_reference_t<decltype(keySelector1(*(TElement*)0))>;
			using TValue1 = std::remove_cv_t< std::remove_reference_t<iterator_type<TIterator>>>;
			using TValue2 = std::remove_cv_t< std::remove_reference_t<iterator_type<TIterator2>>>;
			using TFullJoinPair = join_pair<TKey, linq<TValue1>, linq<TValue2>>;
			using TGroupJoinPair = join_pair<TKey, TValue1, linq<TValue2>>;

			auto f = full_join(e, keySelector1, keySelector2);
			auto g = f.select_many([](const TFullJoinPair& item) -> linq<TGroupJoinPair> {
				linq<TValue1> outers = item.second.first;
				return outers.select([item](const TValue1& outer) -> TGroupJoinPair {
					return TGroupJoinPair({ item.first, {outer, item.second.second} });
					});
				});
			return g;
		}
	public:
		SUPPORT_STL_CONTAINERS_EX(left_join, PROTECT_PARAMETERS(typename TFunction1, typename TFunction2),
			PROTECT_PARAMETERS(const TFunction1& keySelector1, const TFunction2& keySelector2),
			PROTECT_PARAMETERS(keySelector1, keySelector2))

	private:
		template <typename TIterator2, typename TFunction1, typename TFunction2>
		auto right_join_(const linq_enumerable<TIterator2>& e, const TFunction1& keySelector1, const TFunction2& keySelector2) const
			->linq<join_pair<typename std::remove_reference<decltype(keySelector1(*(TElement*)0))>::type,
			linq< std::remove_cv_t<std::remove_reference_t<iterator_type<TIterator>>>>,
			std::remove_cv_t<std::remove_reference_t<iterator_type<TIterator2>>>>> {
			using TKey = std::remove_reference_t<decltype(keySelector1(*(TElement*)0))>;
			using TValue1 = std::remove_cv_t< std::remove_reference_t<iterator_type<TIterator>>>;
			using TValue2 = std::remove_cv_t< std::remove_reference_t<iterator_type<TIterator2>>>;
			using TFullJoinPair = join_pair<TKey, linq<TValue1>, linq<TValue2>>;
			using TGroupJoinPair = join_pair<TKey, linq<TValue1>, TValue2>;

			auto f = full_join(e, keySelector1, keySelector2);
			auto g = f.select_many([](const TFullJoinPair& item) -> linq<TGroupJoinPair> {
				linq<TValue2> inners = item.second.second;
				return inners.select([item](const TValue2& inner) -> TGroupJoinPair {
					return TGroupJoinPair({ item.first, {item.second.first, inner} });
					});
				});
			return g;
		}
	public:
		SUPPORT_STL_CONTAINERS_EX(right_join, PROTECT_PARAMETERS(typename TFunction1, typename TFunction2),
			PROTECT_PARAMETERS(const TFunction1& keySelector1, const TFunction2& keySelector2),
			PROTECT_PARAMETERS(keySelector1, keySelector2))
	private:
		template <typename TIterator2, typename TFunction1, typename TFunction2>
		auto join_(const linq_enumerable<TIterator2>& e, const TFunction1& keySelector1, const TFunction2& keySelector2) const
			->linq<join_pair< std::remove_reference_t<decltype(keySelector1(*(TElement*)0))>,
			std::remove_cv_t< std::remove_reference_t<iterator_type<TIterator>>>,
			std::remove_cv_t< std::remove_reference_t<iterator_type<TIterator2>>>>> {
			using TKey = std::remove_reference_t<decltype(keySelector1(*(TElement*)0))>;
			using TValue1 = std::remove_cv_t< std::remove_reference_t<iterator_type<TIterator>>>;
			using TValue2 = std::remove_cv_t< std::remove_reference_t<iterator_type<TIterator2>>>;
			using TGroupJoinPair = join_pair<TKey, TValue1, linq<TValue2>>;
			using TJoinPair = join_pair<TKey, TValue1, TValue2>;

			auto g = group_join(e, keySelector1, keySelector2);
			auto j = g.select_many([](const TGroupJoinPair& item) -> linq<TJoinPair> {
				linq<TValue2> inners = item.second.second;
				return inners.select([item](const TValue2& inner) -> TJoinPair {
					return TJoinPair({ item.first, {item.second.first, inner} });
					});
				});
			return j;
		}
	public:
		SUPPORT_STL_CONTAINERS_EX(join, PROTECT_PARAMETERS(typename TFunction1, typename TFunction2),
			PROTECT_PARAMETERS(const TFunction1& keySelector1, const TFunction2& keySelector2),
			PROTECT_PARAMETERS(keySelector1, keySelector2))

			template <typename TFunction>
		auto first_order_by(const TFunction& keySelector) const -> linq<linq<TElement>> {
			using TKey = std::remove_reference_t<decltype(keySelector(*(TElement*)0))>;
			return group_by(keySelector).select([](const zip_pair<TKey, linq<TElement>>& p) { return p.second; });
		}

		template <typename TFunction>
		auto then_order_by(const TFunction& keySelector) const -> linq<iterator_type<TIterator>> {
			return select_many([keySelector](const TElement& values) { return values.first_order_by(keySelector); });
		}

		template <typename TFunction>
		auto order_by(const TFunction& keySelector) const -> linq<TElement> {
			return first_order_by(keySelector).select_many([](const linq<TElement>& values) { return values; });
		}

	private:
		template <typename TIterator2>
		linq_enumerable<types::zip_it<TIterator, TIterator2>> zip_with_(const linq_enumerable<TIterator2>& e) const {
			using zip_it = types::zip_it<TIterator, TIterator2>;
			return linq_enumerable<zip_it>{zip_it{ _begin, _end, e.begin(), e.end() },
				zip_it{ _end, _end, e.end(), e.end() }};
		}
	public:
		SUPPORT_STL_CONTAINERS(zip_with)

			//////////////////////////////////////////////////////////////////
			// containers
			//////////////////////////////////////////////////////////////////

			std::vector<TElement> to_vector() const {
			std::vector<TElement> container;
			for (auto it = _begin; it != _end; it++) {
				container.push_back(*it);
			}
			return container;
		}

		std::list<TElement> to_list() const {
			std::list<TElement> container;
			for (auto it = _begin; it != _end; it++) {
				container.push_back(*it);
			}
			return container;
		}

		std::deque<TElement> to_deque() const {
			std::deque<TElement> container;
			for (auto it = _begin; it != _end; it++) {
				container.push_back(*it);
			}
			return container;
		}

		template <typename TFunction>
		auto to_map(const TFunction& keySelector) const -> std::map<decltype(keySelector(*(TElement*)0)), TElement> {
			std::map<decltype(keySelector(*(TElement*)0)), TElement> container;
			for (auto it = _begin; it != _end; it++) {
				container.insert(std::make_pair(keySelector(*it), *it));
			}
			return container;
		}

		template <typename TFunction>
		auto to_multimap(const TFunction& keySelector) const -> std::multimap<decltype(keySelector(*(TElement*)0)), TElement> {
			std::multimap<decltype(keySelector(*(TElement*)0)), TElement> container;
			for (auto it = _begin; it != _end; it++) {
				container.insert(std::make_pair(keySelector(*it), *it));
			}
			return container;
		}

		template <typename TFunction>
		auto to_unordered_map(const TFunction& keySelector) const -> std::unordered_map<decltype(keySelector(*(TElement*)0)), TElement> {
			std::unordered_map<decltype(keySelector(*(TElement*)0)), TElement> container;
			for (auto it = _begin; it != _end; it++) {
				container.insert(std::make_pair(keySelector(*it), *it));
			}
			return container;
		}

		std::set<TElement> to_set() const {
			std::set<TElement> container;
			for (auto it = _begin; it != _end; it++) {
				container.insert(*it);
			}
			return container;
		}

		std::multiset<TElement> to_multiset() const {
			std::multiset<TElement> container;
			for (auto it = _begin; it != _end; it++) {
				container.insert(*it);
			}
			return container;
		}

		std::unordered_set<TElement> to_unordered_set() const {
			std::unordered_set<TElement> container;
			for (auto it = _begin; it != _end; it++) {
				container.insert(*it);
			}
			return container;
		}

#undef SUPPORT_STL_CONTAINERS
#undef PROTECT_PARAMETERS
#undef SUPPORT_STL_CONTAINERS_EX
	};

	template <typename T>
	class linq : public linq_enumerable<iterators::hide_type_iterator<T>> {
	public:
		linq() {}

		template <typename TIterator>
		linq(const linq_enumerable<TIterator>& e)
			: linq_enumerable<iterators::hide_type_iterator<T>>(iterators::hide_type_iterator<T>(e.begin()),
				iterators::hide_type_iterator<T>(e.end())) {}
	};

	template <typename T>
	static linq<T> flatten(const linq<linq<T>>& xs) {
		return xs.select_many([](const linq<T>& ys) -> linq<T> { return ys; });
	}

	template <typename TIterator>
		requires linq_extract::is_iterator<TIterator> linq_enumerable<TIterator> from(const TIterator& begin, const TIterator& end) {
		return linq_enumerable<TIterator>{begin, end};
	}

	template <typename TContainer>
		requires linq_extract::is_container<TContainer>
	auto from(const TContainer& container) -> linq_enumerable<decltype(std::begin(container))> {
		return linq_enumerable<decltype(std::begin(container))>{std::begin(container), std::end(container)};
	}
} // namespace vczh
