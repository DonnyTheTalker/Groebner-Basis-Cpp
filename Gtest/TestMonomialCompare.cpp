#include "gtest/gtest.h"
#include "MonomialCompare.h"

TEST(Compare, Equal) {
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 2, 3}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 3}));
        Monomial z(Rational(1), MonomialDegree(3, {1, 2, 4}));

        EXPECT_TRUE(StraightOrder::IsEqual(x, x));
        EXPECT_TRUE(LexOrder::IsEqual(x, x));
        EXPECT_TRUE(ReverseLexOrder::IsEqual(x, x));
        EXPECT_TRUE(GrlexOrder::IsEqual(x, x));
        EXPECT_TRUE(GrevlexOrder::IsEqual(x, x));

        EXPECT_TRUE(StraightOrder::IsEqual(x, y));
        EXPECT_TRUE(StraightOrder::IsEqual(y, x));
        EXPECT_TRUE(LexOrder::IsEqual(x, y));
        EXPECT_TRUE(LexOrder::IsEqual(y, x));
        EXPECT_TRUE(ReverseLexOrder::IsEqual(x, y));
        EXPECT_TRUE(ReverseLexOrder::IsEqual(y, x));
        EXPECT_TRUE(GrlexOrder::IsEqual(x, y));
        EXPECT_TRUE(GrlexOrder::IsEqual(y, x));
        EXPECT_TRUE(GrevlexOrder::IsEqual(x, y));
        EXPECT_TRUE(GrevlexOrder::IsEqual(y, x));

        EXPECT_TRUE(GrevlexOrder::IsEqual(x + x, y + x));
        EXPECT_TRUE(GrevlexOrder::IsEqual(x + x, y + y));

        EXPECT_FALSE(StraightOrder::IsEqual(x, z));
        EXPECT_FALSE(StraightOrder::IsEqual(z, x));
        EXPECT_FALSE(LexOrder::IsEqual(x, z));
        EXPECT_FALSE(LexOrder::IsEqual(z, x));
        EXPECT_FALSE(ReverseLexOrder::IsEqual(x, z));
        EXPECT_FALSE(ReverseLexOrder::IsEqual(z, x));
        EXPECT_FALSE(GrlexOrder::IsEqual(x, z));
        EXPECT_FALSE(GrlexOrder::IsEqual(z, x));
        EXPECT_FALSE(GrevlexOrder::IsEqual(x, z));
        EXPECT_FALSE(GrevlexOrder::IsEqual(z, x));
    }

    {
        Monomial x(Modulo(3, 1), MonomialDegree(3, {1, 2, 3}));
        Monomial y(Modulo(3, 1), MonomialDegree(3, {1, 2, 3}));
        Monomial z(Modulo(3, 2), MonomialDegree(3, {1, 2, 4}));

        EXPECT_TRUE(StraightOrder::IsEqual(x, x));
        EXPECT_TRUE(LexOrder::IsEqual(x, x));
        EXPECT_TRUE(ReverseLexOrder::IsEqual(x, x));
        EXPECT_TRUE(GrlexOrder::IsEqual(x, x));
        EXPECT_TRUE(GrevlexOrder::IsEqual(x, x));

        EXPECT_TRUE(StraightOrder::IsEqual(x, y));
        EXPECT_TRUE(StraightOrder::IsEqual(y, x));
        EXPECT_TRUE(LexOrder::IsEqual(x, y));
        EXPECT_TRUE(LexOrder::IsEqual(y, x));
        EXPECT_TRUE(ReverseLexOrder::IsEqual(x, y));
        EXPECT_TRUE(ReverseLexOrder::IsEqual(y, x));
        EXPECT_TRUE(GrlexOrder::IsEqual(x, y));
        EXPECT_TRUE(GrlexOrder::IsEqual(y, x));
        EXPECT_TRUE(GrevlexOrder::IsEqual(x, y));
        EXPECT_TRUE(GrevlexOrder::IsEqual(y, x));

        EXPECT_TRUE(GrevlexOrder::IsEqual(x + x, y + x));
        EXPECT_TRUE(GrevlexOrder::IsEqual(x + x, y + y));

        EXPECT_FALSE(StraightOrder::IsEqual(x, z));
        EXPECT_FALSE(StraightOrder::IsEqual(z, x));
        EXPECT_FALSE(LexOrder::IsEqual(x, z));
        EXPECT_FALSE(LexOrder::IsEqual(z, x));
        EXPECT_FALSE(ReverseLexOrder::IsEqual(x, z));
        EXPECT_FALSE(ReverseLexOrder::IsEqual(z, x));
        EXPECT_FALSE(GrlexOrder::IsEqual(x, z));
        EXPECT_FALSE(GrlexOrder::IsEqual(z, x));
        EXPECT_FALSE(GrevlexOrder::IsEqual(x, z));
        EXPECT_FALSE(GrevlexOrder::IsEqual(z, x));
    }
}

