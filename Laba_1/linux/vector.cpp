#include "vector.h"
#include <cmath>
#include <stdexcept>

const Vector Vector::ZERO = Vector(Number::ZERO, Number::ZERO);
const Vector Vector::ONE = Vector(Number::ONE, Number::ONE);

Vector::Vector() : x_(Number::ZERO), y_(Number::ZERO) {}

Vector::Vector(const Number& x, const Number& y) : x_(x), y_(y) {}

Vector::Vector(const Vector& other) : x_(other.x_), y_(other.y_) {}

Vector& Vector::operator=(const Vector& other) {
    if (this != &other) {
        x_ = other.x_;
        y_ = other.y_;
    }
    return *this;
}

Vector Vector::operator+(const Vector& other) const {
    return Vector(x_ + other.x_, y_ + other.y_);
}

Vector Vector::operator-(const Vector& other) const {
    return Vector(x_ - other.x_, y_ - other.y_);
}

Vector Vector::operator*(const Number& scalar) const {
    return Vector(x_ * scalar, y_ * scalar);
}

Vector Vector::operator/(const Number& scalar) const {
    if (scalar.getValue() == 0.0) {
        throw std::runtime_error("Division by zero in vector scalar division");
    }
    return Vector(x_ / scalar, y_ / scalar);
}

Vector Vector::operator-() const {
    return Vector(-x_, -y_);
}

Vector& Vector::operator+=(const Vector& other) {
    x_ += other.x_;
    y_ += other.y_;
    return *this;
}

Vector& Vector::operator-=(const Vector& other) {
    x_ -= other.x_;
    y_ -= other.y_;
    return *this;
}

Vector& Vector::operator*=(const Number& scalar) {
    x_ *= scalar;
    y_ *= scalar;
    return *this;
}

Number Vector::dot(const Vector& other) const {
    return x_ * other.x_ + y_ * other.y_;
}

Number Vector::cross(const Vector& other) const {
    return x_ * other.y_ - y_ * other.x_;
}

Vector Vector::normalize() const {
    Number length = getRho();
    if (length.getValue() == 0.0) {
        return Vector::ZERO;
    }
    return *this / length;
}

Vector Vector::rotate(const Number& angle) const {
    double cos_val = std::cos(angle.getValue());
    double sin_val = std::sin(angle.getValue());
    double x_val = x_.getValue();
    double y_val = y_.getValue();
    
    double new_x = x_val * cos_val - y_val * sin_val;
    double new_y = x_val * sin_val + y_val * cos_val;
    
    return Vector(Number(new_x), Number(new_y));
}

Vector Vector::perpendicular() const {
    return Vector(-y_, x_);
}

bool Vector::isZero() const {
    return x_ == Number::ZERO && y_ == Number::ZERO;
}

bool Vector::isUnit() const {
    Number length = getRho();
    return std::abs(length.getValue() - 1.0) < 1e-10;
}

Number Vector::distanceTo(const Vector& other) const {
    return (*this - other).getRho();
}

Vector Vector::projectOnto(const Vector& other) const {
    Number denominator = other.dot(other);
    if (denominator.getValue() == 0.0) {
        return Vector::ZERO;
    }
    Number scale = dot(other) / denominator;
    return other * scale;
}

Number Vector::angleBetween(const Vector& other) const {
    Number dot_product = dot(other);
    Number mag_product = getRho() * other.getRho();
    
    if (mag_product.getValue() == 0.0) {
        return Number::ZERO;
    }
    
    double cos_angle = dot_product.getValue() / mag_product.getValue();
    cos_angle = std::max(-1.0, std::min(1.0, cos_angle));
    return Number(std::acos(cos_angle));
}

Vector Vector::fromPolar(const Number& rho, const Number& theta) {
    double x = rho.getValue() * std::cos(theta.getValue());
    double y = rho.getValue() * std::sin(theta.getValue());
    return Vector(Number(x), Number(y));
}

Number Vector::getX() const { return x_; }
Number Vector::getY() const { return y_; }

Number Vector::getRho() const {
    double x_val = x_.getValue();
    double y_val = y_.getValue();
    return Number(std::sqrt(x_val * x_val + y_val * y_val));
}

Number Vector::getTheta() const {
    double x_val = x_.getValue();
    double y_val = y_.getValue();
    return Number(std::atan2(y_val, x_val));
}