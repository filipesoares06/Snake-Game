#include "cgGameMovement.h"

Movement::Movement() {
	setup();
}

void Movement::setup() {
	glEnable(GL_DEPTH_TEST);
	ofBackground(0.05, 0.05, 0.05);

	ofDisableArbTex();   //Possibilita o uso de coordenadas de textura normalizadas.
	glShadeModel(GL_SMOOTH);

	//Inicializa variáveis gerais do jogo.
	hardDificult = false;
	gameWon = 15;   //Número de candys necessários comer para vencer o jogo.

	//Inicializa variáveis da câmera.
	viewCount = 0;
	lensAngle = 60;
	alphaValue = 10;
	betaValue = 1000;
	perfectDistance = (gh() * 0.5) / tan(lensAngle * 0.5 * (PI / 180));

	//Inicializa variáveis do chão (Malha).
	resX = 16;   //Quantidade de quadrados da malha, neste caso a malha terá resolução 16x16.
	resY = 16;
	floorWidth = gw() * 0.5;
	floorHeight = gw() * 0.5;
	floorDepth = 1.0;

	//Inicializa variáveis da parede esquerda.
	leftWallWidth = floorWidth / resX;
	leftWallHeight = floorHeight;
	leftWallDepth = floorWidth / resX;
	leftWallPos.x = -(floorWidth / 2) + (floorWidth / resX) * 0.5;
	leftWallPos.y = 0;
	leftWallPos.z = (floorHeight / resY) - ((floorHeight / resY) * 0.5);

	//Inicializa variáveis da parede direita.
	rightWallWidth = floorWidth / resX;
	rightWallHeight = floorHeight;
	rightWallDepth = floorWidth / resX;
	rightWallPos.x = (floorWidth / 2) - (floorWidth / resX) * 0.5;
	rightWallPos.y = 0;
	rightWallPos.z = (floorHeight / resY) - ((floorHeight / resY) * 0.5);

	//Inicializa variáveis da parede de cima.
	topWallWidth = floorHeight;
	topWallHeight = floorWidth / resX;
	topWallDepth = floorWidth / resX;
	topWallPos.x = 0;
	topWallPos.y = -(floorHeight / 2) + (floorHeight / resX) * 0.5;
	topWallPos.z = (floorHeight / resY) - ((floorHeight / resY) * 0.5);

	//Inicializa variáveis da parede de baixo.
	bottomWallWidth = floorHeight;
	bottomWallHeight = floorWidth / resX;
	bottomWallDepth = floorWidth / resX;
	bottomWallPos.x = 0;
	bottomWallPos.y = (floorHeight / 2) - (floorHeight / resX) * 0.5;
	bottomWallPos.z = (floorHeight / resY) - ((floorHeight / resY) * 0.5);

	//Inicializa variáveis de movimento da snake.
	leftMovement = false;
	rightMovement = false;
	topMovement = false;
	bottomMovement = false;

	//Inializa variáveis dos segmentos da snake.
	snakeWidth = floorWidth / resX;
	snakeHeight = floorHeight / resY;
	snakeDepth = (floorWidth / resX) * 0.75;

	//Inicializa variáveis da snake.
	snakePos.x = (floorWidth / resX) * 0.5;   //Posição inicial da cabeça da snake.
	snakePos.y = (floorHeight / resY) * 0.5;
	snakePos.z = (floorHeight / resY) - ((floorHeight / resY) * 0.5);
	snakeBody.clear();

	Segment newSnake;
	newSnake.atualPos = snakePos;
	snakeBody.push_back(newSnake);   //Adiciona o primeiro segmento da snake (Cabeça).

	wallSize = floorWidth / resX;

	//Inicializa variáveis da câmera em primeira pessoa (3D).
	lookAtPos = snakeBody[0].atualPos;
	upVector.x = 0;
	upVector.y = 1;
	upVector.z = 0;

	//Inicializa variáveis do candy.
	candyWidth = (floorWidth / resX) * 0.5;
	candyHeight = (floorWidth / resX) * 0.5;
	candyDepth = (floorWidth / resX) * 0.5;
	candySize = (floorWidth / resX) * 0.5;

	//Inicializa variáveis do movimento do candy.
	candyVelDirection.x = -1;
	candyVelDirection.y = 0;
	candyVelDirection.z = 0;
	candyVelModule = 1;

	//Inicializa variáveis de iluminação.
	ambientOn = true;
	directionalOn = false;
	pointualOn = false;
	spotOn = false;
	localViewer = false;

	turnOffAmb = false;
	turnOffDif = false;
	turnOffSpec = false;
	
	directionalVecTheta = 0;

	spotExponent = 64;
	spotCutOff = 45;
	spotTheta = 0;

	//Inicializa variáveis de materiais.
	numMaterials = 0;

	//Inicializa varáveis do sketch.
	pX = 128;
	pY = 128;
	skyImage.allocate(pX, pY, OF_IMAGE_COLOR);
	skyPixels = new unsigned char[pX * pY * 3];
	phaseX = 0;
	phaseY = 0;

	for (int i = 0; i < 10; i++) {
		frequencyValues[i] = 0.25 + 0.25 * i * 5;
		ampValues[i] = 1 / (i * 0.25 + 1);
	}

	//Inicializa variáveis das texturas.
	textureOn = true;
	sandTexture.load("sandTexture0.jpg");
	brickTexture.load("brickTexture0.jpg");

	brickTexture.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);

	resetPhysics();
}

