#pragma once

#include "FieldFwd.h"
#include "Monomial.h"

class StraightOrder {
public:
    StraightOrder() = delete;

public:
    template<IsField Field>
    static bool IsLess(const Monomial<Field> &lhs, const Monomial<Field> &rhs);

    template<IsField Field>
    static bool IsGreater(const Monomial<Field> &lhs, const Monomial<Field> &rhs);

    template<IsField Field>
    static bool IsEqual(const Monomial<Field> &lhs, const Monomial<Field> &rhs);

    template<IsField Field>
    static bool IsLessOrEqual(const Monomial<Field> &lhs, const Monomial<Field> &rhs);

    template<IsField Field>
    static bool IsGreaterOrEqual(const Monomial<Field> &lhs, const Monomial<Field> &rhs);
};

template<IsField Field>
bool StraightOrder::IsLess(const Monomial<Field> &lhs, const Monomial<Field> &rhs) {
    assert(lhs.GetSize() == rhs.GetSize() && "Can't compare monomials with different set of variables");

    const MonomialDegree &lhs_degree = lhs.GetDegree();
    const MonomialDegree &rhs_degree = rhs.GetDegree();

    for (MonomialDegree::SizeType i = 0; i < lhs.GetSize(); ++i) {
        if (lhs_degree[i] >= rhs_degree[i]) {
            return false;
        }
    }

    return true;
}

template<IsField Field>
bool StraightOrder::IsGreater(const Monomial<Field> &lhs, const Monomial<Field> &rhs) {
    assert(lhs.GetSize() == rhs.GetDegree() && "Can't compare monomials with different set of variables");
    return IsLess(rhs, lhs);
}

template<IsField Field>
bool StraightOrder::IsEqual(const Monomial<Field> &lhs, const Monomial<Field> &rhs) {
    assert(lhs.GetSize() == rhs.GetSize() && "Can't compare monomials with different set of variables");

    const MonomialDegree &lhs_degree = lhs.GetDegree();
    const MonomialDegree &rhs_degree = rhs.GetDegree();

    for (MonomialDegree::SizeType i = 0; i < lhs.GetSize(); ++i) {
        if (lhs_degree[i] != rhs_degree[i]) {
            return false;
        }
    }

    return false;
}

template<IsField Field>
bool StraightOrder::IsLessOrEqual(const Monomial<Field> &lhs, const Monomial<Field> &rhs) {
    assert(lhs.GetSize() == rhs.GetSize() && "Can't compare monomials with different set of variables");

    const MonomialDegree &lhs_degree = lhs.GetDegree();
    const MonomialDegree &rhs_degree = rhs.GetDegree();

    for (MonomialDegree::SizeType i = 0; i < lhs.GetSize(); ++i) {
        if (lhs_degree[i] > rhs_degree[i]) {
            return false;
        }
    }

    return true;
}

template<IsField Field>
bool StraightOrder::IsGreaterOrEqual(const Monomial<Field> &lhs, const Monomial<Field> &rhs) {
    assert(lhs.GetSize() == rhs.GetSize() && "Can't compare monomials with different set of variables");
    return IsLessOrEqual(rhs, lhs);
}

class LexOrder : public StraightOrder {
public:
    LexOrder() = delete;

public:
    template<IsField Field>
    static bool IsLess(const Monomial<Field> &lhs, const Monomial<Field> &rhs);

    template<IsField Field>
    static bool IsGreater(const Monomial<Field> &lhs, const Monomial<Field> &rhs);

    template<IsField Field>
    static bool IsLessOrEqual(const Monomial<Field> &lhs, const Monomial<Field> &rhs);

    template<IsField Field>
    static bool IsGreaterOrEqual(const Monomial<Field> &lhs, const Monomial<Field> &rhs);
};


template<IsField Field>
bool LexOrder::IsLess(const Monomial<Field> &lhs, const Monomial<Field> &rhs) {
    assert(lhs.GetSize() == rhs.GetSize() && "Can't compare monomials with different set of variables");

    const MonomialDegree &lhs_degree = lhs.GetDegree();
    const MonomialDegree &rhs_degree = rhs.GetDegree();

    for (MonomialDegree::SizeType i = 0; i < lhs.GetSize(); ++i) {
        if (lhs_degree[i] < rhs_degree[i]) {
            return true;
        } else if (lhs_degree[i] > rhs_degree[i]) {
            return false;
        }
    }

    return false;
}

template<IsField Field>
bool LexOrder::IsGreater(const Monomial<Field> &lhs, const Monomial<Field> &rhs) {
    assert(lhs.GetSize() == rhs.GetSize() && "Can't compare monomials with different set of variables");
    return IsLess(rhs, lhs);
}

