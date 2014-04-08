#include "Bez.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glut.h>

using namespace std;

/////////////////////
//  BezPatch Defs  //
/////////////////////

BezPatch::BezPatch(){}

BezPatch::BezPatch(vector< vector<Vect> > data,double stepSize){
    this->data = data;
    this->mesh = this->getMesh(stepSize);
}

Vect BezPatch::at(int i, int j) const{
    return this->data[j][i];
}

vector< vector<SurfacePt> > BezPatch::getMesh(){
    return this->mesh;
}

vector< vector<SurfacePt> > BezPatch::getMesh(double stepSize){
    vector< vector<SurfacePt> > mesh;
    for (double u = 0; u < 1.0; u+=stepSize){
        vector<SurfacePt> curr;
        for (double v = 0; v < 1.0; v+=stepSize){
            curr.push_back(this->interpolateBezier2d(u,v));
        }
        curr.push_back(this->interpolateBezier2d(u,1.0));
        mesh.push_back(curr);
    }
    vector<SurfacePt> curr;
    for (double v = 0; v < 1.0; v+=stepSize){
        curr.push_back(this->interpolateBezier2d(1.0,v));
    }
    curr.push_back(this->interpolateBezier2d(1.0,1.0));
    mesh.push_back(curr);
    return mesh;
}

SurfacePt BezPatch::interpolateBezier1d(double u, Vect a, Vect b, Vect c, Vect d){
    Vect e = (1-u)*a+(u)*b;
    Vect f = (1-u)*b+(u)*c;
    Vect g = (1-u)*c+(u)*d;
    Vect h = (1-u)*e+(u)*f;
    Vect i = (1-u)*f+(u)*g;
    SurfacePt out;
    out.pos = (1-u)*h+(u)*i;
    out.deriv = 3*(i-h);
    return out;
}

SurfacePt BezPatch::interpolateBezier2d(double u, double v){
    Vect va = this->interpolateBezier1d(u,this->at(0,0),this->at(0,1),this->at(0,2),this->at(0,3)).pos;
    Vect vb = this->interpolateBezier1d(u,this->at(1,0),this->at(1,1),this->at(1,2),this->at(1,3)).pos;
    Vect vc = this->interpolateBezier1d(u,this->at(2,0),this->at(2,1),this->at(2,2),this->at(2,3)).pos;
    Vect vd = this->interpolateBezier1d(u,this->at(3,0),this->at(3,1),this->at(3,2),this->at(3,3)).pos;
    Vect ua = this->interpolateBezier1d(v,this->at(0,0),this->at(1,0),this->at(2,0),this->at(3,0)).pos;
    Vect ub = this->interpolateBezier1d(v,this->at(0,1),this->at(1,1),this->at(2,1),this->at(3,1)).pos;
    Vect uc = this->interpolateBezier1d(v,this->at(0,2),this->at(1,2),this->at(2,2),this->at(3,2)).pos;
    Vect ud = this->interpolateBezier1d(v,this->at(0,3),this->at(1,3),this->at(2,3),this->at(3,3)).pos;
    SurfacePt p1 = this->interpolateBezier1d(v,va,vb,vc,vd);
    SurfacePt p2 = this->interpolateBezier1d(u,ua,ub,uc,ud);
    SurfacePt out;
    out.deriv = normalized(cross(p1.deriv,p2.deriv));
    out.pos=p1.pos;
    return out;
}

///////////////
//  BezDefs  //
///////////////

Bez::Bez(){
    this->data = vector<BezPatch>();
}
Bez::Bez(string filename, double stepSize){
    ifstream file(filename.c_str());
    string line;
    double x,y,z;

    getline(file,line);

    vector< vector<Vect> > out;

    while (getline(file, line)){
        istringstream lineStream(line);
        vector<Vect> curr;
        while (lineStream >> x >> y >> z){curr.push_back(Vect(x,y,z));}
        if (curr.size() != 0){
            out.push_back(curr);
        }
    }

    vector<BezPatch> bezPatches;
    for (int i = 0; i < out.size(); i+=4){
        vector< vector<Vect> > patchData;
        patchData.push_back(out[i]);
        patchData.push_back(out[i+1]);
        patchData.push_back(out[i+2]);
        patchData.push_back(out[i+3]);
        bezPatches.push_back(BezPatch(patchData,stepSize));
    }
    this->data = bezPatches;
}
BezPatch Bez::at(int i) const{
    return this->data[i];
}
BezPatch Bez::operator[](int i) const{
    return this->at(i);
}

