#include "Obj.hpp"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <GL/glut.h>

using namespace std;

Obj::Obj(){}

Obj::Obj(string filename, double stepSize, bool uniform){
    ifstream file(filename.c_str());
    string line;

    vector<Vect> vertices;
    vector<Vect> normals;
    vector< vector<int> > faceIndices;
    vector< vector<int> > normalIndices;
    while (getline(file,line)){
        if (line == "" || line.at(0)=='#'){
            continue;
        }

        istringstream lstr(line);
        string token;

        lstr >> token;

        if (token=="v"){
            scalar x,y,z;
            lstr >> x >> y >> z;
            vertices.push_back(Vect(x,y,z));
        }else if(token=="f"){
            string vertString;
            vector<int> currFace;
            vector<int> currNormals;
            while (lstr >> vertString){

                int lpos = 0;
                int pos = 0;
                vector<int> vertIndices;
                int currIndex;
                while (true){
                    pos = vertString.find('/',pos);
                    stringstream(vertString.substr(lpos,pos)) >> currIndex;
                    vertIndices.push_back(currIndex);
                    if (pos == -1){
                        break;
                    }
                    pos++;
                    lpos = pos;
                }

                currFace.push_back(vertIndices[0]);
                if (vertIndices.size()==3){
                    currNormals.push_back(vertIndices[2]);
                }
            }
            faceIndices.push_back(currFace);
            normalIndices.push_back(currNormals);
        }else if(token =="vn"){
            scalar x,y,z;
            lstr >> x >> y >> z;
            normals.push_back(Vect(x,y,z));
        }else{
        }
    }
    for (int i = 0; i < faceIndices.size(); i++){
        vector<SurfacePt> poly;
        Vect n = normalized(cross(vertices[faceIndices[i][1]-1]-vertices[faceIndices[i][0]-1],vertices[faceIndices[i][2]-1]-vertices[faceIndices[i][0]-1]));
        for (int j = 0; j < faceIndices[i].size(); j++){
            SurfacePt p;
            if (normalIndices[i].size()==0){
                p.deriv = n;
            }else{
                p.deriv = normalized(normals[normalIndices[i][j]-1]);
            }
            p.pos = vertices[faceIndices[i][j]-1];
            poly.push_back(p);
        }
        this->polygons.push_back(poly);
    }
    file.close();
    this->filename=filename;
}

void Obj::render(){
    for (int i = 0; i < this->polygons.size(); i++){
        glBegin(GL_POLYGON);
        for (int j = 0; j < this->polygons[i].size();j++){
            Vect pos = polygons[i][j].pos;
            Vect normal = polygons[i][j].deriv;
            glColor3d(0.5,0.1,.1);
            glNormal3d(normal.getX(), normal.getY(), normal.getZ());
            glVertex3d(pos.getX(), pos.getY(), pos.getZ());
        }
        glEnd();
    }
}

void Obj::renderMesh(bool showNormals){
    glBegin(GL_LINES);
    for (int i = 0; i < this->polygons.size(); i++){
        for (int j = 0; j < this->polygons[i].size(); j++){
            Vect a = this->polygons[i][j].pos;
            Vect an = this->polygons[i][j].deriv;
            Vect b = this->polygons[i][(j+1)%this->polygons[i].size()].pos;
            Vect bn = this->polygons[i][(j+1)%this->polygons[i].size()].deriv;

            glNormal3d(an.getX(), an.getY(), an.getZ());
            glVertex3f(a.getX(), a.getY(), a.getZ());

            glNormal3d(bn.getX(), bn.getY(), bn.getZ());
            glVertex3f(b.getX(), b.getY(), b.getZ());

            //Show normals
            if (showNormals){
                Vect ad = a+an;
                Vect bd = b+bn;
                glVertex3f(a.getX(), a.getY(), a.getZ());
                glVertex3f(ad.getX(), ad.getY(), ad.getZ());
                glVertex3f(b.getX(), b.getY(), b.getZ());
                glVertex3f(bd.getX(), bd.getY(), bd.getZ());
            }
        }
    }
    glEnd();
}


