#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>        // Header File For The GLut Library
#include <cmath>
#include <glm/glm.hpp>
#define kWindowWidth	1200
#define kWindowHeight	1200

GLfloat axisCam = 20;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0, 0, -20);
glm::vec3 cameraUp = glm::vec3(0, 1, 0);

float shininess = 93;

float ambient[] = {0.5, 0.3, 0.5, 1};
GLfloat dir[] = {-1,-1,-1};

GLfloat mat_specular2[] = {1.0, 1.0, 1.0, 1.0 };
GLfloat mat_specular1[] = { 0.0, 0.0, 0.0, 0.0 };

GLfloat cameraSpeed = 0.1;

GLboolean isCameraSpin = false;
GLboolean isObjectSpin = false;
GLboolean isLightSpin = false;
GLfloat spinObj = 0;
GLfloat spinCamera = 0;
GLfloat spinLight = 0;
GLboolean keys[1024];

GLfloat vertex[] = {-1,-1,0,   1,-1,0,   1,1,0,   -1,1,0};
GLfloat normal[] = {-1,-1,4,   1,-1,4,   1,1,4,   -1,1,4};

using namespace std;

GLvoid InitGL(GLvoid);
GLvoid DrawGLScene(GLvoid);
GLvoid ReSizeGLScene(int Width, int Height);
GLvoid processNormalKeys(unsigned char key, int x, int y);
GLvoid DrawLight(GLvoid);
GLvoid DrawSpheres(GLvoid);
GLvoid DrawAxis(GLvoid);
GLvoid MoveCamera(GLvoid);
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

GLvoid DrawSpheres(GLvoid) {
    GLfloat  diffuse_light1[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    GLfloat  diffuse_light2[] = { 0.6f, 0.6f, 0.6f, 1.0f };

    glDisable(GL_NORMALIZE);
    glDisable(GL_COLOR_MATERIAL);
    glPushMatrix();
    glTranslated(9, 4, 10);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular1);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffuse_light1);
    glutSolidSphere(2.9, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, 0, 0);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffuse_light2);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular2);
    glutSolidSphere(2.9, 20, 20);
    glPopMatrix();
}

GLvoid DrawGLScene(GLvoid) {
    do_movement();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);
    glPushMatrix();
    MoveCamera();
    glPushMatrix();
        glRotatef(-50, 1, 0, 0);
        glRotatef(-spinLight, 1, 0, 0);
        glTranslated(0, 0, 20);
        GLfloat position[] = {0,0,1,0};
        glLightfv(GL_LIGHT0, GL_POSITION, position);
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
        glColor3d(1, 1, 1);
        DrawLight();

    glTranslatef(0,0,1);
        glScalef(0.2,0.2,0.2);
        glColor3f(1, 1, 1);
    glPopMatrix();
    DrawAxis();
    glPushMatrix();
    DrawSpheres();

    glPopMatrix();
    glPopMatrix();
    glutSwapBuffers();
}

GLvoid DrawLight(GLvoid) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, normal);
        glVertexPointer(3, GL_FLOAT, 0, vertex);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

GLvoid DrawAxis(GLvoid) {
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glLineWidth(5);
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
    glEnable(GL_LIGHTING);
}

GLvoid MoveCamera(GLvoid) {

    glMatrixMode(GL_MODELVIEW); //видовая матрица
    if (isCameraSpin) {
        spinCamera += 1;
    }
    if (isObjectSpin) {
        spinObj -= 1;
    }
    if (isLightSpin) {
        spinLight -= 1;
    }
    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, //местоположение камеры
              cameraPos.x + cameraFront.x, cameraPos.y + cameraFront.y,
              cameraPos.z + cameraFront.z, //камера смотрит в эту точку
              cameraUp.x, cameraUp.y, cameraUp.z); //направление вектора «вверх»
    glRotatef(axisCam, 1, 0, 0); //поворот вокруг оси X
    glRotatef(-spinCamera, 0, 1, 0); //поворот вокруг оси Y
}

GLvoid do_movement(GLvoid) {
    if(keys[27])
        exit(0);
    if (keys['u']) {
        if (shininess <= 128)
            shininess += 1;
    }
    if (keys['j']) {
        if (shininess >= 0)
            shininess -= 1;
    }
    if (keys['w'])
        cameraPos += cameraSpeed * cameraFront;
    if (keys['s'])
        cameraPos -= cameraSpeed * cameraFront;
    if (keys['d'])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (keys['a'])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (keys[GLUT_KEY_F1])
        glutFullScreen();
}

GLvoid processNormalKeys(unsigned char key, int x, int y) {
    keys[key] = true;
    switch (key) {
        case 'e':
            isObjectSpin = !isObjectSpin;
            break;
        case 'r':
            isCameraSpin = !isCameraSpin;
            break;
        case 'l':
            isLightSpin = !isLightSpin;
            break;
        default: break;

    }
}

GLvoid processUpKeys(unsigned char key, int, int) {
    keys[key] = false;
}

GLvoid processSpecialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            axisCam += 2;
            break;
        case GLUT_KEY_DOWN:
            axisCam -= 2;
            break;
        case GLUT_KEY_RIGHT:
            spinCamera += 2;
            break;
        case GLUT_KEY_LEFT:
            spinCamera -= 2;
            break;
    }
}

GLvoid InitGL(GLvoid) {
    glClearDepth(1.0f);		// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);    // Enables Depth Testing
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

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