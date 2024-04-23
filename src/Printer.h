#pragma once

#include "FieldFwd.h"
#include "PolySystem.h"

#include <fstream>
#include <iostream>
#include <string>

namespace Groebner {

namespace Details {
    template <typename T>
    struct FieldPrinter {
            static void Print(std::ofstream& out) {}
    };

    template <>
    struct FieldPrinter<Rational> {
            static void Print(std::ofstream& out) {
                out << "Working in $\\mathbb{Q}$ field. ";
            }
    };

    template <int64_t N>
    requires Groebner::IsPrime<N> struct FieldPrinter<Modulo<N>> {
            static void Print(std::ofstream& out) {
                out << "Working in $\\mathbb{Z}_" << std::to_string(N)
                    << "$ field. ";
            }
    };

    template <IsSupportedField T>
    struct CoefPrinter {
            static void Print(T coef, std::ofstream& out) {}
    };

    template <>
    struct CoefPrinter<Rational> {
            static void Print(Rational coef, std::ofstream& out) {
                if (coef.GetDenominator() == 1) {
                    out << "$" << coef.GetNumerator() << "$";
                } else {
                    out << "$\\frac{" << coef.GetNumerator() << "}{"
                        << coef.GetDenominator() << "}$";
                }
            }
    };

    template <size_t N>
    requires Groebner::IsPrime<N> struct CoefPrinter<Modulo<N>> {
            static void Print(Modulo<N> coef, std::ofstream& out) {
                out << "$" << coef.GetValue() << "$";
            }
    };
}  // namespace Details

// TODO also add Details level
template <IsSupportedField Field, IsComparator Comparator>
class PrinterBuffer {
    private:
        PrinterBuffer() = default;
        PrinterBuffer(const PrinterBuffer& other);

    public:
        static PrinterBuffer& Instance() {
            static PrinterBuffer<Field, Comparator> instance;
            return instance;
        }

        void Clear() { buffer_.clear(); }

        void SetBuffer(std::size_t sz) {
            Clear();
            buffer_.resize(sz);
        }

        Polynomial<Field, Comparator>& operator[](size_t index) {
            if (index >= buffer_.size()) {
                SetBuffer(index + 1);
            }
            return buffer_[index];
        }

    private:
        std::vector<Polynomial<Field, Comparator>> buffer_;
};

// TODO maybe print methods will return Printer&
// to make smth like Printner.PrintMessage("").PrintSystem("")
class Printer {
    private:
        Printer() = default;
        Printer(const Printer& other);

    public:
        static Printer& Instance();
        Printer& SetOutputBuffer(std::ofstream& out);

        enum DescriptionLevel {
            NONE = 0x0,
            CONDITIONS = 0x1,
            RESULTS = 0x2,
            DETAILS = 0x4,
            ALL = CONDITIONS | RESULTS | DETAILS
        };

        enum NewLinePolicy {
            NO_NEW_LINE = 0,
            NEW_LINE = 1,
            DOUBLE_NEW_LINE = 2
        };

        Printer& PrintNewLine(NewLinePolicy policy);

        Printer& SetDescriptionLevel(DescriptionLevel level);
        // TODO add enum class NewLinePolicy : NoNewLine, NewLine, DoubleNewLine
        // TODO use \newline as static const string
        Printer& PrintMessage(const std::string& message,
                              DescriptionLevel description,
                              NewLinePolicy policy = NewLinePolicy::NEW_LINE);

        template <IsSupportedField Field>
        Printer& PrintField(DescriptionLevel description,
                            NewLinePolicy policy = NEW_LINE) {
            if (description_level_ & description) {
                Details::FieldPrinter<Field>::Print(*out_);
                PrintNewLine(policy);
            }
            return *this;
        }

        Printer& PrintBuildingSPoly(size_t i, size_t j,
                                    DescriptionLevel description,
                                    NewLinePolicy policy = NEW_LINE);

        Printer& PrintDegree(const Monomial& degree,
                             NewLinePolicy policy = NEW_LINE);

        template <IsSupportedField Field, IsComparator Comparator>
        Printer& PrintPolynomial(const Polynomial<Field, Comparator>& poly,
                                 DescriptionLevel description,
                                 NewLinePolicy policy = NEW_LINE) {
            if (!(description_level_ & description)) {
                return *this;
            }
            if (poly.IsZero()) {
                *out_ << "$0$";
            }
            for (size_t i = 0; i < poly.GetSize(); i++) {
                auto [coef, degree] = poly.GetAt(i);
                if (i > 0 && coef > 0) {
                    *out_ << " + ";
                } else if (i > 0 && coef < 0) {
                    *out_ << " $-$ ";
                }
                if (DoPrintCoef(coef.Abs(), degree)) {
                    Details::CoefPrinter<Field>::Print(coef.Abs(), *out_);
                }
                PrintDegree(degree, NO_NEW_LINE);
            }

            PrintNewLine(policy);
            return *this;
        }

