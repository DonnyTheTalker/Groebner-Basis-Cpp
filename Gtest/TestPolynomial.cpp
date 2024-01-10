#include "gtest/gtest.h"
#include "Polynomial.h"

template<IsField Field, IsComparator Comparator>
void CheckEqual(const Polynomial<Field, Comparator> &poly, const std::vector<Monomial<Field>> &expected) {
    const std::vector<Monomial<Field>> &monoms = poly.GetMonomials();
    ASSERT_EQ(monoms.size(), expected.size());
    for (size_t i = 0; i < monoms.size(); ++i) {
        ASSERT_EQ(monoms[i], expected[i]);
    }
};

template<IsField Field, IsComparator Comparator>
void CheckSorted(const Polynomial<Field, Comparator> &poly) {
    const std::vector<Monomial<Field>> &monoms = poly.GetMonomials();
    for (size_t i = 0; i + 1 < monoms.size(); ++i) {
        ASSERT_TRUE(Comparator::IsGreater(monoms[i], monoms[i + 1]));
    }
};

TEST(PolynomialBasic, Sorting) {
    {
        std::vector<Monomial<Rational>> x{Monomial(Rational(1), MonomialDegree(3, {1, 0, 0})),
                                          Monomial(Rational(2), MonomialDegree(3, {1, 1, 0})),
                                          Monomial(Rational(2), MonomialDegree(3, {1, 1, 1})),
        };

        {
            Polynomial<Rational, LexOrder> poly(x);
            CheckSorted(poly);
            CheckEqual(poly, {x[2], x[1], x[0]});
        }

        {
            Polynomial<Rational, ReverseLexOrder> poly(x);
            CheckSorted(poly);
            CheckEqual(poly, {x[0], x[1], x[2]});
        }

        {
            Polynomial<Rational, GrlexOrder> poly(x);
            CheckSorted(poly);
            CheckEqual(poly, {x[2], x[1], x[0]});
        }

        {
            Polynomial<Rational, GrevlexOrder> poly(x);
            CheckSorted(poly);
            CheckEqual(poly, {x[2], x[1], x[0]});
        }
    }

    {
        std::vector<Monomial<Rational>> x{Monomial(Rational(1), MonomialDegree(3, {2, 0, 0})),
                                          Monomial(Rational(2), MonomialDegree(3, {1, 1, 0})),
                                          Monomial(Rational(2), MonomialDegree(3, {1, 0, 1})),
                                          Monomial(Rational(2), MonomialDegree(3, {0, 2, 0})),
                                          Monomial(Rational(2), MonomialDegree(3, {0, 1, 1})),
                                          Monomial(Rational(2), MonomialDegree(3, {0, 0, 2}))};

        {
            Polynomial<Rational, GrlexOrder> poly(x);
            CheckSorted(poly);
            CheckEqual(poly, x);
        }

        {
            Polynomial<Rational, GrevlexOrder> poly(x);
            CheckSorted(poly);
            CheckEqual(poly, {x[0], x[1], x[3], x[2], x[4], x[5]});
        }
    }
}

// TODO add tests with Zp
TEST(PolynomialArithmetics, Addition) {
    {
        std::vector<Monomial<Rational>> x{Monomial(Rational(1), MonomialDegree(2, {2, 0})),
                                          Monomial(Rational(1), MonomialDegree(2, {1, 1})),
                                          Monomial(Rational(2), MonomialDegree(2, {0, 1}))
        };
        std::vector<Monomial<Rational>> y{Monomial(Rational(3), MonomialDegree(2, {2, 0})),
                                          Monomial(Rational(-1), MonomialDegree(2, {1, 1})),
                                          Monomial(Rational(1, 2), MonomialDegree(2, {0, 2}))
        };
        std::vector<Monomial<Rational>> expected{Monomial(Rational(4), MonomialDegree(2, {2, 0})),
                                                 Monomial(Rational(1, 2), MonomialDegree(2, {0, 2})),
                                                 Monomial(Rational(2), MonomialDegree(2, {0, 1}))
        };

        {
            Polynomial<Rational, LexOrder> poly1(x);
            Polynomial<Rational, LexOrder> poly2(y);
            Polynomial<Rational, LexOrder> poly3 = poly1 + poly2;

            CheckSorted(poly3);
            CheckEqual(poly3, expected);
        }

        {
            Polynomial<Rational, ReverseLexOrder> poly1(x);
            Polynomial<Rational, ReverseLexOrder> poly2(y);
            Polynomial<Rational, ReverseLexOrder> poly3 = poly1 + poly2;

            CheckSorted(poly3);
            CheckEqual(poly3, {expected[0], expected[2], expected[1]});
        }

        {
            Polynomial<Rational, GrlexOrder> poly1(x);
            Polynomial<Rational, GrlexOrder> poly2(y);
            Polynomial<Rational, GrlexOrder> poly3 = poly1 + poly2;

            CheckSorted(poly3);
            CheckEqual(poly3, expected);
        }

        {
            Polynomial<Rational, GrevlexOrder> poly1(x);
            Polynomial<Rational, GrevlexOrder> poly2(y);
            Polynomial<Rational, GrevlexOrder> poly3 = poly1 + poly2;

            CheckSorted(poly3);
            CheckEqual(poly3, expected);
        }
    }
}

