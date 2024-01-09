#include "gtest/gtest.h"
#include "Rational.h"

TEST(Basic, Construction) {
    EXPECT_NO_THROW(Rational());
    EXPECT_NO_THROW(Rational(1));
    EXPECT_NO_THROW(Rational(0));
    EXPECT_NO_THROW(Rational(1, 1));
    EXPECT_DEATH(Rational(3, 0), "Denominator must not be zero!");
}

TEST(Basic, Reduction) {
    EXPECT_EQ(Rational(1, 3), Rational(2, 6));
    EXPECT_EQ(Rational(3), Rational(3, 1));
    EXPECT_EQ(Rational(3), Rational(6, 2));
    EXPECT_EQ(Rational(-4, -8), Rational(1, 2));
    EXPECT_EQ(Rational(-3, 2), Rational(3, -2));
}

TEST(Basic, Getters) {
    Rational x(13, 26);
    EXPECT_FALSE(x.IsZero());
    EXPECT_EQ(x.Numerator(), 1);
    EXPECT_EQ(x.Denominator(), 2);

    x = Rational(2, -1);
    EXPECT_FALSE(x.IsZero());
    EXPECT_EQ(x.Numerator(), -2);
    EXPECT_EQ(x.Denominator(), 1);

    x = Rational(0);
    EXPECT_TRUE(x.IsZero());
    EXPECT_EQ(x.Numerator(), 0);
    EXPECT_EQ(x.Denominator(), 1);
}

TEST(Arithmetics, Addition) {
    Rational x;
    x += Rational(3);
    EXPECT_EQ(x, Rational(3));

    x += Rational(1, 2);
    EXPECT_EQ(x, Rational(7, 2));

    x += Rational(0);
    EXPECT_EQ(x, Rational(7, 2));

    x += Rational(-7, 2);
    EXPECT_EQ(x, Rational(0));

    x += Rational(3, 5);
    EXPECT_EQ(x, Rational(3, 5));

    x += Rational(14, 4);
    EXPECT_EQ(x, Rational(41, 10));

    x += x;
    EXPECT_EQ(x, Rational(41, 5));
}

TEST(Arithmetics, Substitution) {
    Rational x(7, 2);
    x -= Rational(7, 2);
    EXPECT_EQ(x, Rational(0));

    x += Rational(15, 13);
    x -= Rational(3, 2);
    EXPECT_EQ(x, Rational(-9, 26));

    x -= Rational(30);
    EXPECT_EQ(x, Rational(-789, 26));

    x -= x;
    EXPECT_EQ(x, Rational(0));
}

TEST(Arithmetics, Multiplication) {
    Rational x(7, 2);
    x *= Rational(0);
    EXPECT_EQ(x, Rational(0));

    x += Rational(3, 2);
    x *= x;
    EXPECT_EQ(x, Rational(9, 4));

    x *= Rational(1);
    EXPECT_EQ(x, Rational(9, 4));

    x *= Rational(2);
    EXPECT_EQ(x, Rational(9, 2));

    x *= Rational(13, 22);
    EXPECT_EQ(x, Rational(117, 44));
}

TEST(Arithmetics, Division) {
    Rational x(7, 2);
    x /= x;
    EXPECT_EQ(x, Rational(1));

    x = Rational(7, 2);
    x /= Rational(1, 2);
    EXPECT_EQ(x, Rational(7));

    x /= Rational(2);
    EXPECT_EQ(x, Rational(7, 2));

    EXPECT_DEATH(x /= Rational(0), "Can't divide by zero");
}

TEST(Arithmetics, General) {
    Rational x, y, z;

    x = Rational(7, 2);
    y = x + x;
    z = x * Rational(2);
    EXPECT_EQ(y, z);

    x = Rational(1);
    x = x + x + x;
    EXPECT_EQ(x, Rational(3));

    x = x * x * x;
    EXPECT_EQ(x, Rational(27));

    x = Rational(3, 2);
    y = Rational(4);
    z = Rational(5, 7);

    EXPECT_EQ(x + y / z, Rational(71, 10));
}