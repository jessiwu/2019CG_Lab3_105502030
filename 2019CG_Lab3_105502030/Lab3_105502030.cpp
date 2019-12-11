// Lab2_105502030.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "Matrix4by4.h"
#include "Painter.h"
#include "Vertex.h"
#include "Object.h"
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
void objectCommand();
void displayCommand();


float WINDOW_WIDTH;
float WINDOW_HEIGHT;
float H_ProjMatrix, y_ProjMatrix, theta_ProjMatrix;
float viewport_XMin, viewport_XMax, viewport_YMin, viewport_YMax;
float aspectRatio;
string inputFileName;
string obj_ascFileName;
Matrix4by4 TM;
Matrix4by4 EyeMatrix;
Matrix4by4 PerspectiveProjMatrix;
vector<Object> objects_vec;

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
			glClear(GL_COLOR_BUFFER_BIT);
			glutPostRedisplay(); //request display() call ASAP
			
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

	// Vector2 = Vector3 (VectorZ) x Vector1
	tuple<float, float, float> Vector2(
		get<1>(VectorZ)*get<2>(Vector1) - get<2>(VectorZ)*get<1>(Vector1),
		get<2>(VectorZ)*get<0>(Vector1) - get<0>(VectorZ)*get<2>(Vector1),
		get<0>(VectorZ)*get<1>(Vector1) - get<1>(VectorZ)*get<0>(Vector1)
	);

	// Normalize on Vector1 
	normalize_val = sqrt(get<0>(Vector1)*get<0>(Vector1) + get<1>(Vector1)*get<1>(Vector1) + get<2>(Vector1)*get<2>(Vector1));

	get<0>(Vector1) = get<0>(Vector1) / normalize_val;
	get<1>(Vector1) = get<1>(Vector1) / normalize_val;
	get<2>(Vector1) = get<2>(Vector1) / normalize_val;

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

	//cout << "GRMatrix" << endl;
	//GRMatrix.printMatrix();

	//cout << "MirrorMatrix" << endl;
	//MirrorMatrix.printMatrix();

	//cout << "TiltMatrix" << endl;
	//TiltMatrix.printMatrix();

	EyeMatrix.loadIdentityMatrix();
	EyeMatrix.leftMultiplyBy(eyeTranslateMatrix);
	EyeMatrix.leftMultiplyBy(GRMatrix);
	EyeMatrix.leftMultiplyBy(MirrorMatrix);
	EyeMatrix.leftMultiplyBy(TiltMatrix);

	// wait viewport command
	// save the coefficients of Perspective PM in global variables first
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
	aspectRatio = (vp_xRight - vp_xLeft) / (vp_yTop - vp_yBottom);

	return;
}

