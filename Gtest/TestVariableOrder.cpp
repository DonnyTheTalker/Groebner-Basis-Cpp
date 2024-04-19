#include "VariableOrder.h"
#include "gtest/gtest.h"

namespace Groebner::Test {
TEST(VariableOrder, SetSingle) {
    {
        VariableOrder::Instance().SetVar(0, "x");
        VariableOrder::Instance().SetVar(1, "y");
        VariableOrder::Instance().SetVar(2, "z");

        ASSERT_EQ(VariableOrder::Instance().GetAt(0), "x");
        ASSERT_EQ(VariableOrder::Instance().GetAt(1), "y");
        ASSERT_EQ(VariableOrder::Instance().GetAt(2), "z");

        ASSERT_EQ(VariableOrder::Instance().GetAt("x"), 0);
        ASSERT_EQ(VariableOrder::Instance().GetAt("y"), 1);
        ASSERT_EQ(VariableOrder::Instance().GetAt("z"), 2);
    }

    {
        std::vector<std::pair<size_t, std::string>> var_list = {
            {0, "x"}, {1, "y"}, {2, "z"}};

        VariableOrder::Instance().SetVarList(var_list.begin(), var_list.end());

        ASSERT_EQ(VariableOrder::Instance().GetAt(0), "x");
        ASSERT_EQ(VariableOrder::Instance().GetAt(1), "y");
        ASSERT_EQ(VariableOrder::Instance().GetAt(2), "z");

        ASSERT_EQ(VariableOrder::Instance().GetAt("x"), 0);
        ASSERT_EQ(VariableOrder::Instance().GetAt("y"), 1);
        ASSERT_EQ(VariableOrder::Instance().GetAt("z"), 2);

        VariableOrder::Instance().ClearVarList();
        ASSERT_DEATH(VariableOrder::Instance().GetAt(0), "No such index");
        ASSERT_DEATH(VariableOrder::Instance().GetAt("x"), "No such variable");

        VariableOrder::Instance().SetVarList(std::move(var_list));

        ASSERT_EQ(VariableOrder::Instance().GetAt(0), "x");
        ASSERT_EQ(VariableOrder::Instance().GetAt(1), "y");
        ASSERT_EQ(VariableOrder::Instance().GetAt(2), "z");

        ASSERT_EQ(VariableOrder::Instance().GetAt("x"), 0);
        ASSERT_EQ(VariableOrder::Instance().GetAt("y"), 1);
        ASSERT_EQ(VariableOrder::Instance().GetAt("z"), 2);
    }

    {
        VariableOrder::Instance().SetVarList({{0, "x"}, {1, "y"}, {2, "z"}});

        ASSERT_EQ(VariableOrder::Instance().GetAt(0), "x");
        ASSERT_EQ(VariableOrder::Instance().GetAt(1), "y");
        ASSERT_EQ(VariableOrder::Instance().GetAt(2), "z");

        ASSERT_EQ(VariableOrder::Instance().GetAt("x"), 0);
        ASSERT_EQ(VariableOrder::Instance().GetAt("y"), 1);
        ASSERT_EQ(VariableOrder::Instance().GetAt("z"), 2);
    }
}

}  // namespace Groebner::Test
