#include "Printer.h"
#include "VariableOrder.h"

namespace Groebner {

Printer& Printer::Instance() {
    static Printer instance;
    return instance;
}

Printer& Printer::PrintNewLine(Printer::NewLinePolicy policy) {
    assert(out_ && "No output buffer");
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
                               Groebner::Printer::DescriptionLevel description,
                               NewLinePolicy policy) {
    if (description_level_ & description) {
        assert(out_ && "No output buffer");
        *out_ << message;
        PrintNewLine(policy);
    }
    return *this;
}

Printer& Printer::SetOutputBuffer(std::ofstream& out) {
    out_ = &out;
    return *this;
}

Printer& Printer::ResetOutputBuffer() {
    out_ = nullptr;
    return *this;
}

Printer& Printer::PrintBuildingSPoly(size_t i, size_t j,
                                     Printer::DescriptionLevel description,
                                     Printer::NewLinePolicy policy) {
    if (!(description_level_ & description)) {
        return *this;
    }

    assert(out_ && "No output buffer");
    PrintMessage("Building S-Polynomial for $f_{" + std::to_string(i + 1) +
                     "}$ and $f_{" + std::to_string(j + 1) + "}$",
                 description, NO_NEW_LINE)
        .PrintNewLine(policy);
    return *this;
}

// TODO add description level
Printer& Printer::PrintDegree(const Monomial& degree, NewLinePolicy policy) {
    assert(out_ && "No output buffer");
    for (size_t i = 0; i < degree.GetSize(); i++) {
        auto value = degree.GetDegree(i);
        if (value == 0) {
            continue;
        }
        PrintMessage("$" + VariableOrder::Instance().GetAt(i), NONE, NO_NEW_LINE);
        if (value > 1) {
            PrintMessage("^", NONE, NO_NEW_LINE);
        }
        PrintMessage("$", NONE, NO_NEW_LINE);
    }
    PrintNewLine(policy);
    return *this;
}
}  // namespace Groebner
