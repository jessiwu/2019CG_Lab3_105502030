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
		vector<Vertex> getVerticesOfPlane();
};

#endif // !PLANE_H
