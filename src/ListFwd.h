#pragma once

#include "type_traits"

namespace Groebner::Details {
template <class... Ts>
struct List {};

template <class T, class TList>
struct IsInListImpl : std::false_type {};

template <class T, class... Ts>
struct IsInListImpl<T, List<Ts...>> {
        static constexpr bool value =
            std::disjunction_v<std::is_same<T, Ts>...>;
};

template <class T, class TList>
static constexpr bool IsInList = IsInListImpl<T, TList>::value;
}  // namespace Groebner::Details