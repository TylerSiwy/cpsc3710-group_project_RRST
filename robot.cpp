#include <stdlib.h>
#include <stdio.h>    
#include <string.h>
#include <string>
#include <GL/gl.h>   // OpenGL itself.
#include <GL/glu.h>  // GLU support library.
#include <GL/glut.h> // GLUT support library.
#include <iostream>
using namespace std;

int Window_ID;
int Window_Width = 600;
int Window_Height = 600;
#define PROGRAM_TITLE "Tyler Siwy"

float hScale = 2.0;
float eyeX = 0;
float eyeY = 0;
float eyeZ = -5;
float lookAtX = 0;
float lookAtY = 0;
float lookAtZ = 0;
double rotate_pyramid = 0; 
double rotate_cube = 0;
double rotate_sphere = 0;
bool rS = true;
bool rC = true;
bool rP = true;

GLUquadric *eyeQuad = gluNewQuadric();
GLUquadric *antQuad = gluNewQuadric();

void Display(void);
void MyInit();
void myKeyboardKey(unsigned char key, int x, int y);
void myMouse(int button, int state, int x, int y);
void drawHead();
void specialKeys( int key, int x, int y );
void rotateObjects();
void drawEyes();
void drawAntenna();
void drawCube(float xScale, float yScale, float zScale,
	      float xTrans, float yTrans, float zTrans);

int main (int argc, char **argv){
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

   // Set Window size
   glutInitWindowSize(Window_Width, Window_Height);

   // Create and Open a window with its title.
   Window_ID = glutCreateWindow(PROGRAM_TITLE);

   // Register and install the callback function to do the drawing. 
   glutDisplayFunc(&Display);

   // If there's nothing to do, draw.
   glutIdleFunc(&Display);

   glEnable(GL_DEPTH_TEST);
   MyInit();

   // Input functions
   glutKeyboardFunc(&myKeyboardKey);
   glutMouseFunc(myMouse);

   glutMainLoop(); 
   return 0;
}


void Display(void){
   glLoadIdentity();
   glOrtho(-10.0, 10.0, -10.0, 10.0, 200.0, -200.0);
   gluLookAt(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, 0.0, 1.0, 0.0); 
   // Clear the color and depth
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   drawHead();
   glutSwapBuffers();
}

void MyInit(){
   // Color to clear color buffer to.
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
}

void myKeyboardKey(unsigned char key, int x, int y){
   switch(key){
      cout << key;
      case 'z': // z
	 rC = rS = true;
	 rP = false;	 
	 break;
      case 'a':
	 rC = false;
	 rP = rS = true;
	 break;
      case 'q':
	 rS = rP = rC = false;
	 break;
      case 32:
	 rS = rP = rC = true; 
	 break;
      default:
	 printf ("KP: No action for %d.\n", key);
	 break;
   }
}

void myMouse(int button, int state, int x, int y){
   switch(button){
      case GLUT_LEFT_BUTTON:
	 if(state == GLUT_DOWN ){
	    eyeX = 0;
	    eyeY = 0;
	    eyeZ = -15.0;
	 }else{
	    eyeX = eyeY = 0;
	    eyeZ = 5;
	 }
	 break;
      case GLUT_RIGHT_BUTTON:
	 if(state == GLUT_DOWN ){
	    eyeX = eyeY = eyeZ = -5.0;
	 }else{
	    eyeX = eyeY =0;
	    eyeZ = 5;
	 }
	 break;	 
   }
}
void drawEyes(){
   float eyeRadius = 0.5;
   float eyeHeight = 0.25;
   float resolution = 10;
   float headCenterOffset = hScale + 0.01;
   //Right EyebackHeadScale
   glPushMatrix();   
   glTranslatef(hScale/2, hScale/3, headCenterOffset);
   glRotatef(180, 0, 1, 0);
   glColor3f(1.0,1.0, 1.0);
   gluDisk(eyeQuad, 0, eyeRadius, resolution, 1);
   glPopMatrix();
   //Left Eye
   glPushMatrix();
   glTranslatef(-hScale/2, hScale/3, headCenterOffset);
      glRotatef(180, 0, 1, 0);
   glColor3f(1.0, 1.0, 1.0);
   gluDisk(eyeQuad, 0, eyeRadius, resolution, 1);
   glPopMatrix();
}

