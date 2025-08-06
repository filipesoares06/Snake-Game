#pragma once

#include "ofMain.h"
#include "cgExtras.h"

inline void filled() {   //Desenha preenchido.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

inline void wireframe() {   //Desenha so o wireframe.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

inline void drawPoint() {   //Método responsável por desenhar um ponto 3D na origem.
	glBegin(GL_POINTS);

	glVertex3f(0, 0, 0);

	glEnd();
}

inline void axis3d() {   //Método responsável por desenhar o axis 3D.
	glBegin(GL_LINES);

	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glColor3f(1, 1, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);

	glEnd();
}

inline void rectFill_unit() {   //Método responsável por desenhar um quadrado unitário centrado na origem e preenchido.
	glBegin(GL_QUADS);

	glVertex3d(-0.5, -0.5, 0.);
	glVertex3d(-0.5, 0.5, 0.);
	glVertex3d(0.5, 0.5, 0.);
	glVertex3d(0.5, -0.5, 0.);

	glEnd();
}

inline void malha_unit(GLint m, GLint n) {   //Método responsável por desenhar uma malha unitária com resolução mxn.
	GLfloat x_start = -0.5;
	GLfloat y_start = -0.5;
	GLfloat x_step = 1.0 / GLfloat(m);
	GLfloat y_step = 1.0 / GLfloat(n);

	glBegin(GL_QUADS);

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			glVertex2d(i * x_step + x_start, j * y_step + y_start);
			glVertex2d(i * x_step + x_start, (j + 1) * y_step + y_start);
			glVertex2d((i + 1) * x_step + x_start, (j + 1) * y_step + y_start);
			glVertex2d((i + 1) * x_step + x_start, j * y_step + y_start);
		}
	}

	glEnd();
}

inline void cube_unit() {   //Método responsável por desenhar um cubo unitário.
	GLfloat p = 0.5;
	glBegin(GL_QUADS);

	//Frente.
	glVertex3f(-p, -p, p);
	glVertex3f(-p, p, p);
	glVertex3f(p, p, p);
	glVertex3f(p, -p, p);

	//Trás.
	glVertex3f(-p, -p, -p);
	glVertex3f(p, -p, -p);
	glVertex3f(p, p, -p);
	glVertex3f(-p, p, -p);

	//Cima.
	glVertex3f(-p, -p, -p);
	glVertex3f(-p, -p, p);
	glVertex3f(p, -p, p);
	glVertex3f(p, -p, -p);

	//Baixo.
	glVertex3f(-p, p, p);
	glVertex3f(-p, p, -p);
	glVertex3f(p, p, -p);
	glVertex3f(p, p, p);

	//Esquerda.
	glVertex3f(-p, -p, p);
	glVertex3f(-p, -p, -p);
	glVertex3f(-p, p, -p);
	glVertex3f(-p, p, p);

	//Direita.
	glVertex3f(p, -p, p);
	glVertex3f(p, p, p);
	glVertex3f(p, p, -p);
	glVertex3f(p, -p, -p);

	glEnd();
}

inline void cube_malha_unit(GLint m, GLint n) {   //Método responsável por desenhar um cubo unitário com malha_unit().
	//Trás.
	glColor3f(0, 1, 0);
	glPushMatrix();
	glTranslated(0., 0., -0.5);
	glRotated(180, 0, 1, 0);
	malha_unit(m, n);
	glPopMatrix();

	//Cima.
	glColor3f(0, 1, 1);
	glPushMatrix();
	glTranslated(0., -0.5, 0.);
	glRotated(90, 1, 0, 0);
	malha_unit(m, n);
	glPopMatrix();

	//Baixo.
	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslated(0., 0.5, 0.);
	glRotated(-90, 1, 0, 0);
	malha_unit(m, n);
	glPopMatrix();

	//Esquerda.
	glColor3f(1, 0, 1);
	glPushMatrix();
	glTranslated(-0.5, 0., 0.);
	glRotated(-90, 0, 1, 0);
	malha_unit(m, n);
	glPopMatrix();

	//Direita.
	glColor3f(1, 1, 0);
	glPushMatrix();
	glTranslated(0.5, 0., 0.);
	glRotated(90, 0, 1, 0);
	malha_unit(m, n);
	glPopMatrix();

	//Frente.
	glColor3f(0, 0, 1);
	glPushMatrix();
	glTranslated(0., 0., 0.5);
	malha_unit(m, n);
	glPopMatrix();
}

void circle_unit() {   //Método responsável por desenhar um círculo unitário.
	GLfloat p = 0.5;   //Raio do círculo unitário.
	int numSegments = 100;   //Resolução do círculo.
	float angleStep = 2 * PI / numSegments;

	glBegin(GL_TRIANGLE_FAN);   //Círculo preenchido.

	glVertex3f(0, 0, 0);   //Centro do círculo.

	for (int i = 0; i <= numSegments; ++i) {
		float angleValue = i * angleStep;
		float xValue = p * cos(angleValue);
		float yValue = p * sin(angleValue);
		float zValue = 0;   //Encontra-se no plano xy.

		glVertex3f(xValue, yValue, zValue);   //Adiciona o ponto.
	}

	glEnd();
}

void rect_texture(GLfloat tileFactor) {   //Método responsável por desenhar um cubo unitário ao qual serão aplicadas texturas com tiling.
	GLfloat p = 0.5;

	glBegin(GL_QUADS);

	//Frente.
	glNormal3f(0, 0, 1);
	glTexCoord2f(0, 0); glVertex3f(-p, -p, p);
	glTexCoord2f(tileFactor, 0); glVertex3f(p, -p, p);
	glTexCoord2f(tileFactor, tileFactor); glVertex3f(p, p, p);
	glTexCoord2f(0, tileFactor); glVertex3f(-p, p, p);

	//Trás.
	glNormal3f(0, 0, -1);
	glTexCoord2f(0, 0); glVertex3f(-p, -p, -p);
	glTexCoord2f(tileFactor, 0); glVertex3f(p, -p, -p);
	glTexCoord2f(tileFactor, tileFactor); glVertex3f(p, p, -p);
	glTexCoord2f(0, tileFactor); glVertex3f(-p, p, -p);

	//Esquerda.
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(-p, -p, -p);
	glTexCoord2f(tileFactor, 0); glVertex3f(-p, -p, p);
	glTexCoord2f(tileFactor, tileFactor); glVertex3f(-p, p, p);
	glTexCoord2f(0, tileFactor); glVertex3f(-p, p, -p);

	//Direita.
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(p, -p, -p);
	glTexCoord2f(tileFactor, 0); glVertex3f(p, -p, p);
	glTexCoord2f(tileFactor, tileFactor); glVertex3f(p, p, p);
	glTexCoord2f(0, tileFactor); glVertex3f(p, p, p);

	//Cima.
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(-p, p, -p);
	glTexCoord2f(tileFactor, 0); glVertex3f(p, p, -p);
	glTexCoord2f(tileFactor, tileFactor); glVertex3f(p, p, p);
	glTexCoord2f(0, tileFactor); glVertex3f(-p, p, p);

	//Baixo
	glNormal3f(0, -1, 0);
	glTexCoord2f(0, 0); glVertex3f(-p, -p, -p);
	glTexCoord2f(tileFactor, 0); glVertex3f(p, -p, -p);
	glTexCoord2f(tileFactor, tileFactor); glVertex3f(p, -p, p);
	glTexCoord2f(0, tileFactor); glVertex3f(-p, -p, p);

	glEnd();
}

//Filipe Freire Soares 2020238986
