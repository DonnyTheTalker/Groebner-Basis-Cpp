#include "Printer.h"
#include "VariableOrder.h"

namespace Groebner {

Printer& Printer::Instance() {
    static Printer instance;
    return instance;
}

void Printer::SetDescriptionLevel(DescriptionLevel level) {
    description_level_ = level;
}

void Printer::PrintMessage(const std::string& message,
                           Groebner::Printer::DescriptionLevel description) {
    if (description_level_ & description) {
        std::cout << message << " \\\\\n";
    }
}

void Printer::ResetOutputBuffer() {
    std::cout.rdbuf(cout_buf_);
    cout_buf_ = std::cout.rdbuf();
}

void Printer::SetOutputBuffer(std::ofstream& out) {
    ResetOutputBuffer();
    std::cout.rdbuf(out.rdbuf());
}

void Printer::PrintDegree(const Monomial& degree) {
    for (size_t i = 0; i < degree.GetSize(); i++) {
        auto value = degree.GetDegree(i);
        if (value == 0) {
            continue;
        }
        std::cout << "$" << VariableOrder::Instance().GetAt(i);
        if (value > 1) {
            std::cout << "^" << value;
        }
        std::cout << "$";
    }
}

}  // namespace Groebner
