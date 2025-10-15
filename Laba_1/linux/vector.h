#ifndef VECTOR_H
#define VECTOR_H

#include "number.h"

class Vector {
private:
    Number x_;
    Number y_;

public:
    Vector();
    Vector(const Number& x, const Number& y);
    Vector(const Vector& other);
    Vector& operator=(const Vector& other);

    Number getRho() const;
    Number getTheta() const;

    Vector operator+(const Vector& other) const;
    Vector operator-(const Vector& other) const;
    
    Vector operator*(const Number& scalar) const;
    Vector operator/(const Number& scalar) const;
    Vector operator-() const;
    
    Vector& operator+=(const Vector& other);
    Vector& operator-=(const Vector& other);
    Vector& operator*=(const Number& scalar);
    
    Number dot(const Vector& other) const;
    Number cross(const Vector& other) const;
    
    Vector normalize() const;
    Vector rotate(const Number& angle) const;
    Vector perpendicular() const;
    
    bool isZero() const;
    bool isUnit() const;
    Number distanceTo(const Vector& other) const;
    
    Vector projectOnto(const Vector& other) const;
    Number angleBetween(const Vector& other) const;

    Number getX() const;
    Number getY() const;

    static const Vector ZERO;
    static const Vector ONE;
    
    static Vector fromPolar(const Number& rho, const Number& theta);
};

#endif // VECTOR_H