void displayCommand()
{
	const double PI = 3.14159;
	float tangentOfTheta = round(tan(theta_ProjMatrix * PI / 180) * 1000) / 1000;
	float perspectiveDivide_val;
	float sx = (viewport_XMax - viewport_XMin) / 2;
	float sy = (viewport_YMax - viewport_YMin) / 2;
	float new_x, new_y;
	Object tmp_obj;
	vector<Vertex> tmp_points_list;
	Painter magicPen;

	// create Perspective Projection Matrix
	PerspectiveProjMatrix.loadIdentityMatrix();
	PerspectiveProjMatrix.loadPerspectiveProjectionMatrix(
		aspectRatio,
		H_ProjMatrix, y_ProjMatrix, theta_ProjMatrix
	);

	
	for (int ith_obj = 0; ith_obj < objects_vec.size(); ith_obj++) {
		//cout << "The " << ith_obj << "th_obj \n";
		for (int j = 0; j < objects_vec[ith_obj].getVectorOfPoints().size(); j++) {
			Vertex tmp_v = objects_vec[ith_obj].getVectorOfPoints()[j];

			//Step 1. do Matrices Multiplication: multiply TM, EM, PM
			tmp_v.leftMultiplyByOneMatrix4by4(EyeMatrix);
			tmp_v.leftMultiplyByOneMatrix4by4(PerspectiveProjMatrix);

			//Step 2. do Perspective Divide 
			perspectiveDivide_val = get<3>(tmp_v.getVertexCoordinates());

			tmp_v.setVertexCoordinates(
				get<0>(tmp_v.getVertexCoordinates()) / perspectiveDivide_val,
				get<1>(tmp_v.getVertexCoordinates()) / perspectiveDivide_val,
				get<2>(tmp_v.getVertexCoordinates()) / perspectiveDivide_val,
				1
			);

			//Step 3. Window-to-Viewport transformation
			new_x = (get<0>(tmp_v.getVertexCoordinates()) - (-1)) * sx + viewport_XMin;
			new_y = (get<1>(tmp_v.getVertexCoordinates()) - (-1)) * sy + viewport_YMin;

			tmp_v.setVertexCoordinates(
				new_x,
				new_y,
				get<2>(tmp_v.getVertexCoordinates()),
				get<3>(tmp_v.getVertexCoordinates())
			);
			tmp_points_list.push_back(tmp_v);
		}

		for (int ith_polygon = 0; ith_polygon < objects_vec[ith_obj].getVectorOfPolygons().size(); ith_polygon++) {
			//cout << "the " << ith_polygon << "th polygon: " << endl;
			//Step 4. draw on the screen
			for (int nth_vertex = 0; nth_vertex < objects_vec[ith_obj].getVectorOfPolygons()[ith_polygon].size(); nth_vertex++) {
				if (nth_vertex == objects_vec[ith_obj].getVectorOfPolygons()[ith_polygon].size() - 1) {
					/*cout << "1th point: ";
					tmp_points_list[objects_vec[ith_obj].getVectorOfPolygons()[ith_polygon].at(nth_vertex) - 1].printVertexCoordinates();
					cout << "2rd point: ";
					tmp_points_list[objects_vec[ith_obj].getVectorOfPolygons()[ith_polygon].at(0) - 1].printVertexCoordinates();*/
					magicPen.drawLine(
						(int)get<0>(tmp_points_list[objects_vec[ith_obj].getVectorOfPolygons()[ith_polygon].at(nth_vertex) - 1].getVertexCoordinates()),
						(int)get<1>(tmp_points_list[objects_vec[ith_obj].getVectorOfPolygons()[ith_polygon].at(nth_vertex) - 1].getVertexCoordinates()),
						(int)get<0>(tmp_points_list[objects_vec[ith_obj].getVectorOfPolygons()[ith_polygon].at(0) - 1].getVertexCoordinates()),
						(int)get<1>(tmp_points_list[objects_vec[ith_obj].getVectorOfPolygons()[ith_polygon].at(0) - 1].getVertexCoordinates())
					);
				}
				else {
					/*cout << "1th point: ";
					tmp_points_list[objects_vec[ith_obj].getVectorOfPolygons()[ith_polygon].at(nth_vertex) - 1].printVertexCoordinates();
					cout << "2rd point: ";
					tmp_points_list[objects_vec[ith_obj].getVectorOfPolygons()[ith_polygon].at(nth_vertex+1) - 1].printVertexCoordinates();*/
					magicPen.drawLine(
						(int)get<0>(tmp_points_list[objects_vec[ith_obj].getVectorOfPolygons()[ith_polygon].at(nth_vertex) - 1].getVertexCoordinates()),
						(int)get<1>(tmp_points_list[objects_vec[ith_obj].getVectorOfPolygons()[ith_polygon].at(nth_vertex) - 1].getVertexCoordinates()),
						(int)get<0>(tmp_points_list[objects_vec[ith_obj].getVectorOfPolygons()[ith_polygon].at(nth_vertex+1) - 1].getVertexCoordinates()),
						(int)get<1>(tmp_points_list[objects_vec[ith_obj].getVectorOfPolygons()[ith_polygon].at(nth_vertex+1) - 1].getVertexCoordinates())
					);
				}
			}
		}
		tmp_points_list.clear();
		glFinish();
	}
	//magicPen.drawLine(400, 400, 500, 500);
	//TM.loadIdentityMatrix();
	return;
}

