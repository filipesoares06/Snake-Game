#pragma once
#include "ofMain.h"
#include "cgExtras.h"
#include "cgDrawingExtras.h"

class Particle {
	public:
		void setup(float xMax, float xMin, float yMax, float yMin, float rColor, float gColor, float bColor);
		void update();
		void draw();

		bool deadParticle;
		int lifeTime;
		int particleAge;
		float particleSize;
		float rParticleColor;
		float gParticleColor;
		float bParticleColor;
		ofVec3f particlePos;
		ofVec3f particleSpeed;
};

//Filipe Freire Soares 2020238986