TEST(CompareStraightOrder, Less) {
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        EXPECT_FALSE(StraightOrder::IsLess(x, x));
    }
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 2, 2}));

        EXPECT_TRUE(StraightOrder::IsLess(x, y));
        EXPECT_FALSE(StraightOrder::IsLess(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 2, 2}));

        EXPECT_FALSE(StraightOrder::IsLess(x, y));
        EXPECT_FALSE(StraightOrder::IsLess(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 4, 1}));

        EXPECT_FALSE(StraightOrder::IsLess(x, y));
        EXPECT_FALSE(StraightOrder::IsLess(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 2, 2}));
        Monomial z(Rational(1), MonomialDegree(3, {2, 3, 4}));

        EXPECT_TRUE(StraightOrder::IsLess(x, y));
        EXPECT_FALSE(StraightOrder::IsLess(y, x));

        EXPECT_TRUE(StraightOrder::IsLess(x + z, y + z));
        EXPECT_FALSE(StraightOrder::IsLess(y + z, x + z));

        EXPECT_TRUE(StraightOrder::IsLess(x * z, y * z));
        EXPECT_FALSE(StraightOrder::IsLess(y * z, x * z));

        EXPECT_TRUE(StraightOrder::IsLess(x - z, y - z));
        EXPECT_FALSE(StraightOrder::IsLess(y - z, x - z));

        // TODO add / tests
        // TODO add equal monomials test in *OrEqual
    }
}

TEST(CompareStraightOrder, Greater) {
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        EXPECT_FALSE(StraightOrder::IsGreater(x, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 2, 2}));

        EXPECT_FALSE(StraightOrder::IsGreater(x, y));
        EXPECT_TRUE(StraightOrder::IsGreater(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 2, 2}));

        EXPECT_FALSE(StraightOrder::IsGreater(x, y));
        EXPECT_FALSE(StraightOrder::IsGreater(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 4, 1}));

        EXPECT_FALSE(StraightOrder::IsGreater(x, y));
        EXPECT_FALSE(StraightOrder::IsGreater(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 2, 2}));
        Monomial z(Rational(1), MonomialDegree(3, {2, 3, 4}));

        EXPECT_FALSE(StraightOrder::IsGreater(x, y));
        EXPECT_TRUE(StraightOrder::IsGreater(y, x));

        EXPECT_FALSE(StraightOrder::IsGreater(x + z, y + z));
        EXPECT_TRUE(StraightOrder::IsGreater(y + z, x + z));

        EXPECT_FALSE(StraightOrder::IsGreater(x * z, y * z));
        EXPECT_TRUE(StraightOrder::IsGreater(y * z, x * z));

        EXPECT_FALSE(StraightOrder::IsGreater(x - z, y - z));
        EXPECT_TRUE(StraightOrder::IsGreater(y - z, x - z));
    }
}

TEST(CompareStraightOrder, LessOrEqual) {
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        EXPECT_TRUE(StraightOrder::IsLessOrEqual(x, x));
    }
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 2, 2}));

        EXPECT_TRUE(StraightOrder::IsLessOrEqual(x, y));
        EXPECT_FALSE(StraightOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 2, 2}));

        EXPECT_TRUE(StraightOrder::IsLessOrEqual(x, y));
        EXPECT_FALSE(StraightOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 4, 1}));

        EXPECT_FALSE(StraightOrder::IsLessOrEqual(x, y));
        EXPECT_FALSE(StraightOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 3, 2}));

        EXPECT_TRUE(StraightOrder::IsLessOrEqual(x, y));
        EXPECT_TRUE(StraightOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 2, 2}));
        Monomial z(Rational(1), MonomialDegree(3, {2, 3, 4}));

        EXPECT_TRUE(StraightOrder::IsLessOrEqual(x, y));
        EXPECT_FALSE(StraightOrder::IsLessOrEqual(y, x));

        EXPECT_TRUE(StraightOrder::IsLessOrEqual(x + z, y + z));
        EXPECT_FALSE(StraightOrder::IsLessOrEqual(y + z, x + z));

        EXPECT_TRUE(StraightOrder::IsLessOrEqual(x * z, y * z));
        EXPECT_FALSE(StraightOrder::IsLessOrEqual(y * z, x * z));

        EXPECT_TRUE(StraightOrder::IsLessOrEqual(x - z, y - z));
        EXPECT_FALSE(StraightOrder::IsLessOrEqual(y - z, x - z));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial z(Rational(1), MonomialDegree(3, {2, 3, 4}));

        EXPECT_TRUE(StraightOrder::IsLessOrEqual(x, y));
        EXPECT_TRUE(StraightOrder::IsLessOrEqual(y, x));

        EXPECT_TRUE(StraightOrder::IsLessOrEqual(x + z, y + z));
        EXPECT_TRUE(StraightOrder::IsLessOrEqual(y + z, x + z));

        EXPECT_TRUE(StraightOrder::IsLessOrEqual(x * z, y * z));
        EXPECT_TRUE(StraightOrder::IsLessOrEqual(y * z, x * z));

        EXPECT_TRUE(StraightOrder::IsLessOrEqual(x - z, y - z));
        EXPECT_TRUE(StraightOrder::IsLessOrEqual(y - z, x - z));
    }
}

