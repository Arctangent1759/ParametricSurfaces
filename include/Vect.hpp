#ifndef VECT_H
#define VECT_H

#include <iostream>

using namespace std;

class Vect;
class Point;
class Ray;
class Matrix;

typedef double scalar;

class Vect{
    public:
        friend ostream& operator<<(ostream& lhs, Vect& v);
        friend class Ray;
        Vect();
        Vect(Vect* v);
        Vect(scalar x, scalar y, scalar z);

        scalar getX() const;
        scalar getY() const;
        scalar getZ() const;
        bool isDir() const;

        virtual Vect operator+(const Vect rhs) const;
        virtual Vect operator-(const Vect rhs) const;
        scalar operator*(const Vect rhs) const;
        Vect operator*(const scalar rhs) const;
        Vect cross(const Vect rhs) const;
        Vect operator/(const scalar rhs) const;

        virtual Vect operator+=(Vect rhs);
        virtual Vect operator-=(Vect rhs);
        Vect operator/=(scalar rhs);
        Vect operator*=(scalar rhs);

        scalar normSq() const;
        scalar norm() const;
        Vect normalized() const;

    protected:
        scalar x,y,z;
        bool dir;
};

ostream& operator<<(ostream& lhs, Vect v);

Vect operator*(const scalar s, const Vect v);
Vect cross(const Vect u, const Vect v);
scalar normSq(const Vect v);
scalar norm(const Vect v);
Vect normalized(const Vect v);
Vect project(const Vect u, const Vect v);
Vect perp(const Vect u, const Vect v);

#endif
