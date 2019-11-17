#pragma once
#ifndef PAINTER_H
#define PAINTER_H
class Painter {
private:
	void drawPixel(float, float);
	void midPointLineSlopeLessThanOne(float, float, float, float, bool);
	void midPointLineSlopeGreaterThanOne(float, float, float, float, bool);
public:
	void drawDot(float, float);
	void drawLine(float, float, float, float);
};

#endif // !PAINTER_H
