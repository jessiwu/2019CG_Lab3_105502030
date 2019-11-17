// Lab2_105502030.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "Painter.h"
#include <glut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

using namespace std;

// function prototypes
void readInputFile();
void readInputCommand(string);

void init();
void startDrawing();

int WINDOW_WIDTH;
int WINDOW_HEIGHT;
string inputFileName;

void readInputFile()
{
	fstream inputFile;
	inputFile.open(inputFileName, ios::in);

	string line;
	stringstream ss;

	if (inputFile.is_open()) {
		while (getline(inputFile, line)) {
			cout << line << endl;
			readInputCommand(line);
		}
	}
	else {
		cout << "Unable to open input file!" << endl;
	}
	return;
}

void readInputCommand(string command)
{
	stringstream ss(command);
	string command_type;
	while (ss >> command_type) {
		//cout << "the first word is: " << command_type << endl;
		if (command_type == "#" || command_type == "") {
			break;
		}
		else if (command_type == "reset") {
			// reset the TM
		}
		else if (command_type == "scale") {
			
		}
		else if (command_type == "rotate") {

		}
		else if (command_type == "translate") {

		}
		else if (command_type == "object") {

		}
		else if (command_type == "observer") {

		}
		else if (command_type == "viewport") {
			
		}
		else if (command_type == "display") {
			//clear the window
			glClear(GL_COLOR_BUFFER_BIT);
			glutPostRedisplay(); //request display() call ASAP
			
			//draw on the screen
		}
		else if (command_type == "end") {
			glutDestroyWindow(glutGetWindow());
		}
	}
	return;
}

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);	// use black as background color
	glColor3f(1.0f, 1.0f, 0.0f);		// use yellow as drawing color
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
}

void startDrawing()
{
	init();		// initiate display window
	return;
}

int main(int argc, char* argv[])
{
	system("pause");

	if (argc == 2) {
		inputFileName = argv[argc - 1];
	}
	/*else {
		cout << argc << endl;
		cout << argv[0] << endl;
	}*/

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	glutCreateWindow("Display Window");
	glutIdleFunc(readInputFile);
	glutDisplayFunc(startDrawing);
	glutMainLoop();
	return 0;
}