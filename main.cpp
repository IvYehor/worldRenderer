#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <unordered_map>
#include "PerlinNoise.hpp"
#include "mymath.h"
#include "terrain.h"
#include "player.h"

#define W 1000
#define H 750

void displayBegin();
void display();
void onMouseClick(int, int, int, int);
void onKeyboardPress(unsigned char, int, int);
void onKeyboardRelease(unsigned char, int, int);
void onSpecialPress(int, int, int);
void onSpecialRelease(int, int, int);
void onMouseMove(int, int);
void showGround();
void translateCamera();
void initMatrix();

float lightPosition[4] = {0.0f, 0.0f, 1.0f, 0.0f};
float angle = 1.0f;

int mousePosX = 0, mousePosY = 0;

std::unordered_map<unsigned char, bool> keyState;

int main(int argc, char** argv)
{
	generateTerrain(19);
	
	camPos.x = spawnX;
	camPos.y = spawnY;

	keyState['w'] = false;
	keyState['a'] = false;
	keyState['s'] = false;
	keyState['d'] = false;
	keyState['e'] = false;
	keyState['<'] = false; // Left arrow
	keyState['>'] = false; // Right arrow
	keyState['^'] = false; // Up arrow
	keyState['.'] = false; // Down arrow

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowPosition(10, 10);
	glutInitWindowSize(W, H);
	glutCreateWindow("Window 1");

//	glutSetCursor(GLUT_CURSOR_NONE);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	initMatrix();

	glutDisplayFunc(display);
	//glutMouseFunc(onMouseClick);
	//glutMotionFunc(onMouseMove);
	//glutPassiveMotionFunc(onMouseMove);
	glutKeyboardFunc(onKeyboardPress);
	glutKeyboardUpFunc(onKeyboardRelease);
	glutSpecialFunc(onSpecialPress);
	glutSpecialUpFunc(onSpecialRelease);
	glutIdleFunc(display);

	glutMainLoop();
}

void initMatrix()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 2, 1024);
	glMatrixMode(GL_MODELVIEW);
}

void display()
{
	glClearColor(0.6f * (angle / (2.0f*PI)), 0.6f * (angle / (2.0f*PI)), 1.0f * (angle / (2.0f*PI)), 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	translateCamera();
	showGround();
	glPopMatrix();

	glutSwapBuffers();
}

void translateCamera()
{
	MoveCamera(keyState['w'], keyState['a'], keyState['s'], keyState['d'], false);
	RotateCamera(keyState['<'] ? -1.0f : (keyState['>'] ? 1.0f : 0.0f), keyState['^'] ? -1.0f : (keyState['.'] ? 1.0f : 0.0f));

	glRotatef(-camPos.xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(-camPos.zRot, 0.0f, 0.0f, 1.0f);
	glTranslatef(-camPos.x, -camPos.y, -camPos.z);

	//angle += 0.001f;
	if(angle>2.0f*PI)
		angle=0.0f;

	lightPosition[1] = cos(angle);
	lightPosition[2] = sin(angle);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void onMouseClick(int button, int state, int x, int y){
}

void onKeyboardPress(unsigned char key, int x, int y) {
	keyState[key] = true;
}

void onKeyboardRelease(unsigned char key, int x, int y) {
	keyState[key] = false;
}

void onSpecialPress(int key, int, int) {
	switch(key) {
		case GLUT_KEY_UP:
			keyState['^'] = true;
			break;
		case GLUT_KEY_DOWN:
			keyState['.'] = true;
			break;
		case GLUT_KEY_LEFT:
			keyState['<'] = true;
			break;
		case GLUT_KEY_RIGHT:
			keyState['>'] = true;
			break;
	}

}
void onSpecialRelease(int key, int, int) {
	switch(key) {
		case GLUT_KEY_UP:
			keyState['^'] = false;
			break;
		case GLUT_KEY_DOWN:
			keyState['.'] = false;
			break;
		case GLUT_KEY_LEFT:
			keyState['<'] = false;
			break;
		case GLUT_KEY_RIGHT:
			keyState['>'] = false;
			break;
	}
}

void onMouseMove(int x, int y)
{
	if(x == W/2 && y == H/2)
		return;
	float diffX = (float)(x-mousePosX)/(float)(W);
	float diffY = (float)(mousePosY-y)/(float)(H);

	if(diffX != 0 || diffY != 0){
	std::cout << x << ' ' << y << '\n';

	RotateCamera(diffX, diffY);}
	
	mousePosX = x;
	mousePosY = y;
	

}
