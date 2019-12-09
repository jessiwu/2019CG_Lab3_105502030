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
void viewportCommand(float vp_xLeft, float vp_xRight, float vp_yBottom, float vp_yTop);
void displayCommand();
void objectCommand();


float WINDOW_WIDTH;
float WINDOW_HEIGHT;
float H_ProjMatrix, y_ProjMatrix, theta_ProjMatrix;
float viewport_XMin, viewport_XMax, viewport_YMin, viewport_YMax;
string inputFileName;
string obj_ascFileName;
Matrix4by4 TM;
Matrix4by4 EyeMatrix;
Matrix4by4 PerspectiveProjMatrix;
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

			cout << "after translating, TM is: \n";
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
			float vp_xLeft, vp_xRight, vp_yBottom, vp_yTop;
			ss >> vp_xLeft >> vp_xRight >> vp_yBottom >> vp_yTop;
			viewportCommand(vp_xLeft, vp_xRight, vp_yBottom, vp_yTop);

		}
		else if (command_type == "display") {
			//clear the window
			//glClear(GL_COLOR_BUFFER_BIT);
			//glutPostRedisplay(); //request display() call ASAP
			
			//draw on the screen
			// WARNING: implement displayCommand();
			displayCommand();
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
	tuple<float, float, float> VectorZ ( (CoIX - eyeLocX), (CoIY - eyeLocY), (CoIZ - eyeLocZ) );

	// Normalize on VectorZ 
	float normalize_val = sqrt(get<0>(VectorZ)*get<0>(VectorZ) + get<1>(VectorZ)*get<1>(VectorZ) + get<2>(VectorZ)*get<2>(VectorZ));

	get<0>(VectorZ) = get<0>(VectorZ) / normalize_val;
	get<1>(VectorZ) = get<1>(VectorZ) / normalize_val;
	get<2>(VectorZ) = get<2>(VectorZ) / normalize_val;

	// assume top point is ( 0, 1, 0 ), therefore, VectorT(top vector) is: Top Point - EyeLocation
	//tuple<float, float, float> VectorT ( 0 - eyeLocX, 1 - eyeLocY, 1 - eyeLocZ );
	tuple<float, float, float> VectorT ( 0 , 1 , 0 );

	// Vector1 = VectorT x VectorZ 
	tuple<float, float, float> Vector1 ( 
		get<1>(VectorT)*get<2>(VectorZ) - get<2>(VectorT)*get<1>(VectorZ),
		get<2>(VectorT)*get<0>(VectorZ) - get<0>(VectorT)*get<2>(VectorZ), 
		get<0>(VectorT)*get<1>(VectorZ) - get<1>(VectorT)*get<0>(VectorZ)
	);

	// Normalize on Vector1 
	normalize_val = sqrt(get<0>(Vector1)*get<0>(Vector1) + get<1>(Vector1)*get<1>(Vector1) + get<2>(Vector1)*get<2>(Vector1));

	get<0>(Vector1) = get<0>(Vector1) / normalize_val;
	get<1>(Vector1) = get<1>(Vector1) / normalize_val;
	get<2>(Vector1) = get<2>(Vector1) / normalize_val;

	// Vector2 = Vector3 (VectorZ) x Vector1
	tuple<float, float, float> Vector2(
		get<1>(VectorZ)*get<2>(Vector1) - get<2>(VectorZ)*get<1>(Vector1),
		get<2>(VectorZ)*get<0>(Vector1) - get<0>(VectorZ)*get<2>(Vector1),
		get<0>(VectorZ)*get<1>(Vector1) - get<1>(VectorZ)*get<0>(Vector1)
	);

	// Normalize on Vector2 
	normalize_val = sqrt(get<0>(Vector2)*get<0>(Vector2) + get<1>(Vector2)*get<1>(Vector2) + get<2>(Vector2)*get<2>(Vector2));

	get<0>(Vector2) = get<0>(Vector2) / normalize_val;
	get<1>(Vector2) = get<1>(Vector2) / normalize_val;
	get<2>(Vector2) = get<2>(Vector2) / normalize_val;
	

	Matrix4by4 eyeTranslateMatrix;
	eyeTranslateMatrix.loadTranslationMatrix(-eyeLocX, -eyeLocY, -eyeLocZ);

	Matrix4by4 GRMatrix;
	GRMatrix.loadGRMatrix(Vector1, Vector2, VectorZ);

	Matrix4by4 MirrorMatrix;
	MirrorMatrix.loadMirrorMatrix();

	Matrix4by4 TiltMatrix;
	TiltMatrix.loadTiltMatrix(tilt);

	EyeMatrix.leftMultiplyBy(eyeTranslateMatrix);
	EyeMatrix.leftMultiplyBy(GRMatrix);
	EyeMatrix.leftMultiplyBy(MirrorMatrix);
	EyeMatrix.leftMultiplyBy(TiltMatrix);

	// wait viewport command
	// save the coefficients of Perspective PM in global variables
	H_ProjMatrix = H;
	y_ProjMatrix = y;
	theta_ProjMatrix = theta;

	return;

}

