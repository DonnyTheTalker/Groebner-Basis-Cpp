#include "FieldFwd.h"
#include "Printer.h"
#include "GroebnerAlgorithm.h"
#include "VariableOrder.h"

using namespace Groebner;

int main() {
    Printer::Instance().SetDescriptionLevel(Printer::ALL);
    std::ofstream out("temp.tex");
    Printer::Instance().SetOutputBuffer(out);
    VariableOrder::Instance().SetVarList({{0, "x"}, {1, "y"}, {2, "z"}});
    Printer::Instance().PrintField<Rational>(Printer::CONDITIONS);
    Printer::Instance().PrintMessage("", Printer::CONDITIONS);

    Polynomial<Rational, LexOrder> x{{3, {2, 0}}, {1, {1, 1}}, {1, {0, 0}}};
    Polynomial<Rational, LexOrder> y{{1, {1, 1}}, {-1, {0, 2}}};

    PolySystem<Rational, LexOrder> basis({x, y});

    basis = GroebnerAlgorithm::BuildGB(basis, AutoReduction::Enabled);

    Printer::Instance().ResetOutputBuffer();
    out.close();
}