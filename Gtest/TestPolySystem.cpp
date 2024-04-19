#include "PolySystem.h"
#include "gtest/gtest.h"

namespace Groebner::Test {

template <IsSupportedField Field, IsComparator Comparator>
void CheckEqual(const PolySystem<Field, Comparator>& lhs,
                const PolySystem<Field, Comparator>& rhs) {
    ASSERT_EQ(lhs.GetSize(), rhs.GetSize());
    for (size_t i = 0; i < lhs.GetSize(); i++) {
        ASSERT_EQ(lhs[i], rhs[i]);
    }
}

TEST(PolySystemBasic, Construction) {
    Polynomial<Rational, LexOrder> x = {{3, {2, 0}}, {1, {1, 1}}, {2, {0, 1}}};
    Polynomial<Rational, LexOrder> y = {{3, {2, 1}}, {1, {1, 1}}, {2, {0, 1}}};
    Polynomial<Rational, LexOrder> z = {{0, {2, 2}}};

    std::vector temp = {x, y, z};
    PolySystem<Rational, LexOrder> system(temp.begin(), temp.end());
    ASSERT_EQ(system.GetSize(), 2);

    temp.pop_back();
    CheckEqual(system, PolySystem<Rational, LexOrder>(std::move(temp)));
}

TEST(PolySystemBasic, Pop) {
    Polynomial<Rational, LexOrder> x = {{3, {2, 0}}, {1, {1, 1}}, {2, {0, 1}}};
    Polynomial<Rational, LexOrder> y = {{3, {2, 1}}, {1, {1, 1}}, {2, {0, 1}}};
    Polynomial<Rational, LexOrder> z = {{3, {2, 2}}, {1, {1, 1}}, {2, {0, 1}}};

    for (size_t i = 0; i < 3; i++) {
        std::vector<Polynomial<Rational, LexOrder>> temp = {x, y, z};
        PolySystem<Rational, LexOrder> system(temp.begin(), temp.end());
        system.Pop(i);
        temp.erase(std::next(temp.begin(), i));
        PolySystem<Rational, LexOrder> expected(temp.begin(), temp.end());
        CheckEqual(system, expected);
    }

    std::vector<Polynomial<Rational, LexOrder>> temp = {x, y, z};
    PolySystem<Rational, LexOrder> system(temp.begin(), temp.end());
    for (size_t i = 0; i < 3; i++) {
        system.Pop(0);
    }
    CheckEqual(system, PolySystem<Rational, LexOrder>());
    ASSERT_DEATH(system.Pop(0), "Out of bounds");
}

TEST(PolySystemBasic, FastPushPop) {
    Polynomial<Rational, LexOrder> x = {{3, {2, 0}}, {1, {1, 1}}, {2, {0, 1}}};
    Polynomial<Rational, LexOrder> y = {{3, {2, 1}}, {1, {1, 1}}, {2, {0, 1}}};
    Polynomial<Rational, LexOrder> z = {{3, {2, 2}}, {1, {1, 1}}, {2, {0, 1}}};

    PolySystem<Rational, LexOrder> sys({x, y, z});
    PolySystem<Rational, LexOrder> copy(sys);

    {
        PolySystem<Rational, LexOrder> temp({z, y});
        sys.SwapAndPop(0);
        CheckEqual(sys, temp);
        sys.AddAndSwap(0, x);
        CheckEqual(sys, copy);
    }

    {
        PolySystem<Rational, LexOrder> temp({x, y});
        sys.SwapAndPop(2);
        CheckEqual(sys, temp);
        sys.AddAndSwap(2, z);
        CheckEqual(sys, copy);
    }

    {
        ASSERT_DEATH(sys.SwapAndPop(4), "Out of bounds");
        ASSERT_DEATH(sys.AddAndSwap(4, x), "Out of bounds");
    }
}
}  // namespace Groebner::Test