TEST(PolynomialArithmetics, Substitution) {
    {
        std::vector<Monomial<Rational>> x{Monomial(Rational(1), MonomialDegree(2, {2, 0})),
                                          Monomial(Rational(1), MonomialDegree(2, {1, 1})),
                                          Monomial(Rational(1), MonomialDegree(2, {0, 1}))
        };
        std::vector<Monomial<Rational>> y{Monomial(Rational(1), MonomialDegree(2, {1, 1})),
                                          Monomial(Rational(-1), MonomialDegree(2, {2, 0})),
                                          Monomial(Rational(1), MonomialDegree(2, {0, 2}))
        };
        std::vector<Monomial<Rational>> expected{Monomial(Rational(2), MonomialDegree(2, {2, 0})),
                                                 Monomial(Rational(-1), MonomialDegree(2, {0, 2})),
                                                 Monomial(Rational(1), MonomialDegree(2, {0, 1}))
        };

        {
            Polynomial<Rational, LexOrder> poly1(x);
            Polynomial<Rational, LexOrder> poly2(y);
            Polynomial<Rational, LexOrder> poly3 = poly1 - poly2;

            CheckSorted(poly3);
            CheckEqual(poly3, expected);
        }

        {
            Polynomial<Rational, ReverseLexOrder> poly1(x);
            Polynomial<Rational, ReverseLexOrder> poly2(y);
            Polynomial<Rational, ReverseLexOrder> poly3 = poly1 - poly2;

            CheckSorted(poly3);
            CheckEqual(poly3, {expected[0], expected[2], expected[1]});
        }

        {
            Polynomial<Rational, GrlexOrder> poly1(x);
            Polynomial<Rational, GrlexOrder> poly2(y);
            Polynomial<Rational, GrlexOrder> poly3 = poly1 - poly2;

            CheckSorted(poly3);
            CheckEqual(poly3, expected);
        }

        {
            Polynomial<Rational, GrevlexOrder> poly1(x);
            Polynomial<Rational, GrevlexOrder> poly2(y);
            Polynomial<Rational, GrevlexOrder> poly3 = poly1 - poly2;

            CheckSorted(poly3);
            CheckEqual(poly3, expected);
        }
    }
}

TEST(PolynomialArithmetics, Multiplication) {
    {
        std::vector<Monomial<Rational>> x{Monomial(Rational(1), MonomialDegree(2, {2, 0})),
                                          Monomial(Rational(1), MonomialDegree(2, {1, 1}))
        };
        std::vector<Monomial<Rational>> y{Monomial(Rational(1), MonomialDegree(2, {1, 0})),
                                          Monomial(Rational(1), MonomialDegree(2, {0, 1})),
        };
        std::vector<Monomial<Rational>> expected{Monomial(Rational(1), MonomialDegree(2, {3, 0})),
                                                 Monomial(Rational(2), MonomialDegree(2, {2, 1})),
                                                 Monomial(Rational(1), MonomialDegree(2, {1, 2}))
        };

        {
            Polynomial<Rational, LexOrder> poly1(x);
            Polynomial<Rational, LexOrder> poly2(y);
            Polynomial<Rational, LexOrder> poly3 = poly1 * poly2;

            CheckSorted(poly3);
            CheckEqual(poly3, expected);
        }

        {
            Polynomial<Rational, ReverseLexOrder> poly1(x);
            Polynomial<Rational, ReverseLexOrder> poly2(y);
            Polynomial<Rational, ReverseLexOrder> poly3 = poly1 * poly2;

            CheckSorted(poly3);
            CheckEqual(poly3, expected);
        }

        {
            Polynomial<Rational, GrlexOrder> poly1(x);
            Polynomial<Rational, GrlexOrder> poly2(y);
            Polynomial<Rational, GrlexOrder> poly3 = poly1 * poly2;

            CheckSorted(poly3);
            CheckEqual(poly3, expected);
        }

        {
            Polynomial<Rational, GrevlexOrder> poly1(x);
            Polynomial<Rational, GrevlexOrder> poly2(y);
            Polynomial<Rational, GrevlexOrder> poly3 = poly1 * poly2;

            CheckSorted(poly3);
            CheckEqual(poly3, expected);
        }
    }

    {
        std::vector<Monomial<Rational>> x{Monomial(Rational(1), MonomialDegree(2, {2, 0})),
                                          Monomial(Rational(1), MonomialDegree(2, {1, 1}))
        };
        std::vector<Monomial<Rational>> y{Monomial(Rational(1), MonomialDegree(2, {1, 0})),
                                          Monomial(Rational(-1), MonomialDegree(2, {0, 1})),
        };
        std::vector<Monomial<Rational>> expected{Monomial(Rational(1), MonomialDegree(2, {3, 0})),
                                                 Monomial(Rational(-1), MonomialDegree(2, {1, 2}))
        };

        {
            Polynomial<Rational, LexOrder> poly1(x);
            Polynomial<Rational, LexOrder> poly2(y);
            Polynomial<Rational, LexOrder> poly3 = poly1 * poly2;

            CheckSorted(poly3);
            CheckEqual(poly3, expected);
        }

        {
            Polynomial<Rational, ReverseLexOrder> poly1(x);
            Polynomial<Rational, ReverseLexOrder> poly2(y);
            Polynomial<Rational, ReverseLexOrder> poly3 = poly1 * poly2;

            CheckSorted(poly3);
            CheckEqual(poly3, expected);
        }

        {
            Polynomial<Rational, GrlexOrder> poly1(x);
            Polynomial<Rational, GrlexOrder> poly2(y);
            Polynomial<Rational, GrlexOrder> poly3 = poly1 * poly2;

            CheckSorted(poly3);
            CheckEqual(poly3, expected);
        }

        {
            Polynomial<Rational, GrevlexOrder> poly1(x);
            Polynomial<Rational, GrevlexOrder> poly2(y);
            Polynomial<Rational, GrevlexOrder> poly3 = poly1 * poly2;

            CheckSorted(poly3);
            CheckEqual(poly3, expected);
        }
    }
}