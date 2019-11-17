#pragma once
#ifndef MATRIX4BY4_H
#define MATRIX4BY4_H
class Matrix4by4 {
private:
	float matrix[4][4]; 
public:
	Matrix4by4();

	void loadIdentityMatrix();
	void loadScalingMatrix(float sx, float sy, float sz);
	void loadTranslationMatrix(float tx, float ty, float tz);
	void loadRotationMatrix(int axis, int degree);

	void leftMultiplyBy(Matrix4by4 multiplier);
	void printMatrix();
};

#endif // !MATRIX4BY4_H
