#pragma once

#include "ListFwd.h"
#include "Modulo.h"
#include "Rational.h"

#include <type_traits>

namespace Groebner {
    namespace Details {
        using SupportedFields = List<Rational>;

        template <typename T>
        constexpr inline bool IsSupportedFieldV = IsInList<T, SupportedFields>;

        template <size_t N>
        constexpr inline bool IsSupportedFieldV<Modulo<N>> = IsPrime(N);
    }

    template <typename T>
    concept IsSupportedField = Details::IsSupportedFieldV<T>;
}