void Movement::resetPhysics() {
	collideState = 0;

	eatState = 0;

	fireworkState = 0;
}

void Movement::update() {
	if (hardDificult) {   //Atualiza a dificuldade do jogo aumentando a velocidade do candy.
		candyVelModule = 3;
	}

	else {
		candyVelModule = 1;
	}

	if (leftMovement) {   //Movimenta a posição do primeiro segmento da snake (Cabeça).
		snakeBody[0].lastPos = snakeBody[0].atualPos;
		snakeBody[0].atualPos.x -= wallSize;

		updatePositions();

		if (viewCount == 3) {   //Atualiza a lookAt position caso o jogo esteja em 3D.
			lookAtPos = ofVec3f(snakeBody[0].atualPos.x - 100, snakeBody[0].atualPos.y, snakeBody[0].atualPos.z);
		}

		leftMovement = false;
	}

	if (rightMovement) {
		snakeBody[0].lastPos = snakeBody[0].atualPos;
		snakeBody[0].atualPos.x += wallSize;

		updatePositions();

		if (viewCount == 3) {
			lookAtPos = ofVec3f(snakeBody[0].atualPos.x + 100, snakeBody[0].atualPos.y, snakeBody[0].atualPos.z);
		}

		rightMovement = false;
	}

	if (topMovement) {
		snakeBody[0].lastPos = snakeBody[0].atualPos;
		snakeBody[0].atualPos.y += wallSize;

		updatePositions();

		if (viewCount == 3) {
			lookAtPos = ofVec3f(snakeBody[0].atualPos.x, snakeBody[0].atualPos.y + 100, snakeBody[0].atualPos.z);
		}

		topMovement = false;
	}

	if (bottomMovement) {
		snakeBody[0].lastPos = snakeBody[0].atualPos;
		snakeBody[0].atualPos.y -= wallSize;

		updatePositions();

		if (viewCount == 3) {
			lookAtPos = ofVec3f(snakeBody[0].atualPos.x, snakeBody[0].atualPos.y - 100, snakeBody[0].atualPos.z);
		}

		bottomMovement = false;
	}

	if (spotOn) {   //Movimento automatizado para a fonte de luz foco.
		spotDir[0] = cos(spotTheta * PI / 180);
		spotDir[1] = sin(spotTheta * PI / 180);
		spotTheta += 0.75;
	}

	linearMovement();

	collideSnake();

	eatCandy();

	finalFirework();
}

