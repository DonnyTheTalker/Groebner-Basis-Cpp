#include "MonomialCompare.h"
#include "gtest/gtest.h"

namespace Groebner::Test {
TEST(Compare, Equal) {
    Monomial x({1, 2, 3});
    Monomial y({1, 2, 3});
    Monomial z({1, 2, 4});

    EXPECT_TRUE(StraightCoordinateOrder::IsEqual(x, x));
    EXPECT_TRUE(LexOrder::IsEqual(x, x));
    EXPECT_TRUE(ReverseLexOrder::IsEqual(x, x));
    EXPECT_TRUE(GrlexOrder::IsEqual(x, x));
    EXPECT_TRUE(GrevlexOrder::IsEqual(x, x));

    EXPECT_TRUE(StraightCoordinateOrder::IsEqual(x, y));
    EXPECT_TRUE(StraightCoordinateOrder::IsEqual(y, x));
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

    EXPECT_FALSE(StraightCoordinateOrder::IsEqual(x, z));
    EXPECT_FALSE(StraightCoordinateOrder::IsEqual(z, x));
    EXPECT_FALSE(LexOrder::IsEqual(x, z));
    EXPECT_FALSE(LexOrder::IsEqual(z, x));
    EXPECT_FALSE(ReverseLexOrder::IsEqual(x, z));
    EXPECT_FALSE(ReverseLexOrder::IsEqual(z, x));
    EXPECT_FALSE(GrlexOrder::IsEqual(x, z));
    EXPECT_FALSE(GrlexOrder::IsEqual(z, x));
    EXPECT_FALSE(GrevlexOrder::IsEqual(x, z));
    EXPECT_FALSE(GrevlexOrder::IsEqual(z, x));
}

TEST(CompareStraightCoordinateOrder, Less) {
    {
        Monomial x({1, 1, 1});
        EXPECT_FALSE(StraightCoordinateOrder::IsLess(x, x));
    }
    {
        Monomial x({1, 1, 1});
        Monomial y({2, 2, 2});

        EXPECT_TRUE(StraightCoordinateOrder::IsLess(x, y));
        EXPECT_FALSE(StraightCoordinateOrder::IsLess(y, x));
    }
    {
        Monomial x({2, 3, 2});
        Monomial y({1, 4, 1});

        EXPECT_FALSE(StraightCoordinateOrder::IsLess(x, y));
        EXPECT_FALSE(StraightCoordinateOrder::IsLess(y, x));
    }

    {
        Monomial x({1, 1, 1});
        Monomial y({2, 2, 2});
        Monomial z({2, 3, 4});

        EXPECT_TRUE(StraightCoordinateOrder::IsLess(x, y));
        EXPECT_FALSE(StraightCoordinateOrder::IsLess(y, x));

        EXPECT_TRUE(StraightCoordinateOrder::IsLess(x + z, y + z));
        EXPECT_FALSE(StraightCoordinateOrder::IsLess(y + z, x + z));

        EXPECT_TRUE(StraightCoordinateOrder::IsLess(z - y, z - x));
        EXPECT_FALSE(StraightCoordinateOrder::IsLess(z - x, z - y));
    }
}

TEST(CompareStraightCoordinateOrder, Greater) {
    {
        Monomial x({1, 1, 1});
        EXPECT_FALSE(StraightCoordinateOrder::IsGreater(x, x));
    }

    {
        Monomial x({1, 1, 1});
        Monomial y({2, 2, 2});

        EXPECT_FALSE(StraightCoordinateOrder::IsGreater(x, y));
        EXPECT_TRUE(StraightCoordinateOrder::IsGreater(y, x));
    }
    {
        Monomial x({2, 3, 2});
        Monomial y({1, 4, 1});

        EXPECT_FALSE(StraightCoordinateOrder::IsGreater(x, y));
        EXPECT_FALSE(StraightCoordinateOrder::IsGreater(y, x));
    }

    {
        Monomial x({1, 1, 1});
        Monomial y({2, 2, 2});
        Monomial z({2, 3, 4});

        EXPECT_FALSE(StraightCoordinateOrder::IsGreater(x, y));
        EXPECT_TRUE(StraightCoordinateOrder::IsGreater(y, x));

        EXPECT_FALSE(StraightCoordinateOrder::IsGreater(x + z, y + z));
        EXPECT_TRUE(StraightCoordinateOrder::IsGreater(y + z, x + z));

        EXPECT_FALSE(StraightCoordinateOrder::IsGreater(z - y, z - x));
        EXPECT_TRUE(StraightCoordinateOrder::IsGreater(z - x, z - y));
    }
}

TEST(CompareStraightCoordinateOrder, LessOrEqual) {
    {
        Monomial x({1, 1, 1});
        EXPECT_TRUE(StraightCoordinateOrder::IsLessOrEqual(x, x));
    }
    {
        Monomial x({1, 1, 1});
        Monomial y({2, 2, 2});

        EXPECT_TRUE(StraightCoordinateOrder::IsLessOrEqual(x, y));
        EXPECT_FALSE(StraightCoordinateOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x({2, 3, 2});
        Monomial y({1, 4, 1});

        EXPECT_FALSE(StraightCoordinateOrder::IsLessOrEqual(x, y));
        EXPECT_FALSE(StraightCoordinateOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x({2, 3, 2});
        Monomial y({2, 3, 2});

        EXPECT_TRUE(StraightCoordinateOrder::IsLessOrEqual(x, y));
        EXPECT_TRUE(StraightCoordinateOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x({1, 1, 1});
        Monomial y({2, 2, 2});
        Monomial z({2, 3, 4});

        EXPECT_TRUE(StraightCoordinateOrder::IsLessOrEqual(x, y));
        EXPECT_FALSE(StraightCoordinateOrder::IsLessOrEqual(y, x));

        EXPECT_TRUE(StraightCoordinateOrder::IsLessOrEqual(x + z, y + z));
        EXPECT_FALSE(StraightCoordinateOrder::IsLessOrEqual(y + z, x + z));

        EXPECT_TRUE(StraightCoordinateOrder::IsLessOrEqual(z - y, z - x));
        EXPECT_FALSE(StraightCoordinateOrder::IsLessOrEqual(z - x, z - y));
    }

    {
        Monomial x({1, 1, 1});
        Monomial y({1, 1, 1});
        Monomial z({2, 3, 4});

        EXPECT_TRUE(StraightCoordinateOrder::IsLessOrEqual(x, y));
        EXPECT_TRUE(StraightCoordinateOrder::IsLessOrEqual(y, x));

        EXPECT_TRUE(StraightCoordinateOrder::IsLessOrEqual(x + z, y + z));
        EXPECT_TRUE(StraightCoordinateOrder::IsLessOrEqual(y + z, x + z));

        EXPECT_TRUE(StraightCoordinateOrder::IsLessOrEqual(z - y, z - x));
        EXPECT_TRUE(StraightCoordinateOrder::IsLessOrEqual(z - x, z - y));
    }
}

TEST(CompareStraightCoordinateOrder, GreaterOrEqual) {
    {
        Monomial x({1, 1, 1});
        EXPECT_TRUE(StraightCoordinateOrder::IsGreaterOrEqual(x, x));
    }

    {
        Monomial x({1, 1, 1});
        Monomial y({2, 2, 2});

        EXPECT_FALSE(StraightCoordinateOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(StraightCoordinateOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x({2, 3, 2});
        Monomial y({1, 4, 1});

        EXPECT_FALSE(StraightCoordinateOrder::IsGreaterOrEqual(x, y));
        EXPECT_FALSE(StraightCoordinateOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x({2, 3, 2});
        Monomial y({2, 3, 2});

        EXPECT_TRUE(StraightCoordinateOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(StraightCoordinateOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x({1, 1, 1});
        Monomial y({2, 2, 2});
        Monomial z({2, 3, 4});

        EXPECT_FALSE(StraightCoordinateOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(StraightCoordinateOrder::IsGreaterOrEqual(y, x));

        EXPECT_FALSE(StraightCoordinateOrder::IsGreaterOrEqual(x + z, y + z));
        EXPECT_TRUE(StraightCoordinateOrder::IsGreaterOrEqual(y + z, x + z));

        EXPECT_FALSE(StraightCoordinateOrder::IsGreaterOrEqual(z - y, z - x));
        EXPECT_TRUE(StraightCoordinateOrder::IsGreaterOrEqual(z - x, z - y));
    }
}

TEST(CompareLexOrder, Less) {
    {
        Monomial x({1, 1, 1});
        EXPECT_FALSE(LexOrder::IsLess(x, x));
    }
    {
        Monomial x({1, 1, 1});
        Monomial y({1, 2, 2});

        EXPECT_TRUE(LexOrder::IsLess(x, y));
        EXPECT_FALSE(LexOrder::IsLess(y, x));
    }

    {
        Monomial x({2, 3, 2});
        Monomial y({1, 4, 1});

        EXPECT_FALSE(LexOrder::IsLess(x, y));
        EXPECT_TRUE(LexOrder::IsLess(y, x));
    }

    {
        Monomial x({1, 1, 1});
        Monomial y({1, 2, 1});
        Monomial z({2, 3, 4});

        EXPECT_TRUE(LexOrder::IsLess(x, y));
        EXPECT_FALSE(LexOrder::IsLess(y, x));

        EXPECT_TRUE(LexOrder::IsLess(x + z, y + z));
        EXPECT_FALSE(LexOrder::IsLess(y + z, x + z));

        EXPECT_TRUE(LexOrder::IsLess(z - y, z - x));
        EXPECT_FALSE(LexOrder::IsLess(z - x, z - y));
    }
}

TEST(CompareLexOrder, Greater) {
    {
        Monomial x({1, 1, 1});
        EXPECT_FALSE(LexOrder::IsGreater(x, x));
    }

    {
        Monomial x({1, 1, 1});
        Monomial y({1, 2, 2});

        EXPECT_FALSE(LexOrder::IsGreater(x, y));
        EXPECT_TRUE(LexOrder::IsGreater(y, x));
    }

    {
        Monomial x({2, 3, 2});
        Monomial y({1, 4, 1});

        EXPECT_TRUE(LexOrder::IsGreater(x, y));
        EXPECT_FALSE(LexOrder::IsGreater(y, x));
    }

    {
        Monomial x({1, 2, 1});
        Monomial y({1, 1, 1});
        Monomial z({2, 3, 4});

        EXPECT_TRUE(LexOrder::IsGreater(x, y));
        EXPECT_FALSE(LexOrder::IsGreater(y, x));

        EXPECT_TRUE(LexOrder::IsGreater(x + z, y + z));
        EXPECT_FALSE(LexOrder::IsGreater(y + z, x + z));

        EXPECT_TRUE(LexOrder::IsGreater(z - y, z - x));
        EXPECT_FALSE(LexOrder::IsGreater(z - x, z - y));
    }
}

TEST(CompareLexOrder, LessOrEqual) {
    {
        Monomial x({1, 1, 1});
        EXPECT_TRUE(LexOrder::IsLessOrEqual(x, x));
    }
    {
        Monomial x({1, 1, 1});
        Monomial y({1, 2, 2});

        EXPECT_TRUE(LexOrder::IsLessOrEqual(x, y));
        EXPECT_FALSE(LexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x({2, 3, 2});
        Monomial y({1, 4, 1});

        EXPECT_FALSE(LexOrder::IsLessOrEqual(x, y));
        EXPECT_TRUE(LexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x({2, 3, 2});
        Monomial y({2, 3, 2});

        EXPECT_TRUE(LexOrder::IsLessOrEqual(x, y));
        EXPECT_TRUE(LexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x({1, 1, 1});
        Monomial y({1, 2, 1});
        Monomial z({2, 3, 4});

        EXPECT_TRUE(LexOrder::IsLessOrEqual(x, y));
        EXPECT_FALSE(LexOrder::IsLessOrEqual(y, x));

        EXPECT_TRUE(LexOrder::IsLessOrEqual(x + z, y + z));
        EXPECT_FALSE(LexOrder::IsLessOrEqual(y + z, x + z));

        EXPECT_TRUE(LexOrder::IsLessOrEqual(z - y, z - x));
        EXPECT_FALSE(LexOrder::IsLessOrEqual(z - x, z - y));
    }
}

TEST(CompareLexOrder, GreaterOrEqual) {
    {
        Monomial x({1, 1, 1});
        EXPECT_TRUE(LexOrder::IsGreaterOrEqual(x, x));
    }

    {
        Monomial x({1, 1, 1});
        Monomial y({1, 2, 2});

        EXPECT_FALSE(LexOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(LexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x({2, 3, 2});
        Monomial y({1, 4, 1});

        EXPECT_TRUE(LexOrder::IsGreaterOrEqual(x, y));
        EXPECT_FALSE(LexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x({2, 3, 2});
        Monomial y({2, 3, 2});

        EXPECT_TRUE(LexOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(LexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x({1, 2, 1});
        Monomial y({1, 1, 1});
        Monomial z({2, 3, 4});

        EXPECT_TRUE(LexOrder::IsGreaterOrEqual(x, y));
        EXPECT_FALSE(LexOrder::IsGreaterOrEqual(y, x));

        EXPECT_TRUE(LexOrder::IsGreaterOrEqual(x + z, y + z));
        EXPECT_FALSE(LexOrder::IsGreaterOrEqual(y + z, x + z));
    }
}

TEST(CompareReverseLexOrder, Less) {
    {
        Monomial x({1, 1, 1});
        EXPECT_FALSE(ReverseLexOrder::IsLess(x, x));
    }
    {
        Monomial x({1, 1, 1});
        Monomial y({1, 2, 2});

        EXPECT_FALSE(ReverseLexOrder::IsLess(x, y));
        EXPECT_TRUE(ReverseLexOrder::IsLess(y, x));
    }

    {
        Monomial x({1, 1, 2});
        Monomial y({1, 2, 2});

        EXPECT_FALSE(ReverseLexOrder::IsLess(x, y));
        EXPECT_TRUE(ReverseLexOrder::IsLess(y, x));
    }

    {
        Monomial x({2, 3, 2});
        Monomial y({1, 4, 1});

        EXPECT_TRUE(ReverseLexOrder::IsLess(x, y));
        EXPECT_FALSE(ReverseLexOrder::IsLess(y, x));
    }

    {
        Monomial x({2, 3, 2});
        Monomial y({1, 4, 1});
        Monomial z({2, 4, 21});

        EXPECT_TRUE(ReverseLexOrder::IsLess(x, y));
        EXPECT_FALSE(ReverseLexOrder::IsLess(y, x));

        EXPECT_TRUE(ReverseLexOrder::IsLess(x + z, y + z));
        EXPECT_FALSE(ReverseLexOrder::IsLess(y + z, x + z));

        EXPECT_TRUE(ReverseLexOrder::IsLess(z - y, z - x));
        EXPECT_FALSE(ReverseLexOrder::IsLess(z - x, z - y));
    }
}

TEST(CompareReverseLexOrder, Greater) {
    {
        Monomial x({1, 1, 1});
        EXPECT_FALSE(ReverseLexOrder::IsGreater(x, x));
    }

    {
        Monomial x({1, 1, 1});
        Monomial y({1, 2, 2});

        EXPECT_TRUE(ReverseLexOrder::IsGreater(x, y));
        EXPECT_FALSE(ReverseLexOrder::IsGreater(y, x));
    }

    {
        Monomial x({2, 3, 2});
        Monomial y({1, 4, 1});

        EXPECT_FALSE(ReverseLexOrder::IsGreater(x, y));
        EXPECT_TRUE(ReverseLexOrder::IsGreater(y, x));
    }

    {
        Monomial x({2, 3, 2});
        Monomial y({1, 4, 2});
        Monomial z({2, 4, 21});

        EXPECT_TRUE(ReverseLexOrder::IsGreater(x, y));
        EXPECT_FALSE(ReverseLexOrder::IsGreater(y, x));

        EXPECT_TRUE(ReverseLexOrder::IsGreater(x + z, y + z));
        EXPECT_FALSE(ReverseLexOrder::IsGreater(y + z, x + z));

        EXPECT_TRUE(ReverseLexOrder::IsGreater(z - y, z - x));
        EXPECT_FALSE(ReverseLexOrder::IsGreater(z - x, z - y));
    }
}

TEST(CompareReverseLexOrder, LessOrEqual) {
    {
        Monomial x({1, 1, 1});
        EXPECT_TRUE(ReverseLexOrder::IsLessOrEqual(x, x));
    }
    {
        Monomial x({1, 1, 1});
        Monomial y({1, 2, 2});

        EXPECT_FALSE(ReverseLexOrder::IsLessOrEqual(x, y));
        EXPECT_TRUE(ReverseLexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x({2, 3, 2});
        Monomial y({1, 4, 1});

        EXPECT_TRUE(ReverseLexOrder::IsLessOrEqual(x, y));
        EXPECT_FALSE(ReverseLexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x({2, 3, 2});
        Monomial y({2, 3, 2});

        EXPECT_TRUE(ReverseLexOrder::IsLessOrEqual(x, y));
        EXPECT_TRUE(ReverseLexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x({2, 3, 2});
        Monomial y({1, 4, 1});
        Monomial z({2, 4, 21});

        EXPECT_TRUE(ReverseLexOrder::IsLessOrEqual(x, y));
        EXPECT_FALSE(ReverseLexOrder::IsLessOrEqual(y, x));

        EXPECT_TRUE(ReverseLexOrder::IsLessOrEqual(x + z, y + z));
        EXPECT_FALSE(ReverseLexOrder::IsLessOrEqual(y + z, x + z));

        EXPECT_TRUE(ReverseLexOrder::IsLessOrEqual(z - y, z - x));
        EXPECT_FALSE(ReverseLexOrder::IsLessOrEqual(z - x, z - y));
    }
}

TEST(CompareReverseLexOrder, GreaterOrEqual) {
    {
        Monomial x({1, 1, 1});
        EXPECT_TRUE(ReverseLexOrder::IsGreaterOrEqual(x, x));
    }

    {
        Monomial x({1, 1, 1});
        Monomial y({1, 2, 2});

        EXPECT_TRUE(ReverseLexOrder::IsGreaterOrEqual(x, y));
        EXPECT_FALSE(ReverseLexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x({1, 1, 2});
        Monomial y({1, 2, 2});

        EXPECT_TRUE(ReverseLexOrder::IsGreaterOrEqual(x, y));
        EXPECT_FALSE(ReverseLexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x({2, 3, 2});
        Monomial y({1, 4, 1});

        EXPECT_FALSE(ReverseLexOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(ReverseLexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x({2, 3, 2});
        Monomial y({2, 3, 2});

        EXPECT_TRUE(ReverseLexOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(ReverseLexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x({2, 3, 2});
        Monomial y({1, 4, 2});
        Monomial z({2, 4, 21});

        EXPECT_TRUE(ReverseLexOrder::IsGreaterOrEqual(x, y));
        EXPECT_FALSE(ReverseLexOrder::IsGreaterOrEqual(y, x));

        EXPECT_TRUE(ReverseLexOrder::IsGreaterOrEqual(x + z, y + z));
        EXPECT_FALSE(ReverseLexOrder::IsGreaterOrEqual(y + z, x + z));

        EXPECT_TRUE(ReverseLexOrder::IsGreaterOrEqual(z - y, z - x));
        EXPECT_FALSE(ReverseLexOrder::IsGreaterOrEqual(z - x, z - y));
    }
}

TEST(CompareGrlexOrder, Less) {
    {
        Monomial x({1, 1, 1});
        EXPECT_FALSE(GrlexOrder::IsLess(x, x));
    }
    {
        Monomial x({2, 1, 1});
        Monomial y({1, 2, 2});

        EXPECT_TRUE(GrlexOrder::IsLess(x, y));
        EXPECT_FALSE(GrlexOrder::IsLess(y, x));
    }

    {
        Monomial x({1, 2, 2});
        Monomial y({2, 1, 2});

        EXPECT_TRUE(GrlexOrder::IsLess(x, y));
        EXPECT_FALSE(GrlexOrder::IsLess(y, x));
    }

    {
        Monomial x({2, 3, 3});
        Monomial y({2, 5, 1});

        EXPECT_TRUE(GrlexOrder::IsLess(x, y));
        EXPECT_FALSE(GrlexOrder::IsLess(y, x));
    }

    {
        Monomial x({2, 3, 3});
        Monomial y({2, 5, 1});
        Monomial z({2, 5, 6});

        EXPECT_TRUE(GrlexOrder::IsLess(x + z, y + z));
        EXPECT_FALSE(GrlexOrder::IsLess(y + z, x + z));

        EXPECT_TRUE(GrlexOrder::IsLess(z - y, z - x));
        EXPECT_FALSE(GrlexOrder::IsLess(z - x, z - y));
    }
}

TEST(CompareGrlexOrder, Greater) {
    {
        Monomial x({1, 1, 1});
        EXPECT_FALSE(GrlexOrder::IsGreater(x, x));
    }
    {
        Monomial x({2, 1, 1});
        Monomial y({1, 2, 2});

        EXPECT_FALSE(GrlexOrder::IsGreater(x, y));
        EXPECT_TRUE(GrlexOrder::IsGreater(y, x));
    }

    {
        Monomial x({1, 2, 2});
        Monomial y({2, 1, 2});

        EXPECT_FALSE(GrlexOrder::IsGreater(x, y));
        EXPECT_TRUE(GrlexOrder::IsGreater(y, x));
    }

    {
        Monomial x({2, 3, 3});
        Monomial y({2, 5, 1});

        EXPECT_FALSE(GrlexOrder::IsGreater(x, y));
        EXPECT_TRUE(GrlexOrder::IsGreater(y, x));
    }

    {
        Monomial x({2, 5, 1});
        Monomial y({2, 3, 3});
        Monomial z({2, 5, 6});

        EXPECT_TRUE(GrlexOrder::IsGreater(x + z, y + z));
        EXPECT_FALSE(GrlexOrder::IsGreater(y + z, x + z));

        EXPECT_TRUE(GrlexOrder::IsGreater(z - y, z - x));
        EXPECT_FALSE(GrlexOrder::IsGreater(z - x, z - y));
    }
}

TEST(CompareGrlexOrder, LessOrEqual) {
    {
        Monomial x({1, 1, 1});
        EXPECT_TRUE(GrlexOrder::IsLessOrEqual(x, x));
    }
    {
        Monomial x({2, 1, 1});
        Monomial y({1, 2, 2});

        EXPECT_TRUE(GrlexOrder::IsLessOrEqual(x, y));
        EXPECT_FALSE(GrlexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x({1, 2, 2});
        Monomial y({2, 1, 2});

        EXPECT_TRUE(GrlexOrder::IsLessOrEqual(x, y));
        EXPECT_FALSE(GrlexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x({2, 3, 3});
        Monomial y({2, 5, 1});

        EXPECT_TRUE(GrlexOrder::IsLessOrEqual(x, y));
        EXPECT_FALSE(GrlexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x({2, 3, 2});
        Monomial y({2, 3, 2});

        EXPECT_TRUE(GrlexOrder::IsLessOrEqual(x, y));
        EXPECT_TRUE(GrlexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x({2, 3, 3});
        Monomial y({2, 5, 1});
        Monomial z({2, 5, 6});

        EXPECT_TRUE(GrlexOrder::IsLessOrEqual(x + z, y + z));
        EXPECT_FALSE(GrlexOrder::IsLessOrEqual(y + z, x + z));

        EXPECT_TRUE(GrlexOrder::IsLessOrEqual(z - y, z - x));
        EXPECT_FALSE(GrlexOrder::IsLessOrEqual(z - x, z - y));
    }
}

TEST(CompareGrlexOrder, GreaterOrEqual) {
    {
        Monomial x({1, 1, 1});
        EXPECT_TRUE(GrlexOrder::IsGreaterOrEqual(x, x));
    }
    {
        Monomial x({2, 1, 1});
        Monomial y({1, 2, 2});

        EXPECT_FALSE(GrlexOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(GrlexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x({1, 2, 2});
        Monomial y({2, 1, 2});

        EXPECT_FALSE(GrlexOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(GrlexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x({2, 3, 3});
        Monomial y({2, 5, 1});

        EXPECT_FALSE(GrlexOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(GrlexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x({2, 3, 2});
        Monomial y({2, 3, 2});

        EXPECT_TRUE(GrlexOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(GrlexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x({2, 5, 1});
        Monomial y({2, 3, 3});
        Monomial z({2, 5, 6});

        EXPECT_TRUE(GrlexOrder::IsGreaterOrEqual(x + z, y + z));
        EXPECT_FALSE(GrlexOrder::IsGreaterOrEqual(y + z, x + z));

        EXPECT_TRUE(GrlexOrder::IsGreaterOrEqual(z - y, z - x));
        EXPECT_FALSE(GrlexOrder::IsGreaterOrEqual(z - x, z - y));
    }
}

TEST(CompareGrevlexOrder, Less) {
    {
        Monomial x({1, 1, 1});
        EXPECT_FALSE(GrevlexOrder::IsLess(x, x));
    }
    {
        Monomial x({2, 1, 1});
        Monomial y({1, 2, 2});

        EXPECT_TRUE(GrevlexOrder::IsLess(x, y));
        EXPECT_FALSE(GrevlexOrder::IsLess(y, x));
    }

    {
        Monomial x({1, 2, 2});
        Monomial y({2, 1, 2});

        EXPECT_TRUE(GrevlexOrder::IsLess(x, y));
        EXPECT_FALSE(GrevlexOrder::IsLess(y, x));
    }

    {
        Monomial x({4, 3, 1});
        Monomial y({2, 5, 1});

        EXPECT_FALSE(GrevlexOrder::IsLess(x, y));
        EXPECT_TRUE(GrevlexOrder::IsLess(y, x));
    }

    {
        Monomial x({4, 3, 1});
        Monomial y({2, 5, 1});
        Monomial z({4, 5, 3});

        EXPECT_FALSE(GrevlexOrder::IsLess(x + z, y + z));
        EXPECT_TRUE(GrevlexOrder::IsLess(y + z, x + z));

        EXPECT_FALSE(GrevlexOrder::IsLess(z - y, z - x));
        EXPECT_TRUE(GrevlexOrder::IsLess(z - x, z - y));
    }
}

TEST(CompareGrevlexOrder, Greater) {
    {
        Monomial x({1, 1, 1});
        EXPECT_FALSE(GrevlexOrder::IsGreater(x, x));
    }
    {
        Monomial x({2, 1, 1});
        Monomial y({1, 2, 2});

        EXPECT_FALSE(GrevlexOrder::IsGreater(x, y));
        EXPECT_TRUE(GrevlexOrder::IsGreater(y, x));
    }

    {
        Monomial x({1, 2, 2});
        Monomial y({2, 1, 2});

        EXPECT_FALSE(GrevlexOrder::IsGreater(x, y));
        EXPECT_TRUE(GrevlexOrder::IsGreater(y, x));
    }

    {
        Monomial x({4, 3, 1});
        Monomial y({2, 5, 1});

        EXPECT_TRUE(GrevlexOrder::IsGreater(x, y));
        EXPECT_FALSE(GrevlexOrder::IsGreater(y, x));
    }

    {
        Monomial x({2, 5, 1});
        Monomial y({4, 3, 1});
        Monomial z({4, 5, 3});

        EXPECT_FALSE(GrevlexOrder::IsGreater(x + z, y + z));
        EXPECT_TRUE(GrevlexOrder::IsGreater(y + z, x + z));

        EXPECT_FALSE(GrevlexOrder::IsGreater(z - y, z - x));
        EXPECT_TRUE(GrevlexOrder::IsGreater(z - x, z - y));
    }
}

TEST(CompareGrevlexOrder, LessOrEqual) {
    {
        Monomial x({1, 1, 1});
        EXPECT_TRUE(GrevlexOrder::IsLessOrEqual(x, x));
    }
    {
        Monomial x({2, 1, 1});
        Monomial y({1, 2, 2});

        EXPECT_TRUE(GrevlexOrder::IsLessOrEqual(x, y));
        EXPECT_FALSE(GrevlexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x({1, 2, 2});
        Monomial y({2, 1, 2});

        EXPECT_TRUE(GrevlexOrder::IsLessOrEqual(x, y));
        EXPECT_FALSE(GrevlexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x({4, 3, 1});
        Monomial y({2, 5, 1});

        EXPECT_FALSE(GrevlexOrder::IsLessOrEqual(x, y));
        EXPECT_TRUE(GrevlexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x({2, 3, 2});
        Monomial y({2, 3, 2});

        EXPECT_TRUE(GrevlexOrder::IsLessOrEqual(x, y));
        EXPECT_TRUE(GrevlexOrder::IsLessOrEqual(y, x));
    }

    {
        Monomial x({4, 3, 1});
        Monomial y({2, 5, 1});
        Monomial z({4, 5, 3});

        EXPECT_FALSE(GrevlexOrder::IsLessOrEqual(x + z, y + z));
        EXPECT_TRUE(GrevlexOrder::IsLessOrEqual(y + z, x + z));

        EXPECT_FALSE(GrevlexOrder::IsLessOrEqual(z - y, z - x));
        EXPECT_TRUE(GrevlexOrder::IsLessOrEqual(z - x, z - y));
    }
}

TEST(CompareGrevlexOrder, GreaterOrEqual) {
    {
        Monomial x({1, 1, 1});
        EXPECT_TRUE(GrevlexOrder::IsGreaterOrEqual(x, x));
    }
    {
        Monomial x({2, 1, 1});
        Monomial y({1, 2, 2});

        EXPECT_FALSE(GrevlexOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(GrevlexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x({1, 2, 2});
        Monomial y({2, 1, 2});

        EXPECT_FALSE(GrevlexOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(GrevlexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x({4, 3, 1});
        Monomial y({2, 5, 1});

        EXPECT_TRUE(GrevlexOrder::IsGreaterOrEqual(x, y));
        EXPECT_FALSE(GrevlexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x({2, 3, 2});
        Monomial y({2, 3, 2});

        EXPECT_TRUE(GrevlexOrder::IsGreaterOrEqual(x, y));
        EXPECT_TRUE(GrevlexOrder::IsGreaterOrEqual(y, x));
    }

    {
        Monomial x({2, 5, 1});
        Monomial y({4, 3, 1});
        Monomial z({4, 5, 3});

        EXPECT_FALSE(GrevlexOrder::IsGreaterOrEqual(x + z, y + z));
        EXPECT_TRUE(GrevlexOrder::IsGreaterOrEqual(y + z, x + z));

        EXPECT_FALSE(GrevlexOrder::IsGreaterOrEqual(z - y, z - x));
        EXPECT_TRUE(GrevlexOrder::IsGreaterOrEqual(z - x, z - y));
    }
}
}  // namespace Groebner::Test