#include "Matrix4by4.h"
#include "math.h"
#include <iostream>
#include <iomanip>
#include <tuple>

using namespace std;
Matrix4by4::Matrix4by4()
{
	loadIdentityMatrix();
}

void Matrix4by4::loadIdentityMatrix()
{
	for (int i = 0; i < 4; i++) {      
		for (int j = 0; j < 4; j++) { 
			(i==j) ? this->matrix[i][j] = 1 : this->matrix[i][j] = 0;
		}
	}
	
	return;
}

void Matrix4by4::loadScalingMatrix(float sx, float sy, float sz)
{
	this->matrix[0][0] = sx;
	this->matrix[1][1] = sy;
	this->matrix[2][2] = sz;

	return;
}

void Matrix4by4::loadTranslationMatrix(float tx, float ty, float tz)
{
	this->matrix[0][3] = tx;
	this->matrix[1][3] = ty;
	this->matrix[2][3] = tz;
	
	return;
}

void Matrix4by4::loadRotationMatrix(int rotate_axis, int degree)
{
	const double PI = 3.14159;
	float sineOfDegree = round(sin(degree * PI / 180) * 1000) / 1000;
	float cosineOfDegree = round(cos(degree * PI / 180) * 1000) / 1000;

	switch (rotate_axis)
	{
	case 1:
		this->matrix[1][1] = cosineOfDegree;
		this->matrix[1][2] = -sineOfDegree;
		this->matrix[2][1] = sineOfDegree;
		this->matrix[2][2] = cosineOfDegree;
		break;
	case 2:
		this->matrix[0][0] = cosineOfDegree;
		this->matrix[0][2] = sineOfDegree;
		this->matrix[2][0] = -sineOfDegree;
		this->matrix[2][2] = cosineOfDegree;
		break;
	case 3:
		this->matrix[0][0] = cosineOfDegree;
		this->matrix[0][1] = -sineOfDegree;
		this->matrix[1][0] = sineOfDegree;
		this->matrix[1][1] = cosineOfDegree;
		break;
	default:
		cout << "load rotation matrix error!" << endl;
		break;
	}
	return;
}

void Matrix4by4::loadGRMatrix( tuple<float, float, float> v1, tuple<float, float, float> v2, tuple<float, float, float> v3)
{
	this->matrix[0][0] = get<0>(v1);
	this->matrix[0][1] = get<1>(v1);
	this->matrix[0][2] = get<2>(v1);

	this->matrix[1][0] = get<0>(v2);
	this->matrix[1][1] = get<1>(v2);
	this->matrix[1][2] = get<2>(v2);

	this->matrix[2][0] = get<0>(v3);
	this->matrix[2][1] = get<1>(v3);
	this->matrix[2][2] = get<2>(v3);
	
	return;
}

void Matrix4by4::loadMirrorMatrix()
{
	this->matrix[0][0] = -1;
	return;
}

void Matrix4by4::loadTiltMatrix(float degree)
{
	const double PI = 3.14159;
	float sineOfDegree = round(sin(degree * PI / 180) * 1000) / 1000;
	float cosineOfDegree = round(cos(degree * PI / 180) * 1000) / 1000;

	this->matrix[0][0] = cosineOfDegree;
	this->matrix[0][1] = sineOfDegree;
	this->matrix[1][0] = -sineOfDegree;
	this->matrix[1][1] = cosineOfDegree;
	
	return;
}

void Matrix4by4::loadPerspectiveProjectionMatrix(float aspectRatio, float H, float y, float theta)
{
	const double PI = 3.14159;
	float tangentOfTheta = round(tan(theta * PI / 180) * 1000) / 1000;

	this->matrix[1][1] = aspectRatio;
	this->matrix[2][2] = y * tangentOfTheta / (y - H);
	this->matrix[2][3] = H * y * tangentOfTheta / (H - y);
	this->matrix[3][2] = tangentOfTheta;
	this->matrix[3][3] = 0;

	return;
}

float Matrix4by4::getEntryIJOfMatrix(int i, int j)
{
	return this->matrix[i][j];
}

void Matrix4by4::leftMultiplyBy(Matrix4by4 multiplier)
{
	float resultMatrix[4][4] = { {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0} };

	/* the two 4x4 matrices mutiplication */
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				resultMatrix[i][j] = resultMatrix[i][j] + multiplier.matrix[i][k] * this->matrix[k][j];
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->matrix[i][j] = resultMatrix[i][j];
		}
	}
	return;
}

void Matrix4by4::printMatrix()
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << setw(6) << this->matrix[i][j] << setw(6) << " ";
		}
		cout << endl;
	}
	return;
}