void Movement::draw() {
	//glColor3f(0.5, 0.5, 0.5);
	glPushMatrix();   //Master Push matrix.

	//Pontuação (Canto superior direito).
	string scoreNumber = ofToString(snakeBody.size());   //Converte o número para string.
	scoreNumber += "/15";

	if (hardDificult) {
		scoreNumber += "\nHard";
	}

	else {
		scoreNumber += "\nEasy";
	}

	float xScore = gw() - 10 - scoreNumber.length() * 5;   //Garante que a string não saia da tela.
	float yScore = 20;

	ofDrawBitmapString(scoreNumber, xScore, yScore);

	//Câmera.
	switch (viewCount) {
		case 0:   //Viewport principal com vista ortográfica de cima.
			glViewport(0, 0, gw(), gh());   //Mapeamento do volume de projeção.
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			glOrtho(-gw() * 0.5, gw() * 0.5, -gh() * 0.5, gh() * 0.5, -10000, 10000);   //Volume de visualização e planos próximos e distantes.
			lookat(0, 0, 1, 0, 0, 0, 0, 1, 0);

			break;
		case 1:   //Viewport principal com vista perpetiva de cima (Adicionado na Meta 2).
			glViewport(0, 0, gw(), gh());

			perspective(lensAngle, alphaValue, betaValue);   //Configura a projeção em perspetiva.
			lookat(0, 0, perfectDistance, 0, 0, 0, 0, 1, 0);

			break;
		case 2:   //Viewport principal com vista perspetiva de frente.
			glViewport(0, 0, gw(), gh());

			perspective(lensAngle, alphaValue, betaValue);
			lookat(0, -gh() * 0.75, perfectDistance * 0.5, 0, 0, 0, 0, 1, 0);

			break;
		case 3:   //Viewport principal com vista em 1º pessoa.
			glViewport(0, 0, gw(), gh());
			
			perspective(lensAngle, alphaValue, betaValue);
			lookat(snakeBody[0].atualPos.x, snakeBody[0].atualPos.y, snakeBody[0].atualPos.z + (snakeWidth * 1.5), lookAtPos.x, lookAtPos.y, lookAtPos.z, 0, 0, 1);
			
			break;
		default:
			break;
	}

	//Textura.
	if (textureOn) {   //Representa o jogo com o sketch, algumas textuas e materiais.
		sketchTexture();   //Dentro do método sketchTexture() é chamado o método drawGame(), sendo possível a representação do jogo no sketch.
	}

	else {   //Representa o jogo somente com materiais.
		drawGame();
	}

	//Iluminação.
	glEnable(GL_LIGHTING);   //Habilita o uso de iluminação.
	glEnable(GL_NORMALIZE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, localViewer);

	if (ambientOn) {   //Fonte de luz que só possui a componente ambiente.
		ambientLight[0] = 1;   //R.
		ambientLight[1] = 1;   //G.
		ambientLight[2] = 1;   //B.
		ambientLight[3] = 1;   //Constante.
	}

	else {
		ambientLight[0] = 0;
		ambientLight[1] = 0;
		ambientLight[2] = 0;
		ambientLight[3] = 1;
	}

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	
	directionalVec[0] = directionalVec3f.x;   //Fonte de luz direcional. (x, y, z).
	directionalVec[1] = directionalVec3f.y;
	directionalVec[2] = directionalVec3f.z;
	directionalVec[3] = 0;

	directionalVec3f = ofVec3f(0, gh() * 0.5, gh() * 0.25 * 0.95) - ofVec3f(0, 0, 0);   //Direção da fonte de luz direcioal.

	if (turnOffAmb) {   //Desativa a componente ambiente da fonte de luz direcional.
		directionalAmb[0] = 0;   //R.
		directionalAmb[1] = 0;   //G.
		directionalAmb[2] = 0;   //B.
		directionalAmb[3] = 1;   //Constante.
	}

	else {   //Ativa a component ambiente da fonte de luz direcional.
		directionalAmb[0] = 1;
		directionalAmb[1] = 1;
		directionalAmb[2] = 1;
		directionalAmb[3] = 1;
	}

	if (turnOffDif) {   //Desativa a componente difusa da fonte de luz direcional.
		directionalDif[0] = 0;
		directionalDif[1] = 0;
		directionalDif[2] = 0;
		directionalDif[3] = 1;
	}

	else {   //Ativa a componente difusa da fonte de luz direcional.
		directionalDif[0] = 1;
		directionalDif[1] = 1;
		directionalDif[2] = 1;
		directionalDif[3] = 1;
	}

	if (turnOffSpec) {   //Desativa a componente especular da fonte de luz direcional.
		directionalSpec[0] = 0;
		directionalSpec[1] = 0;
		directionalSpec[2] = 0;
		directionalSpec[3] = 1;
	}

	else {   //Ativa a componente especular da fonte de luz direcional.
		directionalSpec[0] = 1;
		directionalSpec[1] = 1;
		directionalSpec[2] = 1;
		directionalSpec[3] = 1;
	}

	glLightfv(GL_LIGHT0, GL_POSITION, directionalVec);
	glLightfv(GL_LIGHT0, GL_AMBIENT, directionalAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directionalDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, directionalSpec);

	if (directionalOn) {
		glEnable(GL_LIGHT0);
	}

	else {
		glDisable(GL_LIGHT0);
	}
	
	pointualPos[0] = 0;   //Fonte de luz pontual. (x, y, z).
	pointualPos[1] = 0;
	pointualPos[2] = gh() * 0.25;
	pointualPos[3] = 1;

	if (turnOffAmb) {   //Desativa a componente ambiente da fonte de luz pontual.
		pointualAmb[0] = 0;   //R.
		pointualAmb[1] = 0;   //G.
		pointualAmb[2] = 0;   //B.
		pointualAmb[3] = 1;   //Constante.
	}

	else {   //Ativa a componente ambiente da fonte de luz pontual.
		pointualAmb[0] = 1;
		pointualAmb[1] = 1;
		pointualAmb[2] = 1;
		pointualAmb[3] = 1;
	}

	if (turnOffDif) {   //Desativa a componente difusa da fonte de luz pontual.
		pointualDif[0] = 0;
		pointualDif[1] = 0;
		pointualDif[2] = 0;
		pointualDif[3] = 1;
	}

	else {   //Ativa a componente difusa da fonte de luz pontual.
		pointualDif[0] = 1;
		pointualDif[1] = 1;
		pointualDif[2] = 1;
		pointualDif[3] = 1;
	}

	if (turnOffSpec) {   //Desativa a componente especular da fonte de luz pontual.
		pointualSpec[0] = 0;
		pointualSpec[1] = 0;
		pointualSpec[2] = 0;
		pointualSpec[3] = 1;
	}

	else {   //Ativa  a componente especular da fonte de luz pontual.
		pointualSpec[0] = 1;
		pointualSpec[1] = 1;
		pointualSpec[2] = 1;
		pointualSpec[3] = 1;
	}
	
	glLightfv(GL_LIGHT1, GL_POSITION, pointualPos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, pointualAmb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, pointualDif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, pointualSpec);

	pointualAtc = 1;   //Atenuação da luz pontual.
	pointualAtl = 0.0001;
	pointualAtq = 0.00001;

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, pointualAtc);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, pointualAtl);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, pointualAtq);

	if (pointualOn) {
		glEnable(GL_LIGHT1);
	}

	else {
		glDisable(GL_LIGHT1);
	}
	
	spotPos[0] = 0;   //Fonte de luz foco. (x, y, z).
	spotPos[1] = 0;
	spotPos[2] = gh() * 0.25;
	spotPos[3] = 1;

	spotDir[2] = -1;   //Os valores spotDir[0] e spotDir[1] são alterados no método update de modo a permitir o movimento automatizado da fonte de luz de foco.

	if (turnOffAmb) {   //Desativa a componente ambiente da fonte de luz de foco.
		spotAmb[0] = 0;   //R.
		spotAmb[1] = 0;   //G.
		spotAmb[2] = 0;   //B.
		spotAmb[3] = 1;   //Constante.
	}

	else {   //Ativa a componente ambiente da fonte de luz de foco.
		spotAmb[0] = 1;
		spotAmb[1] = 1;
		spotAmb[2] = 1;
		spotAmb[3] = 1;
	}

	if (turnOffDif) {   //Desativa a componente difusa da fonte de luz de foco.
		spotDif[0] = 0;
		spotDif[1] = 0;
		spotDif[2] = 0;
		spotDif[3] = 1;
	}

	else {   //Ativa a componente difusa da fonte de luz de foco.
		spotDif[0] = 1;
		spotDif[1] = 1;
		spotDif[2] = 1;
		spotDif[3] = 1;
	}

	if (turnOffSpec) {   //Desativa a componente especular da fonte de luz de foco.
		spotSpec[0] = 0;
		spotSpec[1] = 0;
		spotSpec[2] = 0;
		spotSpec[3] = 1;
	}

	else {   //Ativa a componente especular da fonte de luz de foco.
		spotSpec[0] = 1;
		spotSpec[1] = 1;
		spotSpec[2] = 1;
		spotSpec[3] = 1;
	}

	glLightfv(GL_LIGHT2, GL_POSITION, spotPos);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDir);
	glLightfv(GL_LIGHT2, GL_AMBIENT, spotAmb);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, spotDif);
	glLightfv(GL_LIGHT2, GL_SPECULAR, spotSpec);

	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, spotExponent);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, spotCutOff);

	spotAtc = 1;   //Atenuação da luz foco.
	spotAtl = 0;
	spotAtq = 0;

	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, spotAtc);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, spotAtl);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, spotAtq);

	if (spotOn) {
		glEnable(GL_LIGHT2);
	}

	else {
		glDisable(GL_LIGHT2);
	}

	if (directionalOn) {   //Representa a fonte de luz direcional.
		glPushMatrix();
		loadMaterial(16);   //Se alterarmos o material, a cor da sketch também altera.
		glTranslatef(directionalVec3f.x, directionalVec3f.y, directionalVec3f.z);
		glScalef(wallSize, wallSize, wallSize);
		cube_unit();
		glPopMatrix();
	}

	if (pointualOn) {   //Representa a fonte de luz pontual.
		glPushMatrix();
		loadMaterial(16);
		glTranslatef(pointualPos[0], pointualPos[1], pointualPos[2]);
		glScalef(wallSize, wallSize, wallSize);
		cube_unit();
		glPopMatrix();
	}

	if (spotOn) {   //Representa a fonte de luz foco.
		glPushMatrix();
		loadMaterial(16);
		glTranslatef(spotPos[0], spotPos[1], spotPos[2]);
		glScalef(wallSize, wallSize, wallSize);
		cube_unit();
		glPopMatrix();
	}

	glPopMatrix();   //Master pop matrix.
}