        template <IsSupportedField Field, IsComparator Comparator>
        Printer& PrintPolySystem(
            const PolySystem<Field, Comparator>& poly_system,
            DescriptionLevel description, NewLinePolicy policy = NEW_LINE) {
            if (!(description_level_ & description)) {
                return *this;
            }

            for (size_t i = 0; i < poly_system.GetSize(); i++) {
                *out_ << "$f_{" << i + 1 << "}$: ";
                PrintPolynomial(
                    poly_system[i], description,
                    i + 1 == poly_system.GetSize() ? NO_NEW_LINE : NEW_LINE);
            }

            PrintNewLine(policy);
            return *this;
        }

        template <IsSupportedField Field, IsComparator Comparator>
        Printer& PrintSPolynomial(
            const Polynomial<Field, Comparator>& s_poly,
            const PolySystem<Field, Comparator>& poly_system, size_t lhs_pos,
            size_t rhs_pos, DescriptionLevel description,
            NewLinePolicy policy = NEW_LINE) {
            if (!(description_level_ & description)) {
                return *this;
            }

            *out_ << "$S$ = ";
            *out_ << "$($";
            PrintPolynomial(PrinterBuffer<Field, Comparator>::Instance()[0],
                            description, NO_NEW_LINE);
            *out_ << "$)$";
            *out_ << "$f_{" << lhs_pos + 1 << "}$";
            *out_ << " $-$ " << "$($";
            PrintPolynomial(PrinterBuffer<Field, Comparator>::Instance()[1],
                            description, NO_NEW_LINE);
            *out_ << "$)$" << "$f_{" << rhs_pos + 1 << "}$" << " = ";

            *out_ << "$($";
            PrintPolynomial(PrinterBuffer<Field, Comparator>::Instance()[0],
                            description, NO_NEW_LINE);
            *out_ << "$)$";
            *out_ << "$($";
            PrintPolynomial(poly_system[lhs_pos], description, NO_NEW_LINE);
            *out_ << "$)$ ";
            *out_ << "$-$ ";

            *out_ << "$($";
            PrintPolynomial(PrinterBuffer<Field, Comparator>::Instance()[1],
                            description, NO_NEW_LINE);
            *out_ << "$)$";

            *out_ << "$($";
            PrintPolynomial(poly_system[rhs_pos], description, NO_NEW_LINE);
            *out_ << "$)$ ";
            *out_ << "= ";

            PrintPolynomial(s_poly, description, NO_NEW_LINE);
            PrintNewLine(policy);
            return *this;
        }

        template <IsSupportedField Field, IsComparator Comparator>
        Printer& SkipSPolynomial(
            const PolySystem<Field, Comparator>& poly_system, size_t lhs_pos,
            size_t rhs_pos, DescriptionLevel description,
            NewLinePolicy policy = NEW_LINE) {
            if (!(description_level_ & description)) {
                return *this;
            }
            *out_ << "Leader degree of " << "$f_{" << lhs_pos + 1 << "}$: ";
            PrintPolynomial(
                Polynomial<Field, Comparator>{
                    {poly_system[lhs_pos].GetLeader()}},
                description, NO_NEW_LINE);
            *out_ << ", and of " << "$f_{" << rhs_pos + 1 << "}$: ";
            PrintPolynomial(
                Polynomial<Field, Comparator>{
                    {poly_system[rhs_pos].GetLeader()}},
                description, NO_NEW_LINE);
            *out_ << " are coprime. Skipping them ";
            PrintNewLine(policy);
            return *this;
        }

        template <IsSupportedField Field, IsComparator Comparator>
        Printer& PrintReducePolynomial(
            const Polynomial<Field, Comparator>& poly,
            const PolySystem<Field, Comparator>& poly_system,
            DescriptionLevel decription, NewLinePolicy policy = NEW_LINE) {
            if (!(description_level_ & decription)) {
                return *this;
            }
            PrintMessage("Reducing: ", decription, NO_NEW_LINE);
            PrintPolynomial(poly, decription, NEW_LINE);

            PrintMessage("With system: ", decription, NO_NEW_LINE);
            PrintPolySystem(poly_system, decription, NO_NEW_LINE);
            PrintNewLine(policy);
            return *this;
        }

