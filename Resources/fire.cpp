/*
   Two-dimensional Forest Fire Model
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

int L = 400;                // number of trees in x and y
double pt;                  // initial probability to have a tree
double pf;                  // probability this tree is on fire
double p;                   // probability to grow a tree
enum {LIVE, FIRE, DEAD};    // 3 states of cells
vector< vector<int> > tree, // the trees
     oldTree;               // previous for synchronous updating
GLubyte *image;             // for drawing forest image
bool running;               // boolean variable to start/stop simulation
double pMult = 2;           // factor to increase/decrease p using menu

void initialize() {
    tree = vector< vector<int> >(L, vector<int>(L));
    oldTree = vector< vector<int> >(L, vector<int>(L));
    for (int i = 0; i < L; i++)
    for (int j = 0; j < L; j++) {
        if (rand() / double(RAND_MAX) < pt) {
            tree[i][j] = oldTree[i][j] = LIVE;
            if (rand() / double(RAND_MAX) < pf) {
                tree[i][j] = oldTree[i][j] = FIRE;
            }
        } else {
            tree[i][j] = oldTree[i][j] = DEAD;
        }
    }
    if (image) delete [] image;
    image = new GLubyte [9 * L * L];
}

void takeStep() {

    // save state of the forest
    for (int i = 0; i < L; i++)
    for (int j = 0; j < L; j++)
        oldTree[i][j] = tree[i][j];

    // probabilistic automaton update rule
    for (int i = 0; i < L; i++)
    for (int j = 0; j < L; j++) {
        switch (oldTree[i][j]) {
        case LIVE:
            // use periodic boundary conditions
            if ( oldTree[i > 0 ? i - 1 : L - 1][j] == FIRE ||
                 oldTree[i < L - 1 ? i + 1 : 0][j] == FIRE ||
                 oldTree[i][j > 0 ? j - 1 : L - 1] == FIRE ||
                 oldTree[i][j < L - 1 ? j + 1 : 0] == FIRE    )
                tree[i][j] = FIRE;
            break;
        case FIRE:
                tree[i][j] = DEAD;
            break;
        case DEAD:
            if (rand() / double(RAND_MAX) < p)
                tree[i][j] = LIVE;
            break;
        default:
            break;
        }
    }
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

GLubyte colors[3][3] = { {0, 255, 0}, {255, 255, 0}, {0, 0, 0} };

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < L; i++)
    for (int j = 0; j < L; j++)
        for (int k = 0; k < 3; k++)
            image[3 * (L * i + j) + k] = colors[tree[i][j]][k];
    glRasterPos2i(0, 0);
    glDrawPixels(L, L, GL_RGB, GL_UNSIGNED_BYTE, image);
    glutSwapBuffers();
}

void mouse (int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (running) {
            glutIdleFunc(NULL);
            running = false;
        } else {
            glutIdleFunc(takeStep);
            running = true;
        }
    }
}

void reset(int menuItem) {
    switch (menuItem) {
    case 1:                 // user selects "Increase p" from popup
        p *= pMult;
        if (p > 1)
          p = 1;
        break;
    case 2:                 // user selects "Decrease p" from popup
        p /= pMult;
        break;
    default:
        break;
    }
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    if (argc > 1)
        L = atoi(argv[1]);
    cout << " Enter probabilty p_t: ";
    cin >> pt;
    cout << " Enter probabilty p_f: ";
    cin >> pf;
    cout << " Enter probabilty p: ";
    cin >> p;
    initialize();
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(L, L);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Two-dimensional forest fire automaton");
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glShadeModel(GL_FLAT);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(NULL);
    glutMouseFunc(mouse);
    glutCreateMenu(reset);              // create a popup menu
    glutAddMenuEntry("Increase p", 1);
    glutAddMenuEntry("Decrease p", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);  // attach it to right mouse button
    glutMainLoop();
}