void drawAntenna(){
   float antRadius = 0.15;
   float antHeight = 1.35;
   float antRotation = 270;
   glPushMatrix();
   glTranslatef(0, hScale, 0);
   glRotatef(antRotation, 1.0, 0 ,0.0);
   glColor3f(0.6,0.1,0.3); //Purple
   gluCylinder(antQuad, antRadius, 0.05, antHeight, 10, 10); //Cone Shaped
   drawCube(0.5, 0.1, 0.2, 0, 0, antHeight);
   glPopMatrix();
   
}

void drawHead(){  
   float backHeadScale = hScale * 0.55;
   glPushMatrix();
   glRotatef(rotate_cube, 0.0, 0.0, 1.0);
   drawCube(hScale, hScale, hScale, 0, 0, 0);
   
   drawEyes();
   drawAntenna();
   // square on back of head for identification
   glBegin(GL_POLYGON);
   glColor3f(0.5, 0.5, 1.0);
   glVertex3f(backHeadScale, -backHeadScale, -hScale-0.1);
   glVertex3f(backHeadScale, backHeadScale, -hScale-0.1);
   glVertex3f(-backHeadScale, backHeadScale,-hScale-0.1);
   glVertex3f(-backHeadScale, -backHeadScale, -hScale-0.1);
   glEnd();
   glPopMatrix();
}

void drawCube(float xScale, float yScale, float zScale,
	      float xTrans, float yTrans, float zTrans){  
   glPushMatrix();
   glTranslatef(xTrans ,yTrans, zTrans);
   /*Cutting these in half since each vertex is the scale distance from the
     centerpoint and we want it to be to the scale provided, not twice as large.
     We could probably find a better solution later. */
   /*xScale = xScale / 2;
   yScale = yScale / 2;
   zScale = zScale / 2;*/
   // Puke Green - FRONT
   glBegin(GL_POLYGON);
   glColor3f(0.1, 0.5, 0.5);
   glVertex3f(xScale, -yScale, -zScale); 
   glVertex3f(xScale, yScale, -zScale); 
   glVertex3f(-xScale, yScale, -zScale);
   glVertex3f(-xScale, -yScale, -zScale); 
   glEnd();
 
   // Brown Face - BACK
   glBegin(GL_POLYGON);
   glColor3f(0.8, 0.5, 0.3);  
   glVertex3f(-xScale, -yScale, zScale);
   glVertex3f(-xScale, yScale, zScale);
   glVertex3f(xScale, yScale, zScale);
   glVertex3f(xScale, -yScale, zScale);
   glEnd();
 
   // Purple side - RIGHT
   glBegin(GL_POLYGON);
   glColor3f(1.0, 0.0, 1.0);
   glVertex3f( xScale, -yScale, zScale);
   glVertex3f( xScale, yScale, zScale);
   glVertex3f( xScale, yScale, -zScale);
   glVertex3f( xScale, -yScale, -zScale);
   glEnd();
 
   // Green side - LEFT
   glBegin(GL_POLYGON);
   glColor3f(0.0, 1.0, 0.0);
   glVertex3f(-xScale, -yScale, -zScale);
   glVertex3f(-xScale, yScale, -zScale);
   glVertex3f(-xScale, yScale, zScale);
   glVertex3f(-xScale, -yScale, zScale);
   glEnd();
 
   // Blue side - TOP
   glBegin(GL_POLYGON);
   glColor3f(0.0, 0.0, 1.0);
   glVertex3f(-xScale, yScale, zScale);
   glVertex3f(-xScale, yScale, -zScale);
   glVertex3f(xScale, yScale, -zScale);
   glVertex3f(xScale, yScale, zScale);
   glEnd();
 
   // Red side - BOTTOM
   glBegin(GL_POLYGON);
   glColor3f(1.0, 0.0, 0.0);
   glVertex3f(-xScale, -yScale, -zScale);
   glVertex3f(-xScale, -yScale, zScale);
   glVertex3f(xScale, -yScale, zScale);
   glVertex3f(xScale, -yScale, -zScale);
   glEnd();

   glPopMatrix();
}
