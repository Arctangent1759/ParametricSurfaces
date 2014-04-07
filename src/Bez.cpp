#include "Bez.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

/////////////////////
//  BezPatch Defs  //
/////////////////////

BezPatch::BezPatch(){}

BezPatch::BezPatch(vector< vector<Vect> > data){
    this->data = data;
}

Vect BezPatch::at(int i, int j) const{
    return this->data[j][i];
}

vector< vector<SurfacePt> > BezPatch::getMesh(double stepSize){
    int numdiv = 1.0/stepSize;
    vector< vector<SurfacePt> > mesh(numdiv*numdiv);
    for (int i = 0; i < numdiv; i++){
        for (int j = 0; j < numdiv; j++){
            this->interpolateBezier2d(i*stepSize,j*stepSize, mesh[i][j].pos, mesh[i][j].deriv);
        }
    }
    return mesh;
}

void BezPatch::interpolateBezier1d(double u, Vect a, Vect b, Vect c, Vect d, Vect& pos, Vect& deriv){
    Vect e = (1-u)*a+(u)*b;
    Vect f = (1-u)*b+(u)*c;
    Vect g = (1-u)*c+(u)*d;
    Vect h = (1-u)*e+(u)*f;
    Vect i = (1-u)*f+(u)*g;
    pos = (1-u)*h+(u)*i;
    deriv = 3*(i-h);
}

void BezPatch::interpolateBezier2d(double u, double v, Vect& pos, Vect& deriv){
    Vect va,vb,vc,vd,ua,ub,uc,ud, ddu,ddv;
    this->interpolateBezier1d(u,this->at(0,0),this->at(0,1),this->at(0,2),this->at(0,3),va,deriv);
    this->interpolateBezier1d(u,this->at(1,0),this->at(1,1),this->at(1,2),this->at(1,3),vb,deriv);
    this->interpolateBezier1d(u,this->at(2,0),this->at(2,1),this->at(2,2),this->at(2,3),vc,deriv);
    this->interpolateBezier1d(u,this->at(3,0),this->at(3,1),this->at(3,2),this->at(3,3),vd,deriv);
    this->interpolateBezier1d(v,this->at(0,0),this->at(1,0),this->at(2,0),this->at(3,0),ua,deriv);
    this->interpolateBezier1d(v,this->at(0,1),this->at(1,1),this->at(2,1),this->at(3,1),ub,deriv);
    this->interpolateBezier1d(v,this->at(0,2),this->at(1,2),this->at(2,2),this->at(3,2),uc,deriv);
    this->interpolateBezier1d(v,this->at(0,3),this->at(1,3),this->at(2,3),this->at(3,3),ud,deriv);
    this->interpolateBezier1d(v,va,vb,vc,vd,pos,ddv);
    this->interpolateBezier1d(u,ua,ub,uc,ud,pos,ddu);
    deriv = normalized(cross(ddu,ddv));
}

///////////////
//  BezDefs  //
///////////////

Bez::Bez(){
    this->data = vector<BezPatch>();
}
Bez::Bez(vector<BezPatch> data){
    this->data = data;
}
Bez::Bez(string filename){
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
        bezPatches.push_back(BezPatch(patchData));
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
