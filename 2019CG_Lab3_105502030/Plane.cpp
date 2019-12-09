#include "Plane.h"

vector<Vertex> Plane::getVerticesOfPlane()
{
	return this->plane_vec;
}

void Plane::setVerticesOfPlane(vector<Vertex> plane_vector)
{
	this->plane_vec = plane_vector;
	return;
}
