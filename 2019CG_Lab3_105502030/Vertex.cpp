#include "Vertex.h"

void Vertex::setCoordinates(float x, float y, float z)
{
	get<0>(this->XYZcoordinates) = x;
	get<1>(this->XYZcoordinates) = y;
	get<2>(this->XYZcoordinates) = z;
	return;
}

tuple<float, float, float> Vertex::getCoordinates()
{
	return this->XYZcoordinates;
}
