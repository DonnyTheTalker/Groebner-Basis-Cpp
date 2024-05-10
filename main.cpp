#include "FieldFwd.h"
#include "GroebnerAlgorithm.h"
#include "Printer.h"
#include "VariableOrder.h"

using namespace Groebner;

int main() {
    Printer::Instance().SetDescriptionLevel(Printer::ALL);
    std::ofstream out("temp.tex");
    Printer::Instance().SetOutputBuffer(out);
    VariableOrder::Instance().SetVarList({{0, "x"}, {1, "y"}, {2, "z"}});
    Printer::Instance().PrintField<Rational>(Printer::CONDITIONS, Printer::DOUBLE_NEW_LINE);

    Polynomial<Rational, LexOrder> x{{1, {2, 0}}, {1, {1, 1}}, {1, {0, 0}}};
    Polynomial<Rational, LexOrder> y{{1, {1, 1}}, {-1, {0, 2}}};
    Polynomial<Rational, LexOrder> aim{
        {1, {4, 0}}, {2, {2, 3}}, {1, {2, 2}},  {1, {2, 1}}, {2, {2}},
        {3, {1, 4}}, {1, {1, 2}}, {-1, {0, 5}}, {1, {0, 2}}, {1, {0}}};

    PolySystem<Rational, LexOrder> basis({x, y, y});

    //    basis = GroebnerAlgorithm::BuildGB(basis, AutoReduction::Enabled);
    GroebnerAlgorithm::IsInIdeal(aim, basis);

    out.close();
}
