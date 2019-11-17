// pch.cpp: source file corresponding to pre-compiled header; necessary for compilation to succeed

#include "Painter.h"
#include <iostream>
#include <glut.h>
#include <string>

void Painter::drawPixel(float x, float y) {
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
}

void Painter::drawDot(float x, float y)
{
	drawPixel(x, y);
	glFinish();
}

void Painter::drawLine(float x_start, float y_start, float x_end, float y_end)
{
	bool x_flag = x_end > x_start;
	bool y_flag = y_end > y_start;
	bool greaterThanOneSlope = (y_end - y_start) / (x_end - x_start) > 1;
	bool negSlope = false;

	if (((y_end - y_start) / (x_end - x_start)) >= 1 || ((y_end - y_start) / (x_end - x_start)) <= -1) {
		// for each slope m, |m| >= 1
		if (x_flag && y_flag) {
			midPointLineSlopeGreaterThanOne(x_start, y_start, x_end, y_end, negSlope);
		}
		else if (x_flag && y_flag == false) {
			negSlope = true;
			midPointLineSlopeGreaterThanOne(x_start, y_start, x_end, y_end, negSlope);
		}
		else if (x_flag == false && y_flag) {
			negSlope = true;
			midPointLineSlopeGreaterThanOne(x_end, y_end, x_start, y_start, negSlope);
		}
		else if (x_flag == false && y_flag == false) {
			midPointLineSlopeGreaterThanOne(x_end, y_end, x_start, y_start, negSlope);
		}
	}
	else {
		// for each slope m, 0 < |m| < 1
		if (x_flag && y_flag) {
			// toward NorthEast
			midPointLineSlopeLessThanOne(x_start, y_start, x_end, y_end, negSlope);
		}
		else if (x_flag && y_flag == false) {
			// toward SouthEast
			negSlope = true;
			midPointLineSlopeLessThanOne(x_start, y_start, x_end, y_end, negSlope);
		}
		else if (x_flag == false && y_flag) {
			// toward NorthWest
			negSlope = true;
			midPointLineSlopeLessThanOne(x_end, y_end, x_start, y_start, negSlope);
		}
		else if (x_flag == false && y_flag == false) {
			// toward SouthWest
			midPointLineSlopeLessThanOne(x_end, y_end, x_start, y_start, negSlope);
		}
	}
	return;
}

void Painter::midPointLineSlopeLessThanOne(float x_start, float y_start, float x_end, float y_end, bool negSlope)
{
	drawPixel(x_start, y_start);
	if (negSlope) {
		float a = y_end - y_start;
		float b = -(x_end - x_start);
		float dInt = a - b / 2;
		float increE = a;
		float increSE = a - b;
		float x = x_start, y = y_start, d = dInt;

		while (x < x_end) {
			if (d < 0) {
				x = x + 1;
				y = y - 1;
				d += increSE;
			}
			else {
				x = x + 1;
				d += increE;

			}
			drawPixel(x, y);
		}
	}
	else {
		float a = y_end - y_start;
		float b = -(x_end - x_start);
		float dInt = a + b / 2;
		float increE = a;
		float increNE = a + b;
		float x = x_start, y = y_start, d = dInt;

		while (x < x_end) {
			if (d < 0) {
				x = x + 1;
				d += increE;
			}
			else {
				x = x + 1;
				y = y + 1;
				d += increNE;
			}
			drawPixel(x, y);
		}
	}
	glFinish();
	return;
}

void Painter::midPointLineSlopeGreaterThanOne(float x_start, float y_start, float x_end, float y_end, bool negSlope)
{
	drawPixel(x_start, y_start);
	if (negSlope) {
		float a = y_end - y_start;
		float b = -(x_end - x_start);
		float dInt = -a / 2 + b;
		float increE = b;
		float increSE = -a + b;
		float x = x_start, y = y_start, d = dInt;

		/*float dy = y_end - y_start;
		float dx = x_end - x_start;
		float dInt = - dy / 2 - dx;
		float increE = -dx;
		float increSE = -dx - dy;
		float x = x_start, y = y_start, d = dInt;*/

		while (y > y_end) {
			if (d < 0) {
				x = x + 1;
				y = y - 1;
				d += increSE;
			}
			else {
				y = y - 1;
				d += increE;
			}
			drawPixel(x, y);
		}
	}
	else {
		float a = y_end - y_start;
		float b = -(x_end - x_start);
		float dInt = -b - a / 2;
		float increE = -b;
		float increNE = -a - b;
		float x = x_start, y = y_start, d = dInt;

		/*float dy = y_end - y_start;
		float dx = x_end - x_start;
		float dInt = dx - dy / 2;
		float increE = dx;
		float increNE = dx - dy;
		float x = x_start, y = y_start, d = dInt;*/

		while (y < y_end) {
			if (d < 0) {
				y = y + 1;
				d += increE;
			}
			else {
				x = x + 1;
				y = y + 1;
				d += increNE;
			}
			drawPixel(x, y);
		}
	}
	glFinish();
	return;
}