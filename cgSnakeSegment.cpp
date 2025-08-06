#include "cgSnakeSegment.h"

void Segment::setup(float xAtual, float yAtual, float zAtual, float xLast, float yLast, float zLast, float xDirection, float yDirection, float zDirection) {
	atualPos.x = xAtual;
	atualPos.y = yAtual;
	atualPos.z = zAtual;

	lastPos.x = xLast;
	lastPos.y = yLast;
	lastPos.z = zLast;

	segmentDirection.x = xDirection;
	segmentDirection.y = yDirection;
	segmentDirection.z = zDirection;
}

void Segment::update() {

}

void Segment::draw() {

}

//Filipe Freire Soares 2020238986
