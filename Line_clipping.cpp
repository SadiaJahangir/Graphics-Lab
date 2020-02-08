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
#include<math.h>
#define TOP 8
#define Bottom 4
#define Right 2
#define Left 1

static int slices = 16;
static int stacks = 16;
int x_0, x_1, y_0,y_1,call=0   ;
int xmax=120, ymax=100,xmin=-120,ymin=-100;

/* GLUT callback Handlers */

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
int findZone (int x0, int y0, int x1, int y1)
{
    int dx = x1 - x0, dy = y1 - y0;
    if (dx >= 0 and dy >= 0)
    {
        if (dx >= dy)   return 0;
        else       return 1;
    }
    else if (dx < 0 and dy >= 0)
    {
        if (-dx >= dy)   return 3;
        else    return 2;
    }
    else if (dx < 0 and dy < 0)
    {
        if (-dx >= -dy)   return 4;
        else     return 5;
    }
    else
    {
        if (dx >= -dy)   return 7;
        else   return 6;
    }
}
void drawPixel (int x, int y, int zone)
{
    glBegin(GL_POINTS);
    switch (zone)
    {
    case 0:
        glVertex2i(x, y);
        break;
    case 1:
        glVertex2i(y, x);
        break;
    case 2:
        glVertex2i(-y, x);
        break;
    case 3:
        glVertex2i(-x, y);
        break;
    case 4:
        glVertex2i(-x, -y);
        break;
    case 5:
        glVertex2i(-y, -x);
        break;
    case 6:
        glVertex2i(y, -x);
        break;
    case 7:
        glVertex2i(x, -y);
        break;
    }
    glEnd();
}
void drawLine0 (int x0, int y0, int x1, int y1, int zone)
{
    int dx = x1 - x0, dy = y1 - y0;
    int x = x0, y = y0, d = 2 * dy - dx;
    int de = 2 * dy, dne = 2 * (dy - dx);
    while (x <= x1)
    {
        drawPixel (x, y, zone);
        if (d < 0)
        {
            ++x;
            d += de;
        }
        else
        {
            ++x, ++y;
            d += dne;
        }
    }
}

void drawLine (int x0, int y0, int x1, int y1)
{
    int zone = findZone(x0, y0, x1, y1);
    switch (zone)
    {
    case 0:
        drawLine0(x0, y0, x1, y1, zone);
        break;
    case 1:
        drawLine0(y0, x0, y1, x1, zone);
        break;
    case 2:
        drawLine0(y0, -x0, y1, -x1, zone);
        break;
    case 3:
        drawLine0(-x0, y0, -x1, y1, zone);
        break;
    case 4:
        drawLine0(-x0, -y0, -x1, -y1, zone);
        break;
    case 5:
        drawLine0(-y0, -x0, -y1, -x1, zone);
        break;
    case 6:
        drawLine0(-y0, x0, -y1, x1, zone);
        break;
    case 7:
        drawLine0(x0, -y0, x1, -y1, zone);
        break;
    }
}

int makeCode(int x,int y)
{
    int code =0;
    if(y>ymax) ///TOP
        code +=8;
    else if(y<ymin)
        code+=4; ///bottom

    if(x>xmax)
        code+=2; /// right

    else if(x<xmin)
        code+=1; /// left

    return code;
}

void Cohen(int x0,int y0,int x1,int y1)
{
    int code0, code1, code,flag=0;
    int x,y;
    code0= makeCode(x0,y0);
    code1= makeCode(x1,y1);
    while(1)
    {
        if( !(code0 | code1))
        {
            /// AC
            if(flag==0)
            printf("Accepted\n");
            glColor3d(1,0,1);
        	glPointSize(7);
        	glBegin(GL_POINTS);
        	glVertex2i(x0,y0);
        	glVertex2i(x1,y1);
        	glEnd();
        	glPointSize(1);
        	glColor3d(1,1,1);
            drawLine(x0,y0,x1,y1);
            break;
        }
        else if( code0 & code1)
        {
            /// rejected

            printf("Rejected\n");
            glColor3d(1,0,0);
            drawLine(x0,y0,x1,y1);
            break;
        }
        else
        {
        	/// Partialy AC
        	flag=1;
        	if(flag==1)
        	printf("Partially Accepted\n");

        	if(code0) code=code0;
        	else code = code1;
        	if(code & TOP)
        	{
        		y=ymax;
        		x= x0 + ((y-y0)*(x1-x0)/(y1-y0));
        	}
        	else if(code & Bottom)
        	{
        		y=ymin;
        		x=x0+ ((y-y0)*(x1-x0)/(y1-y0));
        	}
        	else if(code & Right)
        	{
        		x=xmax;
        		y=y0+ ((x-x0)*(y1-y0) / (x1-x0));
        	}
        	else
        	{
        		x=xmin;
        		y=y0+ ((x-x0)*(y1-y0) / (x1-x0));
        	}
        	if( code == code0)
        	{
        		glColor3d(0,1,1);
        		drawLine(x0,y0,x,y);
        		glColor3d(1,1,1);
        		x0=x;
        		y0=y;
        		code0=makeCode(x0,y0);

        	}
        	else
        	{
        		glColor3d(0,1,0);
        		drawLine(x1,y1,x,y);
        		glColor3d(1,1,1);
        		x1=x;
        		y1=y;
        		code1=makeCode(x1,y1);
        	}

        }


    }
}
void myMouse (int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON and state == GLUT_UP)
    {
        if (call % 2 == 0)
        {
            x_0 = x - 320, y_0 = y - 240;
            y_0 *= -1;
        }
        else if (call % 2 == 1)
        {
            x_1 = x - 320, y_1 = y - 240;
            y_1 *= -1;
        }
       // printf("%d %d\n", x, y);
        glVertex2i(x_0, y_0);
        if (call % 2 == 1)
        {
            //printf("zone = %d\n", findZone(x_0, y_0, x_1, y_1));
        }
        ++call;
    }
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,1,1);
    glBegin(GL_LINES);
    glVertex2i(-320, 0);
    glVertex2i(319, 0);
    glVertex2i(0, -240);
    glVertex2i(0, 239);
    glVertex2i(120,100); // first boarder
    glVertex2i(-120,100);
    // 2nd horizontal boarder
    glVertex2i(120,-100);
    glVertex2i(-120,-100);
    // right vertical line
    glVertex2i(120,100);
    glVertex2i(120,-100);
    //left vertical line
    glVertex2i(-120,100);
    glVertex2i(-120,-100);

    glEnd();
    glEnable(GL_PROGRAM_POINT_SIZE);
    glColor3f(1.0,1.0,0.0);//yellow
    if(call%2==0 && call>=2)
    {
        printf("x0=%d y0= %d x1=%d y1=%d\n",x_0,y_0,x_1,y_1);
        glPointSize(7);
        glBegin(GL_POINTS);
        //printf("inside\n");
        glVertex2i(x_0, y_0); /// drawline ekhne call hbe    drawpixel(x,y)  er bodole glVertex2i likhboo
        glVertex2i(x_1, y_1); 
        glEnd();
        ///
       // drawLine(x_0,y_0,x_1,y_1);
        
    }
glPointSize(1);
    	glBegin(GL_POINTS);
    if(call%2==0 && call>=2)
    {

    	
    	Cohen(x_0,y_0,x_1,y_1);

    }
    glEnd();
    //CohenSutherland(x_0,y_0,x_1,y_1);

//   }
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
    glutMouseFunc(myMouse);
//    glutMouseFunc(myMouse);

    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glutMainLoop();

    return EXIT_SUCCESS;
}