void viewportCommand(float vp_xLeft, float vp_xRight, float vp_yBottom, float vp_yTop)
{
	viewport_XMin = WINDOW_WIDTH / 2 + WINDOW_WIDTH / 2 * vp_xLeft;
	viewport_XMax = WINDOW_WIDTH / 2 + WINDOW_WIDTH / 2 * vp_xRight; 
	viewport_YMin = WINDOW_HEIGHT / 2 + WINDOW_HEIGHT / 2 * vp_yBottom;
	viewport_YMax = WINDOW_HEIGHT / 2 + WINDOW_HEIGHT / 2 * vp_yTop;
	//float aspectRatio = (viewport_XMax - viewport_XMin) / (viewport_YMax - viewport_YMin);
	float aspectRatio = (vp_xRight - vp_xLeft) / (vp_yTop - vp_yBottom); //edit

	// create Perspective Projection Matrix
	PerspectiveProjMatrix.loadPerspectiveProjectionMatrix(
		aspectRatio, 
		H_ProjMatrix, y_ProjMatrix, theta_ProjMatrix
	);

	//cout << "Perspective Projectio Matrix is: " << endl;
	//PerspectiveProjMatrix.printMatrix();

	return;
}

void displayCommand()
{
	vector<Vertex> ith_plane_vec;
	const double PI = 3.14159;
	float tangentOfTheta = round(tan(theta_ProjMatrix * PI / 180) * 1000) / 1000;
	float perspectiveDivide_val;
	float sx, sy;
	float new_x, new_y;

	cout << "TM" << endl;
	TM.printMatrix();

	cout << "EM" << endl;
	EyeMatrix.printMatrix();

	cout << "PM" << endl;
	PerspectiveProjMatrix.printMatrix();

	cout << "object_vec size is: " << object_vec.size() << endl;
	
	for (int i = 0; i < object_vec.size(); i++) {
		//cout << "The " << i << "th plane is:" << endl;

		ith_plane_vec = object_vec[i].getVerticesOfPlane();

		for (int j = 0; j < ith_plane_vec.size(); j++) {

			//Step 1. do Matrices Multiplication: multiply TM, EM, PM
			ith_plane_vec[j].leftMultiplyByOneMatrix4by4(TM);
			ith_plane_vec[j].leftMultiplyByOneMatrix4by4(EyeMatrix);
			ith_plane_vec[j].leftMultiplyByOneMatrix4by4(PerspectiveProjMatrix);
			
			//Step 2. do Perspective Divide 
			perspectiveDivide_val = get<2>(ith_plane_vec[j].getCoordinates())*tangentOfTheta;
			ith_plane_vec[j].setCoordinates(
				get<0>(ith_plane_vec[j].getCoordinates()) / perspectiveDivide_val, 
				get<1>(ith_plane_vec[j].getCoordinates()) / perspectiveDivide_val,
				get<2>(ith_plane_vec[j].getCoordinates()) / perspectiveDivide_val);

			//Step 3. Window-to-Viewport transformation
			sx = (viewport_XMax - viewport_XMin) / 2;
			sy = (viewport_YMax - viewport_YMin) / 2;
			//new_x = get<0>(ith_plane_vec[j].getCoordinates())*sx + viewport_XMin;
			//new_y = get<1>(ith_plane_vec[j].getCoordinates())*sy + viewport_YMin;
			new_x = ( get<0>(ith_plane_vec[j].getCoordinates()) - (-1) )*sx + viewport_XMin;
			new_y = ( get<1>(ith_plane_vec[j].getCoordinates()) - (-1) )*sy + viewport_YMin;

			ith_plane_vec[j].setCoordinates(
				new_x,
				new_y,
				get<2>(ith_plane_vec[j].getCoordinates())
			);

		}

		//Step 4. draw on the screen
		Painter magicPen;
		for (int j = 0; j < ith_plane_vec.size(); j++) {

			if (j == ith_plane_vec.size() - 1) {
				/*cout << "j is: " << j << endl;
				cout << "    the 1th point is: " << get<0>(ith_plane_vec[j].getCoordinates()) << ", ";
				cout << get<1>(ith_plane_vec[j].getCoordinates()) << endl;
				cout << "    the 2rd point is: " << get<0>(ith_plane_vec[0].getCoordinates()) << ", ";
				cout << get<1>(ith_plane_vec[0].getCoordinates()) << endl;*/
				magicPen.drawLine(
					(int)get<0>(ith_plane_vec[j].getCoordinates()),
					(int)get<1>(ith_plane_vec[j].getCoordinates()),
					(int)get<0>(ith_plane_vec[0].getCoordinates()),
					(int)get<1>(ith_plane_vec[0].getCoordinates())
				);
			}
			else {
				/*cout << "j is: " << j << endl;
				cout << "    the 1th point is: " << get<0>(ith_plane_vec[j].getCoordinates()) << ", ";
				cout << get<1>(ith_plane_vec[j].getCoordinates()) << endl;
				cout << "    the 2rd point is: " << get<0>(ith_plane_vec[j + 1].getCoordinates()) << ", ";
				cout << get<1>(ith_plane_vec[j + 1].getCoordinates()) << endl;*/
				magicPen.drawLine(
					(int)get<0>(ith_plane_vec[j].getCoordinates()),
					(int)get<1>(ith_plane_vec[j].getCoordinates()),
					(int)get<0>(ith_plane_vec[j + 1].getCoordinates()),
					(int)get<1>(ith_plane_vec[j + 1].getCoordinates())
				);
			}
		}
	}

	// Reset the matrices
	
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

	if (line.empty()) { // for bench.asc file 
		// the first line is empty...
		getline(objectFile, line);
	}
	ss << line;
	ss >> vertex_num >> plane_num;

	ss.clear();

	// 1. read all the vertices
	float x, y, z;
	
	for (int i = 0; i < vertex_num; i++) {
		getline(objectFile, line);
		ss << line;
		stringstream tmp_ss(line);
		tmp_ss >> x >> y >> z;
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
		stringstream tmp_ss(line);
		tmp_ss >> num_SidesOfPolygon;
		
		Plane tmp_polygon;
		vector<Vertex> tmp_plane; 
		
		for (int j = 0; j < num_SidesOfPolygon; j++) {
			tmp_ss >> index;
			tmp_plane.push_back(vertices_vec[index - 1]); 
			tmp_polygon.setVerticesOfPlane(tmp_plane); 
		}
		object_vec.push_back(tmp_polygon);
	}

	// print for tesing
	/*cout << "vertices_vec size: " << vertices_vec.size() << endl;
	cout << "object_vec size: " << object_vec.size() << endl;

	for (int i = 0; i < object_vec.size(); i++) {
		cout << "The " << i << "th plane is: " << endl;
		for (int j = 0; j < object_vec[i].getVerticesOfPlane().size(); j++) {
			object_vec[i].getVerticesOfPlane()[j].printXYZCoordinates();
		}
	}*/

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
	
	glutInitWindowPosition(710, 120);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Display Window");
	glutIdleFunc(readInputFile);
	glutDisplayFunc(startDrawing);
	glutMainLoop();
	return 0;
}