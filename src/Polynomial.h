#pragma once

#include "ComparatorFwd.h"
#include "FieldFwd.h"

#include <algorithm>
#include <map>

namespace Groebner {

template <IsSupportedField Field>
struct Term {
        Field coef;
        Monomial degree;

        bool operator==(const Term& other) const {
            return coef == other.coef && degree == other.degree;
        }

        bool operator!=(const Term& other) const { return !(*this == other); }
};

using RationalTerm = Term<Rational>;
template <int64_t N>
requires IsPrime<N> using ModuloTerm = Term<Modulo<N>>;

template <IsSupportedField Field, IsComparator Comparator = LexOrder>
class Polynomial {
    private:
        using LocalTerm = Term<Field>;

    public:
        Polynomial() = default;
        explicit Polynomial(std::vector<LocalTerm>&& monomials);
        Polynomial(std::initializer_list<LocalTerm> monomials);
        template <Details::IsIterator It>
        Polynomial(It begin, It end);
        // TODO add constructor from Polynomial with different comparator

        size_t GetSize() const;
        bool IsZero() const;
        LocalTerm GetLeader() const;
        // TODO add iterators
        LocalTerm GetAt(size_t i) const;
        void ReduceByLeaderCoef();

        Polynomial<Field, Comparator>& operator+=(
            const Polynomial<Field, Comparator>& other);
        Polynomial<Field, Comparator>& operator-=(
            const Polynomial<Field, Comparator>& other);
        Polynomial<Field, Comparator>& operator*=(
            const Polynomial<Field, Comparator>& other);

        Polynomial<Field, Comparator> operator+(
            const Polynomial<Field, Comparator>& other) const;
        Polynomial<Field, Comparator> operator-(
            const Polynomial<Field, Comparator>& other) const;
        Polynomial<Field, Comparator> operator*(
            const Polynomial<Field, Comparator>& other) const;

        bool operator==(const Polynomial<Field, Comparator>& other) const;
        bool operator!=(const Polynomial<Field, Comparator>& other) const;

        // TODO add == for Polynomial with different comparator
    private:
        void Reduce();

        struct Compare {
                bool operator()(const Monomial& lhs,
                                const Monomial& rhs) const {
                    return Comparator::IsGreater(lhs, rhs);
                }
        };

