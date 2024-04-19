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

        Term<Field>& operator/=(const Term& other) {
            coef /= other.coef;
            degree -= other.degree;
            return *this;
        }

        Term<Field> operator/(const Term& other) const {
            Term temp(*this);
            temp /= other;
            return temp;
        }
};

using RationalTerm = Term<Rational>;
template <int64_t N>
requires IsPrime<N> using ModuloTerm = Term<Modulo<N>>;

template <IsSupportedField Field, IsComparator Comparator = LexOrder>
class Polynomial {
    private:
        using LocalTerm = Term<Field>;
        using LocalPoly = Polynomial<Field, Comparator>;

    public:
        Polynomial() = default;

        // TODO add constructor from Polynomial with different comparator
        explicit Polynomial(std::vector<LocalTerm>&& monomials) {
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

        Polynomial(std::initializer_list<LocalTerm> monomials) {
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

        template <Details::IsIterator It>
        Polynomial(It begin, It end) {
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

        size_t GetSize() const { return monomials_.size(); }
        bool IsZero() const { return GetSize() == 0; }
        LocalTerm GetLeader() const {
            if (IsZero()) {
                return {0, 0};
            }

            auto [degree, coef] = *(monomials_.begin());
            return LocalTerm(coef, degree);
        }
        // TODO add iterators
        LocalTerm GetAt(size_t i) const {
            if (i == 0 && IsZero()) {
                return {0, 0};
            }

            assert(i < GetSize() && "Out of bounds");
            auto [degree, coef] = *(std::next(monomials_.begin(), i));
            return LocalTerm(coef, degree);
        }

        void ReduceByLeaderCoef() {
            auto leader_coef = GetLeader().coef;
            for (auto& [degree, coef] : monomials_) {
                coef /= leader_coef;
            }
        }

        LocalPoly& operator+=(const LocalPoly& other) {
            for (auto& [degree, coef] : other.monomials_) {
                monomials_[degree] += coef;
            }
            Reduce();
            return *this;
        }

        LocalPoly& operator-=(const LocalPoly& other) {
            for (auto& [degree, coef] : other.monomials_) {
                monomials_[degree] -= coef;
            }
            Reduce();
            return *this;
        }

        LocalPoly& operator*=(const LocalPoly& other) {
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

        LocalPoly operator+(const LocalPoly& other) const {
            Polynomial temp(*this);
            temp += other;
            return temp;
        }

        LocalPoly operator-(const LocalPoly& other) const {
            Polynomial temp(*this);
            temp -= other;
            return temp;
        }

        LocalPoly operator*(const LocalPoly& other) const {
            LocalPoly temp(*this);
            temp *= other;
            return temp;
        }

        LocalPoly& operator+=(const LocalTerm& term) {
            monomials_[term.degree] += term.coef;
            if (monomials_[term.degree].IsZero()) {
                monomials_.erase(term.degree);
            }
            return *this;
        }

        LocalPoly& operator-=(const LocalTerm& term) {
            monomials_[term.degree] -= term.coef;
            if (monomials_[term.degree].IsZero()) {
                monomials_.erase(term.degree);
            }
            return *this;
        }

        LocalPoly& operator*=(const LocalTerm& term) {
            PolyTable result;
            for (auto& [degree, coef] : monomials_) {
                result[degree + term.degree] += (coef * term.coef);
            }
            monomials_ = std::move(result);
            Reduce();
            return *this;
        }

        LocalPoly operator+(const LocalTerm& term) const {
            LocalPoly poly(*this);
            poly += term;
            return poly;
        }

        LocalPoly operator-(const LocalTerm& term) const {
            LocalPoly poly(*this);
            poly -= term;
            return poly;
        }

        LocalPoly operator*(const LocalTerm& term) const {
            LocalPoly poly(*this);
            poly *= term;
            return poly;
        }

        // TODO add == for Polynomial with different comparator
        bool operator==(const LocalPoly& other) const {
            return monomials_ == other.monomials_;
        }
        bool operator!=(const LocalPoly& other) const {
            return !(*this == other);
        }

    private:
        void Reduce() {
            for (auto it = monomials_.begin(); it != monomials_.end();) {
                if (it->second.IsZero()) {
                    monomials_.erase(it++);
                } else {
                    ++it;
                }
            }
        }

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
}  // namespace Groebner
