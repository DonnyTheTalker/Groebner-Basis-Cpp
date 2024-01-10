#include "gtest/gtest.h"
#include "Monomial.h"

TEST(MonomialBasic, Construction) {
    MonomialDegree base_degree(3, {1, 2, 3});
    MonomialDegree empty_degree(3);
    {
        EXPECT_NO_THROW(Monomial(Rational(3, 2), base_degree));
        EXPECT_NO_THROW(Monomial(Modulo(7, 3), base_degree));
    }

    // bad coefficient
    {
        EXPECT_DEATH(Monomial(Rational(7, 0), base_degree), "Denominator must not be zero");
        EXPECT_DEATH(Monomial(Modulo(-1), base_degree), "Order must be positive");
        EXPECT_DEATH(Monomial(Modulo(4), base_degree), "Order must be prime number");
    }

    {
        Monomial x(Rational(7, 3), base_degree);
        Monomial y(x);

        EXPECT_EQ(y, Monomial(Rational(7, 3), base_degree));
        EXPECT_EQ(x, Monomial(Rational(7, 3), base_degree));

        Monomial z(Rational(0), empty_degree);
        z = x;
        EXPECT_EQ(x, Monomial(Rational(7, 3), base_degree));
        EXPECT_EQ(y, Monomial(Rational(7, 3), base_degree));
        EXPECT_EQ(z, Monomial(Rational(7, 3), base_degree));
    }

    {
        Monomial x(Rational(7, 3), base_degree);
        Monomial y(std::move(x));

        EXPECT_EQ(y, Monomial(Rational(7, 3), base_degree));
        EXPECT_EQ(x, Monomial(Rational(0), empty_degree));

        Monomial z(Rational(0), empty_degree);
        z = std::move(y);
        EXPECT_EQ(x, Monomial(Rational(0), empty_degree));
        EXPECT_EQ(y, Monomial(Rational(0), empty_degree));
        EXPECT_EQ(z, Monomial(Rational(7, 3), base_degree));
    }
}

TEST(MonomialBasic, Getters) {
    {
        Monomial x(Rational(7, 3), MonomialDegree(3, {1, 2, 3}));
        EXPECT_EQ(x.GetSize(), 3);
        EXPECT_EQ(x.GetDegree().GetSumDegree(), 6);
        EXPECT_EQ(x.GetCoef(), Rational(7, 3));
        EXPECT_EQ(x.GetCoef(), Rational(14, 6));
        EXPECT_EQ(x.GetDegree(), MonomialDegree(3, {1, 2, 3}));
        EXPECT_NE(x.GetDegree(), MonomialDegree(3, {0, 2, 3}));
        EXPECT_FALSE(x.IsZero());
    }

    {
        Monomial x(Modulo(7, 3), MonomialDegree(3, {1, 2, 3}));
        EXPECT_EQ(x.GetSize(), 3);
        EXPECT_EQ(x.GetDegree().GetSumDegree(), 6);
        EXPECT_EQ(x.GetCoef(), Modulo(7, 3));
        EXPECT_EQ(x.GetCoef(), Modulo(7, 10));
        EXPECT_EQ(x.GetDegree(), MonomialDegree(3, {1, 2, 3}));
        EXPECT_NE(x.GetDegree(), MonomialDegree(3, {0, 2, 3}));
        EXPECT_FALSE(x.IsZero());
    }

    {
        Monomial x(Rational(0), MonomialDegree(3));
        EXPECT_EQ(x.GetDegree().GetSumDegree(), 0);
        EXPECT_TRUE(x.IsZero());
    }
}

