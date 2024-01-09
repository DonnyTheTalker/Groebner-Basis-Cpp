#include "gtest/gtest.h"
#include "Modulo.h"

TEST(ModuloBasic, Construction) {
    EXPECT_NO_THROW(Modulo(2, 0));
    EXPECT_NO_THROW(Modulo(2));
    EXPECT_NO_THROW(Modulo(5, 2));
    EXPECT_NO_THROW(Modulo(7, 10));
    EXPECT_NO_THROW(Modulo(100049, 20));
    EXPECT_NO_THROW(Modulo(3, -1));

    EXPECT_DEATH(Modulo(0), "Order must be positive");
    EXPECT_DEATH(Modulo(-200), "Order must be positive");
    EXPECT_DEATH(Modulo(1), "Order must be prime number");
    EXPECT_DEATH(Modulo(4), "Order must be prime number");
    EXPECT_DEATH(Modulo(8), "Order must be prime number");
    EXPECT_DEATH(Modulo(100000), "Order must be prime number");
}

TEST(ModuloBasic, Normalization) {
    EXPECT_EQ(Modulo(7, 0), Modulo(7, 7));
    EXPECT_EQ(Modulo(7, -1), Modulo(7, 6));
    EXPECT_EQ(Modulo(7, 17), Modulo(7, 3));
    EXPECT_EQ(Modulo(7, 17), Modulo(7, -4));
    EXPECT_EQ(Modulo(7, 17), Modulo(7, -11));
}

TEST(ModuloBasic, Getters) {
    Modulo x(7, -5);
    EXPECT_EQ(x.Value(), 2);
    EXPECT_FALSE(x.IsZero());

    x = Modulo(7, 200);
    EXPECT_EQ(x.Value(), 4);
    EXPECT_FALSE(x.IsZero());

    x = Modulo(7, 0);
    EXPECT_EQ(x.Value(), 0);
    EXPECT_TRUE(x.IsZero());

    x = Modulo(7, 14);
    EXPECT_EQ(x.Value(), 0);
    EXPECT_TRUE(x.IsZero());
}

TEST(ModuloBasic, Comparison) {
    EXPECT_TRUE(Modulo(13, 1) < Modulo(13, 2));
    EXPECT_TRUE(Modulo(13, 1) < Modulo(13, 28));
    EXPECT_TRUE(Modulo(13, 1) <= Modulo(13, 1));
    EXPECT_TRUE(Modulo(13, 1) <= Modulo(13, 2));
    EXPECT_TRUE(Modulo(13, 1) == Modulo(13, 1));
    EXPECT_TRUE(Modulo(13, 1) != Modulo(13, 2));
    EXPECT_TRUE(Modulo(13, 1) != Modulo(7, 1));
    EXPECT_TRUE(Modulo(13, 1) > Modulo(13, 0));
    EXPECT_TRUE(Modulo(13, 1) >= Modulo(13, 0));
    EXPECT_TRUE(Modulo(13, 1) >= Modulo(13, 1));
}

TEST(ModuloArithmetics, Inversion) {
    {
        Modulo x(7, 1);

        Modulo y(7, 0);
        EXPECT_DEATH(x / y, "Can't divide by zero");

        y = Modulo(7, 1);
        EXPECT_EQ((x / y).Value(), 1);

        y = Modulo(7, 2);
        EXPECT_EQ((x / y).Value(), 4);

        y = Modulo(7, 3);
        EXPECT_EQ((x / y).Value(), 5);

        y = Modulo(7, 4);
        EXPECT_EQ((x / y).Value(), 2);

        y = Modulo(7, 5);
        EXPECT_EQ((x / y).Value(), 3);

        y = Modulo(7, 6);
        EXPECT_EQ((x / y).Value(), 6);
    }

    {
        Modulo x(13, 1);
        Modulo y(13, 2);
        EXPECT_EQ((x / y).Value(), 7);

        y = Modulo(13, 3);
        EXPECT_EQ((x / y).Value(), 9);

        y = Modulo(13, 7);
        EXPECT_EQ((x / y).Value(), 2);
    }

    {
        Modulo x(100049, 1);
        Modulo y(100049, 333);
        EXPECT_EQ((x / y).Value(), 88632);

        y = Modulo(100049, 12312);
        EXPECT_EQ((x / y).Value(), 86649);
    }
}

TEST(ModuloArithmetics, Addition) {
    Modulo x(13, 0);
    EXPECT_EQ(x += Modulo(13, 21), Modulo(13, 8));
    EXPECT_EQ(x += Modulo(13, -3), Modulo(13, 5));
    EXPECT_EQ(x += Modulo(13, 5), Modulo(13, 10));
    EXPECT_EQ(x += x, Modulo(13, 7));
}

TEST(ModuloArithmetics, Substitution) {
    Modulo x(13, 0);
    EXPECT_EQ(x -= Modulo(13, 21), Modulo(13, 5));
    EXPECT_EQ(x -= Modulo(13, -3), Modulo(13, 8));
    EXPECT_EQ(x -= Modulo(13, 9), Modulo(13, 12));
    EXPECT_EQ(x -= x, Modulo(13, 0));
}

TEST(ModuloArithmetics, Multiplication) {
    Modulo x(13, 1);
    EXPECT_EQ(x *= Modulo(13, 21), Modulo(13, 8));
    EXPECT_EQ(x *= Modulo(13, -3), Modulo(13, 2));
    EXPECT_EQ(x *= Modulo(13, 55), Modulo(13, 6));
    EXPECT_EQ(x *= x, Modulo(13, 10));
}

TEST(ModuloArithmetics, Division) {
    Modulo x(13, 1);
    EXPECT_EQ(x /= Modulo(13, 5), Modulo(13, 8));
    EXPECT_EQ(x /= Modulo(13, 12), Modulo(13, 5));
    EXPECT_EQ(x /= Modulo(13, -3), Modulo(13, 7));
    EXPECT_EQ(x /= x, Modulo(13, 1));
}

TEST(ModuloArithmetics, General) {
    Modulo x(13, 1);
    Modulo y(13, 5);
    Modulo z(13, 3);

    EXPECT_EQ(x + y - z, Modulo(13, 3));
    EXPECT_EQ((x + y) * z, Modulo(13, 5));
    EXPECT_EQ(x * z + y * z, Modulo(13, 5));
    EXPECT_EQ((x + y) / z, Modulo(13, 2));
    EXPECT_EQ(x / z + y / z, Modulo(13, 2));
    EXPECT_EQ(x + x, x * z - x);
}