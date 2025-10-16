#ifndef NUMBER_H
#define NUMBER_H

class Number {
private:
    double value_;

public:
    Number();
    explicit Number(double val);

    Number operator+(const Number& other) const;
    Number operator-(const Number& other) const;
    Number operator*(const Number& other) const;
    Number operator/(const Number& other) const;
    Number operator^(const Number& exponent) const;
    Number operator%(const Number& other) const;
    Number operator-() const;

    Number& operator+=(const Number& other);
    Number& operator-=(const Number& other);
    Number& operator*=(const Number& other);
    Number& operator/=(const Number& other);

    bool operator==(const Number& other) const;
    bool operator!=(const Number& other) const;
    bool operator<(const Number& other) const;
    bool operator>(const Number& other) const;
    bool operator<=(const Number& other) const;
    bool operator>=(const Number& other) const;

    double getValue() const;

    static const Number ZERO;
    static const Number ONE;

    static Number create(double val);
};

#endif // NUMBER_H