TEST(MonomialBasic, Comparison) {
    {
        EXPECT_EQ(Monomial(Rational(3, 2), MonomialDegree(3, {1, 2, 3})),
                  Monomial(Rational(3, 2), MonomialDegree(3, {1, 2, 3})));

        EXPECT_EQ(Monomial(Rational(3, 2), MonomialDegree(3, {1, 2, 3})),
                  Monomial(Rational(6, 4), MonomialDegree(3, {1, 2, 3})));

        EXPECT_EQ(Monomial(Modulo(7, 3), MonomialDegree(1)),
                  Monomial(Modulo(7, 3), MonomialDegree(1)));

        EXPECT_EQ(Monomial(Modulo(7, 3), MonomialDegree(1)),
                  Monomial(Modulo(7, -4), MonomialDegree(1)));
    }

    {
        EXPECT_NE(Monomial(Rational(3, 2), MonomialDegree(1)),
                  Monomial(Rational(3, 3), MonomialDegree(1)));

        EXPECT_NE(Monomial(Rational(3, 2), MonomialDegree(1)),
                  Monomial(Rational(3, 2), MonomialDegree(2)));

        EXPECT_NE(Monomial(Rational(3, 2), MonomialDegree(1, {1})),
                  Monomial(Rational(3, 2), MonomialDegree(1, {2})));

        EXPECT_NE(Monomial(Rational(3, 2), MonomialDegree(2, {1, 2})),
                  Monomial(Rational(3, 2), MonomialDegree(2, {2, 2})));

        EXPECT_NE(Monomial(Rational(3, 2), MonomialDegree(2, {1, 1})),
                  Monomial(Rational(3, 2), MonomialDegree(2, {1, 2})));

        EXPECT_NE(Monomial(Rational(3, 2), MonomialDegree(2, {1, 2})),
                  Monomial(Rational(3, 2), MonomialDegree(2, {1, 1})));
    }

    {
        EXPECT_NE(Monomial(Modulo(3, 2), MonomialDegree(1)),
                  Monomial(Modulo(3, 3), MonomialDegree(1)));

        EXPECT_NE(Monomial(Modulo(3, 2), MonomialDegree(1)),
                  Monomial(Modulo(3, 2), MonomialDegree(2)));

        EXPECT_NE(Monomial(Modulo(3, 2), MonomialDegree(1, {1})),
                  Monomial(Modulo(3, 2), MonomialDegree(1, {2})));

        EXPECT_NE(Monomial(Modulo(3, 2), MonomialDegree(2, {1, 2})),
                  Monomial(Modulo(3, 2), MonomialDegree(2, {2, 2})));

        EXPECT_NE(Monomial(Modulo(3, 2), MonomialDegree(2, {1, 1})),
                  Monomial(Modulo(3, 2), MonomialDegree(2, {1, 2})));

        EXPECT_NE(Monomial(Modulo(3, 2), MonomialDegree(2, {1, 2})),
                  Monomial(Modulo(3, 2), MonomialDegree(2, {1, 1})));
    }
}

TEST(MonomialArithmetics, InvalidCases) {
    {
        Monomial x(Rational(1), MonomialDegree(2, {1, 1}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 1, 1}));

        EXPECT_DEATH(x + y, "Can't add monomials with different degree");
        EXPECT_DEATH(x += y, "Can't add monomials with different degree");

        EXPECT_DEATH(x - y, "Can't substitute monomials with different degree");
        EXPECT_DEATH(x -= y, "Can't substitute monomials with different degree");

        EXPECT_DEATH(x * y, "Can't multiply monomials with different degree");
        EXPECT_DEATH(x *= y, "Can't multiply monomials with different degree");

        EXPECT_DEATH(x / y, "Can't divide monomials with different degree");
        EXPECT_DEATH(x /= y, "Can't divide monomials with different degree");
    }

    {
        Monomial x(Modulo(3, 2), MonomialDegree(2, {1, 1}));
        Monomial y(Modulo(3, 2), MonomialDegree(3, {1, 1, 1}));

        EXPECT_DEATH(x + y, "Can't add monomials with different degree");
        EXPECT_DEATH(x += y, "Can't add monomials with different degree");

        EXPECT_DEATH(x - y, "Can't substitute monomials with different degree");
        EXPECT_DEATH(x -= y, "Can't substitute monomials with different degree");

        EXPECT_DEATH(x * y, "Can't multiply monomials with different degree");
        EXPECT_DEATH(x *= y, "Can't multiply monomials with different degree");

        EXPECT_DEATH(x / y, "Can't divide monomials with different degree");
        EXPECT_DEATH(x /= y, "Can't divide monomials with different degree");
    }

    {
        Monomial x(Rational(1), MonomialDegree(2, {1, 2}));
        Monomial y(Rational(1), MonomialDegree(2, {2, 2}));

        // TODO add is divisible function and change label later
        EXPECT_DEATH(x / y, "Can't substitute from lower degree");
        EXPECT_DEATH(x /= y, "Can't substitute from lower degree");
    }

    {
        Monomial x(Modulo(3, 2), MonomialDegree(2, {1, 2}));
        Monomial y(Modulo(3, 2), MonomialDegree(2, {2, 2}));

        // TODO add is divisible function and change label later
        EXPECT_DEATH(x / y, "Can't substitute from lower degree");
        EXPECT_DEATH(x /= y, "Can't substitute from lower degree");
    }

    {
        Monomial x(Rational(3), MonomialDegree(0));
        Monomial y(Rational(0), MonomialDegree(0));

        EXPECT_DEATH(x / y, "Can't divide by zero");
        EXPECT_DEATH(x /= y, "Can't divide by zero");
    }

    {
        Monomial x(Rational(3, 2), MonomialDegree(1, {2}));
        Monomial y(Rational(0), MonomialDegree(1, {1}));

        EXPECT_DEATH(x / y, "Can't divide by zero");
        EXPECT_DEATH(x /= y, "Can't divide by zero");
    }

    {
        Monomial x(Modulo(3, 1), MonomialDegree(0));
        Monomial y(Modulo(3, 0), MonomialDegree(0));

        EXPECT_DEATH(x / y, "Can't divide by zero");
        EXPECT_DEATH(x /= y, "Can't divide by zero");
    }

    {
        Monomial x(Modulo(3, 1), MonomialDegree(1, {2}));
        Monomial y(Modulo(3, 0), MonomialDegree(1, {1}));

        EXPECT_DEATH(x / y, "Can't divide by zero");
        EXPECT_DEATH(x /= y, "Can't divide by zero");
    }
}

