#pragma once

#include "ListFwd.h"
#include "MonomialCompare.h"

#include <type_traits>

namespace Groebner {
namespace Details {
    using Comparators =
        List<LexOrder, GrlexOrder, GrevlexOrder>;
}

template <typename T>
concept IsComparator = Details::IsInList<T, Details::Comparators>;
}  // namespace Groebner
