#pragma once
#include "ofMain.h"
#include "cgExtras.h"
#include "cgDrawingExtras.h"
#include "cgGameParticle.h"
#include "cgCamExtras.h"
#include "cgSnakeSegment.h"
#include "cgMaterials.h"

class Movement {
	public:
		Movement();
		void setup();
		void resetPhysics();
		void update();
		void draw();
		void keyPressed(int key);
		void keyReleased(int key);
		void exit();
		void drawGame();
		void resetSnake();
		void collideSnake();
		void linearMovement();
		void eatCandy();
		void finalFirework();
		void updatePositions();
		void sketchTexture();
		void floorTexture();
		void wallTexture();

		//Vari�veis gerais do jogo.
		bool hardDificult;
		int gameWon;   //N�mero de candys necess�rios a apanhar para vencer o jogo.

		//Vari�veis da c�mera.
		int viewCount;
		GLfloat lensAngle;
		GLfloat alphaValue, betaValue;
		GLfloat perfectDistance;
		ofVec3f lookAtPos;
		ofVec3f upVector;

		//Vari�veis do ch�o (Malha).
		GLint resX, resY;
		GLfloat floorWidth, floorHeight, floorDepth;

		//Vari�veis da parede esquerda.
		GLfloat leftWallWidth, leftWallHeight, leftWallDepth;
		ofVec3f leftWallPos;

		//Vari�veis da parede direita.
		GLfloat rightWallWidth, rightWallHeight, rightWallDepth;
		ofVec3f rightWallPos;

		//Vari�veis da parede de cima.
		GLfloat topWallWidth, topWallHeight, topWallDepth;
		ofVec3f topWallPos;

		//Vari�veis da parede de baixo.
		GLfloat bottomWallWidth, bottomWallHeight, bottomWallDepth;
		ofVec3f bottomWallPos;

		//Var�aveis de movimento da snake.
		bool leftMovement, rightMovement, topMovement, bottomMovement;

		//Vari�veis dos segmentos da snake.
		GLfloat snakeWidth, snakeHeight, snakeDepth;
		ofVec3f snakePos;   //Vari�vel que representa a posi��o inicial da cobra.

		//Vari�veis da snake.
		vector<Segment> snakeBody;

		//Vari�veis das particulas da snake.
		vector<Particle> snakeExplosion;
		int collideState;
		float wallSize;

		//Vari�veis do candy.
		GLfloat candyWidth, candyHeight, candyDepth, candySize;
		ofVec3f candyPos;
		GLfloat candyVelModule;
		ofVec3f candyVelDirection;
		int xRandomPosition, yRandomPosition, randomDistance;

		//Vari�veis das particulas do candy.
		vector<Particle> candyExplosion;
		int eatState;

		//Vari�veis do firework final.
		vector<Particle> winFirework;
		ofVec3f fireworkPos0;
		ofVec3f fireworkPos1;
		ofVec3f fireworkPos2;
		int fireworkState;

		//Vari�veis da ilumina��o.
		bool ambientOn;
		bool directionalOn;
		bool pointualOn;
		bool spotOn;
		bool localViewer;
		bool turnOffAmb;
		bool turnOffDif;
		bool turnOffSpec;
		
		//Vari�veis da fonte de luz ambiente.
		GLfloat ambientLight[4];
		
		//Vari�veis da fonte de luz direcional.
		GLfloat directionalVec[4];
		GLfloat directionalAmb[4];
		GLfloat directionalDif[4];
		GLfloat directionalSpec[4];
		GLfloat directionalVecTheta;
		ofVec3f directionalVec3f;

		//Vari�veis da fonte de luz pontual.
		GLfloat pointualPos[4];
		GLfloat pointualAmb[4];
		GLfloat pointualDif[4];
		GLfloat pointualSpec[4];
		GLfloat pointualAtc, pointualAtl, pointualAtq;

		//Vari�veis da fonte de luz foco.
		GLfloat spotPos[4];
		GLfloat spotDir[3];
		GLfloat spotAmb[4];
		GLfloat spotDif[4];
		GLfloat spotSpec[4];
		GLfloat spotExponent;
		GLfloat spotCutOff;
		GLfloat spotTheta;
		GLfloat spotAtc, spotAtl, spotAtq;

		//Vari�veis de materiais.
		int numMaterials;

		//Vari�veis do sketch.
		ofImage skyImage;
		unsigned char* skyPixels = NULL;
		int pX, pY;
		float rX, rY, rZ;
		float frequencyValues[10];
		float ampValues[10];
		float phaseX;
		float phaseY;

		//Vari�veis das texturas.
		bool textureOn;
		ofImage sandTexture;
		ofImage brickTexture;
};

//Filipe Freire Soares 2020238986