TEST(MonomialArithmetics, Addition) {
    {
        Monomial x(Rational(1), MonomialDegree(2, {1, 1}));
        Monomial y(Rational(3), MonomialDegree(2, {1, 1}));
        Monomial z(Rational(4), MonomialDegree(2, {1, 1}));

        EXPECT_EQ(x + y, z);
        EXPECT_EQ(y + x, z);
        EXPECT_EQ(x += y, z);
        EXPECT_EQ(x, z);
        EXPECT_EQ(x + x, z + z);
    }

    {
        Monomial x(Rational(3, 2), MonomialDegree(2, {1, 2}));
        Monomial y(Rational(3, 2), MonomialDegree(2, {1, 2}));
        Monomial z(Rational(3), MonomialDegree(2, {1, 2}));

        EXPECT_EQ(x + y, z);
        EXPECT_EQ(y + x, z);
        EXPECT_EQ(x += y, z);
        EXPECT_EQ(x, z);
        EXPECT_EQ(x + x, z + z);
    }

    {
        Monomial x(Rational(3, 2), MonomialDegree(2, {1, 1}));
        Monomial y(Rational(-3, 2), MonomialDegree(2, {1, 1}));
        Monomial z(Rational(0), MonomialDegree(2, {1, 1}));

        EXPECT_EQ(x + y, z);
        EXPECT_EQ(y + x, z);
        EXPECT_EQ(x += y, z);
        EXPECT_EQ(x, z);
        EXPECT_TRUE(x.IsZero());
        EXPECT_EQ(x += x, z + z);
        EXPECT_TRUE(x.IsZero());
    }

    {
        Monomial x(Modulo(7, 5), MonomialDegree(2, {1, 1}));
        Monomial y(Modulo(7, 2), MonomialDegree(2, {1, 1}));
        Monomial z(Modulo(7, 0), MonomialDegree(2, {1, 1}));

        EXPECT_EQ(x + y, z);
        EXPECT_EQ(y + x, z);
        EXPECT_EQ(x += y, z);
        EXPECT_EQ(x, z);
        EXPECT_TRUE(x.IsZero());
        EXPECT_EQ(x += x, z + z);
        EXPECT_TRUE(x.IsZero());
    }

    {
        Monomial x(Modulo(7, 5), MonomialDegree(2, {1, 2}));
        Monomial y(Modulo(7, 5), MonomialDegree(2, {1, 2}));
        Monomial z(Modulo(7, 3), MonomialDegree(2, {1, 2}));

        EXPECT_EQ(x + y, z);
        EXPECT_EQ(y + x, z);
        EXPECT_EQ(x += y, z);
        EXPECT_EQ(x, z);
        EXPECT_EQ(x + x, z + z);
    }
}