template<IsField Field>
bool LexOrder::IsLessOrEqual(const Monomial<Field> &lhs, const Monomial<Field> &rhs) {
    assert(lhs.GetSize() == rhs.GetSize() && "Can't compare monomials with different set of variables");

    const MonomialDegree &lhs_degree = lhs.GetDegree();
    const MonomialDegree &rhs_degree = rhs.GetDegree();

    for (MonomialDegree::SizeType i = 0; i < lhs.GetSize(); ++i) {
        if (lhs_degree[i] < rhs_degree[i]) {
            return true;
        } else if (lhs_degree[i] > rhs_degree[i]) {
            return false;
        }
    }

    return true;
}

template<IsField Field>
bool LexOrder::IsGreaterOrEqual(const Monomial<Field> &lhs, const Monomial<Field> &rhs) {
    assert(lhs.GetSize() == rhs.GetSize() && "Can't compare monomials with different set of variables");
    return IsLessOrEqual(rhs, lhs);
}

class ReverseLexOrder : public StraightOrder {
public:
    ReverseLexOrder() = delete;

public:
    template<IsField Field>
    static bool IsLess(const Monomial<Field> &lhs, const Monomial<Field> &rhs);

    template<IsField Field>
    static bool IsGreater(const Monomial<Field> &lhs, const Monomial<Field> &rhs);

    template<IsField Field>
    static bool IsLessOrEqual(const Monomial<Field> &lhs, const Monomial<Field> &rhs);

    template<IsField Field>
    static bool IsGreaterOrEqual(const Monomial<Field> &lhs, const Monomial<Field> &rhs);
};

template<IsField Field>
bool ReverseLexOrder::IsLess(const Monomial<Field> &lhs, const Monomial<Field> &rhs) {
    assert(lhs.GetSize() == rhs.GetSize() && "Can't compare monomials with different set of variables");

    const MonomialDegree &lhs_degree = lhs.GetDegree();
    const MonomialDegree &rhs_degree = rhs.GetDegree();

    MonomialDegree::SizeType size = lhs.GetSize();
    for (MonomialDegree::SizeType i = 0; i < size; ++i) {
        if (lhs_degree[size - i - 1] < rhs_degree[size - i - 1]) {
            return false;
        } else if (lhs_degree[i] > rhs_degree[i]) {
            return true;
        }
    }

    return false;
}

template<IsField Field>
bool ReverseLexOrder::IsGreater(const Monomial<Field> &lhs, const Monomial<Field> &rhs) {
    assert(lhs.GetSize() == rhs.GetSize() && "Can't compare monomials with different set of variables");
    return IsLess(rhs, lhs);
}

template<IsField Field>
bool ReverseLexOrder::IsLessOrEqual(const Monomial<Field> &lhs, const Monomial<Field> &rhs) {
    assert(lhs.GetSize() == rhs.GetSize() && "Can't compare monomials with different set of variables");

    const MonomialDegree &lhs_degree = lhs.GetDegree();
    const MonomialDegree &rhs_degree = rhs.GetDegree();

    MonomialDegree::SizeType size = lhs.GetSize();
    for (MonomialDegree::SizeType i = 0; i < size; ++i) {
        if (lhs_degree[size - i - 1] < rhs_degree[size - i - 1]) {
            return false;
        } else if (lhs_degree[i] > rhs_degree[i]) {
            return true;
        }
    }

    return true;
}

template<IsField Field>
bool ReverseLexOrder::IsGreaterOrEqual(const Monomial<Field> &lhs, const Monomial<Field> &rhs) {
    return IsLessOrEqual(rhs, lhs);
}

class GrlexOrder : public LexOrder {
public:
    GrlexOrder() = delete;

public:
    template<IsField Field>
    static bool IsLess(const Monomial<Field> &lhs, const Monomial<Field> &rhs);

    template<IsField Field>
    static bool IsGreater(const Monomial<Field> &lhs, const Monomial<Field> &rhs);

    template<IsField Field>
    static bool IsLessOrEqual(const Monomial<Field> &lhs, const Monomial<Field> &rhs);

    template<IsField Field>
    static bool IsGreaterOrEqual(const Monomial<Field> &lhs, const Monomial<Field> &rhs);
};


