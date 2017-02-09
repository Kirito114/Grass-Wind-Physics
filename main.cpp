#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <random>

#include "BezierCurve.hpp"

float windowWidth = 720;
float windowHeight = 480;
float xpos = 0, zpos = -50;
int angle_x = 45;int angle_y = 45;
int mouse_x = 0; int mouse_y = 0;

GLuint VBO1;
GLuint Grass;
GLuint color;

//Grass information
const unsigned int GRASS_AMOUNT = 10000;
const unsigned int GRASS_PRECISION = 8;
//Wind information
point3 wind_position(25.0f,5.0f,0.0f);
point3 wind_vector(-2.0f,-0.2f,0.0f);

static void renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(xpos,0,zpos);
    glRotatef(angle_x,1,0,0);
    glRotatef(angle_y,0,1,0);

    glEnableClientState(GL_VERTEX_ARRAY);
    //Pas bien mais pour une fleche
    glBegin(GL_TRIANGLES);
        glColor3f(0.0f,0.0f,1.0f);
        glVertex3f(wind_position.x,wind_position.y-1,wind_position.z);
        glColor3f(0.0f,0.0f,1.0f);
        glVertex3f(wind_position.x,wind_position.y+1,wind_position.z);
        glColor3f(1.0f,0.0f,0.0f);
        glVertex3f(wind_position.x+wind_vector.x,wind_position.y+wind_vector.y,wind_position.z+wind_vector.z);
    glEnd();
    glBindBuffer(GL_ARRAY_BUFFER,VBO1);
    glVertexPointer(3,GL_FLOAT,0,0);
    glColor3f(0.37f,0.3f,0.14f);
    glDrawArrays(GL_QUADS,0,4);
    glBindBuffer(GL_ARRAY_BUFFER,Grass);
    glVertexPointer(3,GL_FLOAT,0,0);
    glEnableClientState(GL_COLOR_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, color);
	glColorPointer(3, GL_FLOAT, 0, 0);

    for(unsigned int i=0;i<GRASS_AMOUNT;i++)
    {
        //glMultiDrawArrays
        glDrawArrays(GL_LINE_STRIP,i*GRASS_PRECISION,GRASS_PRECISION);
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glutSwapBuffers();
}

void standbyFunc()
{
    //On change la force du vent
}

void ReshapeWindowManager(GLsizei width,GLsizei height)
{
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,1000.0f);

    glMatrixMode(GL_MODELVIEW);
}

void keyboardManager(unsigned char key,int x,int y)
{
    switch(key)
    {

    }
}

void specialKeyboardManager(int key,int x,int y)
{
    switch(key)
    {

    }
}

void mouseManager(int button,int state,int x,int y)
{
    if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
    {
        mouse_x = x;
        mouse_y = y;
    }
}

void mouseMotionManager(int x,int y)
{
    angle_x += y - mouse_y;
    angle_y += x - mouse_x;
    mouse_x = x;
    mouse_y = y;
    glutPostRedisplay();
}

void timer(int value)
{
    glutPostRedisplay();
    //Bridage à 60 FPS 1000/16.66=60
    glutTimerFunc(16.66,timer,0);
}

static void InitializeGlutCallbacks()
{
    glutDisplayFunc(renderScene);
    //Bridage à 60 FPS 1000/16.66=60
    glutTimerFunc(16.66,timer,0);
    glutIdleFunc(standbyFunc);
    glutKeyboardFunc(keyboardManager);
    glutSpecialFunc(specialKeyboardManager);
    glutMouseFunc(mouseManager);
    glutMotionFunc(mouseMotionManager);
    glutReshapeFunc(ReshapeWindowManager);
}

static void InitializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH); //Antialiasing
    glClearColor(0.0f,0.0f,0.0f,1.0f);
}

static void InitializeGeometry()
{
    //Ground
    point3 * vertice = new point3[4];
    vertice[0] = point3(-20,0,-20);
    vertice[1] = point3(-20,0,20);
    vertice[2] = point3(20,0,20);
    vertice[3] = point3(20,0,-20);

    glGenBuffers(1,&VBO1);
    glBindBuffer(GL_ARRAY_BUFFER,VBO1);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*3*4,vertice,GL_STATIC_DRAW);

    delete [] vertice;

    //Grass
    point3 * grass_vertice = new point3[GRASS_AMOUNT*GRASS_PRECISION];
    float grass_x = 0,grass_y = 0,grass_z = 0,coef_deformation_x = 0,coef_deformation_z = 0;
    //Random Engine
    std::default_random_engine generator;
    std::normal_distribution<float> distribution(3.0,0.8);
    for(unsigned int i=0;i<GRASS_AMOUNT;i++)
    {
        grass_x = (rand()/float(RAND_MAX))*40-20;
        grass_y = distribution(generator);
        grass_z = (rand()/float(RAND_MAX))*40-20;
        coef_deformation_x = rand()%2-1;
        coef_deformation_z = rand()%2-1;
        std::deque<point3> controlPoints;
        //On ajoute les points de controle et plus le point de controle est eleve plus il est affecté par le coefficient de deformation
        controlPoints.push_back(point3(grass_x,0.0f,grass_z));
        controlPoints.push_back(point3(grass_x+coef_deformation_x*0.25,grass_y*(1/3.0),grass_z+coef_deformation_z*0.25));
        controlPoints.push_back(point3(grass_x+coef_deformation_x*0.5,grass_y*(2/3.0),grass_z+coef_deformation_z*0.5));
        controlPoints.push_back(point3(grass_x+coef_deformation_x,grass_y,grass_z+coef_deformation_z));
        BezierCurve grass_rode(controlPoints);
        grass_rode.getPoints(GRASS_PRECISION,&grass_vertice[i*GRASS_PRECISION]);
    }

    glGenBuffers(1,&Grass);
    glBindBuffer(GL_ARRAY_BUFFER,Grass);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*3*GRASS_AMOUNT*GRASS_PRECISION,grass_vertice,GL_STATIC_DRAW);

    delete [] grass_vertice;

    //Color buffer
    float * colors = new float[GRASS_AMOUNT*3*GRASS_PRECISION];
    for(unsigned int i=0;i<GRASS_AMOUNT*GRASS_PRECISION*3;i+=3*GRASS_PRECISION)
    {
        colors[i] = (rand()/float(RAND_MAX))*0.2+0.1;
        colors[i+1] = (rand()/float(RAND_MAX))*0.5+0.2;
        colors[i+2] = 0.0f;
        for(unsigned int j=i+3;j<(i+3)+3*(GRASS_PRECISION-1);j+=3)
        {
            colors[j] = colors[i];
            colors[j+1] = colors[i+1];
            colors[j+2] = colors[i+2];
        }
    }

    glGenBuffers(1, &color); //génération d'une référence de buffer object
	glBindBuffer(GL_ARRAY_BUFFER, color); //liaison du buffer avec un type tableau de données
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*GRASS_AMOUNT*GRASS_PRECISION, colors, GL_STATIC_DRAW);

    delete [] colors;
}

int main(int argc, char ** argv)
{
    srand(time(NULL));
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
    glutInitWindowSize(windowWidth,windowHeight);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Champ d'herbe expose au vent");

    GLenum res = glewInit();
    if(res != GLEW_OK)
    {
        std::cerr << "Error" << glewGetErrorString(res) << std::endl;
        return (EXIT_FAILURE);
    }

    InitializeGlutCallbacks();
    InitializeGL();
    InitializeGeometry();

    glutMainLoop();

    return 0;
}