void Movement::keyPressed(int key) {
	switch (key) {
	case '1':
		glDisable(GL_CULL_FACE);

		break;
	case '2':
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		break;
	case '3':
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		break;
	case '4':
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT_AND_BACK);

		break;
	case 'g':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		ofNoFill();

		break;
	case 'f':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		ofFill();

		break;
	case 'a':
		leftMovement = !leftMovement;
		rightMovement = false;
		topMovement = false;
		bottomMovement = false;

		break;
	case 'd':
		rightMovement = !rightMovement;
		leftMovement = false;
		topMovement = false;
		bottomMovement = false;

		break;
	case 'w':
		topMovement = !topMovement;
		leftMovement = false;
		rightMovement = false;
		bottomMovement = false;

		break;
	case 's':
		bottomMovement = !bottomMovement;
		leftMovement = false;
		rightMovement = false;
		topMovement = false;

		break;
	case 'v':
		viewCount++;

		if (viewCount > 3) {
			viewCount = 0;
		}

		break;
	case 'l':
		ambientOn = !ambientOn;

		break;
	case 'k':
		directionalOn = !directionalOn;

		break;
	case 'j':
		pointualOn = !pointualOn;

		break;
	case 'h':
		spotOn = !spotOn;

		break;
	case 'p':
		localViewer = !localViewer;

		break;
	case 'o':
		turnOffAmb = !turnOffAmb;

		break;
	case 'i':
		turnOffDif = !turnOffDif;

		break;
	case 'u':
		turnOffSpec = !turnOffSpec;

		break;
	case 'z':
		textureOn = !textureOn;

		break;
	case '+':
		hardDificult = !hardDificult;

		break;
	}
}

