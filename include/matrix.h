#include <vector>
#include <stdexcept>
#include <algorithm>
#include <fmt/core.h>

template <typename T>
class Matrix
{
public:
    /*!
    Proxy class helper.
    */
    class Row
    {
    public:
        T &operator[](std::size_t column) { return _parent._data[column + _row * _parent._columns]; }
    private:
        friend class Matrix;
        Row(Matrix &parent, std::size_t row)
            : _parent(parent), _row(row) {}

        Matrix      &_parent;
        std::size_t _row;
    };

    /*!
    Proxy class helper.
    */
    class CRow
    {
    public:
        const T &operator[](std::size_t column) const { return _parent._data[column + _row * _parent._columns]; }
    private:
        friend class Matrix;
        CRow(const Matrix &parent, std::size_t row)
            : _parent(parent), _row(row) {}

        const Matrix    &_parent;
        std::size_t     _row;
    };

    using container = std::vector<T>;
    using iterator = typename container::iterator;
    using const_iterator = typename container::const_iterator;

    iterator begin() { return _data.begin(); }
    iterator end() { return _data.end(); }
    const_iterator begin() const { return _data.begin(); }
    const_iterator end() const { return _data.end(); }

    /*!
    Create a matrix [rows*columns]
    \param[in] rows Number of rows
    \param[in] columns Number of columns
    \param[in] val Default value
    */
    Matrix(std::size_t rows, std::size_t columns, const T &val = T())
        : _rows(rows), _columns(columns), _data(rows * columns, val) {}

    std::size_t rows() const noexcept { return _rows; }
    std::size_t columns() const noexcept { return _columns; }

    const T &at(std::size_t row, std::size_t column) const { return _data.at(column + row * _columns); }
    T &at(std::size_t row, std::size_t column) { return _data.at(column + row * _columns); }

    CRow operator[](std::size_t row) const { return CRow(*this, row); }
    Row operator[](std::size_t row) { return Row(*this, row); }

    friend bool operator==(const Matrix &lhs, const Matrix &rhs)
    {
        if ((lhs._rows != rhs._rows) || (lhs._columns != rhs._columns))
            return false;
        return std::equal(lhs._data.cbegin(), lhs._data.cend(), rhs._data.cbegin());
    }

    friend bool operator!=(const Matrix &lhs, const Matrix &rhs) { return !(lhs == rhs); }

    Matrix &operator+=(const Matrix &rhs)
    {
        if (_rows != rhs._rows || _columns != rhs._columns)
        {
            throw std::runtime_error(fmt::format(
                "Matrices have different shapes: ({},{}) and ({}, {}).", _rows, _columns,
                rhs._rows, rhs._columns));
        }
        for (std::size_t i = 0; i < _data.size(); ++i)
            _data[i] += rhs._data[i];
        return *this;
    }

    friend Matrix operator+(Matrix lhs, const Matrix &rhs)
    {
        lhs += rhs;
        return lhs;
    }

    /*!
    Horizontal concatenation. Matrices must have the same number of rows.
    \param[in] lhs Matrix [r*c1]
    \param[in] rhs Matrix [r*c2]
    \return Matrix [r*(c1+c2)]
    */
    Matrix &operator|=(const Matrix &rhs)
    {
        if (_rows != rhs._rows)
        {
            throw std::runtime_error(fmt::format(
                "Matrices have different number of rows: ({},{}) and ({}, {}).", _rows,
                _columns, rhs._rows, rhs._columns));
        }

        const std::size_t totalCol = _columns + rhs._columns;
        std::vector<T> newData(totalCol * _rows);
        for (std::size_t i = 0; i < _rows; ++i)
            for (std::size_t j = 0; j < _columns; ++j)
                newData[j + i * totalCol] = _data[j + i * _columns];
        for (std::size_t i = 0; i < rhs._rows; ++i)
            for (std::size_t j = 0; j < rhs._columns; ++j)
                newData[j + _columns + i * totalCol] = rhs._data[j + i * _columns];

        _columns = totalCol;
        _data.swap(newData);
        return *this;
    }

    /*!
    Horizontal concatenation. Matrices must have the same number of rows.
    \param[in] lhs Matrix [r*c1]
    \param[in] rhs Matrix [r*c2]
    \return Matrix [r*(c1+c2)]
    */
    friend Matrix operator|(Matrix lhs, const Matrix &rhs)
    {
        lhs |= rhs;
        return lhs;
    }

private:
    std::size_t _rows = 0;
    std::size_t _columns = 0;

    container _data;
};

