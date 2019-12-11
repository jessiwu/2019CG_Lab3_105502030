#pragma once
#ifndef PLANE_H
#define PLANE_H
#include "Vertex.h"
#include <vector>
using namespace std;
class Plane
{
	private:
		vector<Vertex> plane_vec;
	public:
		vector<Vertex> getVertexVectorOfPlane();
		void setVerticesOfPlane(vector<Vertex> plane_vector);
};

#endif // !PLANE_H
