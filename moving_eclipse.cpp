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
#include <stdio.h>
#include <math.h>

static int slices = 16;
static int stacks = 16;

/* GLUT callback Handlers */

int call;
int x_0, x_1, y_0, y_1,r_0=10,r_1=15;

int xmax=319, xmin= -320, ymax= 239, ymin= -240;
int dx_0=1, dy_0=1, dx_1=1, dy_1=1;



static void resize(int width, int height)
{
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
        {
            ///SE
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
void draw8way(int x, int y, int a, int b)
{
    glVertex2i(x+a, y+b);
    glVertex2i(y+a, x+b);
    glVertex2i(-y+a, x+b);
    glVertex2i(x+a, -y+b);
    glVertex2i(-x+a, -y+b);
    glVertex2i(-y+a, -x+b);
    glVertex2i(y+a, -x+b);
    glVertex2i(-x+a, y+b);


}

void drawCircle_1(int a, int b, int r)
{

    int x=0, y=r;
    int d= 5 - 4*r;
    draw8way(x,y,a,b);
    while(x<y)
    {
        if(d<0)
        {
            d+=(8*x+12);
            x++;
        }
        else
        {

            d+=(8*x-8*y+20);
            y--;
            x++;
        }
        draw8way(x,y,a,b);
    }


}


void reflectCircle(int xc,int yc,int r)
{
    int t=1;
    int cont=0;
    if (call!=0)
    {
    while(1){

        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_POINTS);
        glVertex2i(xc,yc);
        drawCircle_1(xc,yc,r);
        int y=yc;
        while(yc-r>ymin)
        {

            glClear(GL_COLOR_BUFFER_BIT);

            glBegin(GL_POINTS);

            if(yc < ((int) (y *.25) )) t=2;
            if(yc < ((int) (y *.50) )) t=3;
            if(yc < ((int) (y *.100) )) t=4;
            yc-=t;
            drawCircle_1(xc,yc,r);

            glEnd();

            glutSwapBuffers();


        }
        int A=r;
        int B=r;
        for(int i=0; i<3; i++)
        {
            glClear(GL_COLOR_BUFFER_BIT);

            glBegin(GL_POINTS);
            A+=10;
            B-=1;
            yc-=1;
            Eclipse(A,B,xc,yc);
            glEnd();

            glutSwapBuffers();





        }
        for(int i=0; i<3; i++)
        {
            glClear(GL_COLOR_BUFFER_BIT);
            glBegin(GL_POINTS);
            A-=10;
            B+=1;
            yc+=1;
            Eclipse(A,B,xc,yc);
            glEnd();
            glutSwapBuffers();

        }
    y=ymin;
    /// UPPER BOUNDARY
    while(yc+r <= ymax)
    {
        glClear(GL_COLOR_BUFFER_BIT);

      if(yc>-140) t=3;
      if(yc>-100) t=2;
      if(yc>0) t=1;
        yc+=t;
         glBegin(GL_POINTS);
        drawCircle_1(xc,yc,r);
        glEnd();
        glutSwapBuffers();

    }
        A=r;
        B=r-2; /// yc= 210 comes every time, so for touching boundary Eclipse's B needs to be such that it is less than ymax.


        for(int i=0; i<3; i++)
        {
            glClear(GL_COLOR_BUFFER_BIT);
            glBegin(GL_POINTS);
            A+=10;
            B-=1;
            yc+=1;
            Eclipse(A,B,xc,yc);
            glEnd();
            glutSwapBuffers();
        }

        for(int i=0; i<3; i++)
        {
            glClear(GL_COLOR_BUFFER_BIT);
            glBegin(GL_POINTS);
            A-=10;
            yc-=1;
            B+=1;
            Eclipse(A,B,xc,yc);

            glEnd();
            glutSwapBuffers();

        }

        glEnd();
        glFlush();
        }

    }

}


void myMouse (int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON and state == GLUT_UP)
    {

        x_0 = x - 320, y_0 = y - 240;
        y_0 *= -1;
        printf("%d %d\n", x, y);
        ++call;
    }
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    glBegin(GL_POINTS);

    glutMouseFunc(myMouse);
    int centerx=x_0,centery=y_0,r=30;
    reflectCircle(centerx,centery,r);


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

int main(int argc, char *argv[])
{
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



