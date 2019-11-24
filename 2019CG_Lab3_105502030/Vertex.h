#pragma once
#ifndef VERTEX_H
#define VERTEX_H
#include <tuple>
using namespace std;
class Vertex {
	private:
		tuple<float, float, float> XYZcoordinates;
public:
	void setCoordinates(float x, float y, float z);
	tuple<float, float, float> getCoordinates();
};
#endif // !VERTEX_H
