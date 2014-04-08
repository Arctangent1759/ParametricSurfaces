#include <GL/glut.h>
#include "Bez.hpp"
#include <iostream>

typedef struct {
    int w, h;
} Viewport;
Viewport viewport;
typedef struct {
    double x,y,z;
    double ax,ay;
    bool wireframe;
    bool smooth;
    bool hiddenSurface;
    Bez bez;
} View;
View view;
const double CAMERA_STEP=.15;
const double ANGLE_STEP=1.0;

void myReshape(int x,int y){
    viewport.w=x;
    viewport.h=y;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(20, viewport.w / (float) viewport.h, .01, 100);
    glViewport(0,0,viewport.w,viewport.h);
    glMatrixMode(GL_MODELVIEW);
}

void onKeyDown(unsigned char key, int x, int y){
    if (key == ' '){
        exit(0);
    }else if (key == 's'){
        view.smooth = !view.smooth;
    }else if(key == 'w'){
        view.wireframe = !view.wireframe;
    }else if(key == 'h'){
        view.hiddenSurface = !view.hiddenSurface;
    }
    if (key == '+'){view.z+=CAMERA_STEP;}
    if (key == '-'){view.z-=CAMERA_STEP;}
    glutPostRedisplay();
}

void special(int key, int x, int y){
    if (glutGetModifiers()!=GLUT_ACTIVE_SHIFT){
        if (key == GLUT_KEY_UP){view.ax-=ANGLE_STEP;}
        if (key == GLUT_KEY_DOWN){view.ax+=ANGLE_STEP;}
        if (key == GLUT_KEY_LEFT){view.ay-=ANGLE_STEP;}
        if (key == GLUT_KEY_RIGHT){view.ay+=ANGLE_STEP;}
    }else{
        if (key == GLUT_KEY_UP){view.y+=CAMERA_STEP;}
        if (key == GLUT_KEY_DOWN){view.y-=CAMERA_STEP;}
        if (key == GLUT_KEY_LEFT){view.x-=CAMERA_STEP;}
        if (key == GLUT_KEY_RIGHT){view.x+=CAMERA_STEP;}
    }
    glutPostRedisplay();
}

void myDisplay(){
    static float ax = 0;
    static float ay = 0;
    static Vect cameraPos(0,0,-30);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(view.x, view.y, view.z);
    glRotatef(view.ax, 1, 0, 0);
    glRotatef(view.ay, 0, 0, 1);


    glShadeModel(view.smooth?GL_SMOOTH:GL_FLAT);

    if (view.wireframe){
        view.bez.renderMesh(.07);
    }else{
        view.bez.render(.07);
    }

    glFlush();
    glutSwapBuffers();
}

int main(int argc, char* argv[]){
    viewport.w = 400;
    viewport.h = 400;
    view.x=0;
    view.y=0;
    view.z=-30;
    view.ax=0;
    view.ay=0;
    view.bez = Bez("./bez/teapot.bez",0.1);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(viewport.w, viewport.h);
    glutCreateWindow(argv[0]);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glutDisplayFunc(myDisplay);
    glutReshapeFunc(myReshape);

    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(special);

    glutMainLoop();
}
