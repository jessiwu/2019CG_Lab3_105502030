#include "Vertex.h"
#include <iostream>
#include <iomanip>

void Vertex::setVertexCoordinates(float x, float y, float z, float w)
{
	get<0>(this->VertexCoordinates) = x;
	get<1>(this->VertexCoordinates) = y;
	get<2>(this->VertexCoordinates) = z;
	get<3>(this->VertexCoordinates) = w;
	return;
}

tuple<float, float, float, float> Vertex::getVertexCoordinates()
{
	return this->VertexCoordinates;
}

void Vertex::leftMultiplyByOneMatrix4by4(Matrix4by4 multiplier)
{
	float resultMatrix[4][1] = { {0}, {0}, {0}, {0} };
	float xyzwCoordinatesMatrix[4][1] = { {get<0>(this->VertexCoordinates)}, 
										{get<1>(this->VertexCoordinates)}, 
										{get<2>(this->VertexCoordinates)},
										1 };

	/* A vertex left-multiplied by a 4x4 matrix */
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 1; j++) {
			for (int k = 0; k < 4; k++) {
				resultMatrix[i][j] = resultMatrix[i][j] + multiplier.getEntryIJOfMatrix(i, k) * xyzwCoordinatesMatrix[k][j];
			}
		}
	}

	this->setVertexCoordinates(resultMatrix[0][0], resultMatrix[1][0], resultMatrix[2][0], resultMatrix[3][0]);

	return;
}

void Vertex::printVertexCoordinates()
{
	
	std::cout << setw(3) << get<0>(this->VertexCoordinates) << setw(3) << " ";
	std::cout << setw(3) << get<1>(this->VertexCoordinates) << setw(3) << " "; 
	std::cout << setw(3) << get<2>(this->VertexCoordinates) << setw(3) << " ";
	std::cout << setw(3) << get<3>(this->VertexCoordinates) << setw(3);
	std::cout << std::endl;
	
	return;
}
