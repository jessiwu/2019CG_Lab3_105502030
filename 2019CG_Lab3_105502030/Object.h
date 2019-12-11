#pragma once
#ifndef OBJECT_H
#define OBJECT_H
#include "Vertex.h"
#include <vector>
using namespace std;

class Object
{
private:
	vector<Vertex> points_vec;				// record the coordinates of all the points
	vector< vector<int> > polygons_vec;		// record the access order of the point of each ploygon
public:
	/* Manipulate Function: para1: vector<Vertex> points_vec */
	vector<Vertex> getVectorOfPoints();
	void setVectorOfPoints(vector<Vertex> vertex_vec);

	/* Manipulate Function: para2: vector< vector<int> > polygons_vec */
	vector< vector<int> > getVectorOfPolygons();
	void setVectorOfPolygons(vector< vector<int> > polygon_vec);
	
};
#endif // !OBJECT_H