        using PolyTable = std::map<Monomial, Field, Compare>;
        // assuming monomials have different degrees (3x + 5x -> 8x)
        // and no monomials with coefficient 0
        PolyTable monomials_;
};

template <IsSupportedField Field, IsComparator Comparator>
Polynomial<Field, Comparator>::Polynomial(std::vector<LocalTerm>&& monomials) {
    for (auto& [coef, degree] : monomials) {
        auto it = monomials_.find(degree);
        if (it == monomials_.end()) {
            monomials_[std::move(degree)] = std::move(coef);
        } else {
            it->second += coef;
        }
    }
    Reduce();
}

template <IsSupportedField Field, IsComparator Comparator>
Polynomial<Field, Comparator>::Polynomial(
    std::initializer_list<LocalTerm> monomials) {
    for (auto& [coef, degree] : monomials) {
        auto it = monomials_.find(degree);
        if (it == monomials_.end()) {
            monomials_[degree] = coef;
        } else {
            it->second += coef;
        }
    }
    Reduce();
}

template <IsSupportedField Field, IsComparator Comparator>
template <Details::IsIterator It>
Polynomial<Field, Comparator>::Polynomial(It begin, It end) {
    for (auto cur = begin; cur != end; cur++) {
        auto& [coef, degree] = *cur;
        auto it = monomials_.find(degree);
        if (it == monomials_.end()) {
            monomials_[degree] = coef;
        } else {
            it->second += coef;
        }
    }
    Reduce();
}

template <IsSupportedField Field, IsComparator Comparator>
size_t Polynomial<Field, Comparator>::GetSize() const {
    return monomials_.size();
}

template <IsSupportedField Field, IsComparator Comparator>
bool Polynomial<Field, Comparator>::IsZero() const {
    return GetSize() == 0;
}

template <IsSupportedField Field, IsComparator Comparator>
Polynomial<Field, Comparator>::LocalTerm
Polynomial<Field, Comparator>::GetLeader() const {
    if (IsZero()) {
        return {0, 0};
    }

    auto [degree, coef] = *(monomials_.begin());
    return LocalTerm(coef, degree);
}

template <IsSupportedField Field, IsComparator Comparator>
Polynomial<Field, Comparator>::LocalTerm Polynomial<Field, Comparator>::GetAt(
    size_t i) const {
    if (i == 0 && IsZero()) {
        return {0, 0};
    }

    assert(i < GetSize() && "Out of bounds");
    auto [degree, coef] = *(std::next(monomials_.begin(), i));
    return LocalTerm(coef, degree);
}

template <IsSupportedField Field, IsComparator Comparator>
void Polynomial<Field, Comparator>::ReduceByLeaderCoef() {
    auto leader_coef = GetLeader().coef;
    for (auto& [degree, coef] : monomials_) {
        coef /= leader_coef;
    }
}

template <IsSupportedField Field, IsComparator Comparator>
Polynomial<Field, Comparator>& Polynomial<Field, Comparator>::operator+=(
    const Polynomial<Field, Comparator>& other) {
    for (auto& [degree, coef] : other.monomials_) {
        monomials_[degree] += coef;
    }
    Reduce();
    return *this;
}

template <IsSupportedField Field, IsComparator Comparator>
Polynomial<Field, Comparator>& Polynomial<Field, Comparator>::operator-=(
    const Polynomial<Field, Comparator>& other) {
    for (auto& [degree, coef] : other.monomials_) {
        monomials_[degree] -= coef;
    }
    Reduce();
    return *this;
}

template <IsSupportedField Field, IsComparator Comparator>
Polynomial<Field, Comparator>& Polynomial<Field, Comparator>::operator*=(
    const Polynomial<Field, Comparator>& other) {
    PolyTable result;
    for (auto& [rhs_degree, rhs_coef] : other.monomials_) {
        for (auto& [lhs_degree, lhs_coef] : monomials_) {
            result[rhs_degree + lhs_degree] += (rhs_coef * lhs_coef);
        }
    }
    monomials_ = std::move(result);
    Reduce();
    return *this;
}

template <IsSupportedField Field, IsComparator Comparator>
Polynomial<Field, Comparator> Polynomial<Field, Comparator>::operator+(
    const Polynomial<Field, Comparator>& other) const {
    Polynomial temp(*this);
    temp += other;
    return temp;
}

template <IsSupportedField Field, IsComparator Comparator>
Polynomial<Field, Comparator> Polynomial<Field, Comparator>::operator-(
    const Polynomial<Field, Comparator>& other) const {
    Polynomial temp(*this);
    temp -= other;
    return temp;
}

template <IsSupportedField Field, IsComparator Comparator>
Polynomial<Field, Comparator> Polynomial<Field, Comparator>::operator*(
    const Polynomial<Field, Comparator>& other) const {
    Polynomial<Field, Comparator> temp(*this);
    temp *= other;
    return temp;
}

template <IsSupportedField Field, IsComparator Comparator>
bool Polynomial<Field, Comparator>::operator==(
    const Polynomial<Field, Comparator>& other) const {
    return monomials_ == other.monomials_;
}

template <IsSupportedField Field, IsComparator Comparator>
bool Polynomial<Field, Comparator>::operator!=(
    const Polynomial<Field, Comparator>& other) const {
    return !(*this == other);
}

template <IsSupportedField Field, IsComparator Comparator>
void Polynomial<Field, Comparator>::Reduce() {
    for (auto it = monomials_.begin(); it != monomials_.end();) {
        if (it->second.IsZero()) {
            monomials_.erase(it++);
        } else {
            ++it;
        }
    }
}

}  // namespace Groebner
