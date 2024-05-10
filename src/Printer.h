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

class Printer {
    private:
        Printer() = default;
        Printer(const Printer& other);

    public:
        static Printer& Instance();
        Printer& SetOutputBuffer(std::ofstream& out);
        Printer& ResetOutputBuffer();

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
        // TODO add private PrintMessage without description check
        Printer& PrintMessage(const std::string& message,
                              DescriptionLevel description,
                              NewLinePolicy policy = NewLinePolicy::NEW_LINE);

        template <IsSupportedField Field>
        Printer& PrintField(DescriptionLevel description,
                            NewLinePolicy policy = NEW_LINE) {
            if (description_level_ & description) {
                assert(out_ && "No output buffer");
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

            assert(out_ && "No output buffer");
            if (poly.IsZero()) {
                PrintMessage("$0$", description, NO_NEW_LINE);
            }
            for (size_t i = 0; i < poly.GetSize(); i++) {
                auto [coef, degree] = poly.GetAt(i);
                if (i > 0 && coef > 0) {
                    PrintMessage(" + ", description, NO_NEW_LINE);
                } else if (i > 0 && coef < 0) {
                    PrintMessage(" $-$ ", description, NO_NEW_LINE);
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

            assert(out_ && "No output buffer");
            for (size_t i = 0; i < poly_system.GetSize(); i++) {
                PrintMessage("$f_{" + std::to_string(i + 1) + "}$: ",
                             description, NO_NEW_LINE);
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

            assert(out_ && "No output buffer");

            PrintMessage("$S$ = $($", description, NO_NEW_LINE);
            PrintPolynomial(PrinterBuffer<Field, Comparator>::Instance()[0],
                            description, NO_NEW_LINE);
            PrintMessage(
                "$)$$f_{" + std::to_string(lhs_pos + 1) + "}$" + " $-$ $($",
                description, NO_NEW_LINE);
            PrintPolynomial(PrinterBuffer<Field, Comparator>::Instance()[1],
                            description, NO_NEW_LINE);
            PrintMessage("$)$$f_{" + std::to_string(rhs_pos + 1) + "}$ = $($",
                         description, NO_NEW_LINE);
            PrintPolynomial(PrinterBuffer<Field, Comparator>::Instance()[0],
                            description, NO_NEW_LINE);
            PrintMessage("$)$$($", description, NO_NEW_LINE);
            PrintPolynomial(poly_system[lhs_pos], description, NO_NEW_LINE);
            PrintMessage("$)$ $-$ $($", description, NO_NEW_LINE);
            PrintPolynomial(PrinterBuffer<Field, Comparator>::Instance()[1],
                            description, NO_NEW_LINE);
            PrintMessage("$)$$($", description, NO_NEW_LINE);
            PrintPolynomial(poly_system[rhs_pos], description, NO_NEW_LINE);
            PrintMessage("$)$ = ", description, NO_NEW_LINE);
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

            assert(out_ && "No output buffer");

            PrintMessage(
                "Leader degree of $f_{" + std::to_string(lhs_pos + 1) + "}$: ",
                description, NO_NEW_LINE);
            PrintPolynomial(
                Polynomial<Field, Comparator>{
                    {poly_system[lhs_pos].GetLeader()}},
                description, NO_NEW_LINE);
            PrintMessage(", and of $f_{" + std::to_string(rhs_pos + 1) + "}$: ",
                         description, NO_NEW_LINE);
            PrintPolynomial(
                Polynomial<Field, Comparator>{
                    {poly_system[rhs_pos].GetLeader()}},
                description, NO_NEW_LINE);
            PrintMessage(" are coprime. Skipping them ", description,
                         NO_NEW_LINE);
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
            assert(out_ && "No output buffer");
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

            assert(out_ && "No output buffer");
            PrintPolynomial(poly, description, NO_NEW_LINE);
            PrintMessage(" = ", description, NO_NEW_LINE);

            bool printed = false;
            for (size_t i = 0; i < poly_system.GetSize(); i++) {
                auto& g_i = PrinterBuffer<Field, Comparator>::Instance()[i];
                if (!g_i.IsZero()) {
                    if (printed) {
                        PrintMessage(" + ", description, NO_NEW_LINE);
                    }
                    printed = true;

                    PrintMessage("(", description, NO_NEW_LINE);
                    PrintPolynomial(g_i, description, NO_NEW_LINE);
                    PrintMessage(")$f_{" + std::to_string(i + 1) + "}$",
                                 description, NO_NEW_LINE);
                }
            }

            if (!printed) {
                PrintMessage(" $r$", description, NO_NEW_LINE);
                PrintNewLine(policy);
                return *this;
            }

            PrintMessage(" + $r$ = ", description, NO_NEW_LINE);
            printed = false;
            for (size_t i = 0; i < poly_system.GetSize(); i++) {
                auto& g_i = PrinterBuffer<Field, Comparator>::Instance()[i];
                if (!g_i.IsZero()) {
                    if (printed) {
                        PrintMessage(" + ", description, NO_NEW_LINE);
                    }
                    printed = true;
                    PrintMessage("(", description, NO_NEW_LINE);
                    PrintPolynomial(g_i, description, NO_NEW_LINE);
                    PrintMessage(")(", description, NO_NEW_LINE);
                    PrintPolynomial(poly_system[i], description, NO_NEW_LINE);
                    PrintMessage(")", description, NO_NEW_LINE);
                }
            }

            PrintMessage(" + (", description, NO_NEW_LINE);
            PrintPolynomial(rem, description, NO_NEW_LINE);
            PrintMessage(")", description, NO_NEW_LINE);

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
            assert(out_ && "No output buffer");
            PrintMessage("S-Polynomial reduced to: ", description, NO_NEW_LINE);
            PrintPolynomial(poly, description, NO_NEW_LINE);
            PrintMessage(" = $f_{" + std::to_string(pos + 1) + "}$", description, NO_NEW_LINE);
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

            assert(out_ && "No output buffer");
            PrintMessage("$f_{" + std::to_string(pos + 1) + "}$ = ",
                         description, NO_NEW_LINE);
            PrintPolynomial(poly, description, NO_NEW_LINE);
            PrintMessage(" stays in system", description, NO_NEW_LINE);
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

            assert(out_ && "No output buffer");
            PrintMessage("$f_{" + std::to_string(pos + 1) + "}$ = ",
                         description, NO_NEW_LINE);
            PrintPolynomial(poly, description, NO_NEW_LINE);
            PrintMessage(" is erased from system", description, NO_NEW_LINE);
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

            assert(out_ && "No output buffer");

            PrintMessage("$f_{" + std::to_string(pos + 1) + "}$ = ",
                         description, NO_NEW_LINE);
            PrintPolynomial(poly, description, NO_NEW_LINE);
            PrintMessage(" is reduced to: ", description, NO_NEW_LINE);
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

            assert(out_ && "No output buffer");
            PrintMessage(
                "Leader of $f_{" + std::to_string(reduced_pos + 1) + "}$ = ",
                description, NO_NEW_LINE);
            PrintPolynomial(Polynomial<Field, Comparator>(
                                {poly_system[reduced_pos].GetLeader()}),
                            description, NO_NEW_LINE);
            PrintMessage(" is divisible by leader of $f_{" +
                             std::to_string(reducer_pos + 1) + "}$ = ",
                         description, NO_NEW_LINE);
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
