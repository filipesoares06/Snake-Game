#include "cgGameParticle.h"

void Particle::setup(float xMax, float xMin, float yMax, float yMin, float rColor, float gColor, float bColor) {
	float xValue = ofRandom(xMin, xMax);
	float yValue = ofRandom(yMin, yMax);

	particleSize = gw() * 0.005;   //Tamanho das partículas.

	particlePos.x = xValue;   //Valor aleatório, tal como a posição inicial da particula.
	particlePos.y = yValue;   //Valor aleatório, tal como a posição inicil da particula.
	particlePos.z = 0;

	particleSpeed.x = ofRandom(-2, 2);   //Velocidade e direção da particula aleatórias.
	particleSpeed.y = ofRandom(-2, 2);
	particleSpeed.z = ofRandom(-2, 2);

	deadParticle = false;
	particleAge = 0;
	lifeTime = int(ofRandom(5, 30));   //Tempo de vida da particula aleatório.

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
	if (!deadParticle) {   //A partícula é desenha quando está viva.
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
