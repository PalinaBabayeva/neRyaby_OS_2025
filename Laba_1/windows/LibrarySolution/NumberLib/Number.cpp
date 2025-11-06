#include "pch.h"
#include "Number.h"
#include <stdexcept>
#include <cmath>

const Number Number::ZERO = Number(0.0);
const Number Number::ONE = Number(1.0);

Number::Number() : value_(0.0) {}

Number::Number(double val) : value_(val) {}

Number Number::operator+(const Number& other) const {
    return Number(value_ + other.value_);
}

Number Number::operator-(const Number& other) const {
    return Number(value_ - other.value_);
}

Number Number::operator*(const Number& other) const {
    return Number(value_ * other.value_);
}

Number Number::operator/(const Number& other) const {
    if (other.value_ == 0.0) {
        throw std::runtime_error("Division by zero");
    }
    return Number(value_ / other.value_);
}

Number Number::operator^(const Number& exponent) const {
    return Number(std::pow(value_, exponent.value_));
}

Number Number::operator%(const Number& other) const {
    if (other.value_ == 0.0) {
        throw std::runtime_error("Modulo by zero");
    }
    return Number(std::fmod(value_, other.value_));
}

Number Number::operator-() const {
    return Number(-value_);
}

Number& Number::operator+=(const Number& other) {
    value_ += other.value_;
    return *this;
}

Number& Number::operator-=(const Number& other) {
    value_ -= other.value_;
    return *this;
}

Number& Number::operator*=(const Number& other) {
    value_ *= other.value_;
    return *this;
}

Number& Number::operator/=(const Number& other) {
    if (other.value_ == 0.0) {
        throw std::runtime_error("Division by zero");
    }
    value_ /= other.value_;
    return *this;
}

bool Number::operator==(const Number& other) const {
    return value_ == other.value_;
}

bool Number::operator!=(const Number& other) const {
    return value_ != other.value_;
}

bool Number::operator<(const Number& other) const {
    return value_ < other.value_;
}

bool Number::operator>(const Number& other) const {
    return value_ > other.value_;
}

bool Number::operator<=(const Number& other) const {
    return value_ <= other.value_;
}

bool Number::operator>=(const Number& other) const {
    return value_ >= other.value_;
}

double Number::getValue() const {
    return value_;
}

Number Number::create(double val) {
    return Number(val);
}