void objectCommand()
{
	Object tmp_obj;
	Vertex tmp_vertex;
	vector<int> tmp_access_ploygon_order;
	vector< vector<int> > tmp_polygons_vec;
	vector<Vertex> tmp_vertices_vec;

	fstream objectFile;
	objectFile.open(obj_ascFileName, ios::in);

	int vertex_num, polygon_num;
	string line;
	stringstream ss;

	getline(objectFile, line);		// read the first line of the input file

	if (line.empty()) { // for bench.asc file 
		// the first line is empty...
		getline(objectFile, line);
	}
	ss << line;
	ss >> vertex_num >> polygon_num;

	// Step 1. read all the vertices and store them in tmp_vertices_vec;
	float x, y, z;
	for (int ith_vertex = 0; ith_vertex < vertex_num; ith_vertex++) {
		getline(objectFile, line);
		ss << line;
		stringstream tmp_ss(line);
		tmp_ss >> x >> y >> z;
		tmp_vertex.setVertexCoordinates(x, y, z, 1);
		tmp_vertex.leftMultiplyByOneMatrix4by4(TM);
		tmp_vertices_vec.push_back(tmp_vertex);

		//cout << "i: " << ith_vertex << "  ";
		//tmp_vertex.printVertexCoordinates();
	}
	tmp_obj.setVectorOfPoints(tmp_vertices_vec);	// store them in tmp_vertices_vec;

	// Step 2. read the access order of each polygons of this object
	int num_SidesOfPolygon, index;
	for (int ith_polygon = 0; ith_polygon < polygon_num; ith_polygon++) {
		getline(objectFile, line);
		ss << line;
		stringstream tmp_ss(line);
		tmp_ss >> num_SidesOfPolygon;

		for (int j = 0; j < num_SidesOfPolygon; j++) {
			tmp_ss >> index;
			tmp_access_ploygon_order.push_back(index);
		}
		tmp_polygons_vec.push_back(tmp_access_ploygon_order);
		tmp_access_ploygon_order.clear();
	}
	tmp_obj.setVectorOfPolygons(tmp_polygons_vec);	// store them in tmp_polygons_vec;

	objects_vec.push_back(tmp_obj);		// store the new object in objects_vec

	//clear 
	tmp_polygons_vec.clear();
	tmp_vertices_vec.clear();

	/*for (int i = 0; i < objects_vec.size(); i++) {
		cout << "the " << i << "th object" << endl;
		cout << "the " << i << "th object's points num: " << objects_vec[i].getVectorOfPoints().size() << endl;
		cout << "the " << i << "th object's polygons num: " << objects_vec[i].getVectorOfPolygons().size() << endl;
		cout << "the " << i << "th object's second point of the first plane: ";
		objects_vec[i].getVectorOfPoints().at(objects_vec[i].getVectorOfPolygons().at(0).at(1)- 1).printVertexCoordinates();
		cout << endl;
	}*/
	return;
}

