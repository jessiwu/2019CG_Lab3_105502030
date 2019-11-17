// Lab2_105502030.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "Painter.h"
#include "Matrix4by4.h"
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

void scaleCommand(float sx, float sy, float sz);
void rotateCommand(int rotate_axis, int degree);
void translateCommand(float tx, float ty, float tz);


int WINDOW_WIDTH;
int WINDOW_HEIGHT;
string inputFileName;
Matrix4by4 TM;

void readInputFile()
{
	fstream inputFile;
	inputFile.open(inputFileName, ios::in);

	string line;
	stringstream ss;

	cout << "inputFileName is: " << inputFileName << endl;

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
			TM.loadIdentityMatrix();
			TM.printMatrix();
		}
		else if (command_type == "scale") {
			float sx, sy, sz;
			ss >> sx >> sy >> sz;
			scaleCommand(sx, sy, sz);

			cout << "after scaling, TM is: \n";
			TM.printMatrix();
		}
		else if (command_type == "rotate") {
			int rotate_axis=0;
			int degree=0;
			while (degree == 0){
				ss >> degree;
				rotate_axis++;
			} 
			cout << "axis is: " << rotate_axis << endl;
			cout << "degree is: " << degree << endl;

			rotateCommand(rotate_axis, degree);

			cout << "after rotating, TM is: \n";
			TM.printMatrix();

		}
		else if (command_type == "translate") {
			float tx, ty, tz;
			ss >> tx >> ty >> tz;
			translateCommand(tx, ty, tz);

			cout << "after translatin, TM is: \n";
			TM.printMatrix();
		}
		else if (command_type == "nobackfaces") {

		}
		else if (command_type == "object") {

		}
		else if (command_type == "observer") {
			float Ex, Ey, Ez, COIx, COIy, COIz, Tilt, H, y, tan;
			ss >> Ex>> Ey>> Ez>> COIx>> COIy>> COIz>> Tilt>> H>> y>> tan;
			//cout << "tilt is: " << Tilt << endl;
			//cout << "H is: " << H << endl;
		}
		else if (command_type == "viewport") {

		}
		else if (command_type == "display") {
			//clear the window
			glClear(GL_COLOR_BUFFER_BIT);
			glutPostRedisplay(); //request display() call ASAP
			
			//draw on the screen
			system("pause");
		}
		else if (command_type == "end") {
			system("pause");
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

void scaleCommand(float sx, float sy, float sz)
{
	Matrix4by4 scaleMatrix;
	scaleMatrix.loadScalingMatrix(sx, sy, sz);
	TM.leftMultiplyBy(scaleMatrix);

	cout << "scaling matrix is: \n";
	scaleMatrix.printMatrix();

	return;
}

void rotateCommand(int rotate_axis, int degree)
{
	Matrix4by4 rotateMatrix;
	rotateMatrix.loadRotationMatrix(rotate_axis, degree);
	TM.leftMultiplyBy(rotateMatrix);

	cout << "rotation matrix is: \n";
	rotateMatrix.printMatrix();

	return;
}

void translateCommand(float tx, float ty, float tz)
{
	Matrix4by4 translateMatrix;
	translateMatrix.loadTranslationMatrix(tx, ty, tz);
	TM.leftMultiplyBy(translateMatrix);

	cout << "translating matrix is: \n";
	translateMatrix.printMatrix();

	return;
}

int main(int argc, char* argv[])
{
	system("pause");

	if (argc == 2) {
		inputFileName = argv[argc - 1];

		fstream inputFile;
		string line;
		inputFile.open(inputFileName, ios::in);
		getline(inputFile, line);

		// read the required size of the display window
		stringstream ss(line);
		ss >> WINDOW_WIDTH >> WINDOW_HEIGHT;
		inputFile.close();
	}
	/*else {
		cout << argc << endl;
		cout << argv[0] << endl;
	}*/

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	
	glutInitWindowPosition(750, 200);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Display Window");
	glutIdleFunc(readInputFile);
	glutDisplayFunc(startDrawing);
	glutMainLoop();
	return 0;
}