TEST(CompareStraightOrder, GreaterOrEqual) {
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        EXPECT_TRUE(StraightOrder::IsGreaterOrEqual(x, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 2, 2}));

        EXPECT_FALSE(StraightOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(StraightOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 2, 2}));

        EXPECT_FALSE(StraightOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(StraightOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 4, 1}));

        EXPECT_FALSE(StraightOrder::IsGreaterOrEqual(x, y));
        EXPECT_FALSE(StraightOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 3, 2}));

        EXPECT_TRUE(StraightOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(StraightOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 2, 2}));
        Monomial z(Rational(1), MonomialDegree(3, {2, 3, 4}));

        EXPECT_FALSE(StraightOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(StraightOrder::IsGreaterOrEqual(y, x));

        EXPECT_FALSE(StraightOrder::IsGreaterOrEqual(x + z, y + z));
        EXPECT_TRUE(StraightOrder::IsGreaterOrEqual(y + z, x + z));

        EXPECT_FALSE(StraightOrder::IsGreaterOrEqual(x * z, y * z));
        EXPECT_TRUE(StraightOrder::IsGreaterOrEqual(y * z, x * z));

        EXPECT_FALSE(StraightOrder::IsGreaterOrEqual(x - z, y - z));
        EXPECT_TRUE(StraightOrder::IsGreaterOrEqual(y - z, x - z));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial z(Rational(1), MonomialDegree(3, {2, 3, 4}));

        EXPECT_TRUE(StraightOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(StraightOrder::IsGreaterOrEqual(y, x));

        EXPECT_TRUE(StraightOrder::IsGreaterOrEqual(x + z, y + z));
        EXPECT_TRUE(StraightOrder::IsGreaterOrEqual(y + z, x + z));

        EXPECT_TRUE(StraightOrder::IsGreaterOrEqual(x * z, y * z));
        EXPECT_TRUE(StraightOrder::IsGreaterOrEqual(y * z, x * z));

        EXPECT_TRUE(StraightOrder::IsGreaterOrEqual(x - z, y - z));
        EXPECT_TRUE(StraightOrder::IsGreaterOrEqual(y - z, x - z));
    }
}

TEST(CompareLexOrder, Less) {
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        EXPECT_FALSE(LexOrder::IsLess(x, x));
    }
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 2}));

        EXPECT_TRUE(LexOrder::IsLess(x, y));
        EXPECT_FALSE(LexOrder::IsLess(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 2}));

        EXPECT_TRUE(LexOrder::IsLess(x, y));
        EXPECT_FALSE(LexOrder::IsLess(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 4, 1}));

        EXPECT_FALSE(LexOrder::IsLess(x, y));
        EXPECT_TRUE(LexOrder::IsLess(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 1}));
        Monomial z(Rational(1), MonomialDegree(3, {2, 3, 4}));

        EXPECT_TRUE(LexOrder::IsLess(x, y));
        EXPECT_FALSE(LexOrder::IsLess(y, x));

        EXPECT_TRUE(LexOrder::IsLess(x + z, y + z));
        EXPECT_FALSE(LexOrder::IsLess(y + z, x + z));

        EXPECT_TRUE(LexOrder::IsLess(x - z, y - z));
        EXPECT_FALSE(LexOrder::IsLess(y - z, x - z));

        EXPECT_TRUE(LexOrder::IsLess(x * z, y * z));
        EXPECT_FALSE(LexOrder::IsLess(y * z, x * z));
    }
}

