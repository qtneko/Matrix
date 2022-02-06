#include "matrix.h"
#include "gtest/gtest.h"

template <typename T>
inline std::string toString(const Matrix<T> &m)
{
    if (m.rows() < 1 || m.columns() < 1)
        return std::string();
    auto toString = [&m](size_t row)
    {
        std::string res = "[";
        for (size_t j = 0; j < m.columns() - 1; ++j)
            res += fmt::format("{},", m.at(row, j));
        return res + fmt::format("{}]", m.at(row, m.columns() - 1));
    };
    std::string res = "[";
    for (size_t i = 0; i < m.rows() - 1; ++i)
        res += fmt::format("{},", toString(i));
    return res + fmt::format("{}]", toString(m.rows() - 1));
}

TEST(Matrix, basic)
{
    Matrix<int> m1(2, 3, 42);
    EXPECT_EQ(2, m1.rows());
    EXPECT_EQ(3, m1.columns());

    for (auto it = m1.begin(); it != m1.end(); ++it)
    {
        *it += std::distance(m1.begin(), it);
    }

    EXPECT_EQ(42, m1.at(0, 0));
    EXPECT_EQ(43, m1.at(0, 1));
    EXPECT_EQ(44, m1.at(0, 2));
    EXPECT_EQ(45, m1.at(1, 0));
    EXPECT_EQ(46, m1.at(1, 1));
    m1[1][2] = 14;
    EXPECT_EQ(14, m1[1][2]);
    EXPECT_EQ(14, m1.at(1, 2));

    const Matrix<int> m2(m1);
    EXPECT_EQ(46, m2[1][1]);
    EXPECT_EQ(46, m2.at(1, 1));

    EXPECT_EQ(std::string("[[42,43,44],[45,46,14]]"), toString(m1));
    EXPECT_EQ(std::string("[[84,86,88],[90,92,28]]"), toString(m1 + m2));

    m1 += m2;
    EXPECT_EQ(std::string("[[84,86,88],[90,92,28]]"), toString(m1));

    EXPECT_EQ(Matrix<int>(1, 2, 3), Matrix<int>(1, 2, 1) + Matrix<int>(1, 2, 2));
    EXPECT_NE(Matrix<int>(1, 2, 4), Matrix<int>(1, 2, 3));
    EXPECT_NE(Matrix<int>(2, 2), Matrix<int>(1, 2));
    EXPECT_NE(Matrix<int>(2, 1), Matrix<int>(1, 2));

    EXPECT_EQ(std::string("[[1,1,2,2,2],[1,1,2,2,2]]"),
        toString(Matrix<int>(2,2,1) | Matrix<int>(2,3,2)));

    m1 |= m2;
    EXPECT_EQ(std::string("[[84,86,88,42,43,44],[90,92,28,45,46,14]]"), toString(m1));
}

TEST(Matrix, exceptions)
{
    try
    {
        Matrix<int> m(2, 1);
        m.at(1, 1) = 1;
        FAIL() << "Exception is not thrown. Expected std::out_of_range";
    }
    catch(const std::out_of_range &err)
    {
    }
    catch(...) {
        FAIL() << "Another exception is thrown. Expected std::out_of_range";
    }

    try
    {
        auto sum = Matrix<int>(2, 1) + Matrix<int>(2, 2);
        FAIL() << "Exception is not thrown. Expected std::runtime_error";
    }
    catch(const std::runtime_error &err)
    {
    }
    catch(...) {
        FAIL() << "Another exception is thrown. Expected std::runtime_error";
    }

    try
    {
        auto res = Matrix<int>(2, 1) | Matrix<int>(1, 2);
        FAIL() << "Exception is not thrown. Expected std::runtime_error";
    }
    catch(const std::runtime_error &err)
    {
    }
    catch(...) {
        FAIL() << "Another exception is thrown. Expected std::runtime_error";
    }
}