void Movement::keyReleased(int key) {

}

void Movement::exit() {   //Método responsável por limpar o buffer da sketch.
	skyImage.clear();
}

void Movement::drawGame() {   //Método responável por desenhar o jogo.
	if (textureOn) {   //Representa o jogo com algumas texturas e materiais.
		//Chão (Malha).
		glPushMatrix();
		floorTexture();
		glPopMatrix();

		//Paredes esquerda, direita, cima e baixo.
		glPushMatrix();
		wallTexture();
		glPopMatrix();
	}

	else {   //Representa o jogo somente com materiais.
		//Chão (Malha).
		glPushMatrix();
		loadMaterial(17);
		glScalef(floorWidth, floorHeight, floorDepth);
		malha_unit(resX, resY);
		glPopMatrix();

		//Parede esquerda.
		glPushMatrix();
		loadMaterial(2);
		glTranslatef(leftWallPos.x, leftWallPos.y, leftWallPos.z);
		glScalef(leftWallWidth, leftWallHeight, leftWallDepth);
		cube_unit();
		glPopMatrix();

		//Parede direita.
		glPushMatrix();
		loadMaterial(2);
		glTranslatef(rightWallPos.x, rightWallPos.y, rightWallPos.z);
		glScalef(rightWallWidth, rightWallHeight, rightWallDepth);
		cube_unit();
		glPopMatrix();

		//Parede de cima.
		glPushMatrix();
		loadMaterial(2);
		glTranslatef(topWallPos.x, topWallPos.y, topWallPos.z);
		glScalef(topWallWidth, topWallHeight, topWallDepth);
		cube_unit();
		glPopMatrix();

		//Parede de baixo.
		glPushMatrix();
		loadMaterial(2);
		glTranslatef(bottomWallPos.x, bottomWallPos.y, bottomWallPos.z);
		glScalef(bottomWallWidth, bottomWallHeight, bottomWallDepth);
		cube_unit();
		glPopMatrix();
	}

	//Snake.
	for (int i = 0; i < snakeBody.size(); i++) {
		glPushMatrix();
		loadMaterial(13);
		glTranslatef(snakeBody[i].atualPos.x, snakeBody[i].atualPos.y, snakeBody[i].atualPos.z);
		glScalef(snakeWidth, snakeHeight, snakeDepth);
		cube_unit();
		glPopMatrix();
	}

	loadMaterial(15);   //Material do candy e das particulas da cobra.

	//Explosão da snake.
	for (int i = 0; i < snakeExplosion.size(); i++) {
		snakeExplosion[i].draw();
	}

	//Candy.
	glPushMatrix();
	glTranslatef(candyPos.x, candyPos.y, candyPos.z);
	glScalef(candyWidth, candyHeight, candyDepth);
	circle_unit();
	glPopMatrix();

	//Explosão do candy.
	for (int i = 0; i < candyExplosion.size(); i++) {
		candyExplosion[i].draw();
	}

	loadMaterial(10);   //Material do firework final.

	//Explosão do firework.
	for (int i = 0; i < winFirework.size(); i++) {
		winFirework[i].draw();
	}

	loadMaterial(16);   //Permite que a sketch tenha a cor correta, mesmo com a iluminação desativada.
}

void Movement::resetSnake() {   //Método responsável por reiniciar a snake após uma colisão ou quando o jogo termina.
	snakeBody.clear();

	Segment newSnake;
	newSnake.setup(snakePos.x, snakePos.y, snakePos.z, 0, 0, 0, 0, 0, 0);

	snakeBody.push_back(newSnake);
	
	leftMovement = false;   //Os movimentos da snake são resetados.
	rightMovement = false;
	topMovement = false;
	bottomMovement = false;
}