TEST(MonomialArithmetics, Substitution) {
    {
        Monomial x(Rational(1), MonomialDegree(2, {1, 1}));
        Monomial y(Rational(3), MonomialDegree(2, {1, 1}));
        Monomial z(Rational(-2), MonomialDegree(2, {1, 1}));

        EXPECT_EQ(x - y, z);
        EXPECT_EQ(x -= y, z);
        EXPECT_EQ(x, z);
        EXPECT_EQ(x - x, z - z);
    }

    {
        Monomial x(Rational(5, 2), MonomialDegree(2, {1, 2}));
        Monomial y(Rational(3, 2), MonomialDegree(2, {1, 2}));
        Monomial z(Rational(1), MonomialDegree(2, {1, 2}));

        EXPECT_EQ(x - y, z);
        EXPECT_EQ(x -= y, z);
        EXPECT_EQ(x, z);
        EXPECT_EQ(x - x, z - z);
        EXPECT_TRUE((x -= x).IsZero());
    }

    {
        Monomial x(Rational(3, 2), MonomialDegree(2, {1, 1}));
        Monomial y(Rational(3, 2), MonomialDegree(2, {1, 1}));
        Monomial z(Rational(0), MonomialDegree(2, {1, 1}));

        EXPECT_EQ(x - y, z);
        EXPECT_EQ(x -= y, z);
        EXPECT_EQ(x, z);
        EXPECT_TRUE(x.IsZero());
        EXPECT_EQ(x -= x, z - z);
        EXPECT_TRUE(x.IsZero());
    }

    {
        Monomial x(Modulo(7, 5), MonomialDegree(2, {1, 1}));
        Monomial y(Modulo(7, 2), MonomialDegree(2, {1, 1}));
        Monomial z(Modulo(7, 3), MonomialDegree(2, {1, 1}));

        EXPECT_EQ(x - y, z);
        EXPECT_EQ(x -= y, z);
        EXPECT_EQ(x, z);
        EXPECT_FALSE(x.IsZero());
        EXPECT_EQ(x -= x, z - z);
        EXPECT_TRUE(x.IsZero());
    }

    {
        Monomial x(Modulo(7, 5), MonomialDegree(2, {1, 2}));
        Monomial y(Modulo(7, 12), MonomialDegree(2, {1, 2}));
        Monomial z(Modulo(7, 0), MonomialDegree(2, {1, 2}));

        EXPECT_EQ(x - y, z);
        EXPECT_EQ(x -= y, z);
        EXPECT_EQ(x, z);
        EXPECT_TRUE(x.IsZero());
        EXPECT_EQ(x - x, z + z);
    }
}

TEST(MonomialArithmetics, Multiplication) {
    {
        Monomial x(Rational(1), MonomialDegree(2, {1, 1}));
        Monomial y(Rational(3), MonomialDegree(2, {3, 0}));
        Monomial z(Rational(3), MonomialDegree(2, {4, 1}));

        EXPECT_EQ(x * y, z);
        EXPECT_EQ(y * x, z);
        EXPECT_EQ(x *= y, z);
        EXPECT_EQ(x, z);
    }

    {
        Monomial x(Rational(1), MonomialDegree(2, {1, 0}));
        Monomial y(Rational(1, 2), MonomialDegree(2, {0, 3}));
        Monomial z(Rational(1, 2), MonomialDegree(2, {1, 3}));

        EXPECT_EQ(x * y, z);
        EXPECT_EQ(y * x, z);
        EXPECT_EQ(x *= y, z);
        EXPECT_EQ(x, z);
    }

    {
        Monomial x(Rational(1), MonomialDegree(2, {1, 1}));
        Monomial y(Rational(0), MonomialDegree(2, {3, 0}));
        Monomial z(Rational(0), MonomialDegree(2, {4, 1}));

        EXPECT_EQ(x * y, z);
        EXPECT_EQ(y * x, z);
        EXPECT_EQ(x *= y, z);
        EXPECT_TRUE(x.IsZero());
        EXPECT_EQ(x, z);
    }

    {
        Monomial x(Modulo(7, 1), MonomialDegree(2, {1, 1}));
        Monomial y(Modulo(7, 3), MonomialDegree(2, {3, 0}));
        Monomial z(Modulo(7, 3), MonomialDegree(2, {4, 1}));

        EXPECT_EQ(x * y, z);
        EXPECT_EQ(y * x, z);
        EXPECT_EQ(x *= y, z);
        EXPECT_EQ(x, z);
    }

    {
        Monomial x(Modulo(7, 5), MonomialDegree(2, {1, 0}));
        Monomial y(Modulo(7, 3), MonomialDegree(2, {0, 3}));
        Monomial z(Modulo(7, 1), MonomialDegree(2, {1, 3}));

        EXPECT_EQ(x * y, z);
        EXPECT_EQ(y * x, z);
        EXPECT_EQ(x *= y, z);
        EXPECT_EQ(x, z);
    }

    {
        Monomial x(Modulo(7, 1), MonomialDegree(2, {1, 1}));
        Monomial y(Modulo(7, 7), MonomialDegree(2, {3, 0}));
        Monomial z(Modulo(7, 0), MonomialDegree(2, {4, 1}));

        EXPECT_EQ(x * y, z);
        EXPECT_EQ(y * x, z);
        EXPECT_EQ(x *= y, z);
        EXPECT_TRUE(x.IsZero());
        EXPECT_EQ(x, z);
    }
}

