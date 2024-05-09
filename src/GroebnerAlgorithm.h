#pragma once

#include "PolySystem.h"
#include "Printer.h"

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
                Printer::Instance().PrintMessage(
                    "Trying to build Groebner basis for empty system. "
                    "Exiting.",
                    Printer::CONDITIONS, Printer::NEW_LINE);
                return;
            }
            Printer::Instance()
                .PrintMessage("Building Groebner basis for system:",
                              Printer::CONDITIONS, Printer::NEW_LINE)
                .PrintPolySystem(poly_system, Printer::CONDITIONS,
                                 Printer::DOUBLE_NEW_LINE);

            for (size_t i = 0; i < poly_system.GetSize(); ++i) {
                if (poly_system[i].IsZero()) {
                    continue;
                }
                AddRemindersToPolyAtPos(i, poly_system);
            }

            Printer::Instance()
                .PrintMessage("Basis:", Printer::CONDITIONS, Printer::NEW_LINE)
                .PrintPolySystem(poly_system, Printer::CONDITIONS,
                                 Printer::DOUBLE_NEW_LINE);

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
            // TODO add poly_system.reduce()

            Printer::Instance()
                .PrintMessage("Reducing basis", Printer::CONDITIONS,
                              Printer::NEW_LINE)
                .PrintPolySystem(basis, Printer::CONDITIONS,
                                 Printer::DOUBLE_NEW_LINE);

            PolySystem<Field, Comparator> temp;
            for (size_t i = 0; i < basis.GetSize(); i++) {
                if (!CanEraseFromBasisAtPos(basis, i)) {
                    Printer::Instance().PrintPolyStays(basis[i], i,
                                                       Printer::DETAILS);
                    temp.Add(basis[i]);
                } else {
                    Printer::Instance().PrintPolyErased(basis[i], i,
                                                        Printer::DETAILS);
                }
            }

            Printer::Instance()
                .PrintMessage("", Printer::DETAILS, Printer::NEW_LINE)
                .PrintMessage("Current basis", Printer::DETAILS,
                              Printer::NEW_LINE)
                .PrintPolySystem(temp, Printer::DETAILS,
                                 Printer::DOUBLE_NEW_LINE);

            basis = PolySystem<Field, Comparator>();
            for (size_t i = 0; i < temp.GetSize(); i++) {
                const Polynomial<Field, Comparator> cur = temp.SwapAndPop(i);
                auto reduced = ReducePolynomial(cur, temp);
                Printer::Instance().PrintPolyReplaced(cur, reduced, i,
                                                      Printer::DETAILS,
                                                      Printer::DOUBLE_NEW_LINE);
                basis.Add(std::move(reduced));
                temp.AddAndSwap(i, cur);
            }

            for (size_t i = 0; i < basis.GetSize(); i++) {
                basis[i].ReduceByLeaderCoef();
            }

            Printer::Instance()
                .PrintMessage("Reduced basis", Printer::CONDITIONS,
                              Printer::NEW_LINE)
                .PrintPolySystem(basis, Printer::CONDITIONS,
                                 Printer::DOUBLE_NEW_LINE);
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

            PrinterBuffer<Field, Comparator>::Instance().SetBuffer(2);
            PrinterBuffer<Field, Comparator>::Instance()[0] +=
                (lcm / lhs.GetLeader());
            PrinterBuffer<Field, Comparator>::Instance()[1] +=
                (lcm / rhs.GetLeader());

            return SPolyInfo(std::move(spoly), std::move(common_degree));
        }

        template <IsSupportedField Field, IsComparator Comparator>
        static Polynomial<Field, Comparator> ReducePolynomial(
            Polynomial<Field, Comparator>&& poly,
            const PolySystem<Field, Comparator>& poly_system) {

            Printer::Instance().PrintReducePolynomial(
                poly, poly_system, Printer::DETAILS, Printer::NEW_LINE);
            PrinterBuffer<Field, Comparator>::Instance().SetBuffer(
                poly_system.GetSize());

            Polynomial<Field, Comparator> rem;
            auto copy = poly;
            while (!poly.IsZero()) {
                if (!DividePoly(poly, poly_system)) {
                    rem += poly.GetLeader();
                    poly -= poly.GetLeader();
                }
            }

            Printer::Instance().PrintRemainder(
                copy, poly_system, rem, Printer::DETAILS, Printer::NEW_LINE);
            return rem;
        }

        template <IsSupportedField Field, IsComparator Comparator>
        static Polynomial<Field, Comparator> ReducePolynomial(
            const Polynomial<Field, Comparator>& poly,
            const PolySystem<Field, Comparator>& poly_system) {
            Polynomial<Field, Comparator> temp = poly;
            return ReducePolynomial(std::move(temp), poly_system);
        }

        template <IsSupportedField Field, IsComparator Comparator>
        static bool IsInIdeal(
            const Polynomial<Field, Comparator>& poly,
            const PolySystem<Field, Comparator>& poly_system) {
            auto basis = BuildGB(poly_system);

            Printer::Instance()
                .PrintMessage("Checking if ", Printer::CONDITIONS,
                              Printer::NO_NEW_LINE)
                .PrintPolynomial(poly, Printer::CONDITIONS,
                                 Printer::NO_NEW_LINE)
                .PrintMessage(" belongs to ideal", Printer::CONDITIONS,
                              Printer::DOUBLE_NEW_LINE);

            auto rem = ReducePolynomial(poly, basis);

            if (rem.IsZero()) {
                Printer::Instance()
                    .PrintMessage("Polynomial is reduced to zero",
                                  Printer::CONDITIONS, Printer::NEW_LINE)
                    .PrintMessage("Polynomial belongs to ideal",
                                  Printer::CONDITIONS,
                                  Printer::DOUBLE_NEW_LINE);
                return true;
            }

            Printer::Instance()
                .PrintMessage("Polynomial is reduced to non-zero polynomial",
                              Printer::CONDITIONS, Printer::NEW_LINE)
                .PrintMessage("Polynomial does not belong to ideal",
                              Printer::CONDITIONS, Printer::DOUBLE_NEW_LINE);
            return false;
        }

        template <IsSupportedField Field, IsComparator Comparator>
        static bool AreEqualIdeals(const PolySystem<Field, Comparator>& lhs,
                                  const PolySystem<Field, Comparator>& rhs) {
            auto basis_lhs = BuildGB(lhs, AutoReduction::Enabled);
            auto basis_rhs = BuildGB(rhs, AutoReduction::Enabled);

            Printer::Instance()
                .PrintMessage("First ideal's reduced Groebner basis:",
                              Printer::CONDITIONS, Printer::NEW_LINE)
                .PrintPolySystem(basis_lhs, Printer::CONDITIONS,
                                 Printer::NEW_LINE)
                .PrintMessage("Second ideals' reduced Groebner basis:",
                              Printer::CONDITIONS, Printer::NEW_LINE)
                .PrintPolySystem(basis_rhs, Printer::CONDITIONS,
                                 Printer::NEW_LINE);

            if (AreEqualReducedSystems(basis_lhs, basis_rhs)) {
                Printer::Instance().PrintMessage("Basises are equal, so ideals are equal",
                                                 Printer::CONDITIONS,
                                                 Printer::DOUBLE_NEW_LINE);
                return true;
            }
            
            Printer::Instance().PrintMessage("Basises are not equal, so ideals are not equal",
                                             Printer::CONDITIONS, 
                                             Printer::DOUBLE_NEW_LINE);
            return false;
            
        }

    private:
        template <IsSupportedField Field, IsComparator Comparator>
        static void AddRemindersToPolyAtPos(
            size_t pos, PolySystem<Field, Comparator>& poly_system) {
            auto leader = poly_system[pos].GetLeader();
            for (size_t j = 0; j < pos; j++) {
                auto other_leader = poly_system[j].GetLeader();
                Printer::Instance().PrintBuildingSPoly(
                    pos, j, Printer::CONDITIONS, Printer::NEW_LINE);
                SPolyInfo info = SPolynomial(poly_system[pos], poly_system[j]);

                if (leader.degree + other_leader.degree == info.common_degree) {
                    Printer::Instance().SkipSPolynomial(
                        poly_system, pos, j, Printer::CONDITIONS,
                        Printer::DOUBLE_NEW_LINE);
                    continue;
                }

                Printer::Instance().PrintSPolynomial(info.s_poly, poly_system,
                                                     pos, j, Printer::DETAILS,
                                                     Printer::NEW_LINE);

                auto remainder =
                    ReducePolynomial(std::move(info.s_poly), poly_system);

                if (!remainder.IsZero()) {
                    remainder.ReduceByLeaderCoef();
                    Printer::Instance().PrintAddToSystem(
                        remainder, poly_system.GetSize(), Printer::CONDITIONS,
                        Printer::DOUBLE_NEW_LINE);
                    poly_system.Add(std::move(remainder));
                } else {
                    Printer::Instance().PrintMessage(
                        "S-Polynomial reduced to zero", Printer::CONDITIONS,
                        Printer::DOUBLE_NEW_LINE);
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
                    auto temp = poly.GetLeader() / poly_system[i].GetLeader();
                    PrinterBuffer<Field, Comparator>::Instance()[i] += temp;
                    poly -= poly_system[i] * temp;
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
                if ((pos < j && degree.IsDivisible(other_degree)) ||
                    (pos > j && degree != other_degree &&
                     degree.IsDivisible(other_degree))) {
                    Printer::Instance().PrintPolyInBasisReduced(
                        basis, pos, j, Printer::DETAILS, Printer::NEW_LINE);
                    return true;
                }
            }
            return false;
        }

        template <IsSupportedField Field, IsComparator Comparator>
        static bool AreEqualReducedSystems(
            const PolySystem<Field, Comparator>& lhs,
            const PolySystem<Field, Comparator>& rhs) {

            if (lhs.GetSize() != rhs.GetSize()) {
                return false;
            }

            for (size_t i = 0; i < lhs.GetSize(); i++) {
                if (!ContainsSystem(lhs[i], rhs)) {
                    return false;
                }
            }

            return true;
        }

        template <IsSupportedField Field, IsComparator Comparator>
        static bool ContainsSystem(
            const Polynomial<Field, Comparator>& poly,
            const PolySystem<Field, Comparator>& poly_system) {
            for (size_t i = 0; i < poly_system.GetSize(); i++) {
                if (poly == poly_system[i]) {
                    return true;
                }
            }

            return false;
        }
};

}  // namespace Groebner