int main(int argc, char* argv[])
{
	/*vector<Object> obj_list;
	Object tmp_obj;
	Vertex tmp_vertex;
	vector<int> access_ploygon_order;
	vector< vector<int> > polygons_vec;
	vector<Vertex> v_vec;

	tmp_vertex.setVertexCoordinates(0.1, 0.6, 0.7, 1); 
	v_vec.push_back(tmp_vertex);
	tmp_vertex.setVertexCoordinates(0.2, 0.6, 0.7, 1);
	v_vec.push_back(tmp_vertex);
	tmp_vertex.setVertexCoordinates(0.3, 0.6, 0.7, 1);
	v_vec.push_back(tmp_vertex);
	tmp_vertex.setVertexCoordinates(0.4, 0.6, 0.7, 1);
	v_vec.push_back(tmp_vertex);
	tmp_vertex.setVertexCoordinates(0.5, 0.6, 0.7, 1);
	v_vec.push_back(tmp_vertex);

	access_ploygon_order.push_back(4);
	access_ploygon_order.push_back(1);
	access_ploygon_order.push_back(2);
	access_ploygon_order.push_back(3);
	access_ploygon_order.push_back(4);
	polygons_vec.push_back(access_ploygon_order);
	access_ploygon_order.clear();

	access_ploygon_order.push_back(4);
	access_ploygon_order.push_back(2);
	access_ploygon_order.push_back(1);
	access_ploygon_order.push_back(3);
	access_ploygon_order.push_back(4);
	polygons_vec.push_back(access_ploygon_order);
	access_ploygon_order.clear();

	access_ploygon_order.push_back(4);
	access_ploygon_order.push_back(5);
	access_ploygon_order.push_back(2);
	access_ploygon_order.push_back(1);
	access_ploygon_order.push_back(4);
	polygons_vec.push_back(access_ploygon_order);
	access_ploygon_order.clear();

	tmp_obj.setVectorOfPoints(v_vec);
	tmp_obj.setVectorOfPolygons(polygons_vec);
	obj_list.push_back(tmp_obj);

	
	v_vec.clear(); 
	polygons_vec.clear(); 

	tmp_vertex.setVertexCoordinates(1.1, 0.6, 0.7, 1); 
	v_vec.push_back(tmp_vertex);
	tmp_vertex.setVertexCoordinates(1.2, 0.6, 0.7, 1);
	v_vec.push_back(tmp_vertex);
	tmp_vertex.setVertexCoordinates(1.3, 0.6, 0.7, 1);
	v_vec.push_back(tmp_vertex);
	tmp_vertex.setVertexCoordinates(1.4, 0.6, 0.7, 1);
	v_vec.push_back(tmp_vertex);
	tmp_vertex.setVertexCoordinates(1.5, 0.6, 0.7, 1);
	v_vec.push_back(tmp_vertex);
	tmp_vertex.setVertexCoordinates(1.6, 0.6, 0.7, 1);
	v_vec.push_back(tmp_vertex);

	access_ploygon_order.push_back(4);
	access_ploygon_order.push_back(1);
	access_ploygon_order.push_back(2);
	access_ploygon_order.push_back(3);
	access_ploygon_order.push_back(4);
	polygons_vec.push_back(access_ploygon_order);
	access_ploygon_order.clear();

	access_ploygon_order.push_back(4);
	access_ploygon_order.push_back(2);
	access_ploygon_order.push_back(1);
	access_ploygon_order.push_back(3);
	access_ploygon_order.push_back(4);
	polygons_vec.push_back(access_ploygon_order);
	access_ploygon_order.clear();

	access_ploygon_order.push_back(4);
	access_ploygon_order.push_back(6);
	access_ploygon_order.push_back(2);
	access_ploygon_order.push_back(1);
	access_ploygon_order.push_back(4);
	polygons_vec.push_back(access_ploygon_order);
	access_ploygon_order.clear();

	tmp_obj.setVectorOfPoints(v_vec);
	tmp_obj.setVectorOfPolygons(polygons_vec);
	obj_list.push_back(tmp_obj);

	cout << "AA " << obj_list.size() << endl;
	cout << "BB ";
	obj_list[0].getVectorOfPoints().at(0).printVertexCoordinates();
	cout << "CC " << obj_list[0].getVectorOfPolygons().at(0).size();
	cout << "\nDD " << obj_list[0].getVectorOfPolygons().at(2).at(1);
	cout << endl;
	cout << "\nBB ";
	obj_list[1].getVectorOfPoints().at(1).printVertexCoordinates();
	cout << "CC " << obj_list[1].getVectorOfPolygons().at(0).size();
	cout << "\nDD " << obj_list[1].getVectorOfPolygons().at(2).at(1) << endl;

	for (int i = 0; i < obj_list.size(); i++) {
		cout << "the " << i << "th object" << endl;
		cout << "the " << i << "th object's points num: " << obj_list[i].getVectorOfPoints().size() << endl;
		cout << "the " << i << "th object's polygons num: " << obj_list[i].getVectorOfPolygons().size() << endl;
		cout << endl;
	}*/

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