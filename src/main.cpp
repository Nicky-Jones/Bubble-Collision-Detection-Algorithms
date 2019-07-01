#include <gl/glut.h>
#include "app.h"
#include <iostream>

using namespace std;

extern Application* getApplication();
Application* app;


void display(void)
	{
	app->display();
	}


void createWindow(const char* title, int h, int w)
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(w, h);
    glutCreateWindow(title);
	
}

void TimerFunc(int value)
    {
    app->update();
	float  timeinterval = app->getTimeinterval();
	glutTimerFunc(timeinterval, TimerFunc, 1);
    }

void keyboard(unsigned char key, int x, int y)
{
	app->keyboard(key, x, y);
}

void resize(int width, int height)
{
    app->resize(width, height);
}

int main(int argc, char* argv[])
    {
    glutInit(&argc, argv);
    app = getApplication();
	float  timeinterval = 10;
	app->setTimeinterval(timeinterval);
	createWindow("Blob", app->getheight(), app->getwidth());
	glutReshapeFunc(resize);
	glutDisplayFunc(display); 
	glutTimerFunc(timeinterval, TimerFunc, 1);
	app->initGraphics();
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	delete app; 
	return 0;
    }