int Bez::size(){
    return this->data.size();
}
void Bez::render(double stepSize){
    glBegin(GL_QUADS);
    for (int i = 0; i < this->size(); i++){
        vector< vector<SurfacePt> > vertices = (*this)[i].getMesh();
        for (int j = 0; j < vertices.size()-1; j++){
            for (int k = 0; k < vertices[j].size()-1; k++){
                Vect a = vertices[j][k].pos;
                Vect an = vertices[j][k].deriv;
                Vect b = vertices[j+1][k].pos;
                Vect bn = vertices[j+1][k].deriv;
                Vect c = vertices[j+1][k+1].pos;
                Vect cn = vertices[j+1][k+1].deriv;
                Vect d = vertices[j][k+1].pos;
                Vect dn = vertices[j][k+1].deriv;
                glNormal3d(an.getX(), an.getY(), an.getZ());
                glVertex3f(a.getX(), a.getY(), a.getZ());
                glNormal3d(bn.getX(), bn.getY(), bn.getZ());
                glVertex3f(b.getX(), b.getY(), b.getZ());
                glNormal3d(cn.getX(), cn.getY(), cn.getZ());
                glVertex3f(c.getX(), c.getY(), c.getZ());
                glNormal3d(dn.getX(), dn.getY(), dn.getZ());
                glVertex3f(d.getX(), d.getY(), d.getZ());
            }
        }
    }
    glEnd();

}

void Bez::renderMesh(double stepSize){
    glBegin(GL_LINES);
    for (int i = 0; i < this->size(); i++){
        vector< vector<SurfacePt> > vertices = (*this)[i].getMesh();
        for (int j = 0; j < vertices.size()-1; j++){
            for (int k = 0; k < vertices[j].size()-1; k++){
                Vect a = vertices[j][k].pos;
                Vect an = vertices[j][k].deriv;
                Vect b = vertices[j+1][k].pos;
                Vect bn = vertices[j+1][k].deriv;
                Vect c = vertices[j+1][k+1].pos;
                Vect cn = vertices[j+1][k+1].deriv;
                Vect d = vertices[j][k+1].pos;
                Vect dn = vertices[j][k+1].deriv;

                glNormal3d(an.getX(), an.getY(), an.getZ());
                glVertex3f(a.getX(), a.getY(), a.getZ());

                glNormal3d(bn.getX(), bn.getY(), bn.getZ());
                glVertex3f(b.getX(), b.getY(), b.getZ());

                glNormal3d(bn.getX(), bn.getY(), bn.getZ());
                glVertex3f(b.getX(), b.getY(), b.getZ());

                glNormal3d(cn.getX(), cn.getY(), cn.getZ());
                glVertex3f(c.getX(), c.getY(), c.getZ());

                glNormal3d(cn.getX(), cn.getY(), cn.getZ());
                glVertex3f(c.getX(), c.getY(), c.getZ());

                glNormal3d(dn.getX(), dn.getY(), dn.getZ());
                glVertex3f(d.getX(), d.getY(), d.getZ());

                glNormal3d(dn.getX(), dn.getY(), dn.getZ());
                glVertex3f(d.getX(), d.getY(), d.getZ());

                glNormal3d(an.getX(), an.getY(), an.getZ());
                glVertex3f(a.getX(), a.getY(), a.getZ());
            }
        }
    }
    glEnd();

}


ostream& operator<<(ostream& lhs, Bez rhs){
    lhs << "Bez(" << endl;
    for (int i = 0; i < rhs.size(); i++){
        lhs << rhs[i];
    }
    lhs << ")" << endl;
}
ostream& operator<<(ostream& lhs, BezPatch rhs){
    lhs << "BezPatch(" << endl;
    for (int j = 0; j < 4; j++){
        for (int i = 0; i < 4; i++){
            lhs << rhs.at(i,j) << " ";
        }
        lhs << endl;
    }
    lhs << ")" << endl;
}
