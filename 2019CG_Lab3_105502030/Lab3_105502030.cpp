// Lab2_105502030.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "Matrix4by4.h"
#include "Painter.h"
#include "Vertex.h"
#include "Plane.h"
#include <glut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <tuple>
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
void observerCommand(float eyeLocX, float eyeLocY, float eyeLocZ, 
	float CoIX, float CoIY, float CoIZ, float tilt, 
	float H, float y, float theta);

void objectCommand();


int WINDOW_WIDTH;
int WINDOW_HEIGHT;
string inputFileName;
string obj_ascFileName;
Matrix4by4 TM;
vector<Vertex> vertices_vec;
vector<Plane> object_vec;

void readInputFile()
{
	fstream inputFile;
	inputFile.open(inputFileName, ios::in);

	string line;

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
			// No back faces
		}
		else if (command_type == "object") {
			// read object.asc file
			ss >> obj_ascFileName;
			cout << "\nobject file name is: " << obj_ascFileName << endl;
			objectCommand();
		}
		else if (command_type == "observer") {
			float Ex, Ey, Ez, COIx, COIy, COIz, Tilt, H, y, tan;
			ss >> Ex >> Ey >> Ez >> COIx >> COIy >> COIz >> Tilt >> H >> y >> tan;
			observerCommand(Ex, Ey, Ez, COIx, COIy, COIz, Tilt, H, y, tan);
			
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

void observerCommand(float eyeLocX, float eyeLocY, float eyeLocZ, float CoIX, float CoIY, float CoIZ, float tilt, float H, float y, float theta)
{
	// VectorZ is Vector3
	tuple<float, float, float> VectorZ ( CoIX - eyeLocX, CoIY - eyeLocY, CoIZ - eyeLocZ );

	// assume top point is ( 0, 1, 0 ), therefore, VectorT(top vector) is: Top Point - EyeLocation
	tuple<float, float, float> VectorT ( 0 - eyeLocX, 1 - eyeLocY, 1 - eyeLocZ );

	// Vector1 = VectorT x VectorZ 
	tuple<float, float, float> Vector1 ( 
		get<1>(VectorT)*get<2>(VectorZ) - get<2>(VectorT)*get<1>(VectorZ),
		get<2>(VectorT)*get<0>(VectorZ) - get<0>(VectorT)*get<2>(VectorZ), 
		get<0>(VectorT)*get<1>(VectorZ) - get<1>(VectorT)*get<0>(VectorZ)
	);

	// Vector2 = Vector3 (VectorZ) x Vector1
	tuple<float, float, float> Vector2(
		get<1>(VectorZ)*get<2>(Vector1) - get<2>(VectorZ)*get<1>(Vector1),
		get<2>(VectorZ)*get<0>(Vector1) - get<0>(VectorZ)*get<2>(Vector1),
		get<0>(VectorZ)*get<1>(Vector1) - get<1>(VectorZ)*get<0>(Vector1)
	);
	Matrix4by4 eyeMatrix;

	Matrix4by4 eyeTranslateMatrix;
	eyeTranslateMatrix.loadTranslationMatrix(-eyeLocX, -eyeLocY, -eyeLocZ);

	Matrix4by4 GRMatrix;
	GRMatrix.loadGRMatrix(Vector1, Vector2, VectorZ);

	Matrix4by4 MirrorMatrix;
	MirrorMatrix.loadMirrorMatrix();

	eyeMatrix.leftMultiplyBy(eyeTranslateMatrix);
	eyeMatrix.leftMultiplyBy(GRMatrix);
	eyeMatrix.leftMultiplyBy(MirrorMatrix);

	eyeMatrix.printMatrix();

	return;

}

void objectCommand()
{

	// clear data before reading object .asc file 
	vertices_vec.clear();
	object_vec.clear();

	fstream objectFile;
	objectFile.open(obj_ascFileName, ios::in);

	int vertex_num, plane_num;
	string line;
	stringstream ss;

	getline(objectFile, line);		// read the first line of the input file
	ss << line;
	ss >> vertex_num >> plane_num;

	cout << "vertex_num: " << vertex_num << endl;
	cout << "plane_num: " << plane_num << endl;

	ss.clear();

	// 1. read all the vertices
	float x, y, z;

	for (int i = 0; i < vertex_num; i++) {
		getline(objectFile, line);
		ss << line;
		ss >> x >> y >> z;
		Vertex tmp;
		tmp.setCoordinates(x, y, z);
		vertices_vec.push_back(tmp);

		cout << "i: " << i << "  " << setw(2) << x << setw(2) << " " << y << setw(2) << " " << z << endl;
	}

	// 2. read all the planes
	int num_SidesOfPolygon, index;
	for (int i = 0; i < plane_num; i++) {
		getline(objectFile, line);
		ss << line;
		ss >> num_SidesOfPolygon;
		Plane tmp_polygon;
		for (int j = 0; j < num_SidesOfPolygon; j++) {
			ss >> index;
			tmp_polygon.getVerticesOfPlane().push_back(vertices_vec[index-1]);
		}
		object_vec.push_back(tmp_polygon);
	}

	// print for tesing
	cout << "vertices_vec size: " << vertices_vec.size() << endl;
	cout << "object_vec size: " << object_vec.size() << endl;


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