TEST(CompareLexOrder, Greater) {
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        EXPECT_FALSE(LexOrder::IsGreater(x, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 2}));

        EXPECT_FALSE(LexOrder::IsGreater(x, y));
        EXPECT_TRUE(LexOrder::IsGreater(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 2}));

        EXPECT_FALSE(LexOrder::IsGreater(x, y));
        EXPECT_TRUE(LexOrder::IsGreater(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 4, 1}));

        EXPECT_TRUE(LexOrder::IsGreater(x, y));
        EXPECT_FALSE(LexOrder::IsGreater(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 2, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial z(Rational(1), MonomialDegree(3, {2, 3, 4}));

        EXPECT_TRUE(LexOrder::IsGreater(x, y));
        EXPECT_FALSE(LexOrder::IsGreater(y, x));

        EXPECT_TRUE(LexOrder::IsGreater(x + z, y + z));
        EXPECT_FALSE(LexOrder::IsGreater(y + z, x + z));

        EXPECT_TRUE(LexOrder::IsGreater(x - z, y - z));
        EXPECT_FALSE(LexOrder::IsGreater(y - z, x - z));

        EXPECT_TRUE(LexOrder::IsGreater(x * z, y * z));
        EXPECT_FALSE(LexOrder::IsGreater(y * z, x * z));
    }
}

TEST(CompareLexOrder, LessOrEqual) {
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        EXPECT_TRUE(LexOrder::IsLessOrEqual(x, x));
    }
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 2}));

        EXPECT_TRUE(LexOrder::IsLessOrEqual(x, y));
        EXPECT_FALSE(LexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 2}));

        EXPECT_TRUE(LexOrder::IsLessOrEqual(x, y));
        EXPECT_FALSE(LexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 4, 1}));

        EXPECT_FALSE(LexOrder::IsLessOrEqual(x, y));
        EXPECT_TRUE(LexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 3, 2}));

        EXPECT_TRUE(LexOrder::IsLessOrEqual(x, y));
        EXPECT_TRUE(LexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 1}));
        Monomial z(Rational(1), MonomialDegree(3, {2, 3, 4}));

        EXPECT_TRUE(LexOrder::IsLessOrEqual(x, y));
        EXPECT_FALSE(LexOrder::IsLessOrEqual(y, x));

        EXPECT_TRUE(LexOrder::IsLessOrEqual(x + z, y + z));
        EXPECT_FALSE(LexOrder::IsLessOrEqual(y + z, x + z));

        EXPECT_TRUE(LexOrder::IsLessOrEqual(x - z, y - z));
        EXPECT_FALSE(LexOrder::IsLessOrEqual(y - z, x - z));

        EXPECT_TRUE(LexOrder::IsLessOrEqual(x + z, y + z));
        EXPECT_FALSE(LexOrder::IsLessOrEqual(y * z, x * z));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial z(Rational(1), MonomialDegree(3, {2, 3, 4}));

        EXPECT_TRUE(LexOrder::IsLessOrEqual(x, y));
        EXPECT_TRUE(LexOrder::IsLessOrEqual(y, x));

        EXPECT_TRUE(LexOrder::IsLessOrEqual(x + z, y + z));
        EXPECT_TRUE(LexOrder::IsLessOrEqual(y + z, x + z));

        EXPECT_TRUE(LexOrder::IsLessOrEqual(y - z, x - z));
        EXPECT_TRUE(LexOrder::IsLessOrEqual(y - z, x - z));

        EXPECT_TRUE(LexOrder::IsLessOrEqual(y * z, x * z));
        EXPECT_TRUE(LexOrder::IsLessOrEqual(y + z, x + z));
    }
}