void Movement::collideSnake() {   //Método responsável por implementar a colisão entre a snake e as paredes.
	if (collideState == 0) {
		snakeExplosion.clear();

		collideState = 1;
	}

	else if (collideState == 1) {
		bool snakeTouching = false;
		int numParticles = 150;   //Variável que representa o número de particulas.

		if (snakeBody[0].atualPos.x < leftWallPos.x + wallSize || snakeBody[0].atualPos.x > rightWallPos.x - wallSize) {   //Verifica se colide com a parede esquerda ou com a parede direita.
			snakeTouching = true;
		}

		if (snakeBody[0].atualPos.y > bottomWallPos.y - wallSize || snakeBody[0].atualPos.y < topWallPos.y + wallSize) {   //Verifica se colide com a parede de cima ou com a parede de baixo. TopWall encontra-se no valor de y negativo e BottomWall encontra-se no valor de y positivo.
			snakeTouching = true;
		}
		
		for (int i = 1; i < snakeBody.size(); i++) {   //Verifica se a snake colide com ela própria.
			if (snakeBody[0].atualPos.x == snakeBody[i].atualPos.x && snakeBody[0].atualPos.y == snakeBody[i].atualPos.y) {
				snakeTouching = true;
			}
		}

		if (snakeTouching) {
			collideState = 2;

			for (int i = 0; i < numParticles; i++) {
				Particle auxParticle;

				auxParticle.setup(snakeBody[0].atualPos.x + (wallSize * 0.5), snakeBody[0].atualPos.x - (wallSize * 0.5), snakeBody[0].atualPos.y - (wallSize * 0.5), snakeBody[0].atualPos.y + (wallSize * 0.5), 1, 0, 0);   //wallSize == snakeSize.

				snakeExplosion.push_back(auxParticle);
			}
		}
	}

	else if (collideState == 2) {
		for (int i = 0; i < snakeExplosion.size(); i++) {
			snakeExplosion[i].update();

			if (snakeExplosion[i].deadParticle) {
				snakeExplosion.erase(snakeExplosion.begin() + i);   //Apaga as partículas da explosão da snake.
			}
		}

		if (snakeExplosion.size() == 0) {
			resetSnake();   //Pára e coloca a snake na posição inicial.

			collideState = 0;
		}
	}
}

void Movement::linearMovement() {   //Método responsável por implementar o movimento linear do candy.	
	candyPos += candyVelDirection * candyVelModule;   //Faz o candy mover-se linearmente.

	if (candyPos.x + (candySize * 0.5) > rightWallPos.x - (wallSize * 2)) {   //Verifica se o candy atinge a posição máxima à direita.
		candyVelDirection.x *= -1;   //Inverte a direção da velocidade do candy, logo irá mover-se em sentido contrário.

		candyPos.x = (rightWallPos.x - (wallSize * 2) - (candySize * 0.5));
	}

	if (candyPos.x - (candySize * 0.5) < leftWallPos.x + (wallSize * 2)) {   //Verifica se o candy atinge a posição máxima à esquerdda.
		candyVelDirection.x *= -1;

		candyPos.x = (leftWallPos.x + (wallSize * 2)) + (candySize * 0.5);
	}
}

void Movement::eatCandy() {   //Método responsável por implementar a ação da snake comer um candy.
	if (eatState == 0) {
		ofSeedRandom();
		xRandomPosition = (int)ofRandom(2, 14);   //O número 14 é exclusivo, logo 13 é o limite máximo incluído.
		yRandomPosition = (int)ofRandom(2, 14);

		candyPos.x = rightWallPos.x - (wallSize * xRandomPosition);   //Posição inicial do candy.
		candyPos.y = topWallPos.y + (wallSize * yRandomPosition);
		candyPos.z = (floorHeight / resY) - ((floorHeight / resY) * 0.5);

		candyExplosion.clear();

		eatState = 1;
	}

	else if (eatState == 1) {
		bool snakeEating = false;
		int numParticles = 150;   //Variável que representa o número de particulas.

		if (snakeBody[0].atualPos.x >= candyPos.x - (candySize * 0.5) && snakeBody[0].atualPos.x <= candyPos.x + (candySize * 0.5)) {   //Verifica se a snake come o candy.
			if (snakeBody[0].atualPos.y >= candyPos.y - (candySize * 0.5) && snakeBody[0].atualPos.y <= candyPos.y + (candySize * 0.5)) {
				snakeEating = true;
			}
		}

		if (snakeEating) {
			eatState = 2;

			Segment newSegment;
			ofVec3f lastSegmentPos = snakeBody.back().lastPos;
			ofVec3f newSegmentDirection = snakeBody.back().segmentDirection;

			newSegment.setup(lastSegmentPos.x, lastSegmentPos.y, lastSegmentPos.z, lastSegmentPos.x, lastSegmentPos.y, lastSegmentPos.z, newSegmentDirection.x, newSegmentDirection.y, newSegmentDirection.z);
			snakeBody.push_back(newSegment);   //Adiciona o novo segmento à snake.

			for (int i = 0; i < numParticles; i++) {
				Particle auxParticle;

				auxParticle.setup(snakeBody[0].atualPos.x + (wallSize * 0.5), snakeBody[0].atualPos.x - (wallSize * 0.5), snakeBody[0].atualPos.y - (wallSize * 0.5), snakeBody[0].atualPos.y + (wallSize * 0.5), 1, 0, 0);

				candyExplosion.push_back(auxParticle);
			}
		}
	}

	else if (eatState == 2) {
		for (int i = 0; i < candyExplosion.size(); i++) {
			candyExplosion[i].update();

			if (candyExplosion[i].deadParticle) {
				candyExplosion.erase(candyExplosion.begin() + i);   //Apaga as partículas da explosão do candy.
			}
		}

		if (candyExplosion.size() == 0) {
			eatState = 0;
		}
	}
}

