#include "MonomialDegree.h"
#include "gtest/gtest.h"

namespace Groebner::Test {
TEST(DegreeBasic, Constructors) {
    EXPECT_NO_THROW(MonomialDegree(10));
    EXPECT_NO_THROW(MonomialDegree({1, 2, 3}));

    {
        std::vector<MonomialDegree::DegreeType> temp = {1, 2, 3};
        EXPECT_NO_THROW(MonomialDegree(temp.begin(), temp.end()));
        EXPECT_NO_THROW(MonomialDegree(std::move(temp)));
    }

    {
        std::array<MonomialDegree::DegreeType, 3> temp = {1, 2, 3};
        EXPECT_NO_THROW(MonomialDegree(temp.begin(), temp.end()));
    }
}

TEST(DegreeBasic, Getters) {
    MonomialDegree x({3, 4, 120, 30});
    EXPECT_EQ(x.GetSize(), 4);
    EXPECT_EQ(x.GetSumDegree(), 157);
    EXPECT_EQ(x.GetDegree(0), 3);
    EXPECT_EQ(x.GetDegree(1), 4);
    EXPECT_EQ(x.GetDegree(2), 120);
    EXPECT_EQ(x.GetDegree(3), 30);
    EXPECT_EQ(x.GetDegree(4), 0);
}

TEST(DegreeBasic, Setters) {
    MonomialDegree x({3, 4, 120, 30});

    x.SetDegree(0, 5);
    EXPECT_EQ(x.GetSize(), 4);
    EXPECT_EQ(x.GetDegree(0), 5);
    EXPECT_EQ(x.GetSumDegree(), 159);

    x.SetDegree(4, 6);
    EXPECT_EQ(x.GetSize(), 5);
    EXPECT_EQ(x.GetDegree(4), 6);
    EXPECT_EQ(x.GetSumDegree(), 165);
}

TEST(DegreeBasic, Comparison) {
    {
        std::vector<MonomialDegree::DegreeType> temp = {1, 2, 3};
        EXPECT_EQ(MonomialDegree(temp.begin(), temp.end()), MonomialDegree({1, 2, 3}));
        EXPECT_EQ(MonomialDegree(std::move(temp)), MonomialDegree({1, 2, 3}));
    }

    {
        MonomialDegree x = {1, 2, 3};
        MonomialDegree y = {1, 2, 3, 0};
        EXPECT_EQ(x, y);

        x.SetDegree(3, 0);
        EXPECT_EQ(x, y);
        x.SetDegree(4, 0);
        EXPECT_EQ(x, y);
        y.SetDegree(0, 0);
        EXPECT_NE(x, y);
    }
}

TEST(DegreeArithmetics, Addition) {
    {
        MonomialDegree x(3);
        MonomialDegree y(4);
        EXPECT_EQ(x + y, MonomialDegree());
    }

    {
        MonomialDegree x(3);
        MonomialDegree y({1, 2, 3});
        EXPECT_EQ(x += y, MonomialDegree({1, 2, 3}));
        EXPECT_EQ(x + y, MonomialDegree({2, 4, 6}));
        EXPECT_EQ(x, MonomialDegree({1, 2, 3}));
        EXPECT_EQ((x + y).GetSumDegree(), x.GetSumDegree() + y.GetSumDegree());
    }

    {
        MonomialDegree x({1, 1, 1});
        MonomialDegree y({1, 2, 3, 4});
        EXPECT_EQ(x += y, MonomialDegree({2, 3, 4, 4}));
        EXPECT_EQ(x + y, MonomialDegree({3, 5, 7, 8}));
        EXPECT_EQ(x, MonomialDegree({2, 3, 4, 4}));
        EXPECT_EQ((x + y).GetSumDegree(), x.GetSumDegree() + y.GetSumDegree());
    }

    {
        MonomialDegree x({1, 1, 1});
        MonomialDegree y({1, 2,});
        EXPECT_EQ(x += y, MonomialDegree({2, 3, 1}));
        EXPECT_EQ(x + y, MonomialDegree({3, 5, 1}));
        EXPECT_EQ(x, MonomialDegree({2, 3, 1}));
        EXPECT_EQ((x + y).GetSumDegree(), x.GetSumDegree() + y.GetSumDegree());
    }
}

TEST(DegreeArithmetics, Substituion) {
    {
        MonomialDegree x(3);
        MonomialDegree y(4);
        EXPECT_EQ(x - y, MonomialDegree());
    }

    {
        MonomialDegree x({1, 2, 3});
        MonomialDegree y({1, 2, 3});
        EXPECT_EQ(x -= y, MonomialDegree(3));
        EXPECT_EQ(x.GetSumDegree(), 0);
        EXPECT_EQ(x += (y + y), MonomialDegree({2, 4, 6}));
        EXPECT_EQ(x.GetSumDegree(), 12);
        EXPECT_EQ(x -= y, MonomialDegree({1, 2, 3}));
        EXPECT_EQ(x.GetSumDegree(), 6);
        EXPECT_EQ(x - y, MonomialDegree(3));
        EXPECT_EQ(x -= y, MonomialDegree(3));
        EXPECT_EQ(x.GetSumDegree(), 0);
        EXPECT_DEATH(x - y, "Can't substitute from lower degree");
    }

    {
        MonomialDegree x({1, 2, 3});
        MonomialDegree y({1, 2});
        EXPECT_EQ(x - y, MonomialDegree({0, 0, 3}));
        EXPECT_EQ(x -= y, MonomialDegree({0, 0, 3}));
    }

    {
        MonomialDegree x({1, 2});
        MonomialDegree y({0, 1, 0});
        EXPECT_EQ(x - y, MonomialDegree({1, 1}));
        EXPECT_EQ(x -= y, MonomialDegree({1, 1}));
    }
}
}  // namespace Groebner::Test