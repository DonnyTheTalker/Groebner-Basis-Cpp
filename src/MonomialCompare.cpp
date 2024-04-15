#include "MonomialCompare.h"

namespace Groebner {

bool StraightCoordinateOrder::IsLess(const Monomial& lhs, const Monomial& rhs) {
    for (size_t i = 0; i < std::max(lhs.GetSize(), rhs.GetSize()); ++i) {
        if (lhs.GetDegree(i) >= rhs.GetDegree(i)) {
            return false;
        }
    }
    return true;
}

bool StraightCoordinateOrder::IsGreater(const Monomial& lhs,
                                        const Monomial& rhs) {
    return IsLess(rhs, lhs);
}

bool StraightCoordinateOrder::IsEqual(const Monomial& lhs,
                                      const Monomial& rhs) {
    for (size_t i = 0; i < std::max(lhs.GetSize(), rhs.GetSize()); ++i) {
        if (lhs.GetDegree(i) != rhs.GetDegree(i)) {
            return false;
        }
    }
    return true;
}

bool StraightCoordinateOrder::IsLessOrEqual(const Monomial& lhs,
                                            const Monomial& rhs) {
    for (size_t i = 0; i < std::max(lhs.GetSize(), rhs.GetSize()); ++i) {
        if (lhs.GetDegree(i) > rhs.GetDegree(i)) {
            return false;
        }
    }
    return true;
}

bool StraightCoordinateOrder::IsGreaterOrEqual(const Monomial& lhs,
                                               const Monomial& rhs) {
    return IsLessOrEqual(rhs, lhs);
}

bool LexOrder::IsLess(const Monomial& lhs, const Monomial& rhs) {
    for (size_t i = 0; i < std::max(lhs.GetSize(), rhs.GetSize()); ++i) {
        auto lhs_degree = lhs.GetDegree(i);
        auto rhs_degree = rhs.GetDegree(i);
        if (lhs_degree != rhs_degree) {
            return lhs_degree < rhs_degree;
        }
    }

    return false;
}

bool LexOrder::IsGreater(const Monomial& lhs, const Monomial& rhs) {
    return IsLess(rhs, lhs);
}

bool LexOrder::IsLessOrEqual(const Monomial& lhs, const Monomial& rhs) {
    return IsLess(lhs, rhs) || IsEqual(lhs, rhs);
}

bool LexOrder::IsGreaterOrEqual(const Monomial& lhs, const Monomial& rhs) {
    return IsLessOrEqual(rhs, lhs);
}

bool ReverseLexOrder::IsLess(const Monomial& lhs, const Monomial& rhs) {
    auto sz = std::max(lhs.GetSize(), rhs.GetSize());
    for (size_t i = 0; i < sz; ++i) {
        auto lhs_degree = lhs.GetDegree(sz - i - 1);
        auto rhs_degree = rhs.GetDegree(sz - i - 1);
        if (lhs_degree != rhs_degree) {
            return lhs_degree > rhs_degree;
        }
    }

    return false;
}

bool ReverseLexOrder::IsGreater(const Monomial& lhs, const Monomial& rhs) {
    return IsLess(rhs, lhs);
}

bool ReverseLexOrder::IsLessOrEqual(const Monomial& lhs, const Monomial& rhs) {
    return IsLess(lhs, rhs) || IsEqual(lhs, rhs);
}

bool ReverseLexOrder::IsGreaterOrEqual(const Monomial& lhs,
                                       const Monomial& rhs) {
    return IsLessOrEqual(rhs, lhs);
}

bool GrlexOrder::IsLess(const Monomial& lhs, const Monomial& rhs) {
    auto lhs_sum_degree = lhs.GetSumDegree();
    auto rhs_sum_degree = rhs.GetSumDegree();

    if (lhs_sum_degree != rhs_sum_degree) {
        return lhs_sum_degree < rhs_sum_degree;
    }

    return LexOrder::IsLess(lhs, rhs);
}

bool GrlexOrder::IsGreater(const Monomial& lhs, const Monomial& rhs) {
    return IsLess(rhs, lhs);
}

bool GrlexOrder::IsLessOrEqual(const Monomial& lhs, const Monomial& rhs) {
    return IsLess(lhs, rhs) || IsEqual(lhs, rhs);
}

bool GrlexOrder::IsGreaterOrEqual(const Monomial& lhs, const Monomial& rhs) {
    return IsLessOrEqual(rhs, lhs);
}

bool GrevlexOrder::IsLess(const Monomial& lhs, const Monomial& rhs) {
    auto lhs_sum_degree = lhs.GetSumDegree();
    auto rhs_sum_degree = rhs.GetSumDegree();

    if (lhs_sum_degree != rhs_sum_degree) {
        return lhs_sum_degree < rhs_sum_degree;
    }

    return ReverseLexOrder::IsLess(lhs, rhs);
}

bool GrevlexOrder::IsGreater(const Monomial& lhs, const Monomial& rhs) {
    return IsLess(rhs, lhs);
}

bool GrevlexOrder::IsLessOrEqual(const Monomial& lhs, const Monomial& rhs) {
    return IsLess(lhs, rhs) || IsEqual(lhs, rhs);
}

bool GrevlexOrder::IsGreaterOrEqual(const Monomial& lhs, const Monomial& rhs) {
    return IsLessOrEqual(rhs, lhs);
}
}  // namespace Groebner