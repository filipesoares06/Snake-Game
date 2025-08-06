#pragma once
#include "ofMain.h"
#include "cgExtras.h"
#include "cgDrawingExtras.h"

class Segment {
	public:
		void setup(float xAtual, float yAtual, float zAtual, float xLast, float yLast, float zLast, float xDirection, float yDirection, float zDirection);
		void update();
		void draw();

		ofVec3f atualPos;
		ofVec3f lastPos;
		ofVec3f segmentDirection;
};

//Filipe Freire Soares 2020238986
