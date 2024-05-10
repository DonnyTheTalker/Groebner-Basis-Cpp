#pragma once

#include "Monomial.h"

#include <cassert>

namespace Groebner {

class StraightCoordinateOrder {
    public:
        StraightCoordinateOrder() = delete;
        static bool IsLess(const Monomial& lhs, const Monomial& rhs);
        static bool IsGreater(const Monomial& lhs, const Monomial& rhs);
        static bool IsEqual(const Monomial& lhs, const Monomial& rhs);
        static bool IsLessOrEqual(const Monomial& lhs, const Monomial& rhs);
        static bool IsGreaterOrEqual(const Monomial& lhs, const Monomial& rhs);
};

class LexOrder : public StraightCoordinateOrder {
    public:
        LexOrder() = delete;
        static bool IsLess(const Monomial& lhs, const Monomial& rhs);
        static bool IsGreater(const Monomial& lhs, const Monomial& rhs);
        static bool IsLessOrEqual(const Monomial& lhs, const Monomial& rhs);
        static bool IsGreaterOrEqual(const Monomial& lhs, const Monomial& rhs);
};

class ReverseLexOrder : public StraightCoordinateOrder {
    public:
        ReverseLexOrder() = delete;

        static bool IsLess(const Monomial& lhs, const Monomial& rhs);
        static bool IsGreater(const Monomial& lhs, const Monomial& rhs);
        static bool IsLessOrEqual(const Monomial& lhs, const Monomial& rhs);
        static bool IsGreaterOrEqual(const Monomial& lhs, const Monomial& rhs);
};

class GrlexOrder : public LexOrder {
    public:
        GrlexOrder() = delete;

        static bool IsLess(const Monomial& lhs, const Monomial& rhs);
        static bool IsGreater(const Monomial& lhs, const Monomial& rhs);
        static bool IsLessOrEqual(const Monomial& lhs, const Monomial& rhs);
        static bool IsGreaterOrEqual(const Monomial& lhs, const Monomial& rhs);
};

class GrevlexOrder : public ReverseLexOrder {
    public:
        GrevlexOrder() = delete;

        static bool IsLess(const Monomial& lhs, const Monomial& rhs);
        static bool IsGreater(const Monomial& lhs, const Monomial& rhs);
        static bool IsLessOrEqual(const Monomial& lhs, const Monomial& rhs);
        static bool IsGreaterOrEqual(const Monomial& lhs, const Monomial& rhs);
};

}  // namespace Groebner
