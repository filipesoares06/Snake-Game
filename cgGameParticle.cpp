#include "cgGameParticle.h"

void Particle::setup(float xMax, float xMin, float yMax, float yMin, float rColor, float gColor, float bColor) {
	float xValue = ofRandom(xMin, xMax);
	float yValue = ofRandom(yMin, yMax);

	particleSize = gw() * 0.005;   //Tamanho das part�culas.

	particlePos.x = xValue;   //Valor aleat�rio, tal como a posi��o inicial da particula.
	particlePos.y = yValue;   //Valor aleat�rio, tal como a posi��o inicil da particula.
	particlePos.z = 0;

	particleSpeed.x = ofRandom(-2, 2);   //Velocidade e dire��o da particula aleat�rias.
	particleSpeed.y = ofRandom(-2, 2);
	particleSpeed.z = ofRandom(-2, 2);

	deadParticle = false;
	particleAge = 0;
	lifeTime = int(ofRandom(5, 30));   //Tempo de vida da particula aleat�rio.

	rParticleColor = rColor;
	gParticleColor = gColor;
	bParticleColor = bColor;
}

void Particle::update() {
	particlePos += particleSpeed;
	particleAge++;

	if (particleAge > lifeTime) {
		deadParticle = true;
	}
}

void Particle::draw() {
	if (!deadParticle) {   //A part�cula � desenha quando est� viva.
		glColor3f(rParticleColor, gParticleColor, bParticleColor);
		filled();
		glPushMatrix();
		glTranslatef(particlePos.x, particlePos.y, particlePos.z);
		glScalef(particleSize, particleSize, particleSize);
		cube_unit();
		glPopMatrix();
	}
}

//Filipe Freire Soares 2020238986
