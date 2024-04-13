#pragma once

#include "Modulo.h"
#include "Rational.h"

#include <type_traits>

namespace Groebner {
    namespace GroebnerDetails {
        template <typename T>
        constexpr inline bool IsSupportedFieldV = std::is_same_v<T, Rational>;

        template <size_t N>
        constexpr inline bool IsSupportedFieldV<Modulo<N>> = IsPrime(N);
    }

    template <typename T>
    concept IsSupportedField = GroebnerDetails::IsSupportedFieldV<T>;
}