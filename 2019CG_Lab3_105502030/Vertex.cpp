#include "Vertex.h"
#include <iostream>
#include <iomanip>

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

void Vertex::leftMultiplyByOneMatrix4by4(Matrix4by4 multiplier)
{
	float resultMatrix[4][1] = { {0}, {0}, {0}, {0} };
	float xyzCoordinatesMatrix[4][1] = { {get<0>(this->XYZcoordinates)}, 
										{get<1>(this->XYZcoordinates)}, 
										{get<2>(this->XYZcoordinates)},
										1 };

	/* A vertex lefy-multiplied by a 4x4 matrix */
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 1; j++) {
			for (int k = 0; k < 4; k++) {
				resultMatrix[i][j] = resultMatrix[i][j] + multiplier.getEntryIJOfMatrix(i, k) * xyzCoordinatesMatrix[k][j];
			}
		}
	}

	this->setCoordinates(resultMatrix[0][0], resultMatrix[1][0], resultMatrix[2][0]);

	return;
}

void Vertex::printXYZCoordinates()
{
	
	std::cout << setw(3) << get<0>(this->XYZcoordinates) << setw(3) << " ";
	std::cout << setw(3) << get<1>(this->XYZcoordinates) << setw(3) << " "; 
	std::cout << setw(3) << get<2>(this->XYZcoordinates) << setw(3);
	std::cout << std::endl;
	
	return;
}