TEST(MonomialArithmetics, Division) {
    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 1}));
        Monomial y(Rational(3), MonomialDegree(3, {1, 1, 0}));
        Monomial z(Rational(1, 3), MonomialDegree(3, {0, 0, 1}));

        EXPECT_EQ(x / y, z);
        EXPECT_EQ(x /= y, z);
        EXPECT_EQ(x, z);
    }

    {
        Monomial x(Rational(1), MonomialDegree(3, {1, 1, 0}));
        Monomial y(Rational(1), MonomialDegree(3, {1, 1, 0}));
        Monomial z(Rational(1), MonomialDegree(3, {0, 0, 0}));

        EXPECT_EQ(x / y, z);
        EXPECT_EQ(x /= y, z);
        EXPECT_EQ(x, z);
    }

    {
        Monomial x(Rational(2), MonomialDegree(3, {1, 1, 0}));
        Monomial y(Rational(2), MonomialDegree(3, {0, 0, 0}));
        Monomial z(Rational(1), MonomialDegree(3, {1, 1, 0}));

        EXPECT_EQ(x / y, z);
        EXPECT_EQ(x /= y, z);
        EXPECT_EQ(x, z);
    }

    {
        Monomial x(Modulo(7, 1), MonomialDegree(3, {1, 1, 1}));
        Monomial y(Modulo(7, 3), MonomialDegree(3, {1, 1, 0}));
        Monomial z(Modulo(7, 5), MonomialDegree(3, {0, 0, 1}));

        EXPECT_EQ(x / y, z);
        EXPECT_EQ(x /= y, z);
        EXPECT_EQ(x, z);
    }

    {
        Monomial x(Modulo(7, 1), MonomialDegree(3, {1, 1, 0}));
        Monomial y(Modulo(7, 1), MonomialDegree(3, {1, 1, 0}));
        Monomial z(Modulo(7, 1), MonomialDegree(3, {0, 0, 0}));

        EXPECT_EQ(x / y, z);
        EXPECT_EQ(x /= y, z);
        EXPECT_EQ(x, z);
    }

    {
        Monomial x(Modulo(7, 2), MonomialDegree(3, {1, 1, 0}));
        Monomial y(Modulo(7, 2), MonomialDegree(3, {0, 0, 0}));
        Monomial z(Modulo(7, 1), MonomialDegree(3, {1, 1, 0}));

        EXPECT_EQ(x / y, z);
        EXPECT_EQ(x /= y, z);
        EXPECT_EQ(x, z);
    }
}

TEST(MonomialArithmetics, General) {
    {
        Monomial x(Rational(3), MonomialDegree(3, {1, 1, 0}));
        Monomial y(Rational(5), MonomialDegree(3, {1, 1, 0}));
        Monomial z(Rational(6, 8), MonomialDegree(3, {0, 0, 1}));
        Monomial res(Rational(6), MonomialDegree(3, {1, 1, 1}));

        EXPECT_EQ((x + y) * z, x * z + y * z);
        EXPECT_EQ(z * (x + y), x * z + y * z);
        EXPECT_EQ((x + y) * z, res);
    }

    {
        Monomial x(Rational(3), MonomialDegree(3, {1, 1, 0}));
        Monomial y(Rational(5), MonomialDegree(3, {1, 1, 0}));
        Monomial z(Rational(1), MonomialDegree(3, {1, 0, 0}));
        Monomial res(Rational(8), MonomialDegree(3, {0, 1, 0}));

        EXPECT_EQ((x + y) / z, x / z + y / z);
        EXPECT_EQ((x + y) / z, res);
    }

    {
        Monomial x(Modulo(7, 3), MonomialDegree(3, {1, 1, 0}));
        Monomial y(Modulo(7, 5), MonomialDegree(3, {1, 1, 0}));
        Monomial z(Modulo(7, 6), MonomialDegree(3, {0, 0, 1}));
        Monomial res(Modulo(7, 6), MonomialDegree(3, {1, 1, 1}));

        EXPECT_EQ((x + y) * z, x * z + y * z);
        EXPECT_EQ(z * (x + y), x * z + y * z);
        EXPECT_EQ((x + y) * z, res);
    }

    {
        Monomial x(Modulo(7, 3), MonomialDegree(3, {1, 1, 0}));
        Monomial y(Modulo(7, 5), MonomialDegree(3, {1, 1, 0}));
        Monomial z(Modulo(7, 2), MonomialDegree(3, {1, 0, 0}));
        Monomial res(Modulo(7, 4), MonomialDegree(3, {0, 1, 0}));

        EXPECT_EQ((x + y) / z, x / z + y / z);
        EXPECT_EQ((x + y) / z, res);
    }
}

