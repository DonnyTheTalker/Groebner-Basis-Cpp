#include "gtest/gtest.h"
#include "MonomialDegree.h"

TEST(DegreeBasic, Constructors) {
    {
        MonomialDegree x(10);
        std::vector<MonomialDegree::DegreeType> temp(10);
        MonomialDegree y(10, temp);
        EXPECT_EQ(x, y);
        EXPECT_EQ(x.GetSumDegree(), 0);
    }

    {
        MonomialDegree x(3, {1, 2, 3});
        MonomialDegree y = std::move(x);
        EXPECT_EQ(y.GetSize(), 3);
        EXPECT_EQ(x.GetSize(), 3);
        EXPECT_NE(x, y);

        for (size_t i = 0; i < 3; i++) {
            EXPECT_EQ(x[i], 0);
            EXPECT_EQ(y[i], i + 1);
        }

        EXPECT_EQ(x.GetSumDegree(), 0);
        EXPECT_EQ(y.GetSumDegree(), 6);
    }

    {
        MonomialDegree x(3, {1, 2, 3});
        MonomialDegree y = x;
        EXPECT_EQ(x, y);
        EXPECT_EQ(x.GetSumDegree(), y.GetSumDegree());
        EXPECT_EQ(x.GetSumDegree(), 6);
    }
}

TEST(DegreeBasic, Getters) {
    MonomialDegree x(4, {3, 4, 120, 30});
    EXPECT_EQ(x.GetSize(), 4);
    EXPECT_EQ(x.GetSumDegree(), 157);
    EXPECT_EQ(x[0], 3);
    EXPECT_EQ(x[1], 4);
    EXPECT_EQ(x[2], 120);
    EXPECT_EQ(x[3], 30);
    EXPECT_DEATH(x[4], "Out of bounds");
}

TEST(DegreeArithmetics, Addition) {
    {
        MonomialDegree x(3);
        MonomialDegree y(4);
        EXPECT_DEATH(x + y, "Number of variables must be equal");
    }

    {
        MonomialDegree x(3);
        MonomialDegree y(3, {1, 2, 3});
        EXPECT_EQ(x += y, MonomialDegree(3, {1, 2, 3}));
        EXPECT_EQ(x + y, MonomialDegree(3, {2, 4, 6}));
        EXPECT_EQ(x, MonomialDegree(3, {1, 2, 3}));
        EXPECT_EQ((x + y).GetSumDegree(), x.GetSumDegree() + y.GetSumDegree());
    }

    {
        MonomialDegree x(3, {1, 1, 1});
        MonomialDegree y(3, {1, 2, 3});
        EXPECT_EQ(x += y, MonomialDegree(3, {2, 3, 4}));
        EXPECT_EQ(x + y, MonomialDegree(3, {3, 5, 7}));
        EXPECT_EQ(x, MonomialDegree(3, {2, 3, 4}));
        EXPECT_EQ((x + y).GetSumDegree(), x.GetSumDegree() + y.GetSumDegree());
    }
}

TEST(DegreeArithmetics, Substituion) {
    {
        MonomialDegree x(3);
        MonomialDegree y(4);
        EXPECT_DEATH(x - y, "Number of variables must be equal");
    }

    {
        MonomialDegree x(3, {1, 2, 3});
        MonomialDegree y(3, {1, 2, 3});
        EXPECT_EQ(x -= y, MonomialDegree(3));
        EXPECT_EQ(x.GetSumDegree(), 0);
        EXPECT_EQ(x += (y + y), MonomialDegree(3, {2, 4, 6}));
        EXPECT_EQ(x.GetSumDegree(), 12);
        EXPECT_EQ(x -= y, MonomialDegree(3, {1, 2, 3}));
        EXPECT_EQ(x.GetSumDegree(), 6);
        EXPECT_EQ(x - y, MonomialDegree(3));
        EXPECT_EQ(x -= y, MonomialDegree(3));
        EXPECT_EQ(x.GetSumDegree(), 0);
        EXPECT_DEATH(x - y, "Can't substitute from lower degree");
    }
}