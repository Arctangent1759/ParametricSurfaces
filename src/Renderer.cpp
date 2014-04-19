#include "Renderer.hpp"

Renderer::Renderer(){}
Renderer::Renderer(string filename, double stepSize, bool uniform){
    if (filename.substr(filename.rfind('.'),-1)==".obj"){
        this->o = Obj(filename,stepSize,uniform);
        this->isBez=false;
    }else{
        this->b = Bez(filename,stepSize,uniform);
        this->isBez=true;
    }
}
void Renderer::render(){
    if (this->isBez){
        this->b.render();
    }else{
        this->o.render();
    }
}
void Renderer::renderMesh(bool showNormals){
    if (this->isBez){
        this->b.renderMesh(showNormals);
    }else{
        this->o.renderMesh(showNormals);
    }
}
void Renderer::write(string filename){
    if (this->isBez){
        this->b.toObj(filename);
    }else{
        this->o.toBez(filename);
    }
}