TEST(MonomialArithmetics, SumDegree) {
    {
        Monomial x(Rational(1), MonomialDegree(2, {1, 1}));
        Monomial y(Rational(3), MonomialDegree(2, {1, 1}));

        EXPECT_EQ((x + y).GetDegree().GetSumDegree(), 2);
        EXPECT_EQ((x + y).GetDegree().GetSumDegree(), x.GetDegree().GetSumDegree());
        EXPECT_EQ((x + y).GetDegree().GetSumDegree(), y.GetDegree().GetSumDegree());
        EXPECT_EQ((x += y).GetDegree().GetSumDegree(), y.GetDegree().GetSumDegree());
        EXPECT_EQ(x.GetDegree().GetSumDegree(), 2);
    }

    {
        Monomial x(Rational(1), MonomialDegree(2, {1, 1}));
        Monomial y(Rational(3), MonomialDegree(2, {1, 1}));

        EXPECT_EQ((x - y).GetDegree().GetSumDegree(), 2);
        EXPECT_EQ((x - y).GetDegree().GetSumDegree(), x.GetDegree().GetSumDegree());
        EXPECT_EQ((x - y).GetDegree().GetSumDegree(), y.GetDegree().GetSumDegree());
        EXPECT_EQ((x -= y).GetDegree().GetSumDegree(), y.GetDegree().GetSumDegree());
        EXPECT_EQ(x.GetDegree().GetSumDegree(), 2);
    }

    {
        Monomial x(Rational(1), MonomialDegree(2, {1, 3}));
        Monomial y(Rational(3), MonomialDegree(2, {1, 2}));

        EXPECT_EQ((x * y).GetDegree().GetSumDegree(), 7);
        EXPECT_EQ((x * y).GetDegree().GetSumDegree(), x.GetDegree().GetSumDegree() + y.GetDegree().GetSumDegree());
        EXPECT_EQ((x * y).GetDegree().GetSumDegree(), x.GetDegree().GetSumDegree() + y.GetDegree().GetSumDegree());
        EXPECT_EQ((x *= y).GetDegree().GetSumDegree(), 7);
        EXPECT_EQ(x.GetDegree().GetSumDegree(), 7);
    }

    {
        Monomial x(Rational(1), MonomialDegree(2, {1, 3}));
        Monomial y(Rational(3), MonomialDegree(2, {1, 2}));

        EXPECT_EQ((x / y).GetDegree().GetSumDegree(), 1);
        EXPECT_EQ((x / y).GetDegree().GetSumDegree(), x.GetDegree().GetSumDegree() - y.GetDegree().GetSumDegree());
        EXPECT_EQ((x / y).GetDegree().GetSumDegree(), x.GetDegree().GetSumDegree() - y.GetDegree().GetSumDegree());
        EXPECT_EQ((x /= y).GetDegree().GetSumDegree(), 1);
        EXPECT_EQ(x.GetDegree().GetSumDegree(), 1);
    }
}

TEST(MonomialArithmetics, UnaryMinus) {
    {
        Monomial x(Rational(3, 2), MonomialDegree(2, {1, 1}));
        EXPECT_EQ(-x, Monomial(Rational(-3, 2), MonomialDegree(2, {1, 1})));
        EXPECT_EQ((-x).GetCoef(), Rational(-3, 2));
        EXPECT_EQ(-(-x), x);
    }
}
