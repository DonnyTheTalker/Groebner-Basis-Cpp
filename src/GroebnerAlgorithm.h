#pragma once

#include "PolySystem.h"

namespace Groebner {

enum class AutoReduction { Enabled, Disabled };

template <IsSupportedField Field, IsComparator Comparator>
struct SPolyInfo {
        Polynomial<Field, Comparator> s_poly;
        Monomial common_degree;
};

class GroebnerAlgorithm {
    public:
        GroebnerAlgorithm() = delete;

        template <IsSupportedField Field, IsComparator Comparator = LexOrder>
        static void BuildGBInplace(
            PolySystem<Field, Comparator>& poly_system,
            AutoReduction reduction = AutoReduction::Disabled);

        template <IsSupportedField Field, IsComparator Comparator = LexOrder>
        static PolySystem<Field, Comparator> BuildGB(
            const PolySystem<Field, Comparator>& poly_system,
            AutoReduction reduction = AutoReduction::Disabled);

        // TODO rename to FindCommonDegree
        static Monomial FindLcm(const Monomial& lhs, const Monomial& rhs);

        template <IsSupportedField Field, IsComparator Comparator = LexOrder>
        static PolySystem<Field, Comparator> ReduceBasis(
            const PolySystem<Field, Comparator>& basis);

        template <IsSupportedField Field, IsComparator Comparator = LexOrder>
        static void ReduceBasisInplace(PolySystem<Field, Comparator>& basis);

        template <IsSupportedField Field, IsComparator Comparator = LexOrder>
        static SPolyInfo<Field, Comparator> SPolynomial(
            const Polynomial<Field, Comparator>& lhs,
            const Polynomial<Field, Comparator>& rhs);

        template <IsSupportedField Field, IsComparator Comparator = LexOrder>
        static Polynomial<Field, Comparator> ReducePolynomial(
            const Polynomial<Field, Comparator>& poly,
            const PolySystem<Field, Comparator>& poly_system);
        // TODO rename inplace functions
        template <IsSupportedField Field, IsComparator Comparator = LexOrder>
        static Polynomial<Field, Comparator> ReducePolynomial(
            Polynomial<Field, Comparator>&& poly,
            const PolySystem<Field, Comparator>& poly_system);

    private:
        // TODO add pos argument - optimisation
        template <IsSupportedField Field, IsComparator Comparator>
        static void AddReminders(const Polynomial<Field, Comparator>& poly,
                                 PolySystem<Field, Comparator>& poly_system);

        template <IsSupportedField Field, IsComparator Comparator>
        static bool DividePoly(
            Polynomial<Field, Comparator>& poly,
            const PolySystem<Field, Comparator>& poly_system);

