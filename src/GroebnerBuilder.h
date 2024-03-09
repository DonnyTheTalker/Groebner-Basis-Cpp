#pragma once

#include "PolySystem.h"

class GroebnerBuilder {
public:
    GroebnerBuilder() = delete;

    template<IsField Field, IsComparator Comparator = LexOrder>
    static PolySystem<Field, Comparator> Build(const PolySystem<Field, Comparator> &system, bool reduce = false);

    template<IsField Field>
    static Monomial<Field> FindLcm(const Monomial<Field> &lhs, const Monomial<Field> &rhs);

// TODO move below to private: and fix tests
    template<IsField Field, IsComparator Comparator = LexOrder>
    static PolySystem<Field, Comparator> ReduceBasis(const PolySystem<Field, Comparator> &basis);

    template<IsField Field, IsComparator Comparator = LexOrder>
    static Polynomial<Field, Comparator> SPolynomial(const Polynomial<Field, Comparator> &lhs,
                                                     const Polynomial<Field, Comparator> &rhs,
                                                     const Monomial<Field> &lcm);

    template<IsField Field, IsComparator Comparator = LexOrder>
    static Polynomial<Field, Comparator> ReducePolynomial(Polynomial<Field, Comparator> poly,
                                                          const PolySystem<Field, Comparator> &system);
};

template<IsField Field, IsComparator Comparator>
PolySystem<Field, Comparator> GroebnerBuilder::Build(const PolySystem<Field, Comparator> &system, bool reduce) {
    // TODO log this and return 0 as polynomial
    assert(!system.IsEmpty() && "Can't find basis in empty system");

    PolySystem<Field, Comparator> result(system);
    for (size_t i = 0; i < result.GetSize(); ++i) {
        for (size_t j = 0; j < i; j++) {
            const Monomial<Field> l1 = result[i].GetLeader();
            const Monomial<Field> l2 = result[j].GetLeader();
            const Monomial<Field> lcm = FindLcm(l1, l2);

            if (l1 * l2 == lcm) {
                continue;
            }

            const Polynomial<Field, Comparator> rem = ReducePolynomial(SPolynomial(result[i], result[j], lcm), result);
            if (!rem.IsZero()) {
                // TODO maybe reduce coefficient ?
                result.Add(std::move(rem));
            }
        }
    }

    return reduce ? ReduceBasis(result) : result;
}

// TODO move this to separate find lcm methods inside monomial
template<IsField Field>
Monomial<Field> GroebnerBuilder::FindLcm(const Monomial<Field> &lhs, const Monomial<Field> &rhs) {
    assert(lhs.GetSize() == rhs.GetSize() && "Number of variables must be same");

    Field coef = lhs.GetCoef() * rhs.GetCoef();
    MonomialDegree degree(lhs.GetSize());

    const MonomialDegree &lhs_degree = lhs.GetDegree();
    const MonomialDegree &rhs_degree = rhs.GetDegree();

    MonomialDegree::DegreeType sum_degree = 0;
    for (size_t i = 0; i < lhs.GetSize(); i++) {
        degree[i] = std::max(lhs_degree[i], rhs_degree[i]);
        sum_degree += degree[i];
    }

    // TODO change this
    degree.SetSumDegree(sum_degree);
    return Monomial<Field>(coef, degree);
}

template<IsField Field, IsComparator Comparator>
Polynomial<Field, Comparator>
GroebnerBuilder::SPolynomial(const Polynomial<Field, Comparator> &lhs, const Polynomial<Field, Comparator> &rhs,
                             const Monomial<Field> &lcm) {
    // TODO add poly * monom method
//    Monomial<Field> left_mod = lcm / lhs.GetLeader();
//    Polynomial<Field, Comparator> left_part = lhs * Polynomial<Field, Comparator>({left_mod});
//
//    Monomial<Field> right_mod = lcm / rhs.GetLeader();
//    Polynomial<Field, Comparator> right_part = rhs * Polynomial<Field, Comparator>({right_mod});
//
//    left_part -= right_part;
//    return left_part;
    return (lhs * Polynomial<Field, Comparator>({lcm / lhs.GetLeader()})) -
           (rhs * Polynomial<Field, Comparator>({lcm / rhs.GetLeader()}));
}

template<IsField Field, IsComparator Comparator>
Polynomial<Field, Comparator> GroebnerBuilder::ReducePolynomial(Polynomial<Field, Comparator> poly,
                                                                const PolySystem<Field, Comparator> &system) {
    Polynomial<Field, Comparator> rem;

    while (!poly.IsZero()) {
        bool divisible = false;

        for (size_t i = 0; i < system.GetSize(); ++i) {
            // TODO add IsDivisible method somewhere
            if (StraightOrder::IsGreaterOrEqual(poly.GetLeader(), system[i].GetLeader())) {
                poly -= system[i] * Polynomial<Field, Comparator>({poly.GetLeader() / system[i].GetLeader()});
                divisible = true;
                break;
            }
        }

        if (!divisible) {
            rem += Polynomial<Field, Comparator>({poly.GetLeader()});
            poly -= Polynomial<Field, Comparator>({poly.GetLeader()});
        }
    }

    return rem;
}

template<IsField Field, IsComparator Comparator>
PolySystem<Field, Comparator> GroebnerBuilder::ReduceBasis(const PolySystem<Field, Comparator> &basis) {
    // TODO add method poly /= coef
    PolySystem<Field, Comparator> temp;
    PolySystem<Field, Comparator> result;
    for (size_t i = 0; i < basis.GetSize(); i++) {
        bool can_throw_out = false;
        for (size_t j = 0; j < basis.GetSize(); j++) {
            // TODO add IsDivisible method somewhere
            if (i != j && ((i < j && StraightOrder::IsGreaterOrEqual(basis[i].GetLeader(), basis[j].GetLeader()) || (
                    i > j && StraightOrder::IsGreater(basis[i].GetLeader(), basis[j].GetLeader()))))) {
                can_throw_out = true;
                break;
            }
        }

        if (!can_throw_out) {
            temp.Add(basis[i]);
        }
    }

    for (size_t i = 0; i < temp.GetSize(); i++) {
        const Polynomial<Field, Comparator> cur = temp.SwapAndPop(i);
        result.Add(ReducePolynomial(cur, temp));
        temp.AddAndSwap(i, cur);
    }

    for (size_t i = 0; i < result.GetSize(); i++) {
        // TODO add poly /= coeff
        result[i].ReduceCoef();
    }

    return result;
}