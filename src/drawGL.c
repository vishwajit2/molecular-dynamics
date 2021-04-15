#include <stdio.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#include "Particle.h"
#include "CollisionSystem.h"
#include "utilities.h"
#include "Simulation.h"
#include "fileIO.h"

#define windowWidth 1000  // windowed mode's width
#define windowHeight 1000 // Windowed mode's height
#define windowPosX 50     // Windowed mode's top-left corner x
#define windowPosY 20     // Windowed mode's top-left corner y

#define title "Molecular Dynamics"
#define margin 2

int boxDim = 1000 - 2 * margin;
CollisionSystem *cs;
Simulation *sim;

void drawSquare(float r, float x, float y)
{
    float i = 0.0f;
    glPolygonMode(GL_FRONT, GL_FILL);
    glBegin(GL_QUADS);
    glVertex2f(x - r, y - r);
    glVertex2f(x - r, y + r);
    glVertex2f(x + r, y + r);
    glVertex2f(x + r, y - r);
    glEnd();
    glEnd();
}

void drawCircle(float r, float x, float y)
{
    float i = 0.0f;

    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(x, y); // Center
    // smaller the increment in i for each iteration of loop, circle would be more perfect.
    // 4 is good for our particles
    for (i = 0.0f; i <= 360; i += 4)
        glVertex2f(r * cos(M_PI * i / 180.0) + x, r * sin(M_PI * i / 180.0) + y);

    glEnd();
}

void drawTrails()
{
    if (!sim->traildata)
        return;
    TrailNode *t = sim->traildata;
    iPair *data;
    glBegin(GL_POINTS);
    while (t)
    {
        glColor3ub(t->color.R, t->color.G, t->color.B);
        data = t->data;
        if (t->isFull)
        {
            for (int i = 0; i < max_trail_length; i++)
                glVertex2i(data[i].x, data[i].y);
        }
        else
        {
            for (int i = 0; i < t->head; i++)
                glVertex2i(data[i].x, data[i].y);
        }

        // add current position to trails
        data[t->head].x = margin + boxDim * cs->particles[t->index]->rx;
        data[t->head].y = margin + boxDim * cs->particles[t->index]->ry;
        t->head++;
        if (t->head == max_trail_length)
        {
            t->head = 0;
            t->isFull = true;
        }
        t = t->next;
    }
    glEnd();
}

/* Initialize OpenGL Graphics */
void initGL()
{
    glClearColor(0.0, 0.0, 0.0, 1.0); // Set background (clear) color to black
    glPointSize(2.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // setting window dimension in X- and Y- direction
    gluOrtho2D(0, 1000, 0, 1000);
}

void reshape(int w, int h)
{
    boxDim = min(w, h) - 2 * margin;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3ub(0, 255, 255);
    glBegin(GL_LINE_LOOP);
    glVertex2f(margin, margin);
    glVertex2f(margin + boxDim, margin);
    glVertex2f(margin + boxDim, margin + boxDim);
    glVertex2f(margin, margin + boxDim);
    glEnd();

    drawTrails();

    glColor3ub(cs->particles[0]->color.R, cs->particles[0]->color.G, cs->particles[0]->color.B);
    int i = 0;
    for (; i < sim->wallBalls; i++)
    {
        drawSquare(boxDim * cs->particles[i]->radius, margin + boxDim * cs->particles[i]->rx, margin + boxDim * cs->particles[i]->ry);
    }

    for (; i < cs->n; i++)
    {
        // glColor3f(i % 3, (i + 1) % 3, (i + 2) % 3);
        glColor3ub(cs->particles[i]->color.R, cs->particles[i]->color.G, cs->particles[i]->color.B);
        drawCircle(boxDim * cs->particles[i]->radius, margin + boxDim * cs->particles[i]->rx, margin + boxDim * cs->particles[i]->ry);
    }

    glutSwapBuffers();
}

void simulate(int k)
{
    if (cs->t >= sim->limit)
    {
        saveStateToFile(sim);
        glutLeaveMainLoop();
    }

    if (!sim->pause)
    {
        if (sim->records)
            updateParticleRecord(sim);

        if (sim->sp)
        {
            advanceSP(sim);
            if ((int)sim->cs->t % sampling_interval == 0 && (int)sim->cs->t != 0)
            {
                system("clear");
                displaySystemProperties(sim->sp);
            }
        }
        else
            advance(cs, sim->limit);
        glutPostRedisplay(); // Post a re-paint request to activate display()
    }
    glutTimerFunc(refresh_interval, simulate, k);
}

void keyboard(unsigned char key, int mx, int my)
{

    switch (key)
    {
    case 112:
        sim->pause = !sim->pause;
        // glutTimerFunc(0, advance, 0);
        break; // "p" toggles pause
    }
}
void setGL(int argc, char **argv)
{
    glutInit(&argc, argv);                          // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);    // Enable double buffered mode
    glutInitWindowSize(windowWidth, windowHeight);  // Initial window width and height
    glutInitWindowPosition(windowPosX, windowPosY); // Initial window top-left corner (x, y)
    glutCreateWindow(title);                        // Create window with given title
    glutDisplayFunc(display);                       // Register callback handler for window re-paint
    glutReshapeFunc(reshape);                       // Register callback handler for window re-shape
    glutTimerFunc(0, simulate, 0);                  // First timer call immediately
    // glutSpecialFunc(keyboard);                      // Register callback handler for special-key event
    glutKeyboardFunc(keyboard);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
    initGL();       // Our own OpenGL initialization
    glutMainLoop(); // Enter event-processing loop
}

int main(int argc, char *argv[])
{
    // ParticleConfig conf = getParticleConfig();
    // conf.radius = 0.1;
    // conf.vel_hi = 0.02;
    // conf.vel_lo = -0.02;
    double limit = 500;
    sim = newSimulation(limit, "input/diffusion.txt", false, 3, NULL);
    cs = sim->cs;
    buildEventQueue(sim->cs, limit);
    setGL(argc, argv);
    return 0;
}
