#ifndef BEZPARSER_H
#define BEZPARSER_H

#include <string>
#include <vector>
#include "Vect.hpp"

typedef struct{
    Vect pos;
    Vect deriv;
} SurfacePt;

class BezPatch{
    public:
        BezPatch();
        BezPatch(std::vector< std::vector<Vect> > data);
        Vect at(int i, int j) const;
        vector< vector<SurfacePt> > getMesh(double stepSize);
    protected:
        std::vector< std::vector<Vect> > data;
        static void interpolateBezier1d(double u, Vect a, Vect b, Vect c, Vect d, Vect& pos, Vect& deriv);
        void interpolateBezier2d(double u, double v, Vect& pos, Vect& deriv);
};

class Bez{
    public:
        Bez();
        Bez(std::vector<BezPatch> data);
        Bez(std::string filename);
        BezPatch at(int i) const;
        BezPatch operator[](int i) const;
        int size();
    protected:
        std::vector<BezPatch> data;
};

ostream& operator<<(ostream& lhs, Bez rhs);
ostream& operator<<(ostream& lhs, BezPatch rhs);

#endif
