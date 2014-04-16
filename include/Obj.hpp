#ifndef OBJ_H
#define OBJ_H

#include <string>
#include <vector>
#include "Vect.hpp"
#include "Bez.hpp"

class Obj{
    public:
        Obj();
        Obj(std::string filename, double stepSize, bool uniform);
        int size();
        void render();
        void renderMesh(bool showNormals);
        void toObj(string filename);
    protected:
        vector< vector<SurfacePt> > polygons;
        string filename;
};

#endif