TEST(CompareLexOrder, GreaterOrEqual) {
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        EXPECT_TRUE(LexOrder::IsGreaterOrEqual(x, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 2}));

        EXPECT_FALSE(LexOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(LexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 2}));

        EXPECT_FALSE(LexOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(LexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 4, 1}));

        EXPECT_TRUE(LexOrder::IsGreaterOrEqual(x, y));
        EXPECT_FALSE(LexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 3, 2}));

        EXPECT_TRUE(LexOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(LexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 2, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial z(Rational(1), MonomialDegree(3, {2, 3, 4}));

        EXPECT_TRUE(LexOrder::IsGreaterOrEqual(x, y));
        EXPECT_FALSE(LexOrder::IsGreaterOrEqual(y, x));

        EXPECT_TRUE(LexOrder::IsGreaterOrEqual(x + z, y + z));
        EXPECT_FALSE(LexOrder::IsGreaterOrEqual(y + z, x + z));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial z(Rational(1), MonomialDegree(3, {2, 3, 4}));

        EXPECT_TRUE(LexOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(LexOrder::IsGreaterOrEqual(y, x));

        EXPECT_TRUE(LexOrder::IsGreaterOrEqual(x + z, y + z));
        EXPECT_TRUE(LexOrder::IsGreaterOrEqual(y + z, x + z));

        EXPECT_TRUE(LexOrder::IsGreaterOrEqual(x - z, y - z));
        EXPECT_TRUE(LexOrder::IsGreaterOrEqual(y - z, x - z));

        EXPECT_TRUE(LexOrder::IsGreaterOrEqual(x * z, y * z));
        EXPECT_TRUE(LexOrder::IsGreaterOrEqual(y * z, x * z));
    }
}

TEST(CompareReverseLexOrder, Less) {
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        EXPECT_FALSE(ReverseLexOrder::IsLess(x, x));
    }
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 2}));

        EXPECT_FALSE(ReverseLexOrder::IsLess(x, y));
        EXPECT_TRUE(ReverseLexOrder::IsLess(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 2}));

        EXPECT_FALSE(ReverseLexOrder::IsLess(x, y));
        EXPECT_TRUE(ReverseLexOrder::IsLess(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 4, 1}));

        EXPECT_TRUE(ReverseLexOrder::IsLess(x, y));
        EXPECT_FALSE(ReverseLexOrder::IsLess(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 4, 1}));
        Monomial z(Rational(1), MonomialDegree(3, {1, 4, 21}));

        EXPECT_TRUE(ReverseLexOrder::IsLess(x, y));
        EXPECT_FALSE(ReverseLexOrder::IsLess(y, x));

        EXPECT_TRUE(ReverseLexOrder::IsLess(x + z, y + z));
        EXPECT_FALSE(ReverseLexOrder::IsLess(y + z, x + z));

        EXPECT_TRUE(ReverseLexOrder::IsLess(x - z, y - z));
        EXPECT_FALSE(ReverseLexOrder::IsLess(y - z, x - z));

        EXPECT_TRUE(ReverseLexOrder::IsLess(x * z, y * z));
        EXPECT_FALSE(ReverseLexOrder::IsLess(y * z, x * z));
    }
}

TEST(CompareReverseLexOrder, Greater) {
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        EXPECT_FALSE(ReverseLexOrder::IsGreater(x, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 2}));

        EXPECT_TRUE(ReverseLexOrder::IsGreater(x, y));
        EXPECT_FALSE(ReverseLexOrder::IsGreater(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 2}));

        EXPECT_TRUE(ReverseLexOrder::IsGreater(x, y));
        EXPECT_FALSE(ReverseLexOrder::IsGreater(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 4, 1}));

        EXPECT_FALSE(ReverseLexOrder::IsGreater(x, y));
        EXPECT_TRUE(ReverseLexOrder::IsGreater(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 4, 2}));
        Monomial z(Rational(1), MonomialDegree(3, {1, 4, 21}));

        EXPECT_TRUE(ReverseLexOrder::IsGreater(x, y));
        EXPECT_FALSE(ReverseLexOrder::IsGreater(y, x));

        EXPECT_TRUE(ReverseLexOrder::IsGreater(x + z, y + z));
        EXPECT_FALSE(ReverseLexOrder::IsGreater(y + z, x + z));

        EXPECT_TRUE(ReverseLexOrder::IsGreater(x - z, y - z));
        EXPECT_FALSE(ReverseLexOrder::IsGreater(y - z, x - z));

        EXPECT_TRUE(ReverseLexOrder::IsGreater(x * z, y * z));
        EXPECT_FALSE(ReverseLexOrder::IsGreater(y * z, x * z));
    }
}

