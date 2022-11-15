#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>        // Header File For The GLut Library
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "iostream"
#define kWindowWidth	1200
#define kWindowHeight	1200

GLfloat axisCam = 20;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0, 0, -20);
glm::vec3 cameraUp = glm::vec3(0, 1, 0);

float theta = 1;


GLfloat cameraSpeed = 0.1;

GLboolean isCameraSpin = false;
GLboolean isObjectSpin = false;
GLfloat spinObj = 0;
GLfloat spinCamera = 0;
GLboolean keys[1024];

GLfloat vertex[] = {-1,-1,0,   1,-1,0,   1,1,0,   -1,1,0};
GLfloat normal[] = {-1,-1,4,   1,-1,4,   1,1,4,   -1,1,4};

using namespace std;

GLvoid InitGL(GLvoid);
GLvoid DrawGLScene(GLvoid);
GLvoid ReSizeGLScene(int Width, int Height);
GLvoid processNormalKeys(unsigned char key, int x, int y);
GLvoid DrawObject(GLvoid);
GLvoid DrawAxis(GLvoid);
GLvoid processSpecialKeys(int key, int x, int y);
GLvoid processUpKeys(unsigned char, int, int);
GLvoid do_movement(GLvoid);



int main(int argc, char **argv) {

    // инициализация
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(kWindowWidth, kWindowHeight);
    glutCreateWindow("Лаба 6");

    InitGL();
    glutDisplayFunc(DrawGLScene);
    glutReshapeFunc(ReSizeGLScene);
    glutIdleFunc(DrawGLScene);

    // обработка клавиш
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);
    glutKeyboardUpFunc(processUpKeys);
//    glutMouseFunc(procerssMouseKeys);

    // Основной цикл GLUT
    glutMainLoop();

    return 1;
}

GLvoid DrawGLScene(GLvoid) {
    do_movement();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);
    glPushMatrix();
    glRotated(-60, 1, 0, 0);
    glRotated(33, 0, 0, 1);
    glTranslated(3, 4, -3);
    glPushMatrix();
        glRotatef(theta, 0, 1, 0);
        GLfloat position[] = {0,0,1,0};
        glLightfv(GL_LIGHT0, GL_POSITION, position);

        glTranslatef(0,0,1);
        glScalef(0.2,0.2,0.2);
        glColor3f(1, 1, 1);
        DrawObject();
    glPopMatrix();
    DrawAxis();
    glColor3f(0, 1, 0);
    glutSolidSphere(0.5, 40, 40);
    glPopMatrix();

    glutSwapBuffers();
    theta += 2;
}

GLvoid DrawObject(GLvoid) {
    glNormal3f(0,0,1);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, normal);
        glVertexPointer(3, GL_FLOAT, 0, vertex);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

GLvoid DrawAxis(GLvoid) {
    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1000, 0, 0);
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1000, 0);
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1000);
    glEnd();
}

GLvoid do_movement(GLvoid) {
    if(keys[27])
        exit(0);
    if (keys['w'])
        cout << "Key W" << endl;
}

GLvoid processNormalKeys(unsigned char key, int x, int y) {
    keys[key] = true;
    switch (key) {
        case 'e':
            cout << "Key E" << endl;
        default: break;

    }
}

GLvoid processUpKeys(unsigned char key, int, int) {
    keys[key] = false;
}

GLvoid processSpecialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            cout << "Key Up" << endl;
            break;
        default: break;
    }
}

GLvoid InitGL(GLvoid) {
//    glShadeModel(GL_SMOOTH);	// Enable Smooth Shading (blends colours across a polygon/smoothed lighting
//
//     Set-up the depth buffer
//    glClearDepth(-1.0f);		// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);    // Enables Depth Testing
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
//    glDepthFunc(GL_LEQUAL);		// The Type Of Depth Testing To Do
//
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

}

GLvoid ReSizeGLScene(int w, int h) {
    // предотвращение деления на ноль
    if (h == 0)
        h = 1;
    float ratio = w * 1.0 / h;
    // используем матрицу проекции
    glMatrixMode(GL_PROJECTION);
    // обнуляем матрицу
    glLoadIdentity();
    // установить параметры вьюпорта
    glViewport(0, 0, w, h);
    // установить корректную перспективу
    gluPerspective(40.0f, ratio, 0.1f, 1000.0f);
    // вернуться к матрице проекции
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}