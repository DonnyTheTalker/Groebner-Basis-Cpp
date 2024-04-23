#include "Printer.h"
#include "VariableOrder.h"

namespace Groebner {

Printer& Printer::Instance() {
    static Printer instance;
    return instance;
}


Printer& Printer::PrintNewLine(Printer::NewLinePolicy policy) {
    for (size_t i = 0; i < policy; i++) {
        *out_ << "\\\\\n";
    }
    return *this;
}

Printer& Printer::SetDescriptionLevel(DescriptionLevel level) {
    description_level_ = level;
    return *this;
}

Printer& Printer::PrintMessage(const std::string& message,
                           Groebner::Printer::DescriptionLevel description, NewLinePolicy policy) {
    if (description_level_ & description) {
        *out_ << message;
        PrintNewLine(policy);
    }
    return *this;
}

Printer& Printer::SetOutputBuffer(std::ofstream& out) {
    out_ = &out;
    return *this;
}

Printer& Printer::PrintBuildingSPoly(size_t i, size_t j,
                                     Printer::DescriptionLevel description,
                                     Printer::NewLinePolicy policy) {
    if (!(description_level_ & description)) {
        return *this;
    }

    *out_ << "Building S-Polynomial for $f_{" + std::to_string(i + 1) +
                     "}$ and $f_{" + std::to_string(j + 1) + "}$";
    PrintNewLine(policy);
    return *this;
}

Printer& Printer::PrintDegree(const Monomial& degree, NewLinePolicy policy) {
    for (size_t i = 0; i < degree.GetSize(); i++) {
        auto value = degree.GetDegree(i);
        if (value == 0) {
            continue;
        }
        *out_ << "$" << VariableOrder::Instance().GetAt(i);
        if (value > 1) {
            *out_ << "^" << value;
        }
        *out_ << "$";
    }
    PrintNewLine(policy);
    return *this;
}

}  // namespace Groebner
