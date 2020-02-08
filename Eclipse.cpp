/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include<stdio.h>
#include <math.h>

static int slices = 16;
static int stacks = 16;

/* GLUT callback Handlers */

static void resize(int width, int height){
    const float ar = (float) width / (float) height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-320, 319, -240, 239, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

void Draw4Way(int xc,int yc,int x,int y)
{
    glVertex2i(xc+x,yc+y);
	glVertex2i(xc+x,yc-y);
	glVertex2i(xc-x,yc+y);
	glVertex2i(xc-x,yc-y);

}

void Eclipse(int a, int b,int center_x,int center_y)
{
    int x=0,y=b;
    Draw4Way(center_x,center_y,x,y);
    double d1=  (b*b) - ((a*a)*b) + (.25*(a*a));
    while( ((a*a)*(y-.05)) > ((b*b)*(x+1)) )
    {
        if(d1<0) /// E
        {
            d1+= (b*b) * ((2*x) + 3);
            x++;
        }
        else
        { ///SE
            d1+= ((b*b) * ((2*x) + 3)) + ((a*a)*((-2*y)+2)) ;
            y--;
            x++;
        }
        Draw4Way(center_x,center_y,x,y);

    }
    double d2= ((b*b)*(x+ 0.5)*(x+0.5)) + ((a*a)*(y-1)*(y-1)) - ((a*a)*(b*b));
    while(y>=0)
    {
        if(d2<0) ///SE
        {
            d2+= ((b*b)*(2*x+2)) + ((a*a)*((-2*y)+3));
            x++;
            y--;
        }
        else ///s
        {
            d2+=  ((a*a)*((-2*y)+3));
            y--;
        }
        Draw4Way(center_x,center_y,x,y);
    }
}

static void display(void){
    int x = 10, y = 20;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,1,1);
    glBegin(GL_LINES);
    glVertex2i(-320, 0);
    glVertex2i(319, 0);
    glVertex2i(0, -240);
    glVertex2i(0, 239);
    glEnd();
    glPointSize(4);
    glBegin(GL_POINTS);
    int centerx=100,centery=50,a=100,b=50;
    glVertex2i(centerx,centery);
    Eclipse(a,b,centerx,centery);
    glEnd();


    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

        case '+':
            slices++;
            stacks++;
            break;

        case '-':
            if (slices>3 && stacks>3)
            {
                slices--;
                stacks--;
            }
            break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

/* Program entry point */

int main(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Experiment 01");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glutMainLoop();

    return EXIT_SUCCESS;
}

