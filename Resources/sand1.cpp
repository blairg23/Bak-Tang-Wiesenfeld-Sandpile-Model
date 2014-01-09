/*
   One dimensional sandpile cellular automaton model
 */

#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

int L = 20;                 // length of sandpile (number of columns)
int criticalSlope = 1;      // sand topples if slope exceeds this value

vector<int> height;         // height of pile of grains
vector<int> slope;          // slope of sandpile
vector<bool> stable;        // true if slope is <= critical slope

void initialize() {         // initialize to empty sandpile
    height = vector<int>(L);
    slope = vector<int>(L);
    stable = vector<bool>(L);
    for (int i = 0; i < L; i++) {
        slope[i] = height[i] = 0;
        stable[i] = true;
    }
}

bool pileIsStable() {       // check pile stability and reset slopes
    bool pileStable = true;
    for (int i = 0; i < L; i++) {
        slope[i] = height[i];
        if (i < L - 1)
            slope[i] -= height[i + 1];
        stable[i] = true;
        if (slope[i] > criticalSlope)
            stable[i] = pileStable = false;
    }
    return pileStable;
}

void takeStep() {           // local update rule
    if (pileIsStable()) {   // add a grain to a randomly chosen column
        int i = int(rand() / double(RAND_MAX) * L);
        ++height[i];
    } else {                // topple one grain from each unstable column
        for (int i = 0; i < L; i++)
            if (!stable[i]) {
                --height[i];           // decrease unstable column height by 1
                if (i < L - 1)
                    ++height[i + 1];   // unstable grain topples to right
            }
    }

    glutPostRedisplay();    // request glut to repaint the window
}

int xPixels = 400;          // width of window in pixels
int yPixels = 400;          // height of window in pixels
bool running;               // is the animation running?

void display() {            // display callback function - repaint window
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.4, 0.0);      // this is a shade of orange in RGB

    for (int i = 0; i < L; i++) {
        for (int j = 0; j < height[i]; j++) {
            glRectd(i, j, i + 0.9, j + 0.9);
        }
    }

    glutSwapBuffers();      // draw the buffer in memory to the screen
}

void reshape(int w, int h) {   // invoked when shape of window changes
    glViewport(0, 0, w, h);    // use the full window for drawing
    glMatrixMode(GL_PROJECTION);    // select projection matrix
    glLoadIdentity();               // set it to the identity
    glOrtho(0, L, 0, L * h / double(w), -1.0, 1.0);  // orthographic projection
    // columns fill window, and grains are drawn square
}

void reset(int menuItem) {  // callback function for popup menu
    switch (menuItem) {
    case 1:                 // user selects "Reset" from popup
        initialize();
        break;
    case 2:                 // user selects "++ critical slope"
        criticalSlope += 1;
        break;
    case 3:                 // user selects "-- critical slope"
        criticalSlope -= 1;
        break;
    default:
        break;
    }
}

void mouse(int button, int state, int x, int y) {
    switch (button) {
    case GLUT_LEFT_BUTTON:  // use left button to toggle animation on/off
        if (state == GLUT_DOWN) {
            if (running) {              // animation is running
                glutIdleFunc(NULL);     // set idle callback to do nothing
                running = false;        // now not running
            } else {
                glutIdleFunc(takeStep); // set idle callback to take step
                running = true;         // now running
            }
        }
        break;
    default:
        break;
    }
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(xPixels, yPixels);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("One Dimensional Sandpile Automaton");
    if (argc > 1)                       // first argument of command line
        L = atoi(argv[1]);              //  is the number of columns
    initialize();
    glClearColor(1.0, 1.0, 1.0, 0.0);   // background color white
    glShadeModel(GL_FLAT);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutCreateMenu(reset);              // create a popup menu
    glutAddMenuEntry("Reset", 1);
    glutAddMenuEntry("++ critical slope", 2);
    glutAddMenuEntry("-- critical slope", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);  // attach it to right mouse button
    glutMouseFunc(mouse);
    glutMainLoop();
}
