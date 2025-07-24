#pragma once

#include "ofMain.h"
#include "cg_extras.h"

//desenha ponto 3D na origem
inline void drawPoint() {
	glBegin(GL_POINTS);
	glVertex3f(0, 0, 0);
	glEnd();
}

//desenha axis 3D
inline void axis3d() {
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

//função que desenha quadrado unitário 
//centrado na origem e preenchido
inline void rectFill_unit() {
	glBegin(GL_QUADS);
	glVertex3d(-0.5, -0.5, 0.);
	glVertex3d(-0.5, 0.5, 0.);
	glVertex3d(0.5, 0.5, 0.);
	glVertex3d(0.5, -0.5, 0.);
	glEnd();
}



inline void cube_unit() {
    GLfloat p = 0.5;

    // Habilita o teste de profundidade
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glShadeModel(GL_SMOOTH);
    glBegin(GL_QUADS);

    // frente
    glNormal3f(0, 0, 1);
    glVertex3f(-p, p, p);
    glVertex3f(-p, -p, p);
    glVertex3f(p, -p, p);
    glVertex3f(p, p, p);

    // tras
    glNormal3f(0, 0, -1);
    glVertex3f(-p, p, -p);
    glVertex3f(p, p, -p);
    glVertex3f(p, -p, -p);
    glVertex3f(-p, -p, -p);

    // cima
    glNormal3f(0, 1, 0);
    glVertex3f(-p, p, -p);
    glVertex3f(-p, p, p);
    glVertex3f(p, p, p);
    glVertex3f(p, p, -p);

    // baixo
    glNormal3f(0, -1, 0);
    glVertex3f(-p, -p, p);
    glVertex3f(-p, -p, -p);
    glVertex3f(p, -p, -p);
    glVertex3f(p, -p, p);

    // esq
    glNormal3f(-1, 0, 0);
    glVertex3f(-p, p, p);
    glVertex3f(-p, p, -p);
    glVertex3f(-p, -p, -p);
    glVertex3f(-p, -p, p);

    // dir
    glNormal3f(1, 0, 0);
    glVertex3f(p, p, p);
    glVertex3f(p, -p, p);
    glVertex3f(p, -p, -p);
    glVertex3f(p, p, -p);

    glEnd();

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST); // Desabilita o teste de profundidade após desenhar
}



//função que desenha malha unitária com resolução mxn
inline void malha_unit(GLint m, GLint n) {
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

inline void malha_unit1(GLint m, GLint n) {
    // Calcula o offset para centralizar a malha em (0, 0, 0)
    float offsetX = -(m - 1) / 2.0;
    float offsetY = -(n - 1) / 2.0;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            glPushMatrix();
            // Aplica a tradução ajustada para centralizar a malha
            glTranslated(i + offsetX, j + offsetY, 0);

            // Configura o material para cada cubo
			glDisable(GL_COLOR_MATERIAL);
            GLfloat mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
            GLfloat mat_diffuse[] = { 0.8f, 0.0f, 0.0f, 1.0f }; // Cor vermelha
            GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            GLfloat mat_shininess[] = { 50.0f };

            glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

            cube_unit();
            glPopMatrix();
			glEnable(GL_COLOR_MATERIAL);
    		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        }
    }
}

inline void rect_unit() {
	glBegin(GL_QUADS);
	glVertex3f(-0.5, -0.75, 0);
	glVertex3f(0.5, -0.75, 0);
	glVertex3f(0.5, 0.75, 0);
	glVertex3f(-0.5, 0.75, 0);
	
	glEnd();
}


//funcao que desenha cubo unit�rio com malha_unit()
inline void cube_malha_unit(GLint m, GLint n) {


	//tras
	glColor3f(0, 1, 0);
	glPushMatrix();
	glTranslated(0., 0., -0.5);
	glRotated(180, 0, 1, 0);
	malha_unit(m, n);
	glPopMatrix();

	//cima
	glColor3f(0, 1, 1);
	glPushMatrix();
	glTranslated(0., -0.5, 0.);
	glRotated(90, 1, 0, 0);
	malha_unit(m, n);
	glPopMatrix();

	//baixo
	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslated(0., 0.5, 0.);
	glRotated(-90, 1, 0, 0);
	malha_unit(m, n);
	glPopMatrix();

	//esquerda
	glColor3f(1, 0, 1);
	glPushMatrix();
	glTranslated(-0.5, 0., 0.);
	glRotated(-90, 0, 1, 0);
	malha_unit(m, n);
	glPopMatrix();

	//direita
	glColor3f(1, 1, 0);
	glPushMatrix();
	glTranslated(0.5, 0., 0.);
	glRotated(90, 0, 1, 0);
	malha_unit(m, n);
	glPopMatrix();

	//frente
	glColor3f(0, 0, 1);
	glPushMatrix();
	glTranslated(0., 0., 0.5);
	malha_unit(m, n);
	glPopMatrix();


}


//desenha preenchido
inline void filled(){
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

//desenha so o wireframe
inline void wireframe(){
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}


inline void crackedCube(ofImage img1) {
    // Define as propriedades do material
    GLfloat mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat_specular[] = { .0f, .0f, .0f, 1.0f }; // Sem brilho
    GLfloat mat_shininess[] = { 0.0f }; // Brilho zero

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // Ativa texturas
    glEnable(GL_TEXTURE_2D);

    // Configura a textura do cubo
    img1.bind();
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    // Desenha o cubo com texturas aplicadas
    glPushMatrix();
    glBegin(GL_QUADS);

    // Frente
    glNormal3f(0, 0, 1);
    glTexCoord2f(0, 0); glVertex3f(-0.5,  0.5,  0.5);
    glTexCoord2f(0, 1); glVertex3f(-0.5, -0.5,  0.5);
    glTexCoord2f(1, 1); glVertex3f( 0.5, -0.5,  0.5);
    glTexCoord2f(1, 0); glVertex3f( 0.5,  0.5,  0.5);

    // Direita
    glNormal3f(1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(0.5,  0.5,  0.5);
    glTexCoord2f(0, 1); glVertex3f(0.5, -0.5,  0.5);
    glTexCoord2f(1, 1); glVertex3f(0.5, -0.5, -0.5);
    glTexCoord2f(1, 0); glVertex3f(0.5,  0.5, -0.5);

    // Superior
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0); glVertex3f(-0.5,  0.5,  0.5);
    glTexCoord2f(0, 1); glVertex3f( 0.5,  0.5,  0.5);
    glTexCoord2f(1, 1); glVertex3f( 0.5,  0.5, -0.5);
    glTexCoord2f(1, 0); glVertex3f(-0.5,  0.5, -0.5);

    glEnd();
    glPopMatrix();

    img1.unbind();
    glDisable(GL_TEXTURE_2D);
}
