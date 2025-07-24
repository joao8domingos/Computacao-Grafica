#pragma once
#include "ofMain.h"

        
class TetrisPiece {
public:
    TetrisPiece(int type);
    void update();
    void draw();
    void rotate(int n, int m, int (*matrix)[10]);
    void moveLeft(int n, int m, int (*matrix)[10]);
    void moveRight(int n, int m, int (*matrix)[10]);
    void moveDown(int n, int m, int (*matrix)[10], float (*red)[10], float (*green)[10], float (*blue)[10]);
    ofVec3f getPosition();
    void updateExtremes();
    void matrix_update(int n, int m, int (*matrix)[10], float (*red)[10], float (*green)[10], float (*blue)[10]);
    int colision_left(int n, int m, int (*matrix)[10]);
    int colision_right(int n, int m, int (*matrix)[10]);
    int clean_line(int n, int m, int (*matrix)[10], float (*red)[10], float (*green)[10], float (*blue)[10]);
    void draw_lines(int n, int m, int (*matrix)[10], float (*red)[10], float (*green)[10], float (*blue)[10]);
    void points_now(int points, int dif);


    int fps = 1;
    int hard = 0;
    int hard_new = 0;
		
	int m = 10;
	int n = 20;
    bool stop = false;

    GLfloat lim_esq;
	GLfloat lim_dir;
	GLfloat lim_baixo;

    ofVec3f pos;
    ofVec3f pos1;
    ofVec3f pos2;
    ofVec3f pos3;
    ofVec3f pos4;

    int points_check = 0;


private:
    
    // Variáveis para guardar posições extremas
    float leftmost_x;
    float rightmost_x;
    float lowest_y;
    int type_rotate = 0;
    GLfloat rotator_angle;
    int type; // Tipo de peça (1 para L, 2 para T, 3 para Cube)
    GLfloat color[3]; // Cor da peça (r, g, b)

    void generateRandomColor();
    void drawL();
    void drawT();
    void drawCube();
    void drawLine();
};