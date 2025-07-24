#pragma once

#include "ofMain.h"
#include "cg_extras.h"
#include "cg_drawing_extras.h"
#include "cg_cam_extras.h"
#include "pieceTetris.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void drawGame();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		int dif = 0;

		bool noFill;
		ofImage img1;
		ofImage img2;
		ofImage img3;
		ofImage img4;
		ofImage img5;
		ofImage img6;


		bool ambientOn;

		bool dirOn;
		bool dirAmbOn = true;
		bool dirDifOn = true;
		bool dirSpecOn = true;

		bool pointOn;
		bool pointAmbOn = true;
		bool pointDifOn = true;
		bool pointSpecOn = true;

		bool autoMove;

		bool focoOn;
		bool focoAmbOn = true;
		bool focoDifOn = true;
		bool focoSpecOn = true;

		GLfloat ambientLight[4];

		GLfloat dirVec[4];
		GLfloat dirAmb[4];
		GLfloat dirDif[4];
		GLfloat dirSpec[4];
		ofVec3f dirVec3f;
		GLfloat dirVecTheta;


		GLfloat pointPos[4];
		GLfloat pointAmb[4];
		GLfloat pointDif[4];
		GLfloat pointSpec[4];
		GLfloat pointZtheta;

		GLfloat focoPos[4];
		GLfloat focoAmb[4];
		GLfloat focoDif[4];
		GLfloat focoSpec[4];
		GLfloat spotDir[3];
		GLfloat spotExponent = 90.0f;
		GLfloat spotCutoff = 45.0f;

		int fps = 1;
		
		int m = 10;
		int n = 20;

		int random_number;
		int rand_object = 3;
		GLfloat rand_r;
		GLfloat rand_g;
		GLfloat rand_b;

		int view_mode = 1;
		float orbitAngle = 0;
		int raio = 0;

		int cleanup = 0;
		int clean_count = 0;
		int need_clean = 0;
		int clean_aux = 0;
		int clean_aux1 = 0;

		GLfloat lim_esq;
		GLfloat lim_dir;
		GLfloat lim_baixo;

		TetrisPiece *currentPiece;
		std::vector<TetrisPiece> pieces;

		std::vector<int> lines;

		int count = 0;
		int count_l = 0;

		int matrix[20][10] = {{0}};
		int matrix_ant[20][10] = {{0}};

		float red[20][10] = {{0}};
		float red_ant[20][10] = {{0}};
		float green[20][10] = {{0}};
		float green_ant[20][10] = {{0}};
		float blue[20][10] = {{0}};
		float blue_ant[20][10] = {{0}};

		int points = 0;
		ofTrueTypeFont scoreFont;

		int highScore = 0;       // Pontuação mais alta (high score)

		int finish = 0;
		int count_f = 0;

    	void saveHighScore(int score);
    	void loadHighScore();

		
};