TEST(CompareReverseLexOrder, LessOrEqual) {
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        EXPECT_TRUE(ReverseLexOrder::IsLessOrEqual(x, x));
    }
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 2}));

        EXPECT_FALSE(ReverseLexOrder::IsLessOrEqual(x, y));
        EXPECT_TRUE(ReverseLexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 2}));

        EXPECT_FALSE(ReverseLexOrder::IsLessOrEqual(x, y));
        EXPECT_TRUE(ReverseLexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 4, 1}));

        EXPECT_TRUE(ReverseLexOrder::IsLessOrEqual(x, y));
        EXPECT_FALSE(ReverseLexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 3, 2}));

        EXPECT_TRUE(ReverseLexOrder::IsLessOrEqual(x, y));
        EXPECT_TRUE(ReverseLexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 4, 1}));
        Monomial z(Rational(1), MonomialDegree(3, {1, 4, 21}));

        EXPECT_TRUE(ReverseLexOrder::IsLessOrEqual(x, y));
        EXPECT_FALSE(ReverseLexOrder::IsLessOrEqual(y, x));

        EXPECT_TRUE(ReverseLexOrder::IsLessOrEqual(x + z, y + z));
        EXPECT_FALSE(ReverseLexOrder::IsLessOrEqual(y + z, x + z));

        EXPECT_TRUE(ReverseLexOrder::IsLessOrEqual(x - z, y - z));
        EXPECT_FALSE(ReverseLexOrder::IsLessOrEqual(y - z, x - z));

        EXPECT_TRUE(ReverseLexOrder::IsLessOrEqual(x * z, y * z));
        EXPECT_FALSE(ReverseLexOrder::IsLessOrEqual(y * z, x * z));
    }
}

TEST(CompareReverseLexOrder, GreaterOrEqual) {
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        EXPECT_TRUE(ReverseLexOrder::IsGreaterOrEqual(x, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 2}));

        EXPECT_TRUE(ReverseLexOrder::IsGreaterOrEqual(x, y));
        EXPECT_FALSE(ReverseLexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 2}));

        EXPECT_TRUE(ReverseLexOrder::IsGreaterOrEqual(x, y));
        EXPECT_FALSE(ReverseLexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 4, 1}));

        EXPECT_FALSE(ReverseLexOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(ReverseLexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 3, 2}));

        EXPECT_TRUE(ReverseLexOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(ReverseLexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 4, 2}));
        Monomial z(Rational(1), MonomialDegree(3, {1, 4, 21}));

        EXPECT_TRUE(ReverseLexOrder::IsGreaterOrEqual(x, y));
        EXPECT_FALSE(ReverseLexOrder::IsGreaterOrEqual(y, x));

        EXPECT_TRUE(ReverseLexOrder::IsGreaterOrEqual(x + z, y + z));
        EXPECT_FALSE(ReverseLexOrder::IsGreaterOrEqual(y + z, x + z));

        EXPECT_TRUE(ReverseLexOrder::IsGreaterOrEqual(x - z, y - z));
        EXPECT_FALSE(ReverseLexOrder::IsGreaterOrEqual(y - z, x - z));

        EXPECT_TRUE(ReverseLexOrder::IsGreaterOrEqual(x * z, y * z));
        EXPECT_FALSE(ReverseLexOrder::IsGreaterOrEqual(y * z, x * z));
    }
}

TEST(CompareGrlexOrder, Less) {
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        EXPECT_FALSE(GrlexOrder::IsLess(x, x));
    }
    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 2}));

        EXPECT_TRUE(GrlexOrder::IsLess(x, y));
        EXPECT_FALSE(GrlexOrder::IsLess(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 2, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 1, 2}));

        EXPECT_TRUE(GrlexOrder::IsLess(x, y));
        EXPECT_FALSE(GrlexOrder::IsLess(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 3}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 5, 1}));

        EXPECT_TRUE(GrlexOrder::IsLess(x, y));
        EXPECT_FALSE(GrlexOrder::IsLess(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 3}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 5, 1}));
        Monomial z(Rational(1), MonomialDegree(3, {2, 5, 6}));

        EXPECT_TRUE(GrlexOrder::IsLess(x + z, y + z));
        EXPECT_FALSE(GrlexOrder::IsLess(y + z, x + z));

        EXPECT_TRUE(GrlexOrder::IsLess(x * z, y * z));
        EXPECT_FALSE(GrlexOrder::IsLess(y * z, x * z));

        EXPECT_TRUE(GrlexOrder::IsLess(x - z, y - z));
        EXPECT_FALSE(GrlexOrder::IsLess(y - z, x - z));
    }
}

TEST(CompareGrlexOrder, Greater) {
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        EXPECT_FALSE(GrlexOrder::IsGreater(x, x));
    }
    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 2}));

        EXPECT_FALSE(GrlexOrder::IsGreater(x, y));
        EXPECT_TRUE(GrlexOrder::IsGreater(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 2, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 1, 2}));

        EXPECT_FALSE(GrlexOrder::IsGreater(x, y));
        EXPECT_TRUE(GrlexOrder::IsGreater(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 3}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 5, 1}));

        EXPECT_FALSE(GrlexOrder::IsGreater(x, y));
        EXPECT_TRUE(GrlexOrder::IsGreater(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 5, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 3, 3}));
        Monomial z(Rational(1), MonomialDegree(3, {2, 5, 6}));

        EXPECT_TRUE(GrlexOrder::IsGreater(x + z, y + z));
        EXPECT_FALSE(GrlexOrder::IsGreater(y + z, x + z));

        EXPECT_TRUE(GrlexOrder::IsGreater(x * z, y * z));
        EXPECT_FALSE(GrlexOrder::IsGreater(y * z, x * z));

        EXPECT_TRUE(GrlexOrder::IsGreater(x - z, y - z));
        EXPECT_FALSE(GrlexOrder::IsGreater(y - z, x - z));
    }
}