        template <IsSupportedField Field, IsComparator Comparator>
        Printer& PrintRemainder(
            const Polynomial<Field, Comparator>& poly,
            const PolySystem<Field, Comparator>& poly_system,
            const Polynomial<Field, Comparator>& rem,
            DescriptionLevel description, NewLinePolicy policy = NEW_LINE) {
            if (!(description & description_level_)) {
                return *this;
            }

            PrintPolynomial(poly, description, NO_NEW_LINE);
            *out_ << " = ";

            bool printed = false;
            for (size_t i = 0; i < poly_system.GetSize(); i++) {
                auto& g_i = PrinterBuffer<Field, Comparator>::Instance()[i];
                if (!g_i.IsZero()) {
                    if (printed) {
                        *out_ << " + ";
                    }
                    printed = true;
                    *out_ << "(";
                    PrintPolynomial(g_i, description, NO_NEW_LINE);
                    *out_ << ")";
                    *out_ << "$f_{" << i + 1 << "}$";
                }
            }

            if (!printed) {
                *out_ << " $r$";
                PrintNewLine(policy);
                return *this;
            }

            *out_ << " + $r$ = ";
            printed = false;
            for (size_t i = 0; i < poly_system.GetSize(); i++) {
                auto& g_i = PrinterBuffer<Field, Comparator>::Instance()[i];
                if (!g_i.IsZero()) {
                    if (printed) {
                        *out_ << " + ";
                    }
                    printed = true;
                    *out_ << "(";
                    PrintPolynomial(g_i, description, NO_NEW_LINE);
                    *out_ << ")";
                    *out_ << "(";
                    PrintPolynomial(poly_system[i], description, NO_NEW_LINE);
                    *out_ << ")";
                }
            }

            *out_ << " + (";
            PrintPolynomial(rem, description, NO_NEW_LINE);
            *out_ << ")";

            PrintNewLine(policy);
            return *this;
        }

        template <IsSupportedField Field, IsComparator Comparator>
        Printer& PrintAddToSystem(const Polynomial<Field, Comparator>& poly,
                                  size_t pos, DescriptionLevel description,
                                  NewLinePolicy policy = NEW_LINE) {
            if (!(description_level_ & description)) {
                return *this;
            }

            *out_ << "S-Polynomial reduced to: ";
            PrintPolynomial(poly, description, NO_NEW_LINE);
            *out_ << " = $f_{" << pos + 1 << "}$";
            PrintNewLine(policy);
            return *this;
        };

        template <IsSupportedField Field, IsComparator Comparator>
        Printer& PrintPolyStays(const Polynomial<Field, Comparator>& poly,
                                size_t pos, DescriptionLevel description,
                                NewLinePolicy policy = NEW_LINE) {
            if (!(description_level_ & description)) {
                return *this;
            }

            *out_ << "$f_{" << pos + 1 << "}$ = ";
            PrintPolynomial(poly, description, NO_NEW_LINE);
            *out_ << " stays in system";
            PrintNewLine(policy);
            return *this;
        }

        template <IsSupportedField Field, IsComparator Comparator>
        Printer& PrintPolyErased(const Polynomial<Field, Comparator>& poly,
                                 size_t pos, DescriptionLevel description,
                                 NewLinePolicy policy = NEW_LINE) {
            if (!(description_level_ & description)) {
                return *this;
            }

            *out_ << "$f_{" << pos + 1 << "}$ = ";
            PrintPolynomial(poly, description, NO_NEW_LINE);
            *out_ << " is erased from system";
            PrintNewLine(policy);
            return *this;
        }

        template <IsSupportedField Field, IsComparator Comparator>
        Printer& PrintPolyReplaced(const Polynomial<Field, Comparator>& poly,
                                   const Polynomial<Field, Comparator>& reduced,
                                   size_t pos, DescriptionLevel description,
                                   NewLinePolicy policy = NEW_LINE) {
            if (!(description_level_ & description)) {
                return *this;
            }

            *out_ << "$f_{" << pos + 1 << "}$ = ";
            PrintPolynomial(poly, description, NO_NEW_LINE);
            *out_ << " is reduced to: ";
            PrintPolynomial(reduced, description, NO_NEW_LINE);
            PrintNewLine(policy);
            return *this;
        }

        template <IsSupportedField Field, IsComparator Comparator>
        Printer& PrintPolyInBasisReduced(
            const PolySystem<Field, Comparator>& poly_system,
            size_t reduced_pos, size_t reducer_pos,
            DescriptionLevel description, NewLinePolicy policy = NEW_LINE) {
            if (!(description_level_ & description)) {
                return *this;
            }

            *out_ << "Leader of $f_{" << reduced_pos + 1 << "}$ = ";
            PrintPolynomial(Polynomial<Field, Comparator>(
                                {poly_system[reduced_pos].GetLeader()}),
                            description, NO_NEW_LINE);
            *out_ << " is divisible by leader of $f_{" << reducer_pos + 1
                  << "}$ = ";
            PrintPolynomial(Polynomial<Field, Comparator>(
                                {poly_system[reducer_pos].GetLeader()}),
                            description, NO_NEW_LINE);
            PrintNewLine(policy);
            return *this;
        }

    private:
        template <IsSupportedField Field>
        bool DoPrintCoef(Field coef, const Monomial& degree) {
            if (degree == Monomial(0)) {
                return true;
            }
            if (coef == 1) {
                return false;
            }
            if (std::is_same_v<Field, Rational> && coef == -1) {
                return false;
            }

            return true;
        }

        DescriptionLevel description_level_ = DescriptionLevel::NONE;
        std::ofstream* out_;
};

}  // namespace Groebner
