#include "Object.h"

vector<Vertex> Object::getVectorOfPoints()
{
	return this->points_vec;
}

void Object::setVectorOfPoints(vector<Vertex> vertex_vec)
{
	this->points_vec = vertex_vec;
	return;
}

vector<vector<int>> Object::getVectorOfPolygons()
{
	return this->polygons_vec;
}

void Object::setVectorOfPolygons(vector<vector<int>> polygon_vec)
{
	this->polygons_vec = polygon_vec;
	return;
}