TEST(CompareGrlexOrder, LessOrEqual) {
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        EXPECT_TRUE(GrlexOrder::IsLessOrEqual(x, x));
    }
    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 2}));

        EXPECT_TRUE(GrlexOrder::IsLessOrEqual(x, y));
        EXPECT_FALSE(GrlexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 2, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 1, 2}));

        EXPECT_TRUE(GrlexOrder::IsLessOrEqual(x, y));
        EXPECT_FALSE(GrlexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 3}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 5, 1}));

        EXPECT_TRUE(GrlexOrder::IsLessOrEqual(x, y));
        EXPECT_FALSE(GrlexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 3, 2}));

        EXPECT_TRUE(GrlexOrder::IsLessOrEqual(x, y));
        EXPECT_TRUE(GrlexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 3}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 5, 1}));
        Monomial z(Rational(1), MonomialDegree(3, {2, 5, 6}));

        EXPECT_TRUE(GrlexOrder::IsLessOrEqual(x + z, y + z));
        EXPECT_FALSE(GrlexOrder::IsLessOrEqual(y + z, x + z));

        EXPECT_TRUE(GrlexOrder::IsLessOrEqual(x * z, y * z));
        EXPECT_FALSE(GrlexOrder::IsLessOrEqual(y * z, x * z));

        EXPECT_TRUE(GrlexOrder::IsLessOrEqual(x - z, y - z));
        EXPECT_FALSE(GrlexOrder::IsLessOrEqual(y - z, x - z));
    }
}

TEST(CompareGrlexOrder, GreaterOrEqual) {
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        EXPECT_TRUE(GrlexOrder::IsGreaterOrEqual(x, x));
    }
    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 2}));

        EXPECT_FALSE(GrlexOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(GrlexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 2, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 1, 2}));

        EXPECT_FALSE(GrlexOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(GrlexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 3}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 5, 1}));

        EXPECT_FALSE(GrlexOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(GrlexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 3, 2}));

        EXPECT_TRUE(GrlexOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(GrlexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 5, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 3, 3}));
        Monomial z(Rational(1), MonomialDegree(3, {2, 5, 6}));

        EXPECT_TRUE(GrlexOrder::IsGreaterOrEqual(x + z, y + z));
        EXPECT_FALSE(GrlexOrder::IsGreaterOrEqual(y + z, x + z));

        EXPECT_TRUE(GrlexOrder::IsGreaterOrEqual(x * z, y * z));
        EXPECT_FALSE(GrlexOrder::IsGreaterOrEqual(y * z, x * z));

        EXPECT_TRUE(GrlexOrder::IsGreaterOrEqual(x - z, y - z));
        EXPECT_FALSE(GrlexOrder::IsGreaterOrEqual(y - z, x - z));
    }
}

TEST(CompareGrevlexOrder, Less) {
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        EXPECT_FALSE(GrevlexOrder::IsLess(x, x));
    }
    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 2}));

        EXPECT_TRUE(GrevlexOrder::IsLess(x, y));
        EXPECT_FALSE(GrevlexOrder::IsLess(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 2, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 1, 2}));

        EXPECT_TRUE(GrevlexOrder::IsLess(x, y));
        EXPECT_FALSE(GrevlexOrder::IsLess(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {4, 3, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 5, 1}));

        EXPECT_FALSE(GrevlexOrder::IsLess(x, y));
        EXPECT_TRUE(GrevlexOrder::IsLess(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {4, 3, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 5, 1}));
        Monomial z(Rational(1), MonomialDegree(3, {3, 3, 3}));

        EXPECT_FALSE(GrevlexOrder::IsLess(x + z, y + z));
        EXPECT_TRUE(GrevlexOrder::IsLess(y + z, x + z));

        EXPECT_FALSE(GrevlexOrder::IsLess(x * z, y * z));
        EXPECT_TRUE(GrevlexOrder::IsLess(y * z, x * z));

        EXPECT_FALSE(GrevlexOrder::IsLess(x - z, y -z));
        EXPECT_TRUE(GrevlexOrder::IsLess(y - z, x - z));
    }
}

