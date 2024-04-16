#include "Polynomial.h"
#include "gtest/gtest.h"

namespace Groebner::Test {
template <IsSupportedField Field, IsComparator Comparator>
void CheckEqual(const Polynomial<Field, Comparator>& poly,
                const std::vector<Term<Field>>& expected) {
    for (size_t i = 0; i < poly.GetSize(); i++) {
        ASSERT_EQ(poly.GetAt(i), expected[i]);
    }
}

template <IsSupportedField Field, IsComparator Comparator>
void CheckSorted(const Polynomial<Field, Comparator>& poly) {
    for (size_t i = 0; i + 1 < poly.GetSize(); ++i) {
        ASSERT_TRUE(Comparator::IsGreater(poly.GetAt(i).degree,
                                          poly.GetAt(i + 1).degree));
    }
};

TEST(PolynomialBasic, Sorting) {
    {
        std::vector<RationalTerm> x = {
            {1, {1, 0, 0}}, {2, {1, 1, 0}}, {2, {1, 1, 1}}};

        {
            Polynomial<Rational, LexOrder> poly(x.begin(), x.end());
            CheckSorted(poly);
            CheckEqual(poly, {x[2], x[1], x[0]});
        }

        {
            Polynomial<Rational, ReverseLexOrder> poly(x.begin(), x.end());
            CheckSorted(poly);
            CheckEqual(poly, {x[0], x[1], x[2]});
        }

        {
            Polynomial<Rational, GrlexOrder> poly(x.begin(), x.end());
            CheckSorted(poly);
            CheckEqual(poly, {x[2], x[1], x[0]});
        }

        {
            Polynomial<Rational, GrevlexOrder> poly(x.begin(), x.end());
            CheckSorted(poly);
            CheckEqual(poly, {x[2], x[1], x[0]});
        }
    }

    {
        std::vector<RationalTerm> x{{1, {2, 0, 0}}, {2, {1, 1, 0}},
                                    {2, {1, 0, 1}}, {2, {0, 2, 0}},
                                    {2, {0, 1, 1}}, {2, {0, 0, 2}}};

        {
            Polynomial<Rational, GrlexOrder> poly(x.begin(), x.end());
            CheckSorted(poly);
            CheckEqual(poly, x);
        }

        {
            Polynomial<Rational, GrevlexOrder> poly(x.begin(), x.end());
            CheckSorted(poly);
            CheckEqual(poly, {x[0], x[1], x[3], x[2], x[4], x[5]});
        }
    }
}

TEST(PolynomialBasic, MonomialSquash) {
    {
        std::vector<RationalTerm> x = {{1, {1, 0, 0}}, {2, {1, 0, 0}}};
        Polynomial<Rational> poly(x.begin(), x.end());
        ASSERT_EQ(poly.GetSize(), 1);
        ASSERT_EQ(poly.GetAt(0), RationalTerm(3, {1}));
    }

    {
        std::vector<RationalTerm> x = {
            {1, {1}}, {2, {1}}, {-3, {1}}, {1, {0, 1}}};
        Polynomial<Rational> poly(x.begin(), x.end());
        ASSERT_EQ(poly.GetSize(), 1);
        ASSERT_EQ(poly.GetAt(0), RationalTerm(1, {0, 1}));
    }

    {
        std::vector<RationalTerm> x = {{1, {1}}, {2, {1}}, {-3, {1}}};
        Polynomial<Rational> poly(x.begin(), x.end());
        ASSERT_EQ(poly.GetSize(), 0);
        ASSERT_TRUE(poly.IsZero());
    }

    {
        std::vector<ModuloTerm<7>> x = {{2, {1}}, {12, {1}}};
        Polynomial<Modulo<7>> poly(x.begin(), x.end());
        ASSERT_EQ(poly.GetSize(), 0);
        ASSERT_TRUE(poly.IsZero());
    }
}

TEST(PolynomialBasic, Leading) {
    {
        std::vector<RationalTerm> x{
            {1, {1, 0, 0}}, {2, {1, 1, 0}}, {2, {1, 1, 1}}};

        {
            Polynomial<Rational, LexOrder> poly(x.begin(), x.end());
            EXPECT_EQ(poly.GetLeader(), x[2]);
        }

        {
            Polynomial<Rational, ReverseLexOrder> poly(x.begin(), x.end());
            EXPECT_EQ(poly.GetLeader(), x[0]);
        }

        {
            Polynomial<Rational, GrlexOrder> poly(x.begin(), x.end());
            EXPECT_EQ(poly.GetLeader(), x[2]);
        }

        {
            Polynomial<Rational, GrevlexOrder> poly(x.begin(), x.end());
            EXPECT_EQ(poly.GetLeader(), x[2]);
        }
    }

    {
        std::vector<RationalTerm> x{{1, {2, 0, 0}}, {2, {1, 1, 0}},
                                    {2, {1, 0, 1}}, {2, {0, 2, 0}},
                                    {2, {0, 1, 1}}, {2, {0, 0, 2}}};

        {
            Polynomial<Rational, LexOrder> poly(x.begin(), x.end());
            EXPECT_EQ(poly.GetLeader(), x[0]);
        }

        {
            Polynomial<Rational, ReverseLexOrder> poly(x.begin(), x.end());
            EXPECT_EQ(poly.GetLeader(), x[0]);
        }

        {
            Polynomial<Rational, GrlexOrder> poly(x.begin(), x.end());
            EXPECT_EQ(poly.GetLeader(), x[0]);
        }

        {
            Polynomial<Rational, GrevlexOrder> poly(x.begin(), x.end());
            EXPECT_EQ(poly.GetLeader(), x[0]);
        }
    }

    {
        std::vector<RationalTerm> x{
            {1, {4, 2}}, {1, {2, 3, 3}}, {1, {3, 1, 4}}};

        {
            Polynomial<Rational, LexOrder> poly(x.begin(), x.end());
            EXPECT_EQ(poly.GetLeader(), x[0]);
        }

        {
            Polynomial<Rational, ReverseLexOrder> poly(x.begin(), x.end());
            EXPECT_EQ(poly.GetLeader(), x[0]);
        }

        {
            Polynomial<Rational, GrlexOrder> poly(x.begin(), x.end());
            EXPECT_EQ(poly.GetLeader(), x[2]);
        }

        {
            Polynomial<Rational, GrevlexOrder> poly(x.begin(), x.end());
            EXPECT_EQ(poly.GetLeader(), x[1]);
        }
    }
}

TEST(PolynomialBasic, IsZero) {
    {
        std::vector<RationalTerm> x{
            {1, {1, 0, 0}}, {2, {1, 1, 0}}, {2, {1, 1, 1}}};

        {
            Polynomial<Rational> poly(x.begin(), x.end());
            poly -= poly;
            ASSERT_TRUE(poly.IsZero());
            ASSERT_EQ(poly.GetLeader(), RationalTerm(0, {}));
            ASSERT_EQ(poly.GetAt(0), RationalTerm(0, {}));
            ASSERT_DEATH(poly.GetAt(1), "Out of bounds");
        }
    }

    {
        std::vector<ModuloTerm<3>> x = {{2, {1}}, {12, {1}}};
        Polynomial<Modulo<3>> poly(x.begin(), x.end());
        poly = poly + poly + poly;
        ASSERT_EQ(poly.GetSize(), 0);
        ASSERT_TRUE(poly.IsZero());
    }
}

TEST(Polynomial, ReduceByLeaderCoef) {
    {
        Polynomial<Rational, LexOrder> x = {
            {3, {2, 0}}, {1, {1, 1}}, {2, {0, 1}}};
        Polynomial<Rational, LexOrder> expected = {
            {1, {2, 0}}, {{1, 3}, {1, 1}}, {{2, 3}, {0, 1}}};
        x.ReduceByLeaderCoef();
        EXPECT_EQ(x, expected);
    }

    {
        Polynomial<Modulo<5>, LexOrder> x = {
            {3, {2, 0}}, {1, {1, 1}}, {2, {0, 1}}};
        Polynomial<Modulo<5>, LexOrder> expected = {
            {1, {2, 0}}, {2, {1, 1}}, {4, {0, 1}}};
        x.ReduceByLeaderCoef();
        EXPECT_EQ(x, expected);
    }
}

TEST(PolynomialArithmetics, Addition) {
    {
        std::vector<RationalTerm> x{{1, {2, 0}}, {1, {1, 1}}, {2, {0, 1}}};
        std::vector<RationalTerm> y{
            {3, {2, 0}}, {-1, {1, 1}}, {{1, 2}, {0, 2}}};

        std::vector<RationalTerm> expected{
            {4, {2, 0}}, {{1, 2}, {0, 2}}, {2, {0, 1}}};

        {
            Polynomial<Rational, LexOrder> poly1(x.begin(), x.end());
            Polynomial<Rational, LexOrder> poly2(y.begin(), y.end());

            Polynomial<Rational, LexOrder> poly3 = poly1 + poly2;
            Polynomial<Rational, LexOrder> expected_poly(expected.begin(),
                                                         expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }

        {
            Polynomial<Rational, ReverseLexOrder> poly1(x.begin(), x.end());
            Polynomial<Rational, ReverseLexOrder> poly2(y.begin(), y.end());
            Polynomial<Rational, ReverseLexOrder> poly3 = poly1 + poly2;
            Polynomial<Rational, ReverseLexOrder> expected_poly(
                expected.begin(), expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }

        {
            Polynomial<Rational, GrlexOrder> poly1(x.begin(), x.end());
            Polynomial<Rational, GrlexOrder> poly2(y.begin(), y.end());
            Polynomial<Rational, GrlexOrder> poly3 = poly1 + poly2;
            Polynomial<Rational, GrlexOrder> expected_poly(expected.begin(),
                                                           expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }

        {
            Polynomial<Rational, GrevlexOrder> poly1(x.begin(), x.end());
            Polynomial<Rational, GrevlexOrder> poly2(y.begin(), y.end());
            Polynomial<Rational, GrevlexOrder> poly3 = poly1 + poly2;
            Polynomial<Rational, GrevlexOrder> expected_poly(expected.begin(),
                                                             expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }
    }

    {
        std::vector<ModuloTerm<7>> x{{1, {2, 0}}, {1, {1, 1}}, {2, {0, 1}}};
        std::vector<ModuloTerm<7>> y{{3, {2, 0}}, {13, {1, 1}}, {4, {0, 2}}};

        std::vector<ModuloTerm<7>> expected{
            {4, {2, 0}}, {4, {0, 2}}, {2, {0, 1}}};

        {
            Polynomial<Modulo<7>, LexOrder> poly1(x.begin(), x.end());
            Polynomial<Modulo<7>, LexOrder> poly2(y.begin(), y.end());

            Polynomial<Modulo<7>, LexOrder> poly3 = poly1 + poly2;
            Polynomial<Modulo<7>, LexOrder> expected_poly(expected.begin(),
                                                          expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }

        {
            Polynomial<Modulo<7>, ReverseLexOrder> poly1(x.begin(), x.end());
            Polynomial<Modulo<7>, ReverseLexOrder> poly2(y.begin(), y.end());
            Polynomial<Modulo<7>, ReverseLexOrder> poly3 = poly1 + poly2;
            Polynomial<Modulo<7>, ReverseLexOrder> expected_poly(
                expected.begin(), expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }

        {
            Polynomial<Modulo<7>, GrlexOrder> poly1(x.begin(), x.end());
            Polynomial<Modulo<7>, GrlexOrder> poly2(y.begin(), y.end());
            Polynomial<Modulo<7>, GrlexOrder> poly3 = poly1 + poly2;
            Polynomial<Modulo<7>, GrlexOrder> expected_poly(expected.begin(),
                                                            expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }

        {
            Polynomial<Modulo<7>, GrevlexOrder> poly1(x.begin(), x.end());
            Polynomial<Modulo<7>, GrevlexOrder> poly2(y.begin(), y.end());
            Polynomial<Modulo<7>, GrevlexOrder> poly3 = poly1 + poly2;
            Polynomial<Modulo<7>, GrevlexOrder> expected_poly(expected.begin(),
                                                              expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }
    }
}

TEST(PolynomialArithmetics, Substitution) {
    {
        std::vector<RationalTerm> x{{1, {2, 0}}, {1, {1, 1}}, {2, {0, 1}}};
        std::vector<RationalTerm> y{
            {1, {2, 0}}, {-1, {1, 1}}, {{1, 2}, {0, 2}}};

        std::vector<RationalTerm> expected{
            {2, {1, 1}}, {{-1, 2}, {0, 2}}, {2, {0, 1}}};

        {
            Polynomial<Rational, LexOrder> poly1(x.begin(), x.end());
            Polynomial<Rational, LexOrder> poly2(y.begin(), y.end());

            Polynomial<Rational, LexOrder> poly3 = poly1 - poly2;
            Polynomial<Rational, LexOrder> expected_poly(expected.begin(),
                                                         expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }

        {
            Polynomial<Rational, ReverseLexOrder> poly1(x.begin(), x.end());
            Polynomial<Rational, ReverseLexOrder> poly2(y.begin(), y.end());
            Polynomial<Rational, ReverseLexOrder> poly3 = poly1 - poly2;
            Polynomial<Rational, ReverseLexOrder> expected_poly(
                expected.begin(), expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }

        {
            Polynomial<Rational, GrlexOrder> poly1(x.begin(), x.end());
            Polynomial<Rational, GrlexOrder> poly2(y.begin(), y.end());
            Polynomial<Rational, GrlexOrder> poly3 = poly1 - poly2;
            Polynomial<Rational, GrlexOrder> expected_poly(expected.begin(),
                                                           expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }

        {
            Polynomial<Rational, GrevlexOrder> poly1(x.begin(), x.end());
            Polynomial<Rational, GrevlexOrder> poly2(y.begin(), y.end());
            Polynomial<Rational, GrevlexOrder> poly3 = poly1 - poly2;
            Polynomial<Rational, GrevlexOrder> expected_poly(expected.begin(),
                                                             expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }
    }

    {
        std::vector<ModuloTerm<7>> x{{1, {2, 0}}, {1, {1, 1}}, {2, {0, 1}}};
        std::vector<ModuloTerm<7>> y{{3, {2, 0}}, {8, {1, 1}}, {4, {0, 2}}};

        std::vector<ModuloTerm<7>> expected{
            {5, {2, 0}}, {3, {0, 2}}, {2, {0, 1}}};

        {
            Polynomial<Modulo<7>, LexOrder> poly1(x.begin(), x.end());
            Polynomial<Modulo<7>, LexOrder> poly2(y.begin(), y.end());

            Polynomial<Modulo<7>, LexOrder> poly3 = poly1 - poly2;
            Polynomial<Modulo<7>, LexOrder> expected_poly(expected.begin(),
                                                          expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }

        {
            Polynomial<Modulo<7>, ReverseLexOrder> poly1(x.begin(), x.end());
            Polynomial<Modulo<7>, ReverseLexOrder> poly2(y.begin(), y.end());
            Polynomial<Modulo<7>, ReverseLexOrder> poly3 = poly1 - poly2;
            Polynomial<Modulo<7>, ReverseLexOrder> expected_poly(
                expected.begin(), expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }

        {
            Polynomial<Modulo<7>, GrlexOrder> poly1(x.begin(), x.end());
            Polynomial<Modulo<7>, GrlexOrder> poly2(y.begin(), y.end());
            Polynomial<Modulo<7>, GrlexOrder> poly3 = poly1 - poly2;
            Polynomial<Modulo<7>, GrlexOrder> expected_poly(expected.begin(),
                                                            expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }

        {
            Polynomial<Modulo<7>, GrevlexOrder> poly1(x.begin(), x.end());
            Polynomial<Modulo<7>, GrevlexOrder> poly2(y.begin(), y.end());
            Polynomial<Modulo<7>, GrevlexOrder> poly3 = poly1 - poly2;
            Polynomial<Modulo<7>, GrevlexOrder> expected_poly(expected.begin(),
                                                              expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }
    }
}

TEST(PolynomialArithmetics, Multiplication) {
    {
        std::vector<RationalTerm> x{{1, {2, 0}}, {1, {1, 1}}};
        std::vector<RationalTerm> y{{1, {1, 0}}, {1, {0, 1}}};
        std::vector<RationalTerm> expected{
            {1, {3, 0}}, {2, {2, 1}}, {1, {1, 2}}};

        {
            Polynomial<Rational, LexOrder> poly1(x.begin(), x.end());
            Polynomial<Rational, LexOrder> poly2(y.begin(), y.end());
            Polynomial<Rational, LexOrder> poly3 = poly1 * poly2;
            Polynomial<Rational, LexOrder> expected_poly(expected.begin(),
                                                         expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }

        {
            Polynomial<Rational, ReverseLexOrder> poly1(x.begin(), x.end());
            Polynomial<Rational, ReverseLexOrder> poly2(y.begin(), y.end());
            Polynomial<Rational, ReverseLexOrder> poly3 = poly1 * poly2;
            Polynomial<Rational, ReverseLexOrder> expected_poly(
                expected.begin(), expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }

        {
            Polynomial<Rational, GrlexOrder> poly1(x.begin(), x.end());
            Polynomial<Rational, GrlexOrder> poly2(y.begin(), y.end());
            Polynomial<Rational, GrlexOrder> poly3 = poly1 * poly2;
            Polynomial<Rational, GrlexOrder> expected_poly(expected.begin(),
                                                           expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }

        {
            Polynomial<Rational, GrevlexOrder> poly1(x.begin(), x.end());
            Polynomial<Rational, GrevlexOrder> poly2(y.begin(), y.end());
            Polynomial<Rational, GrevlexOrder> poly3 = poly1 * poly2;
            Polynomial<Rational, GrevlexOrder> expected_poly(expected.begin(),
                                                             expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }
    }

    {
        std::vector<RationalTerm> x{{1, {2, 0}}, {1, {1, 1}}};
        std::vector<RationalTerm> y{{1, {1, 0}}, {-1, {0, 1}}};
        std::vector<RationalTerm> expected{{1, {3, 0}}, {-1, {1, 2}}};

        {
            Polynomial<Rational, LexOrder> poly1(x.begin(), x.end());
            Polynomial<Rational, LexOrder> poly2(y.begin(), y.end());
            Polynomial<Rational, LexOrder> poly3 = poly1 * poly2;
            Polynomial<Rational, LexOrder> expected_poly(expected.begin(),
                                                         expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }

        {
            Polynomial<Rational, ReverseLexOrder> poly1(x.begin(), x.end());
            Polynomial<Rational, ReverseLexOrder> poly2(y.begin(), y.end());
            Polynomial<Rational, ReverseLexOrder> poly3 = poly1 * poly2;
            Polynomial<Rational, ReverseLexOrder> expected_poly(
                expected.begin(), expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }

        {
            Polynomial<Rational, GrlexOrder> poly1(x.begin(), x.end());
            Polynomial<Rational, GrlexOrder> poly2(y.begin(), y.end());
            Polynomial<Rational, GrlexOrder> poly3 = poly1 * poly2;
            Polynomial<Rational, GrlexOrder> expected_poly(expected.begin(),
                                                           expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }

        {
            Polynomial<Rational, GrevlexOrder> poly1(x.begin(), x.end());
            Polynomial<Rational, GrevlexOrder> poly2(y.begin(), y.end());
            Polynomial<Rational, GrevlexOrder> poly3 = poly1 * poly2;
            Polynomial<Rational, GrevlexOrder> expected_poly(expected.begin(),
                                                             expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }
    }

    {
        std::vector<ModuloTerm<7>> x{{3, {1, 1}}, {5, {1, 0}}};
        std::vector<ModuloTerm<7>> y{{2, {1, 0}}, {5, {0, 1}}};
        std::vector<ModuloTerm<7>> expected{
            {6, {2, 1}}, {1, {1, 2}}, {10, {2, 0}}, {4, {1, 1}}};

        {
            Polynomial<Modulo<7>, LexOrder> poly1(x.begin(), x.end());
            Polynomial<Modulo<7>, LexOrder> poly2(y.begin(), y.end());
            Polynomial<Modulo<7>, LexOrder> poly3 = poly1 * poly2;
            Polynomial<Modulo<7>, LexOrder> expected_poly(expected.begin(),
                                                          expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }

        {
            Polynomial<Modulo<7>, ReverseLexOrder> poly1(x.begin(), x.end());
            Polynomial<Modulo<7>, ReverseLexOrder> poly2(y.begin(), y.end());
            Polynomial<Modulo<7>, ReverseLexOrder> poly3 = poly1 * poly2;
            Polynomial<Modulo<7>, ReverseLexOrder> expected_poly(
                expected.begin(), expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }

        {
            Polynomial<Modulo<7>, GrlexOrder> poly1(x.begin(), x.end());
            Polynomial<Modulo<7>, GrlexOrder> poly2(y.begin(), y.end());
            Polynomial<Modulo<7>, GrlexOrder> poly3 = poly1 * poly2;
            Polynomial<Modulo<7>, GrlexOrder> expected_poly(expected.begin(),
                                                            expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }

        {
            Polynomial<Modulo<7>, GrevlexOrder> poly1(x.begin(), x.end());
            Polynomial<Modulo<7>, GrevlexOrder> poly2(y.begin(), y.end());
            Polynomial<Modulo<7>, GrevlexOrder> poly3 = poly1 * poly2;
            Polynomial<Modulo<7>, GrevlexOrder> expected_poly(expected.begin(),
                                                              expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }
    }

    {
        std::vector<ModuloTerm<7>> x{{1, {1, 0}}, {5, {0, 1}}};
        std::vector<ModuloTerm<7>> y{{4, {1, 0}}, {8, {0, 1}}};
        std::vector<ModuloTerm<7>> expected{{4, {2, 0}}, {5, {0, 2}}};

        {
            Polynomial<Modulo<7>, LexOrder> poly1(x.begin(), x.end());
            Polynomial<Modulo<7>, LexOrder> poly2(y.begin(), y.end());
            Polynomial<Modulo<7>, LexOrder> poly3 = poly1 * poly2;
            Polynomial<Modulo<7>, LexOrder> expected_poly(expected.begin(),
                                                          expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }

        {
            Polynomial<Modulo<7>, ReverseLexOrder> poly1(x.begin(), x.end());
            Polynomial<Modulo<7>, ReverseLexOrder> poly2(y.begin(), y.end());
            Polynomial<Modulo<7>, ReverseLexOrder> poly3 = poly1 * poly2;
            Polynomial<Modulo<7>, ReverseLexOrder> expected_poly(
                expected.begin(), expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }

        {
            Polynomial<Modulo<7>, GrlexOrder> poly1(x.begin(), x.end());
            Polynomial<Modulo<7>, GrlexOrder> poly2(y.begin(), y.end());
            Polynomial<Modulo<7>, GrlexOrder> poly3 = poly1 * poly2;
            Polynomial<Modulo<7>, GrlexOrder> expected_poly(expected.begin(),
                                                            expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }

        {
            Polynomial<Modulo<7>, GrevlexOrder> poly1(x.begin(), x.end());
            Polynomial<Modulo<7>, GrevlexOrder> poly2(y.begin(), y.end());
            Polynomial<Modulo<7>, GrevlexOrder> poly3 = poly1 * poly2;
            Polynomial<Modulo<7>, GrevlexOrder> expected_poly(expected.begin(),
                                                              expected.end());

            CheckSorted(poly3);
            ASSERT_EQ(poly3, expected_poly);
        }
    }
}
}  // namespace Groebner::Test