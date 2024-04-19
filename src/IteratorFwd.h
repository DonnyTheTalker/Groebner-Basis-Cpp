#pragma once

#include <iterator>
#include <type_traits>

namespace Groebner::Details {
template <typename T, typename = void>
struct IsIteratorV {
        static constexpr bool value = false;
};

template <typename T>
struct IsIteratorV<
    T, typename std::enable_if<!std::is_same<
           typename std::iterator_traits<T>::value_type, void>::value>::type> {
        static constexpr bool value = true;
};

template <typename T>
concept IsIterator = IsIteratorV<T>::value;
}  // namespace Groebner::Details