TEST(CompareGrevlexOrder, Greater) {
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        EXPECT_FALSE(GrevlexOrder::IsGreater(x, x));
    }
    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 2}));

        EXPECT_FALSE(GrevlexOrder::IsGreater(x, y));
        EXPECT_TRUE(GrevlexOrder::IsGreater(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 2, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 1, 2}));

        EXPECT_FALSE(GrevlexOrder::IsGreater(x, y));
        EXPECT_TRUE(GrevlexOrder::IsGreater(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {4, 3, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 5, 1}));

        EXPECT_TRUE(GrevlexOrder::IsGreater(x, y));
        EXPECT_FALSE(GrevlexOrder::IsGreater(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 5, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {4, 3, 1}));
        Monomial z(Rational(1), MonomialDegree(3, {3, 3, 3}));

        EXPECT_FALSE(GrevlexOrder::IsGreater(x + z, y + z));
        EXPECT_TRUE(GrevlexOrder::IsGreater(y + z, x + z));

        EXPECT_FALSE(GrevlexOrder::IsGreater(x * z, y * z));
        EXPECT_TRUE(GrevlexOrder::IsGreater(y * z, x * z));

        EXPECT_FALSE(GrevlexOrder::IsGreater(x - z, y -z));
        EXPECT_TRUE(GrevlexOrder::IsGreater(y - z, x - z));
    }
}

TEST(CompareGrevlexOrder, LessOrEqual) {
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        EXPECT_TRUE(GrevlexOrder::IsLessOrEqual(x, x));
    }
    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 2}));

        EXPECT_TRUE(GrevlexOrder::IsLessOrEqual(x, y));
        EXPECT_FALSE(GrevlexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 2, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 1, 2}));

        EXPECT_TRUE(GrevlexOrder::IsLessOrEqual(x, y));
        EXPECT_FALSE(GrevlexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {4, 3, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 5, 1}));

        EXPECT_FALSE(GrevlexOrder::IsLessOrEqual(x, y));
        EXPECT_TRUE(GrevlexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 3, 2}));

        EXPECT_TRUE(GrevlexOrder::IsLessOrEqual(x, y));
        EXPECT_TRUE(GrevlexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {4, 3, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 5, 1}));
        Monomial z(Rational(1), MonomialDegree(3, {3, 3, 3}));

        EXPECT_FALSE(GrevlexOrder::IsLessOrEqual(x + z, y + z));
        EXPECT_TRUE(GrevlexOrder::IsLessOrEqual(y + z, x + z));

        EXPECT_FALSE(GrevlexOrder::IsLessOrEqual(x * z, y * z));
        EXPECT_TRUE(GrevlexOrder::IsLessOrEqual(y * z, x * z));

        EXPECT_FALSE(GrevlexOrder::IsLessOrEqual(x - z, y -z));
        EXPECT_TRUE(GrevlexOrder::IsLessOrEqual(y - z, x - z));
    }
}

TEST(CompareGrevlexOrder, GreaterOrEqual) {
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        EXPECT_TRUE(GrevlexOrder::IsGreaterOrEqual(x, x));
    }
    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 2, 2}));

        EXPECT_FALSE(GrevlexOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(GrevlexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 2, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 1, 2}));

        EXPECT_FALSE(GrevlexOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(GrevlexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {4, 3, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 5, 1}));

        EXPECT_TRUE(GrevlexOrder::IsGreaterOrEqual(x, y));
        EXPECT_FALSE(GrevlexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 3, 2}));
        Monomial y(Rational(1), MonomialDegree(3, {2, 3, 2}));

        EXPECT_TRUE(GrevlexOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(GrevlexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {2, 5, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {4, 3, 1}));
        Monomial z(Rational(1), MonomialDegree(3, {3, 3, 3}));

        EXPECT_FALSE(GrevlexOrder::IsGreaterOrEqual(x + z, y + z));
        EXPECT_TRUE(GrevlexOrder::IsGreaterOrEqual(y + z, x + z));

        EXPECT_FALSE(GrevlexOrder::IsGreaterOrEqual(x * z, y * z));
        EXPECT_TRUE(GrevlexOrder::IsGreaterOrEqual(y * z, x * z));

        EXPECT_FALSE(GrevlexOrder::IsGreaterOrEqual(x - z, y -z));
        EXPECT_TRUE(GrevlexOrder::IsGreaterOrEqual (y - z, x - z));
    }
}