void Movement::finalFirework() {   //Método responsável por implementar o firework após vencer o jogo.
	if (fireworkState == 0) {
		fireworkPos0.x = rightWallPos.x - (wallSize * 12);   //Coordenadas do primeiro firework.
		fireworkPos0.y = topWallPos.y + (wallSize * 3);
		fireworkPos0.z = (floorHeight / resY) - ((floorHeight / resY) * 0.5);

		fireworkPos1.x = 0;   //Coordenadas do segundo firework.
		fireworkPos1.y = 0;
		fireworkPos1.z = (floorHeight / resY) - ((floorHeight / resY) * 0.5);

		fireworkPos2.x = rightWallPos.x - (wallSize * 3);   //Coordenadas do terceiro firework.
		fireworkPos2.y = topWallPos.y + (wallSize * 12);
		fireworkPos2.z = (floorHeight / resY) - ((floorHeight / resY) * 0.5);

		winFirework.clear();

		fireworkState = 1;
	}

	else if (fireworkState == 1) {
		bool fireworkStarting = false;
		int numParticles = 5000;

		if (snakeBody.size() == gameWon) {   //Quando a snake atinge o tamanho máximo, o jogo termina com firework.
			fireworkStarting = true;
		}

		if (fireworkStarting) {
			fireworkState = 2;

			for (int i = 0; i < numParticles; i++) {
				Particle auxParticle0;
				Particle auxParticle1;
				Particle auxParticle2;

				auxParticle0.setup(fireworkPos0.x + (wallSize * 0.5), fireworkPos0.x - (wallSize * 0.5), fireworkPos0.y - (wallSize * 0.5), fireworkPos0.y + (wallSize * 0.5), 1, 0.9, 0);   //Os 3 últimos parâmetros definem a cor das partículas.
				winFirework.push_back(auxParticle0);
				
				auxParticle1.setup(fireworkPos1.x + (wallSize * 0.5), fireworkPos1.x - (wallSize * 0.5), fireworkPos1.y - (wallSize * 0.5), fireworkPos1.y + (wallSize * 0.5), 1, 0.9, 0);
				winFirework.push_back(auxParticle1);

				auxParticle2.setup(fireworkPos2.x + (wallSize * 0.5), fireworkPos2.x - (wallSize * 0.5), fireworkPos2.y - (wallSize * 0.5), fireworkPos2.y + (wallSize * 0.5), 1, 0.9, 0);
				winFirework.push_back(auxParticle2);
			}
		}
	}

	else if (fireworkState == 2) {
		for (int i = 0; i < winFirework.size(); i++) {
			winFirework[i].update();   //Executa o firework durante o seu lifetime (Ver método update em cgGameParticle.cpp).

			if (winFirework[i].deadParticle) {
				winFirework.erase(winFirework.begin() + i);   //Apaga o firework quando este termina.
			}
		}

		if (winFirework.size() == 0) {   //Quando o vetor do firework encontra-se vazio (Firework terminou) faz-se reset da snake.
			resetSnake();   //Pára e coloca a snake na posição inicial.
			
			fireworkState = 0;
		}
	}
}

void Movement::updatePositions() {   //Método responsável por atualizar as posições dos segmentos da snake de acordo com o seu movimento.
	for (int i = 1; i < snakeBody.size(); i++) {
		snakeBody[i].lastPos = snakeBody[i].atualPos;
		snakeBody[i].atualPos = snakeBody[i - 1].lastPos;
	}
}

