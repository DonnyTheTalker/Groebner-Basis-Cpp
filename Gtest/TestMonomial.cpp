#include "Monomial.h"
#include "gtest/gtest.h"

namespace Groebner::Test {
TEST(DegreeBasic, Constructors) {
    ASSERT_NO_THROW(Monomial(10));
    ASSERT_NO_THROW(Monomial({1, 2, 3}));

    {
        std::vector<Monomial::DegreeType> temp = {1, 2, 3};
        ASSERT_NO_THROW(Monomial(temp.begin(), temp.end()));
        ASSERT_NO_THROW(Monomial(std::move(temp)));
    }

    {
        std::array<Monomial::DegreeType, 3> temp = {1, 2, 3};
        ASSERT_NO_THROW(Monomial(temp.begin(), temp.end()));
    }
}

TEST(DegreeBasic, Getters) {
    Monomial x({3, 4, 120, 30});
    ASSERT_EQ(x.GetSize(), 4);
    ASSERT_EQ(x.GetSumDegree(), 157);
    ASSERT_EQ(x.GetDegree(0), 3);
    ASSERT_EQ(x.GetDegree(1), 4);
    ASSERT_EQ(x.GetDegree(2), 120);
    ASSERT_EQ(x.GetDegree(3), 30);
    ASSERT_EQ(x.GetDegree(4), 0);
}

TEST(DegreeBasic, Setters) {
    Monomial x({3, 4, 120, 30});

    x.SetDegree(0, 5);
    ASSERT_EQ(x.GetSize(), 4);
    ASSERT_EQ(x.GetDegree(0), 5);
    ASSERT_EQ(x.GetSumDegree(), 159);

    x.SetDegree(4, 6);
    ASSERT_EQ(x.GetSize(), 5);
    ASSERT_EQ(x.GetDegree(4), 6);
    ASSERT_EQ(x.GetSumDegree(), 165);
}

TEST(DegreeBasic, Comparison) {
    {
        std::vector<Monomial::DegreeType> temp = {1, 2, 3};
        ASSERT_EQ(Monomial(temp.begin(), temp.end()), Monomial({1, 2, 3}));
        ASSERT_EQ(Monomial(std::move(temp)), Monomial({1, 2, 3}));
    }

    {
        Monomial x = {1, 2, 3};
        Monomial y = {1, 2, 3, 0};
        ASSERT_EQ(x, y);

        x.SetDegree(3, 0);
        ASSERT_EQ(x, y);
        x.SetDegree(4, 0);
        ASSERT_EQ(x, y);
        y.SetDegree(0, 0);
        ASSERT_NE(x, y);
    }
}

TEST(DegreeArithmetics, Addition) {
    {
        Monomial x(3);
        Monomial y(4);
        ASSERT_EQ(x + y, Monomial());
    }

    {
        Monomial x(3);
        Monomial y({1, 2, 3});
        ASSERT_EQ(x += y, Monomial({1, 2, 3}));
        ASSERT_EQ(x + y, Monomial({2, 4, 6}));
        ASSERT_EQ(x, Monomial({1, 2, 3}));
        ASSERT_EQ((x + y).GetSumDegree(), x.GetSumDegree() + y.GetSumDegree());
    }

    {
        Monomial x({1, 1, 1});
        Monomial y({1, 2, 3, 4});
        ASSERT_EQ(x += y, Monomial({2, 3, 4, 4}));
        ASSERT_EQ(x + y, Monomial({3, 5, 7, 8}));
        ASSERT_EQ(x, Monomial({2, 3, 4, 4}));
        ASSERT_EQ((x + y).GetSumDegree(), x.GetSumDegree() + y.GetSumDegree());
    }

    {
        Monomial x({1, 1, 1});
        Monomial y({
            1,
            2,
        });
        ASSERT_EQ(x += y, Monomial({2, 3, 1}));
        ASSERT_EQ(x + y, Monomial({3, 5, 1}));
        ASSERT_EQ(x, Monomial({2, 3, 1}));
        ASSERT_EQ((x + y).GetSumDegree(), x.GetSumDegree() + y.GetSumDegree());
    }
}

TEST(DegreeArithmetics, Substituion) {
    {
        Monomial x(3);
        Monomial y(4);
        ASSERT_EQ(x - y, Monomial());
    }

    {
        Monomial x({1, 2, 3});
        Monomial y({1, 2, 3});
        ASSERT_EQ(x -= y, Monomial(3));
        ASSERT_EQ(x.GetSumDegree(), 0);
        ASSERT_EQ(x += (y + y), Monomial({2, 4, 6}));
        ASSERT_EQ(x.GetSumDegree(), 12);
        ASSERT_EQ(x -= y, Monomial({1, 2, 3}));
        ASSERT_EQ(x.GetSumDegree(), 6);
        ASSERT_EQ(x - y, Monomial(3));
        ASSERT_EQ(x -= y, Monomial(3));
        ASSERT_EQ(x.GetSumDegree(), 0);
        ASSERT_DEATH(x - y, "Can't substitute from lower degree");
    }

    {
        Monomial x({1, 2, 3});
        Monomial y({1, 2});
        ASSERT_EQ(x - y, Monomial({0, 0, 3}));
        ASSERT_EQ(x -= y, Monomial({0, 0, 3}));
    }

    {
        Monomial x({1, 2});
        Monomial y({0, 1, 0});
        ASSERT_EQ(x - y, Monomial({1, 1}));
        ASSERT_EQ(x -= y, Monomial({1, 1}));
    }
}
}  // namespace Groebner::Test
