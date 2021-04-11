
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>

#include "Particle.h"
#include "CollisionSystem.h"
#include "utilities.h"
#include "fileIO.h"

int n;
int rad = 5;
CollisionSystem *cs;
PQueue *pq;
Particle **particles;
double limit = 100000;

Event *e;
Particle *a, *b;

void drawCircle(float r, float x, float y)
{
    float i = 0.0f;

    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(x, y); // Center
    for (i = 0.0f; i <= 360; i++)
        glVertex2f(r * cos(M_PI * i / 180.0) + x, r * sin(M_PI * i / 180.0) + y);

    glEnd();
}

// function to initialize
void myInit(void)
{
    // making background color black as first
    // 3 arguments all are 0.0
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // making picture color green (in RGB mode), as middle argument is 1.0
    glColor3f(0.0, 1.0, 0.0);

    // breadth of picture boundary is 1 pixel
    glPointSize(2.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // setting window dimension in X- and Y- direction
    gluOrtho2D(0, 1000, 0, 1000);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    float x, y;

    for (int i = 0; i < n; i++)
    {
        // glColor3f(i % 3, (i + 1) % 3, (i + 2) % 3);
        glColor3ub(particles[i]->color.R, particles[i]->color.G, particles[i]->color.B);
        drawCircle(1000 * particles[i]->radius, 1000 * particles[i]->rx, 1000 * particles[i]->ry);
    }

    glEnd();
    glFlush();
}

void idle()
{
    // printf("idle\n");
    // the main event-driven simulation loop
    while (!isEmptyPQ(pq))
    {

        // get next event, discard if invalidated
        e = dequeuePQ(pq);
        if (!isValid(e))
            continue;
        a = e->particle1;
        b = e->particle2;

        // printf("\n%lf %I64ld\n", cs->t, cs->pq->size);

        // physical collision, so update positions, and then simulation clock
        if (e->time > cs->t)
            for (int i = 0; i < cs->n; i++)
            {
                move(particles[i], e->time - cs->t);
            }
        cs->t = e->time;

        // process event
        if (e->type == particleCollision)
            bounceOff(a, b); // particle-particle collision
        else if (e->type == wallCollisionX)
            bounceOffVerticalWall(a); // particle-wall collision
        else if (e->type == wallCollisionY)
            bounceOffHorizontalWall(b); // particle-wall collision
        else if (e->type == noEvent)
        {
            if (cs->t < limit)
            {
                enqueuePQ(cs->pq, newEvent(NULL, NULL, (cs->t + 1)));
            }
            sleep_ms(20);
            glutPostRedisplay(); // Post a re-paint request to activate display()
            return;
        }

        // update the priority queue with new collisions involving a or b
        predict(cs, a, limit);
        predict(cs, b, limit);
        // printf("here\n");
    }
}

int main(int argc, char **argv)
{
    cs = CollisionSystemFromFile("input/p10.txt");
    n = cs->n;
    pq = cs->pq;
    particles = cs->particles;
    for (size_t i = 0; i < cs->n; i++)
    {
        predict(cs, particles[i], limit);
    }
    enqueuePQ(pq, newEvent(NULL, NULL, 0.0)); // redraw event

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // giving window size in X- and Y- direction
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(300, 0);

    // Giving name to window
    glutCreateWindow("Molecular Dynamics Simulation");
    myInit();

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMainLoop();
}