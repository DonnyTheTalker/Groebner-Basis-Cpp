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

        template <IsSupportedField Field, IsComparator Comparator>
        static void BuildGBInplace(
            PolySystem<Field, Comparator>& poly_system,
            AutoReduction reduction = AutoReduction::Disabled) {
            poly_system.Reduce();
            if (poly_system.IsEmpty()) {
                return;
            }
            for (size_t i = 0; i < poly_system.GetSize(); ++i) {
                if (poly_system[i].IsZero()) {
                    continue;
                }
                AddRemindersToPolyAtPos(i, poly_system);
            }
            if (reduction == AutoReduction::Enabled) {
                ReduceBasisInplace(poly_system);
            }
        }

        template <IsSupportedField Field, IsComparator Comparator>
        static PolySystem<Field, Comparator> BuildGB(
            const PolySystem<Field, Comparator>& poly_system,
            AutoReduction reduction = AutoReduction::Disabled) {
            PolySystem<Field, Comparator> result(poly_system);
            BuildGBInplace(result, reduction);
            return result;
        }

        static Monomial FindMinimalCommonDegree(const Monomial& lhs,
                                                const Monomial& rhs) {
            size_t res_size = std::max(lhs.GetSize(), rhs.GetSize());
            Monomial common_degree(res_size);
            for (size_t i = 0; i < res_size; i++) {
                common_degree.SetDegree(
                    i, std::max(lhs.GetDegree(i), rhs.GetDegree(i)));
            }
            return common_degree;
        }

        template <IsSupportedField Field, IsComparator Comparator>
        static void ReduceBasisInplace(PolySystem<Field, Comparator>& basis) {
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
                basis[i].ReduceByLeaderCoef();
            }
        }

        template <IsSupportedField Field, IsComparator Comparator>
        static PolySystem<Field, Comparator> ReduceBasis(
            const PolySystem<Field, Comparator>& basis) {
            PolySystem<Field, Comparator> result(basis);
            ReduceBasisInplace(result);
            return result;
        }

        template <IsSupportedField Field, IsComparator Comparator>
        static SPolyInfo<Field, Comparator> SPolynomial(
            const Polynomial<Field, Comparator>& lhs,
            const Polynomial<Field, Comparator>& rhs) {

            const auto& [lhs_coef, lhs_degree] = lhs.GetLeader();
            const auto& [rhs_coef, rhs_degree] = rhs.GetLeader();

            auto common_degree =
                FindMinimalCommonDegree(lhs_degree, rhs_degree);
            Term<Field> lcm{lhs_coef * rhs_coef, common_degree};

            auto spoly =
                lhs * (lcm / lhs.GetLeader()) - rhs * (lcm / rhs.GetLeader());
            return SPolyInfo(std::move(spoly), std::move(common_degree));
        }

        template <IsSupportedField Field, IsComparator Comparator>
        static Polynomial<Field, Comparator> ReducePolynomial(
            Polynomial<Field, Comparator>&& poly,
            const PolySystem<Field, Comparator>& poly_system) {
            Polynomial<Field, Comparator> rem;

            while (!poly.IsZero()) {
                if (!DividePoly(poly, poly_system)) {
                    rem += poly.GetLeader();
                    poly -= poly.GetLeader();
                }
            }

            return rem;
        }

        template <IsSupportedField Field, IsComparator Comparator>
        static Polynomial<Field, Comparator> ReducePolynomial(
            const Polynomial<Field, Comparator>& poly,
            const PolySystem<Field, Comparator>& poly_system) {
            Polynomial<Field, Comparator> temp = poly;
            return ReducePolynomial(std::move(temp), poly_system);
        }

        template<IsSupportedField Field, IsComparator Comparator>
        static bool InIdeal(const Polynomial<Field, Comparator>& poly,
                            const PolySystem<Field, Comparator>& poly_system) {
            auto basis = BuildGB(poly_system);
            auto rem = ReducePolynomial(poly, basis);
            return rem.IsZero();
        }

    private:
        template <IsSupportedField Field, IsComparator Comparator>
        static void AddRemindersToPolyAtPos(
            size_t pos, PolySystem<Field, Comparator>& poly_system) {
            auto leader = poly_system[pos].GetLeader();
            for (size_t j = 0; j < pos; j++) {
                auto other_leader = poly_system[j].GetLeader();
                SPolyInfo info = SPolynomial(poly_system[pos], poly_system[j]);

                if (leader.degree + other_leader.degree == info.common_degree) {
                    continue;
                }
                auto remainder =
                    ReducePolynomial(std::move(info.s_poly), poly_system);
                if (!remainder.IsZero()) {
                    remainder.ReduceByLeaderCoef();
                    poly_system.Add(std::move(remainder));
                }
            }
        }

        template <IsSupportedField Field, IsComparator Comparator>
        static bool DividePoly(
            Polynomial<Field, Comparator>& poly,
            const PolySystem<Field, Comparator>& poly_system) {
            const auto& [coef, degree] = poly.GetLeader();
            for (size_t i = 0; i < poly_system.GetSize(); ++i) {
                const auto& [other_coef, other_degree] =
                    poly_system[i].GetLeader();
                if (degree.IsDivisible(other_degree)) {
                    assert(!poly_system[i].GetLeader().coef.IsZero() &&
                           "Can't divide by zero");
                    poly -= poly_system[i] *
                            (poly.GetLeader() / poly_system[i].GetLeader());
                    return true;
                }
            }

            return false;
        }

        template <IsSupportedField Field, IsComparator Comparator>
        static bool CanEraseFromBasisAtPos(
            const PolySystem<Field, Comparator>& basis, size_t pos) {
            const auto& [coef, degree] = basis[pos].GetLeader();
            for (size_t j = 0; j < basis.GetSize(); j++) {
                const auto& [other_coef, other_degree] = basis[j].GetLeader();
                if (pos != j &&
                    ((pos < j && degree.IsDivisible(other_degree)) ||
                     (pos > j && degree != other_degree &&
                      degree.IsDivisible(other_degree)))) {
                    return true;
                }
            }
            return false;
        }
};

}  // namespace Groebner
