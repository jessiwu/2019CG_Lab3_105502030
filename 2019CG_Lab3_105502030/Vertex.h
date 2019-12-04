#pragma once
#ifndef VERTEX_H
#define VERTEX_H
#include "Matrix4by4.h"
#include <tuple>
using namespace std;
class Vertex {
	private:
		tuple<float, float, float> XYZcoordinates;
public:
	void setCoordinates(float x, float y, float z);
	tuple<float, float, float> getCoordinates();
	void leftMultiplyByOneMatrix4by4(Matrix4by4 multiplier);
	void printXYZCoordinates();
};
#endif // !VERTEX_H