void Movement::sketchTexture() {   //Método responsável por implementar a sketch.
	phaseX += 0.0005 * cos(directionalVecTheta * 0.001);
	phaseY -= 0.0006 * cos(directionalVecTheta * 0.0001);

	skyPixels = skyImage.getPixels().getData();

	ofColor colorA(0, 30, 180);
	ofColor colorB(255, 255, 255);

	for (int i = 0; i < pY; i++) {
		for (int j = 0; j < pX; j++) {
			int numIndex = 3 * (i * pX + j);

			float noiseValues = getMultiNoise(0.008 * (j) + phaseX, 0.008 * (i) + phaseY, frequencyValues, ampValues, 10);

			ofColor auxColor = colorA.getLerped(colorB, noiseValues);

			skyPixels[numIndex] = auxColor.r;
			skyPixels[numIndex + 1] = auxColor.g;
			skyPixels[numIndex + 2] = auxColor.b;
		}
	}

	skyImage.setFromPixels(skyPixels, pX, pY, OF_IMAGE_COLOR);
	skyImage.update();

	glEnable(GL_TEXTURE);
	skyImage.bind();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glPushMatrix();
	glColor3f(1, 1, 1);
	glRotatef(90, 0, 0, 1);   //Roda a sketch de forma a esconder a sua origem.
	glScalef(gw() * 50, gw() * 50., gw() * 50.);
	ofDrawSphere(1);
	glPopMatrix();

	skyImage.unbind();
	glDisable(GL_TEXTURE);

	glPushMatrix();
	glColor3f(0, 0, 0);
	drawGame();   //Método que representa o jogo todo. Permite a sua representação dentro da sketch.
	glPopMatrix();

	rX += 0.1;
	rY += 0.2;
	rZ += 0.15;
}

void Movement::floorTexture() {   //Método responsável por representar o chão com uma textura.
	glEnable(GL_TEXTURE);
	sandTexture.bind();

	GLint mValue = resX;
	GLint nValue = resY;
	GLfloat xStart = -0.5;
	GLfloat yStart = 0.5;
	GLfloat xStep = 1 / GLfloat(mValue);
	GLfloat yStep = 1 / GLfloat(nValue);

	glPushMatrix();

	glScalef(floorWidth, floorHeight, floorDepth);

	glBegin(GL_QUADS);

	for (int i = 0; i < mValue; i++) {
		for (int j = 0; j < nValue; j++) {
			glNormal3f(0, 0, 1);
			glTexCoord2f((i * 1) / GLfloat(mValue), (j * 1) / GLfloat(nValue));
			glVertex3f(i * xStep + xStart, -j * yStep + yStart, 0);

			glNormal3f(0, 0, 1);
			glTexCoord2f((i * 1) / GLfloat(mValue), (j + 1) * 1 / GLfloat(nValue));
			glVertex3f(i * xStep + xStart, -(j + 1) * yStep + yStart, 0);

			glNormal3f(0, 0, 1);
			glTexCoord2f((i + 1) * 1 / GLfloat(mValue), (j + 1) * 1 / GLfloat(nValue));
			glVertex3f((i + 1) * xStep + xStart, -(j + 1) * yStep + yStart, 0);

			glNormal3f(0, 0, 1);
			glTexCoord2f((i + 1) * 1 / GLfloat(mValue), (j) * 1 / GLfloat(nValue));
			glVertex3f((i + 1) * xStep + xStart, -(j)*yStep + yStart, 0);
		}
	}

	glEnd();

	glPopMatrix();

	sandTexture.unbind();
	glDisable(GL_TEXTURE);
}

void Movement::wallTexture() {   //Método responsável por representar as paredes com texturas.
	GLfloat tileFactor = 1.5;
	
	glEnable(GL_TEXTURE);
	brickTexture.bind();

	//Parede esquerda.
	glPushMatrix();

	glTranslatef(leftWallPos.x, leftWallPos.y, leftWallPos.z);
	glScalef(leftWallWidth, leftWallHeight, leftWallDepth);
	rect_texture(tileFactor);

	glPopMatrix();

	brickTexture.unbind();
	glDisable(GL_TEXTURE);
	
	glEnable(GL_TEXTURE);
	brickTexture.bind();
	
	//Parede direita.
	glPushMatrix();

	glTranslatef(rightWallPos.x, rightWallPos.y, rightWallPos.z);
	glScalef(leftWallWidth, leftWallHeight, leftWallDepth);
	rect_texture(tileFactor);

	glPopMatrix();

	brickTexture.unbind();
	glDisable(GL_TEXTURE);
	
	glEnable(GL_TEXTURE);
	brickTexture.bind();
	
	//Parede de cima.
	glPushMatrix();

	glTranslatef(topWallPos.x, topWallPos.y, topWallPos.z);
	glRotatef(90, 0, 0, 1);
	glScalef(leftWallWidth, leftWallHeight, leftWallDepth);
	rect_texture(tileFactor);

	glPopMatrix();

	brickTexture.unbind();
	glDisable(GL_TEXTURE);
	
	glEnable(GL_TEXTURE);
	brickTexture.bind();

	//Parede de baixo.
	glPushMatrix();

	glTranslatef(bottomWallPos.x, bottomWallPos.y, bottomWallPos.z);
	glRotatef(-90, 0, 0, 1);
	glScalef(leftWallWidth, leftWallHeight, leftWallDepth);
	rect_texture(tileFactor);
	
	glPopMatrix();
	
	brickTexture.unbind();
	glDisable(GL_TEXTURE);
}

//Filipe Freire Soares 2020238986
