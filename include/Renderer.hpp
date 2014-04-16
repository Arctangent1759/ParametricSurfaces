#ifndef RENDERER_H
#define RENDERER_H

#include "Bez.hpp"
#include "Obj.hpp"

class Renderer{
    public:
        Renderer();
        Renderer(string filename, double stepSize, bool uniform);
        virtual void render();
        virtual void renderMesh(bool showNormals);
        void write(string filename);
    private:
        Bez b;
        Obj o;
        bool isBez;
};

#endif
