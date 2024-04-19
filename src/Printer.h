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
            static void Print() {}
    };

    template <>
    struct FieldPrinter<Rational> {
            static void Print() {
                std::cout << "Working in $\\mathbb{Q}$ field. \\\\\n";
            }
    };

    template <int64_t N>
    requires Groebner::IsPrime<N> struct FieldPrinter<Modulo<N>> {
            static void Print() {
                std::cout << "Working in $\\mathbb{Z}_" << std::to_string(N)
                          << "$ field. \\\\\n";
            }
    };

    template <IsSupportedField T>
    struct CoefPrinter {
            static void Print(T coef) {}
    };

    template <>
    struct CoefPrinter<Rational> {
            static void Print(Rational coef) {
                if (coef.GetDenominator() == 1) {
                    std::cout << "$" << coef.GetNumerator() << "$";
                } else {
                    std::cout << "$\\frac{" << coef.GetNumerator() << "}{"
                              << coef.GetDenominator() << "}$";
                }
            }
    };

    template <size_t N>
    requires Groebner::IsPrime<N> struct CoefPrinter<Modulo<N>> {
            static void Print(Modulo<N> coef) {
                std::cout << "$" << coef.GetValue() << "$";
            }
    };
}  // namespace Details

// TODO also add Details level
template <IsSupportedField Field, IsComparator Comparator>
class PrinterBuffer {
    private:
        PrinterBuffer() = default;
        PrinterBuffer(const PrinterBuffer& other) = delete;

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
        void SetOutputBuffer(std::ofstream& out);
        void ResetOutputBuffer();

        enum DescriptionLevel {
            NONE = 0x0,
            CONDITIONS = 0x1,
            RESULTS = 0x2,
            DETAILS = 0x4,
            ALL = CONDITIONS | RESULTS | DETAILS
        };

        void SetDescriptionLevel(DescriptionLevel level);
        // TODO add enum class NewLinePolicy : NoNewLine, NewLine
        // TODO use \newline as static const string
        void PrintMessage(const std::string& message,
                          DescriptionLevel description);

        template <IsSupportedField Field>
        void PrintField(DescriptionLevel description) {
            if (description_level_ & description) {
                Details::FieldPrinter<Field>::Print();
            }
        }

        void PrintDegree(const Monomial& degree);

        template <IsSupportedField Field, IsComparator Comparator>
        void PrintPolynomial(const Polynomial<Field, Comparator>& poly,
                             DescriptionLevel description) {
            if (!(description_level_ & description)) {
                return;
            }
            if (poly.IsZero()) {
                std::cout << "$0$";
            }
            for (size_t i = 0; i < poly.GetSize(); i++) {
                auto [coef, degree] = poly.GetAt(i);
                if (i > 0 && coef > 0) {
                    std::cout << " + ";
                } else if (i > 0 && coef < 0) {
                    std::cout << " $-$ ";
                }
                // TODO add Monomial(0) as static const of Monomial class
                // or add method is empty
                if (PrintCoef(coef.Abs(), degree)) {
                    Details::CoefPrinter<Field>::Print(coef.Abs());
                }
                PrintDegree(degree);
            }
        }

        template <IsSupportedField Field, IsComparator Comparator>
        void PrintPolySystem(const PolySystem<Field, Comparator>& poly_system,
                             DescriptionLevel description) {
            if (!(description_level_ & description)) {
                return;
            }

            for (size_t i = 0; i < poly_system.GetSize(); i++) {
                std::cout << "$f_{" << i + 1 << "}$: ";
                PrintPolynomial(poly_system[i], description);
                std::cout << "\\\\\n";
            }
        }

        template <IsSupportedField Field, IsComparator Comparator>
        void PrintSPolynomial(const Polynomial<Field, Comparator>& s_poly,
                              const PolySystem<Field, Comparator>& poly_system,
                              size_t lhs_pos, size_t rhs_pos,
                              DescriptionLevel description) {
            if (!(description_level_ & description)) {
                return;
            }

            std::cout << "$($";
            PrintPolynomial(PrinterBuffer<Field, Comparator>::Instance()[0],
                            description);
            std::cout << "$)$";
            std::cout << "$f_{" << lhs_pos + 1 << "}$";
            std::cout << " $-$ " << "$($";
            PrintPolynomial(PrinterBuffer<Field, Comparator>::Instance()[1],
                            description);
            std::cout << "$)$" << "$f_{" << rhs_pos + 1 << "}$" << " = ";

            std::cout << "$($";
            PrintPolynomial(PrinterBuffer<Field, Comparator>::Instance()[0],
                            description);
            std::cout << "$)$";
            std::cout << "$($";
            PrintPolynomial(poly_system[lhs_pos], description);
            std::cout << "$)$ ";
            std::cout << "$-$ ";

            std::cout << "$($";
            PrintPolynomial(PrinterBuffer<Field, Comparator>::Instance()[1],
                            description);
            std::cout << "$)$";

            std::cout << "$($";
            PrintPolynomial(poly_system[rhs_pos], description);
            std::cout << "$)$ ";
            std::cout << "= ";

            PrintPolynomial(s_poly, description);
            std::cout << "\\\\";
        }

        template <IsSupportedField Field, IsComparator Comparator>
        void PrintSPolynomialSkip(
            const PolySystem<Field, Comparator>& poly_system, size_t lhs_pos,
            size_t rhs_pos, DescriptionLevel description) {
            if (!(description_level_ & description)) {
                return;
            }
            std::cout << "Leader degree of " << "$f_{" << lhs_pos + 1 << "}$: ";
            PrintPolynomial(
                Polynomial<Field, Comparator>{
                    {poly_system[lhs_pos].GetLeader()}},
                description);
            std::cout << ", and of " << "$f_{" << rhs_pos + 1 << "}$: ";
            PrintPolynomial(
                Polynomial<Field, Comparator>{
                    {poly_system[rhs_pos].GetLeader()}},
                description);
            std::cout << " are coprime. Skipping them \\\\";
        }

    private:
        template <IsSupportedField Field>
        bool PrintCoef(Field coef, const Monomial& degree) {
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
        std::streambuf* cout_buf_ = std::cout.rdbuf();
};

}  // namespace Groebner
