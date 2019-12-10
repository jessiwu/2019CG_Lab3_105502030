#pragma once
#ifndef VERTEX_H
#define VERTEX_H
#include "Matrix4by4.h"
#include <tuple>
using namespace std;
class Vertex {
	private:
		tuple<float, float, float, float> VertexCoordinates;
public:
	void setVertexCoordinates(float x, float y, float z, float w);
	tuple<float, float, float, float> getVertexCoordinates();
	void leftMultiplyByOneMatrix4by4(Matrix4by4 multiplier);
	void printVertexCoordinates();
};
#endif // !VERTEX_H
