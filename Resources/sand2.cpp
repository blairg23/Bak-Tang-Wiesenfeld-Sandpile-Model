/*
   Two-dimensional sandpile automaton
 */

#include <cstdlib>
#include <iostream>
using namespace std;

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

int L = 200;                    // number of columns in x and y
int **slope;                    // slope at each column
bool **stable;                  // is the column stable?
GLubyte *image;                 // bitmap image of pile
int selectedSlope = 4;          // initial slope
bool addGrain;                  // add grain to steady state?

void allocate() {
    static int oldL = 0;
    if (oldL != L) {
        if (slope) {
            for (int i = 0; i < oldL; i++) {
                delete [] slope[i];
                delete [] stable[i];
            }
            delete [] slope;
            delete [] stable;
            delete [] image;
        }
        slope = new int* [L];
        stable = new bool* [L];
        image = new GLubyte [9 * L * L];
        for (int i = 0; i < L; i++) {
            slope[i] = new int [L];
            stable[i] = new bool [L];
        }
        oldL = L;
    }
}

void initialize() {
    allocate();
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < L; j++) {
            slope[i][j] = selectedSlope;
        }
    }
}

bool pileIsStable() {
    bool isStable = true;
    for (int i = 0; i < L; i++)
    for (int j = 0; j < L; j++) {
        if (slope[i][j] > 3)
            isStable = stable[i][j] = false;
        else
            stable[i][j] = true;
    }
    return isStable;
}

void topple(int i, int j) {
    slope[i][j] -= 4;
    if (i + 1 < L)
        ++slope[i + 1][j];
    if (i - 1 >= 0)
        ++slope[i - 1][j];
    if (j + 1 < L)
        ++slope[i][j + 1];
    if (j - 1 >= 0)
        ++slope[i][j - 1];
}

int mainWindow, sandpileWindow, colorsWindow;
int xSize, ySize, margin, colorSize;

void takeStep() {
    if (pileIsStable()) {
        if (addGrain) {
            int i = int((rand() / double(RAND_MAX)) * L);
            int j = int((rand() / double(RAND_MAX)) * L);
            ++slope[i][j];
        } else {
            glutIdleFunc(NULL);
        }
    } else {
        for (int i = 0; i < L; i++)
        for (int j = 0; j < L; j++)
            if (!stable[i][j])
                topple(i, j);
    }
    glutSetWindow(sandpileWindow);
    glutPostRedisplay();
}

GLubyte colors[8][3] = { {255, 255, 255}, {255, 255, 0},
                         {255, 127, 0},   {255, 0, 0},
                         {0, 255, 0},     {0, 255, 255},
                         {0, 0, 255},     {0, 0, 0}      };
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
    glutSetWindow(sandpileWindow);
    glutPostRedisplay();
}

void displaySandpile() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < L; i++)
    for (int j = 0; j < L; j++)
       for (int k = 0; k < 3; k++)
           image[3 * (L * i + j) + k] = colors[slope[i][j]][k];
    glRasterPos2i(0, 0);
    glDrawPixels(L, L, GL_RGB, GL_UNSIGNED_BYTE, image);
    glutSwapBuffers();
}

void displayColors() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3ub(255, 0, 255);
    glRectd(0, selectedSlope * colorSize, colorSize,
            (selectedSlope + 1) * colorSize);
    for (int i = 0; i < 8; i++) {
        glColor3ubv(colors[i]);
        glRectd(2, i * colorSize + 2, colorSize - 2, (i + 1) * colorSize - 2);
    }
    glutSwapBuffers();
}

void redraw() {
    glutSetWindow(sandpileWindow);
    glutPostRedisplay();
    glutSetWindow(colorsWindow);
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

bool running;
bool scribble;

void reset(int menuItem) {
    switch (menuItem) {
    case 1:                 // user selects "Reset" from popup
        initialize();
        redraw();
        break;
    case 2:                 // user selects "Scribble" from popup
        scribble = true;
        glutSetCursor(GLUT_CURSOR_SPRAY);
        break;
    default:
        break;
    }
}

void mouseSandpile (int button, int state, int x, int y) {
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN && !scribble) {
            if (running) {
                glutIdleFunc(NULL);
                running = false;
            } else {
                glutIdleFunc(takeStep);
                running = true;
            }
        } else if (state == GLUT_UP && scribble) {
            scribble = false;
            glutSetCursor(GLUT_CURSOR_INHERIT);
        }
        break;
    default:
        break;
    }
}

void motionSandpile(int x, int y) {
    if (scribble) {
        slope[L - y][x] = selectedSlope;
        glutPostRedisplay();
    }
}

void mouseColors (int button, int state, int x, int y) {
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN) {
            selectedSlope = int((1 - y / double(L)) * 8);
            redraw();
        }
        break;
    default:
        break;
    }
}

void makeSandpileWindow() {
    sandpileWindow = glutCreateSubWindow(mainWindow, margin, margin, L, L);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glShadeModel(GL_FLAT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glutDisplayFunc(displaySandpile);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseSandpile);
    glutMotionFunc(motionSandpile);
    glutCreateMenu(reset);              // create a popup menu
    glutAddMenuEntry("Reset", 1);
    glutAddMenuEntry("Scribble", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);  // attach it to right mouse button
}

void makeColorsWindow() {
    colorsWindow = glutCreateSubWindow(mainWindow, 2 * margin + L, margin,
                                       colorSize, L);
    glClearColor(0.5, 0.5, 0.5, 0.0);
    glutReshapeFunc(reshape);
    glutDisplayFunc(displayColors);
    glutMouseFunc(mouseColors);
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    if (argc > 1)
        L = atoi(argv[1]);
    initialize();
    margin = 20;
    colorSize = L / 8;
    xSize = L + colorSize + 3 * margin;
    ySize = L + 2 * margin;
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(xSize, ySize);
    glutInitWindowPosition(100, 100);
    mainWindow = glutCreateWindow("Two-dimensional sandpile automaton");
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glShadeModel(GL_FLAT);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    makeColorsWindow();
    makeSandpileWindow();
    glutMainLoop();
}
