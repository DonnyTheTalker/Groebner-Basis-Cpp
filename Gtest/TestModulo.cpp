#include "Modulo.h"
#include "gtest/gtest.h"

namespace Groebner::Test {
TEST(ModuloBasic, Construction) {
    ASSERT_NO_THROW(Modulo<2>(0));
    ASSERT_NO_THROW(Modulo<2>());
    ASSERT_NO_THROW(Modulo<5>(2));
    ASSERT_NO_THROW(Modulo<7>(10));
    ASSERT_NO_THROW(Modulo<100049>(20));
    ASSERT_NO_THROW(Modulo<3>(-1));
}

TEST(ModuloBasic, Normalization) {
    Modulo<7>(-1);
    ASSERT_EQ(Modulo<7>(0), Modulo<7>(7));
    ASSERT_EQ(Modulo<7>(-1), Modulo<7>(6));
    ASSERT_EQ(Modulo<7>(3), Modulo<7>(17));
    ASSERT_EQ(Modulo<7>(-4), Modulo<7>(17));
    ASSERT_EQ(Modulo<7>(-11), Modulo<7>(17));
}

TEST(ModuloBasic, Getters) {
    Modulo<7> x(-5);
    ASSERT_EQ(x.GetValue(), 2);
    ASSERT_FALSE(x.IsZero());

    x = Modulo<7>(200);
    ASSERT_EQ(x.GetValue(), 4);
    ASSERT_FALSE(x.IsZero());

    x = Modulo<7>(0);
    ASSERT_EQ(x.GetValue(), 0);
    ASSERT_TRUE(x.IsZero());

    x = Modulo<7>(14);
    ASSERT_EQ(x.GetValue(), 0);
    ASSERT_TRUE(x.IsZero());
}

TEST(ModuloBasic, Comparison) {
    ASSERT_TRUE(Modulo<13>(1) < Modulo<13>(2));
    ASSERT_TRUE(Modulo<13>(1) < Modulo<13>(28));
    ASSERT_TRUE(Modulo<13>(1) <= Modulo<13>(1));
    ASSERT_TRUE(Modulo<13>(1) <= Modulo<13>(2));
    ASSERT_TRUE(Modulo<13>(1) == Modulo<13>(1));
    ASSERT_TRUE(Modulo<13>(1) != Modulo<13>(2));
    ASSERT_TRUE(Modulo<13>(1) > Modulo<13>(0));
    ASSERT_TRUE(Modulo<13>(1) >= Modulo<13>(0));
    ASSERT_TRUE(Modulo<13>(1) >= Modulo<13>(1));
}

TEST(ModuloArithmetics, Inversion) {
    {
        Modulo<7> x(1);

        Modulo<7> y(0);
        ASSERT_DEATH(x / y, "Can't divide by zero");

        y = Modulo<7>(1);
        ASSERT_EQ((x / y).GetValue(), 1);

        y = Modulo<7>(2);
        ASSERT_EQ((x / y).GetValue(), 4);

        y = Modulo<7>(3);
        ASSERT_EQ((x / y).GetValue(), 5);

        y = Modulo<7>(4);
        ASSERT_EQ((x / y).GetValue(), 2);

        y = Modulo<7>(5);
        ASSERT_EQ((x / y).GetValue(), 3);

        y = Modulo<7>(6);
        ASSERT_EQ((x / y).GetValue(), 6);
    }

    {
        Modulo<13> x(1);
        Modulo<13> y(2);
        ASSERT_EQ((x / y).GetValue(), 7);

        y = Modulo<13>(3);
        ASSERT_EQ((x / y).GetValue(), 9);

        y = Modulo<13>(7);
        ASSERT_EQ((x / y).GetValue(), 2);
    }

    {
        Modulo<100049> x(1);
        Modulo<100049> y(333);
        ASSERT_EQ((x / y).GetValue(), 88632);

        y = Modulo<100049>(12312);
        ASSERT_EQ((x / y).GetValue(), 86649);
    }
}

TEST(ModuloArithmetics, Addition) {
    Modulo<13> x;
    ASSERT_EQ(x += Modulo<13>(21), Modulo<13>(8));
    ASSERT_EQ(x += Modulo<13>(-3), Modulo<13>(5));
    ASSERT_EQ(x += Modulo<13>(5), Modulo<13>(10));
    ASSERT_EQ(x += x, Modulo<13>(7));
}

TEST(ModuloArithmetics, Substitution) {
    Modulo<13> x;
    ASSERT_EQ(x -= Modulo<13>(21), Modulo<13>(5));
    ASSERT_EQ(x -= Modulo<13>(-3), Modulo<13>(8));
    ASSERT_EQ(x -= Modulo<13>(9), Modulo<13>(12));
    ASSERT_EQ(x -= x, Modulo<13>(0));
}

TEST(ModuloArithmetics, Multiplication) {
    Modulo<13> x(1);
    ASSERT_EQ(x *= Modulo<13>(21), Modulo<13>(8));
    ASSERT_EQ(x *= Modulo<13>(-3), Modulo<13>(2));
    ASSERT_EQ(x *= Modulo<13>(55), Modulo<13>(6));
    ASSERT_EQ(x *= x, Modulo<13>(10));
}

TEST(ModuloArithmetics, Division) {
    Modulo<13> x(1);
    ASSERT_EQ(x /= Modulo<13>(5), Modulo<13>(8));
    ASSERT_EQ(x /= Modulo<13>(12), Modulo<13>(5));
    ASSERT_EQ(x /= Modulo<13>(-3), Modulo<13>(7));
    ASSERT_EQ(x /= x, Modulo<13>(1));
}

TEST(ModuloArithmetics, General) {
    Modulo<13> x(1);
    Modulo<13> y(5);
    Modulo<13> z(3);

    ASSERT_EQ(x + y - z, Modulo<13>(3));
    ASSERT_EQ((x + y) * z, Modulo<13>(5));
    ASSERT_EQ(x * z + y * z, Modulo<13>(5));
    ASSERT_EQ((x + y) / z, Modulo<13>(2));
    ASSERT_EQ(x / z + y / z, Modulo<13>(2));
    ASSERT_EQ(x + x, x * z - x);
}

TEST(ModuloArithmetics, UnaryMinus) {
    {
        Modulo<7> x(1);
        ASSERT_EQ(-x, Modulo<7>(6));
    }

    {
        Modulo<7> x(-1);
        ASSERT_EQ(-x, Modulo<7>(1));
    }

    {
        Modulo<7> x(3);
        ASSERT_EQ(-x, Modulo<7>(-3));
        ASSERT_EQ(-(-x), x);
        ASSERT_EQ(-(-x), Modulo<7>(3));
    }
}
}  // namespace Groebner::Test