template<IsField Field>
bool GrlexOrder::IsLess(const Monomial<Field> &lhs, const Monomial<Field> &rhs) {
    assert(lhs.GetSize() == rhs.GetSize() && "Can't compare monomials with different set of variables");

    const MonomialDegree &lhs_degree = lhs.GetDegree();
    const MonomialDegree &rhs_degree = rhs.GetDegree();

    MonomialDegree::DegreeType lhs_sum_degree = lhs_degree.GetSumDegree();
    MonomialDegree::DegreeType rhs_sum_degree = rhs_degree.GetSumDegree();

    if (lhs_sum_degree < rhs_sum_degree) {
        return true;
    } else if (lhs_sum_degree > rhs_sum_degree) {
        return false;
    }

    return LexOrder::IsLess(lhs, rhs);
}

template<IsField Field>
bool GrlexOrder::IsGreater(const Monomial<Field> &lhs, const Monomial<Field> &rhs) {
    assert(lhs.GetSize() == rhs.GetSize() && "Can't compare monomials with different set of variables");
    return IsLess(rhs, lhs);
}

template<IsField Field>
bool GrlexOrder::IsLessOrEqual(const Monomial<Field> &lhs, const Monomial<Field> &rhs) {
    assert(lhs.GetSize() == rhs.GetSize() && "Can't compare monomials with different set of variables");

    const MonomialDegree &lhs_degree = lhs.GetDegree();
    const MonomialDegree &rhs_degree = rhs.GetDegree();

    MonomialDegree::DegreeType lhs_sum_degree = lhs_degree.GetSumDegree();
    MonomialDegree::DegreeType rhs_sum_degree = rhs_degree.GetSumDegree();

    if (lhs_sum_degree < rhs_sum_degree) {
        return true;
    } else if (lhs_sum_degree > rhs_sum_degree) {
        return false;
    }

    return LexOrder::IsLessOrEqual(lhs, rhs);
}

template<IsField Field>
bool GrlexOrder::IsGreaterOrEqual(const Monomial<Field> &lhs, const Monomial<Field> &rhs) {
    assert(lhs.GetSize() == rhs.GetSize() && "Can't compare monomials with different set of variables");
    return IsLessOrEqual(rhs, lhs);
}

class GrevlexOrder : public ReverseLexOrder {
public:
    GrevlexOrder() = delete;

public:
    template<IsField Field>
    static bool IsLess(const Monomial<Field> &lhs, const Monomial<Field> &rhs);

    template<IsField Field>
    static bool IsGreater(const Monomial<Field> &lhs, const Monomial<Field> &rhs);

    template<IsField Field>
    static bool IsLessOrEqual(const Monomial<Field> &lhs, const Monomial<Field> &rhs);

    template<IsField Field>
    static bool IsGreaterOrEqual(const Monomial<Field> &lhs, const Monomial<Field> &rhs);
};

template<IsField Field>
bool GrevlexOrder::IsLess(const Monomial<Field> &lhs, const Monomial<Field> &rhs) {
    assert(lhs.GetSize() == rhs.GetSize() && "Can't compare monomials with different set of variables");

    const MonomialDegree &lhs_degree = lhs.GetDegree();
    const MonomialDegree &rhs_degree = rhs.GetDegree();

    MonomialDegree::DegreeType lhs_sum_degree = lhs_degree.GetSumDegree();
    MonomialDegree::DegreeType rhs_sum_degree = rhs_degree.GetSumDegree();

    if (lhs_sum_degree < rhs_sum_degree) {
        return true;
    } else if (lhs_sum_degree > rhs_sum_degree) {
        return false;
    }

    return ReverseLexOrder::IsLess(lhs, rhs);
}

template<IsField Field>
bool GrevlexOrder::IsGreater(const Monomial<Field> &lhs, const Monomial<Field> &rhs) {
    assert(lhs.GetSize() == rhs.GetSize() && "Can't compare monomials with different set of variables");
    return IsLess(rhs, lhs);
}

template<IsField Field>
bool GrevlexOrder::IsLessOrEqual(const Monomial<Field> &lhs, const Monomial<Field> &rhs) {
    assert(lhs.GetSize() == rhs.GetSize() && "Can't compare monomials with different set of variables");

    const MonomialDegree &lhs_degree = lhs.GetDegree();
    const MonomialDegree &rhs_degree = rhs.GetDegree();

    MonomialDegree::DegreeType lhs_sum_degree = lhs_degree.GetSumDegree();
    MonomialDegree::DegreeType rhs_sum_degree = rhs_degree.GetSumDegree();

    if (lhs_sum_degree < rhs_sum_degree) {
        return true;
    } else if (lhs_sum_degree > rhs_sum_degree) {
        return false;
    }

    return ReverseLexOrder::IsLessOrEqual(lhs, rhs);
}

template<IsField Field>
bool GrevlexOrder::IsGreaterOrEqual(const Monomial<Field> &lhs, const Monomial<Field> &rhs) {
    return IsLessOrEqual(rhs, lhs);
}