void Obj::toObj(string filename){
    ofstream file(filename.c_str());
    ifstream file2(this->filename.c_str());
    file << file2.rdbuf();
    file.close();
    
    file2.close();
}
void Obj::toBez(string filename){
    vector< vector< vector<Vect> > > bezList;
    for (int i = 0; i < polygons.size(); i++){
        vector<SurfacePt> poly = polygons[i];
        vector< vector<Vect> > bezPatch(4);
        for (int j = 0; j < 4; j++){bezPatch[j]=vector<Vect>(4);}
        if (poly.size() == 3){
            bezPatch[0][0]=poly[0].pos;

            bezPatch[0][1]=0.75*poly[0].pos+0.25*poly[1].pos;
            bezPatch[0][2]=0.25*poly[0].pos+0.75*poly[1].pos;

            bezPatch[0][3]=poly[1].pos;

            bezPatch[1][3]=0.75*poly[1].pos+0.25*poly[2].pos;
            bezPatch[2][3]=0.25*poly[1].pos+0.75*poly[2].pos;

            bezPatch[3][3]=poly[2].pos;

            bezPatch[3][2]=poly[2].pos;
            bezPatch[3][1]=poly[2].pos;

            bezPatch[3][0]=poly[2].pos;

            bezPatch[2][0]=0.75*poly[2].pos+0.25*poly[0].pos;
            bezPatch[1][0]=0.25*poly[2].pos+0.75*poly[0].pos;

            bezPatch[1][1]=0.75*bezPatch[0][1]+0.25*bezPatch[3][1];
            bezPatch[1][2]=0.75*bezPatch[0][2]+0.25*bezPatch[3][2];
            bezPatch[2][2]=0.25*bezPatch[0][2]+0.75*bezPatch[3][2];
            bezPatch[2][1]=0.25*bezPatch[0][1]+0.75*bezPatch[3][1];

        }else if(poly.size()==4){
            bezPatch[0][0]=poly[0].pos;

            bezPatch[0][1]=0.75*poly[0].pos+0.25*poly[1].pos;
            bezPatch[0][2]=0.25*poly[0].pos+0.75*poly[1].pos;

            bezPatch[0][3]=poly[1].pos;

            bezPatch[1][3]=0.75*poly[1].pos+0.25*poly[2].pos;
            bezPatch[2][3]=0.25*poly[1].pos+0.75*poly[2].pos;

            bezPatch[3][3]=poly[2].pos;

            bezPatch[3][2]=0.75*poly[2].pos+0.25*poly[3].pos;
            bezPatch[3][1]=0.25*poly[2].pos+0.75*poly[3].pos;

            bezPatch[3][0]=poly[3].pos;

            bezPatch[2][0]=0.75*poly[3].pos+0.25*poly[0].pos;
            bezPatch[1][0]=0.25*poly[3].pos+0.75*poly[0].pos;

            bezPatch[1][1]=0.75*bezPatch[0][1]+0.25*bezPatch[3][1];
            bezPatch[1][2]=0.75*bezPatch[0][2]+0.25*bezPatch[3][2];
            bezPatch[2][2]=0.25*bezPatch[0][2]+0.75*bezPatch[3][2];
            bezPatch[2][1]=0.25*bezPatch[0][1]+0.75*bezPatch[3][1];

        }else{
        }
        bezList.push_back(bezPatch);
    }
    ofstream file(filename.c_str());
    file << bezList.size() << endl;
    for (int i = 0; i < bezList.size(); i++){
        for (int j = 0; j < 4; j++){
            for (int k = 0; k < 4; k++){
                file << bezList[i][j][k].getX() << " "<< bezList[i][j][k].getY() << " "<< bezList[i][j][k].getZ() << "   ";
            }
            file << endl;
        }
        file << endl;
    }
    file.close();

}