        // TODO change to better name
        template <IsSupportedField Field, IsComparator Comparator>
        static bool CanEraseFromBasisAtPos(
            const PolySystem<Field, Comparator>& basis, size_t pos);
};

template <IsSupportedField Field, IsComparator Comparator>
void GroebnerAlgorithm::BuildGBInplace(
    PolySystem<Field, Comparator>& poly_system, AutoReduction reduction) {
    if (poly_system.IsEmpty()) {
        return;
    }
    for (size_t i = 0; i < poly_system.GetSize(); ++i) {
        if (poly_system[i].IsZero()) {
            continue;
        }
        AddReminders(poly_system[i], poly_system);
    }
    if (reduction == AutoReduction::Enabled) {
        ReduceBasisInplace(poly_system);
    }
}

template <IsSupportedField Field, IsComparator Comparator>
PolySystem<Field, Comparator> GroebnerAlgorithm::BuildGB(
    const PolySystem<Field, Comparator>& poly_system, AutoReduction reduction) {
    PolySystem<Field, Comparator> result(poly_system);
    BuildGBInplace(result, reduction);
    return result;
}

template <IsSupportedField Field, IsComparator Comparator>
void GroebnerAlgorithm::AddReminders(
    const Polynomial<Field, Comparator>& poly,
    PolySystem<Field, Comparator>& poly_system) {
    auto leader = poly.GetLeader();
    for (size_t j = 0; j < poly_system.GetSize(); j++) {
        auto other_leader = poly_system[j].GetLeader();
        SPolyInfo info = SPolynomial(poly, poly_system[j]);

        if (leader.degree + other_leader.degree == info.common_degree) {
            continue;
        }
        auto remainder = ReducePolynomial(std::move(info.s_poly), poly_system);
        if (!remainder.IsZero()) {
            remainder.ReduceByLeaderCoef();
            poly_system.Add(std::move(remainder));
        }
    }
}

Monomial GroebnerAlgorithm::FindLcm(const Monomial& lhs, const Monomial& rhs) {
    size_t res_size = std::max(lhs.GetSize(), rhs.GetSize());
    Monomial common_degree(res_size);
    for (size_t i = 0; i < res_size; i++) {
        common_degree.SetDegree(i,
                                std::max(lhs.GetDegree(i), rhs.GetDegree(i)));
    }
    return common_degree;
}

template <IsSupportedField Field, IsComparator Comparator>
bool GroebnerAlgorithm::DividePoly(
    Polynomial<Field, Comparator>& poly,
    const PolySystem<Field, Comparator>& poly_system) {

    for (size_t i = 0; i < poly_system.GetSize(); ++i) {
        // TODO add IsDivisible method somewhere in Monomial
        if (StraightCoordinateOrder::IsGreaterOrEqual(
                poly.GetLeader().degree, poly_system[i].GetLeader().degree)) {
            // TODO add / operator for Term
            // add checks for 0 coef
            poly -=
                poly_system[i] *
                Polynomial<Field, Comparator>(
                    {{poly.GetLeader().coef / poly_system[i].GetLeader().coef,
                      {poly.GetLeader().degree -
                       poly_system[i].GetLeader().degree}}});
            return true;
        }
    }

    return false;
}

template <IsSupportedField Field, IsComparator Comparator>
Polynomial<Field, Comparator> GroebnerAlgorithm::ReducePolynomial(
    const Polynomial<Field, Comparator>& poly,
    const PolySystem<Field, Comparator>& poly_system) {
    auto temp = poly;
    return ReducePolynomial(std::move(temp), poly_system);
}

template <IsSupportedField Field, IsComparator Comparator>
SPolyInfo<Field, Comparator> GroebnerAlgorithm::SPolynomial(
    const Polynomial<Field, Comparator>& lhs,
    const Polynomial<Field, Comparator>& rhs) {

    auto lhs_leader = lhs.GetLeader();
    auto rhs_leader = rhs.GetLeader();

    auto common_degree = FindLcm(lhs_leader.degree, rhs_leader.degree);
    // TODO add /= method for Term
    // and poly * term
    auto spoly =
        (lhs * Polynomial<Field, Comparator>(
                   {{rhs_leader.coef, {common_degree - lhs_leader.degree}}})) -
        (rhs * Polynomial<Field, Comparator>(
                   {{lhs_leader.coef, {common_degree - rhs_leader.degree}}}));

    return SPolyInfo(spoly, common_degree);
}

template <IsSupportedField Field, IsComparator Comparator>
Polynomial<Field, Comparator> GroebnerAlgorithm::ReducePolynomial(
    Polynomial<Field, Comparator>&& poly,
    const PolySystem<Field, Comparator>& poly_system) {
    Polynomial<Field, Comparator> rem;

    while (!poly.IsZero()) {
        if (!DividePoly(poly, poly_system)) {
            rem += Polynomial<Field, Comparator>({poly.GetLeader()});
            poly -= Polynomial<Field, Comparator>({poly.GetLeader()});
        }
    }

    return rem;
}

template <IsSupportedField Field, IsComparator Comparator>
bool GroebnerAlgorithm::CanEraseFromBasisAtPos(
    const PolySystem<Field, Comparator>& basis, size_t pos) {
    for (size_t j = 0; j < basis.GetSize(); j++) {
        // TODO add IsDivisible method somewhere
        if (pos != j && ((pos < j && StraightCoordinateOrder::IsGreaterOrEqual(
                                         basis[pos].GetLeader().degree,
                                         basis[j].GetLeader().degree) ||
                          (pos > j && StraightCoordinateOrder::IsGreater(
                                          basis[pos].GetLeader().degree,
                                          basis[j].GetLeader().degree))))) {
            return true;
        }
    }
    return false;
}

template <IsSupportedField Field, IsComparator Comparator>
void GroebnerAlgorithm::ReduceBasisInplace(
    PolySystem<Field, Comparator>& basis) {
    // TODO add method poly /= coef
    PolySystem<Field, Comparator> temp;
    for (size_t i = 0; i < basis.GetSize(); i++) {
        if (!CanEraseFromBasisAtPos(basis, i)) {
            temp.Add(basis[i]);
        }
    }
    basis = PolySystem<Field, Comparator>();
    for (size_t i = 0; i < temp.GetSize(); i++) {
        const Polynomial<Field, Comparator> cur = temp.SwapAndPop(i);
        basis.Add(std::move(ReducePolynomial(cur, temp)));
        temp.AddAndSwap(i, cur);
    }

    for (size_t i = 0; i < basis.GetSize(); i++) {
        // TODO add poly /= coeff
        basis[i].ReduceByLeaderCoef();
    }
}

template <IsSupportedField Field, IsComparator Comparator>
PolySystem<Field, Comparator> GroebnerAlgorithm::ReduceBasis(
    const PolySystem<Field, Comparator>& basis) {
    PolySystem<Field, Comparator> result(basis);
    ReduceBasisInplace(result);
    return result;
}

}  // namespace Groebner
