#include "Rational.h"
#include "gtest/gtest.h"

namespace Groebner::Test {
TEST(RationalBasic, Construction) {
    ASSERT_NO_THROW(Rational());
    ASSERT_NO_THROW(Rational(1));
    ASSERT_NO_THROW(Rational(0));
    ASSERT_NO_THROW(Rational(1, 1));
    ASSERT_DEATH(Rational(3, 0), "Denominator must not be zero");
}

TEST(RationalBasic, Reduction) {
    ASSERT_EQ(Rational(1, 3), Rational(2, 6));
    ASSERT_EQ(Rational(3), Rational(3, 1));
    ASSERT_EQ(Rational(3), Rational(6, 2));
    ASSERT_EQ(Rational(-4, -8), Rational(1, 2));
    ASSERT_EQ(Rational(-3, 2), Rational(3, -2));
}

TEST(RationalBasic, Getters) {
    Rational x(13, 26);
    ASSERT_FALSE(x.IsZero());
    ASSERT_EQ(x.GetNumerator(), 1);
    ASSERT_EQ(x.GetDenominator(), 2);

    x = Rational(2, -1);
    ASSERT_FALSE(x.IsZero());
    ASSERT_EQ(x.GetNumerator(), -2);
    ASSERT_EQ(x.GetDenominator(), 1);

    x = Rational(0);
    ASSERT_TRUE(x.IsZero());
    ASSERT_EQ(x.GetNumerator(), 0);
    ASSERT_EQ(x.GetDenominator(), 1);
}

TEST(RationalBasic, Comparison) {
    ASSERT_TRUE(Rational(1) < Rational(2));
    ASSERT_TRUE(Rational(1, 2) == Rational(2, 4));
    ASSERT_TRUE(Rational(1, 3) <= Rational(2, 4));
    ASSERT_TRUE(Rational(5, 4) != Rational(6, 4));
    ASSERT_TRUE(Rational(5, 4) < Rational(6, 4));
    ASSERT_TRUE(Rational(10, 2) == Rational(5));
    ASSERT_TRUE(Rational(10, 3) > Rational(8, 3));
    ASSERT_TRUE(Rational(10, 3) >= Rational(8, 3));
}

TEST(RationalArithmetics, Addition) {
    Rational x;
    x += Rational(3);
    ASSERT_EQ(x, Rational(3));

    x += Rational(1, 2);
    ASSERT_EQ(x, Rational(7, 2));

    x += Rational(0);
    ASSERT_EQ(x, Rational(7, 2));

    x += Rational(-7, 2);
    ASSERT_EQ(x, Rational(0));

    x += Rational(3, 5);
    ASSERT_EQ(x, Rational(3, 5));

    x += Rational(14, 4);
    ASSERT_EQ(x, Rational(41, 10));

    x += x;
    ASSERT_EQ(x, Rational(41, 5));
}

TEST(RationalArithmetics, Substitution) {
    Rational x(7, 2);
    x -= Rational(7, 2);
    ASSERT_EQ(x, Rational(0));

    x += Rational(15, 13);
    x -= Rational(3, 2);
    ASSERT_EQ(x, Rational(-9, 26));

    x -= Rational(30);
    ASSERT_EQ(x, Rational(-789, 26));

    x -= x;
    ASSERT_EQ(x, Rational(0));
}

TEST(RationalArithmetics, Multiplication) {
    Rational x(7, 2);
    x *= Rational(0);
    ASSERT_EQ(x, Rational(0));

    x += Rational(3, 2);
    x *= x;
    ASSERT_EQ(x, Rational(9, 4));

    x *= Rational(1);
    ASSERT_EQ(x, Rational(9, 4));

    x *= Rational(2);
    ASSERT_EQ(x, Rational(9, 2));

    x *= Rational(13, 22);
    ASSERT_EQ(x, Rational(117, 44));
}

TEST(RationalArithmetics, Division) {
    Rational x(7, 2);
    x /= x;
    ASSERT_EQ(x, Rational(1));

    x = Rational(7, 2);
    x /= Rational(1, 2);
    ASSERT_EQ(x, Rational(7));

    x /= Rational(2);
    ASSERT_EQ(x, Rational(7, 2));

    ASSERT_DEATH(x /= Rational(0), "Can't divide by zero");
}

TEST(RationalArithmetics, General) {
    Rational x, y, z;

    x = Rational(7, 2);
    y = x + x;
    z = x * Rational(2);
    ASSERT_EQ(y, z);

    x = Rational(1);
    x = x + x + x;
    ASSERT_EQ(x, Rational(3));

    x = x * x * x;
    ASSERT_EQ(x, Rational(27));

    x = Rational(3, 2);
    y = Rational(4);
    z = Rational(5, 7);

    ASSERT_EQ(x + y / z, Rational(71, 10));
}

TEST(RationalArithmetics, UnaryMinus) {
    Rational x(7, 2);
    ASSERT_EQ(-x, Rational(-7, 2));
    ASSERT_EQ(-(-x), Rational(7, 2));
    ASSERT_EQ(-(-x), x);
}
}  // namespace Groebner::Test
