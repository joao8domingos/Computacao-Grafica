#include "ofApp.h"
#include <random>

//--------------------------------------------------------------
// Função para salvar o high score 
void ofApp::saveHighScore(int score) {
    // Verifica se o score atual é maior ou igual ao high score
    if (score >= highScore) {
        highScore = score; // Atualiza o high score

        // Salva o high score em "bin/data/high.txt" usando ofToDataPath
        std::ofstream outFile(ofToDataPath("high.txt"));
        if (outFile.is_open()) {
            outFile << highScore; // Escreve o high score no arquivo
            outFile.close();      
            std::cout << "High score salvo: " << highScore << std::endl;
        } else {
            std::cerr << "Erro ao abrir o arquivo para salvar o high score." << std::endl;
        }
    }
}

// Função para carregar o high score do arquivo
void ofApp::loadHighScore() {
    // Carrega o arquivo de "bin/data/high.txt"
    std::ifstream inFile(ofToDataPath("high.txt"));
    if (inFile.is_open()) {
        inFile >> highScore; // Lê o high score do arquivo
        inFile.close();
        std::cout << "High score carregado: " << highScore << std::endl;
    } else {
        std::cerr << "Arquivo de high score não encontrado, iniciando com high score zero." << std::endl;
        highScore = 0; // Define como zero se o arquivo não for encontrado
    }
}

void ofApp::setup() {

	// Inicializa o gerador de números aleatórios
    std::random_device rd;            // Semente
    std::mt19937 gen(rd());           
    // Distribuição para números inteiros entre 1 e 4
    std::uniform_int_distribution<int> distrib_int(1, 4);
    random_number = distrib_int(gen); // Gera um número aleatório entre 1 e 4


	currentPiece = new TetrisPiece(random_number);
	//pieces[0] = *currentPiece;
	pieces.push_back(*currentPiece);

	loadHighScore();
	scoreFont.load("verdana.ttf", 20);

	ambientOn = true;
	dirOn = false;
	pointOn = false;
	focoOn = false;
	autoMove = true;

	pointZtheta = 0;
	dirVecTheta = 0.;


	//ofSetVerticalSync(false);
	//glEnable(GL_DEPTH_TEST);
	//ofBackground(200, 200, 200);
	ofDisableArbTex();//para utilizar coordenadas de textura normalizadas

	noFill = false;
	img1.load("stone.jpg");
	img2.load("sky.JPG");
	img4.load("sky.JPG");
	img5.load("sky.JPG");
	img6.load("sky.JPG");
	img3.load("grass.jpg");
}

//--------------------------------------------------------------
void ofApp::update() {
	if (finish == 1){
		return;
	}
	orbitAngle += 0.005;

	
	if (autoMove) {

		//calculo da posicao da luz pontual
		float x = 10 * cos(orbitAngle);
		float y = 0;
		float z = 10 * sin(orbitAngle);
		
		pointPos[0] = x;
		pointPos[1] = y;
		pointPos[2] = z;

		
		dirVec3f = ofVec3f(x, y, z) - ofVec3f(0, 0, 0);
		dirVecTheta += 1.;


	}
	else {
		//luz pontual
		//centro acima
		pointPos[0] = 0;
		pointPos[1] = 0;
		pointPos[2] = 90;


		//luz direcional
		dirVec3f = ofVec3f(0, gh()*0.5, gh()*0.25*0.95) - ofVec3f(0, 0, 0);

	}


	if (need_clean == 1){
		if (clean_aux > lines.size() * 10){
			//usleep(500000);
			//protecao = 1;
			lines.clear();
			std::cout << "Lines 1: ";
    		for (int line : lines) {  // Itera por cada elemento no vetor
        		std::cout << line << " ";  // Imprime cada elemento seguido de um espaço
    		}
    		std::cout << std::endl;

			need_clean = 0;
			clean_aux = 0;
			usleep(50000);
			return;
		}
		clean_aux++;
		usleep(50000);
		return;
	}
	else {

	if (pieces[count].stop) {
		count++;
		points += 4;
		random_number = 1 + rand() % 4;
		currentPiece = new TetrisPiece(random_number);
		pieces.push_back(*currentPiece);
	}
	
	for (int j = 0; j < n; j++) {
        count_l = 0;

        // Conta o número de células ocupadas na linha `j`
        for (int i = 0; i < m; i++) {
            if (matrix[j][i] == 1) {
                count_l++;
            }
        }

        // Se a linha estiver cheia, limpa-a e "deixa cair" as linhas de cima
        if (count_l == m) {
			lines.push_back(j);
		}
	}
	
	//tam_lines = lines.size();

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			matrix_ant[i][j] = matrix[i][j];
			red_ant[i][j] = red[i][j];
			green_ant[i][j] = green[i][j];
			blue_ant[i][j] = blue[i][j];
		}
	}

	cleanup = currentPiece->clean_line(n, m, matrix, red, green, blue);
	points += cleanup * 10;

	if (cleanup > 0) {
		std::cout << "CLEANUP need_clean: " << cleanup << std::endl;
		need_clean = 1;
		clean_count++;
	}

	// Atualiza a peça atual
    //currentPiece->update();
	if (clean_count == 0) {
		for (int i = 0; i < pieces.size(); i++) {
			pieces[i].matrix_update(n, m, matrix, red, green, blue);
			pieces[i].points_now(points, dif);
			pieces[i].update();
		}
	}
	else {
		for (int i = 0; i < pieces.size(); i++) {
			pieces[i].points_now(points, dif);
			pieces[i].update();
		}
		pieces[pieces.size() - 1].matrix_update(n, m, matrix, red, green, blue);
	}
	
	if (matrix[n - 1][int(m/2)] == 1 || matrix[n - 1][int(m/2) - 1] == 1 || matrix[n - 1][int(m/2) + 1] == 1) {
		saveHighScore(points);
		finish = 1;
		
		//points = 0;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				matrix[i][j] = 0;
				red[i][j] = 0;
				green[i][j] = 0;
				blue[i][j] = 0;
			}
		}
		pieces.clear();
		lines.clear();
		count = 0;
		clean_count = 0;
		orbitAngle = 0;
		raio = 0;

		cleanup = 0;
		need_clean = 0;
		clean_aux = 0;
		clean_aux1 = 0;

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				matrix[i][j] = 0;
				red[i][j] = 0;
				red_ant[i][j] = 0;
				green[i][j] = 0;
				green_ant[i][j] = 0;
				blue[i][j] = 0;
				blue_ant[i][j] = 0;
			}
		}

		highScore = 0;
		count_f = 0;

	}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	if (noFill) {
		wireframe();
	}
	else {
		filled();
	}

	switch (view_mode) {
		case 1:
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-20, 20, -20, 20, -50, 50);
			lookat(0, 0, 1, 0, 0, 0, 0, 1, 0);
			break;
		case 2:
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			perspective(60, 100, 10000);
			lookat(-15, 25, 20, 0, 0, 0, 0, 1, 0);
			break;
		case 3:
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			perspective(60, 100, 10000);
			lookat(0, 30, 20, 0, 0, 0, 0, 1, 0);
			break;
		case 4:
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			perspective(60, 100, 10000);
			lookat(30 * cos(orbitAngle), 0, 30 * sin(orbitAngle), 0, 0, 0, 0, 1, 0);
			break;
		case 5:
			glMatrixMode(GL_PROJECTION);
    		glLoadIdentity();
    		perspective(60, 100, 10000); // Configura a projeção em perspectiva
    		lookat(0, 15, 0, 0, 0, 0, 0, 0, -1);// Ponto para onde a câmera está olhando (centro do jogo)
			break;
	}	

	//define luzes
	glEnable(GL_LIGHTING);//habilita o uso de ilumina��o
	glEnable(GL_NORMALIZE);//utiliza versores para normais (normais normalizadas)
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);//considera vis�o local

	//glDisable(GL_COLOR_MATERIAL);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
	//acho que em Mac tem que usar este também
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	
	if (ambientOn) {
		ambientLight[0] = .5;//R
		ambientLight[1] = .5;//G
		ambientLight[2] = .5;//B
		ambientLight[3] = 1;//useless
	}
	else {
		ambientLight[0] = 0.15;
		ambientLight[1] = 0.15;
		ambientLight[2] = 0.15;
		ambientLight[3] = 1;
	}
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	

	dirVec[0] = 0;//x
	dirVec[1] = gh()*0.5;//y
	dirVec[2] = gh()*0.25*0.95;//z

	if (dirAmbOn) {
		dirAmb[0] = .1;//R
		dirAmb[1] = .1;//G
		dirAmb[2] = .1;//B
		dirAmb[3] = 1.;//constante
	}
	else {
		dirAmb[0] = 0;
		dirAmb[1] = 0;
		dirAmb[2] = 0;
		dirAmb[3] = 1;
	}

	if (dirDifOn) {
		dirDif[0] = .4;//R
		dirDif[1] = .4;//G
		dirDif[2] = .4;//B
		dirDif[3] = 1.;//constante
	}
	else {
		dirDif[0] = 0;
		dirDif[1] = 0;
		dirDif[2] = 0;
		dirDif[3] = 1;
	}
	
	if (dirSpecOn) {
		dirSpec[0] = .2;//R
		dirSpec[1] = .2;//G
		dirSpec[2] = .2;//B
		dirSpec[3] = 1;//constante
	}
	else {
		dirSpec[0] = 0;
		dirSpec[1] = 0;
		dirSpec[2] = 0;
		dirSpec[3] = 1;
	}
	

	glLightfv(GL_LIGHT0, GL_POSITION, dirVec);
	glLightfv(GL_LIGHT0, GL_AMBIENT, dirAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dirDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, dirSpec);
	if (dirOn) {
		glEnable(GL_LIGHT0);
	}
	else {
		glDisable(GL_LIGHT0);
	}
	
	//luz pontual
	pointPos[3] = 1;//ponto - posi��o!

	if (pointAmbOn) {
		pointAmb[0] = .35;//R
		pointAmb[1] = .35;//G
		pointAmb[2] = .35;//B
		pointAmb[3] = 1.;//constante
	}
	else {
		pointAmb[0] = 0;
		pointAmb[1] = 0;
		pointAmb[2] = 0;
		pointAmb[3] = 1;
	}
	
	if (pointDifOn) {
		pointDif[0] = 0.5;//R
		pointDif[1] = 0.5;//G
		pointDif[2] = 0.5;//B
		pointDif[3] = 1.;//constante
	}
	else {
		pointDif[0] = 0;
		pointDif[1] = 0;
		pointDif[2] = 0;
		pointDif[3] = 1;
	}

	if (pointSpecOn) {
		pointSpec[0] = 0.2;//R
		pointSpec[1] = 0.2;//G
		pointSpec[2] = 0.2;//B
		pointSpec[3] = 1.;//constante
	}
	else {
		pointSpec[0] = 0;
		pointSpec[1] = 0;
		pointSpec[2] = 0;
		pointSpec[3] = 1;
	}
	

	glLightfv(GL_LIGHT1, GL_POSITION, pointPos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, pointAmb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, pointDif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, pointSpec);


	
	if (pointOn) {
		glEnable(GL_LIGHT1);
	}
	else {
		glDisable(GL_LIGHT1);
	}

	//luz foco
	focoPos[0] = 0;//x
	focoPos[1] = 0;//y
	focoPos[2] = 30;//z
	focoPos[3] = 1;

	if (focoAmbOn) {
		focoAmb[0] = .75;//R
		focoAmb[1] = .75;//G
		focoAmb[2] = .75;//B
		focoAmb[3] = 1.;//constante
	}
	else {
		focoAmb[0] = 0;
		focoAmb[1] = 0;
		focoAmb[2] = 0;
		focoAmb[3] = 1;
	}
	
	if (focoDifOn) {
		focoDif[0] = .5;//R
		focoDif[1] = .5;//G
		focoDif[2] = .5;//B
		focoDif[3] = 1.;//constante
	}
	else {
		focoDif[0] = 0;
		focoDif[1] = 0;
		focoDif[2] = 0;
		focoDif[3] = 1;
	}

	if (focoSpecOn) {
		focoSpec[0] = .35;//R
		focoSpec[1] = .35;//G
		focoSpec[2] = .35;//B
		focoSpec[3] = 1.;//constante
	}
	else {
		focoSpec[0] = 0;
		focoSpec[1] = 0;
		focoSpec[2] = 0;
		focoSpec[3] = 1;
	}
	
	spotDir[0] = 0;
	spotDir[1] = 0;
	spotDir[2] = -1;
	

	if (focoOn) {
		glEnable(GL_LIGHT2);
	}
	else {
		glDisable(GL_LIGHT2);
	}

	glLightfv(GL_LIGHT2, GL_POSITION, focoPos);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDir);

	glLightfv(GL_LIGHT2, GL_AMBIENT, focoAmb);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, focoDif);
	glLightfv(GL_LIGHT2, GL_SPECULAR, focoSpec);

	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, spotCutoff);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, spotExponent);


	
	drawGame();

	if (view_mode == 2 || view_mode == 3 || view_mode == 4 || view_mode == 5) {
		glViewport(gw() * 0.65, gh() * 0.65, gw() * 0.35, gh() * 0.35);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-20, 20, -20, 20, -50, 50);
		lookat(0, 0, 1, 0, 0, 0, 0, 1, 0);
		drawGame();
	}

	
	
}

void ofApp::drawGame() {
	
	glEnable(GL_TEXTURE);
	

	img2.bind();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glDisable(GL_COLOR_MATERIAL);
    // Configura o material para os cubos
    GLfloat mat_ambient[] = { 0.6f, 0.4f, 0.4f, 1.0f };
    GLfloat mat_diffuse[] = { 0.8f, 0.0f, 0.0f, 1.0f }; // Cor vermelha
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[] = { 50.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glPushMatrix();
	glTranslatef(0, 0, -45);
	glScalef(gw() * 0.75, gh() * 0.75, 1.);

	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);//direcao da normal

	glTexCoord2f(0, 0);//coordenada de textura
	glVertex3f(-0.5, 0.5, -1);//vertex(x, y, z)

	glTexCoord2f(0, 1);//coordenada de textura
	glVertex3f(-0.5, -0.5, -1);//vertex(x, y, z)

	glTexCoord2f(1, 1);//coordenada de textura
	glVertex3f(0.5, -0.5, -1);//vertex(x, y, z)

	glTexCoord2f(1, 0);//coordenada de textura
	glVertex3f(0.5, 0.5, -1);//vertex(x, y, z)

	glEnd();
	glPopMatrix();

	glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	img2.unbind();
	glDisable(GL_TEXTURE);


	glEnable(GL_TEXTURE_2D);

	img3.bind();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glDisable(GL_COLOR_MATERIAL);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glPushMatrix();
	glTranslatef(0, 0, -1);
	glRotatef(90, 1, 0, 0);
	glScalef(gw() * 0.75, gh() * 0.75, 1.);

	// Face perpendicular ao eixo Y
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);

	// Ajuste as coordenadas de textura para repetir a imagem
	float repeat = 20.0f; // Número de repetições da textura

	glTexCoord2f(0, 0); glVertex3f(-0.5, 0.5, 0);
	glTexCoord2f(0, repeat); glVertex3f(-0.5, -0.5, 0);
	glTexCoord2f(repeat, repeat); glVertex3f(0.5, -0.5, 0);
	glTexCoord2f(repeat, 0); glVertex3f(0.5, 0.5, 0);
	glEnd();

	glPopMatrix();

	glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	img3.unbind();
	glDisable(GL_TEXTURE_2D);

	
	glEnable(GL_TEXTURE);

	img4.bind();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glDisable(GL_COLOR_MATERIAL);
    

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glPushMatrix();
	glTranslatef(1150, 0, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(gw() * 0.75, gh() * 1.8, 1.);

	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);//direcao da normal

	glTexCoord2f(0, 0);//coordenada de textura
	glVertex3f(-0.5, 0.5, -1);//vertex(x, y, z)

	glTexCoord2f(0, 1);//coordenada de textura
	glVertex3f(-0.5, -0.5, -1);//vertex(x, y, z)

	glTexCoord2f(1, 1);//coordenada de textura
	glVertex3f(0.5, -0.5, -1);//vertex(x, y, z)

	glTexCoord2f(1, 0);//coordenada de textura
	glVertex3f(0.5, 0.5, -1);//vertex(x, y, z)

	glEnd();
	glPopMatrix();

	glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	img4.unbind();
	glDisable(GL_TEXTURE);

	
	glEnable(GL_TEXTURE);

	img5.bind();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glDisable(GL_COLOR_MATERIAL);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glPushMatrix();
	glTranslatef(-550, 0, 0);
	glRotatef(90, 0, 1, 0);
	//glScalef(gw() * 0.75, gh() * 0.75, 1.);
	glScalef(gw(), 1.2 * gh(), 1.);

	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);//direcao da normal

	glTexCoord2f(0, 0);//coordenada de textura
	glVertex3f(-0.5, 0.5, -1);//vertex(x, y, z)

	glTexCoord2f(0, 1);//coordenada de textura
	glVertex3f(-0.5, -0.5, -1);//vertex(x, y, z)

	glTexCoord2f(1, 1);//coordenada de textura
	glVertex3f(0.5, -0.5, -1);//vertex(x, y, z)

	glTexCoord2f(1, 0);//coordenada de textura
	glVertex3f(0.5, 0.5, -1);//vertex(x, y, z)

	glEnd();
	glPopMatrix();

	glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	img5.unbind();
	glDisable(GL_TEXTURE);
	

	glEnable(GL_TEXTURE);

	img6.bind();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glDisable(GL_COLOR_MATERIAL);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glPushMatrix();
	glTranslatef(0, 0, 85);
	glScalef(gw() * 0.75, gh() * 0.75, 1.);

	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);//direcao da normal

	glTexCoord2f(0, 0);//coordenada de textura
	glVertex3f(-0.5, 0.5, -1);//vertex(x, y, z)

	glTexCoord2f(0, 1);//coordenada de textura
	glVertex3f(-0.5, -0.5, -1);//vertex(x, y, z)

	glTexCoord2f(1, 1);//coordenada de textura
	glVertex3f(0.5, -0.5, -1);//vertex(x, y, z)

	glTexCoord2f(1, 0);//coordenada de textura
	glVertex3f(0.5, 0.5, -1);//vertex(x, y, z)

	glEnd();
	glPopMatrix();

	glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	img6.unbind();
	glDisable(GL_TEXTURE);
	

	/*
	glPushMatrix();
	glTranslatef(10 * cos(orbitAngle), 0, 10 * sin(orbitAngle));
	cube_unit();
	glPopMatrix();
	*/


	//Desenhar malha branca
	//glColor3f(0,0,0);	
	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	malha_unit1(m, n);
	glPopMatrix();

	if (need_clean == 1) {
		pieces[pieces.size() - 1].draw_lines(n, m, matrix_ant, red_ant, green_ant, blue_ant);

		for (int i = 0; i < lines.size(); i++) {
			for (int j = 0; j < m; j++) {
				if (i * 10 + j >= clean_aux){
					break;
				}
					
				glPushMatrix();
				
				glTranslated(j - m / 2 + 0.5, lines[i] - n/2 + 0.5, 0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Restaura o modo de desenho preenchido
            		// Desenha o cubo preenchido
            	//cube_unit();
				crackedCube(img1);

            		// Desenha o contorno do cubo em preto
            	//glColor3f(0, 0, 0);
            	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            	//cube_unit();
				crackedCube(img1);
				glPopMatrix();	

			}
		}

		ofSetColor(255); // Define a cor como vermelha para garantir contraste
    	ofDrawBitmapString("SCORE: " + std::to_string(points), -1, 16); // Desenha o texto na tela

		if (highScore >= points){
			ofDrawBitmapString("HIGH SCORE: " + std::to_string(highScore), -2, 14);
		}
		else {
			ofDrawBitmapString("HIGH SCORE: " + std::to_string(points), -2, 14);
		}

		glPushMatrix();
		ofSetColor(0, 0, 0);
		glScalef(0.05, -0.05, 0.1);
		scoreFont.drawString("Cracked", -50, 230);
		ofSetColor(255);
		glPopMatrix();
	}
	else if (finish == 1){
		if (count_f == m*n){

			glDisable(GL_COLOR_MATERIAL);

			glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

			for (int i = 0; i < n; i++) {
				for (int j = 0; j < m; j++) {
					
					glPushMatrix();
					//glColor3f(0.8, 0.8, 0.8);
					glTranslated(j - m / 2 + 0.5, i - n/2 + 0.5, 0);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Restaura o modo de desenho preenchido
            		// Desenha o cubo preenchido
            		cube_unit();

            		// Desenha o contorno do cubo em preto
            		//glColor3f(0, 0, 0);
            		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            		cube_unit();
					glPopMatrix();	
				}
			}

			glEnable(GL_COLOR_MATERIAL);
    		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

			glPushMatrix();
			ofSetColor(50, 50, 50);
			glScalef(0.1, -0.1, 0.1);
			scoreFont.drawString("GAME OVER", -80, 5);
			ofSetColor(255);
			glPopMatrix();

			ofSetColor(0, 0, 255); // Define a cor como vermelha para garantir contraste
    		ofDrawBitmapString("(PRESS SPACE TO PLAY AGAIN)  SCORE: " + std::to_string(points), -5.90, -2);

			return;
		}
		else {
			glDisable(GL_COLOR_MATERIAL);

    		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

			for (int i = 0; i < n; i++) {
				for (int j = 0; j < m; j++) {
					if (i*m + j >= count_f){
						break;
					}
					glPushMatrix();
					//glColor3f(0.8, 0.8, 0.8);
					glTranslated(j - m / 2  + 0.5, i - n/2 + 0.5, 0);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Restaura o modo de desenho preenchido
            			// Desenha o cubo preenchido
            		cube_unit();

            			// Desenha o contorno do cubo em preto
            		//glColor3f(0, 0, 0);
            		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            		cube_unit();
					glPopMatrix();	
					}
				}
			glEnable(GL_COLOR_MATERIAL);
    		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
			
		count_f++;
		}
		//count_pontos = 0;
	}
	else {
		pieces[pieces.size() - 1].draw_lines(n, m, matrix, red, green, blue);
		pieces[pieces.size() - 1].draw();

		ofSetColor(255); // Define a cor como vermelha para garantir contraste
    	ofDrawBitmapString("SCORE: " + std::to_string(points), -1, 16); // Desenha o texto na tela

		if (highScore >= points){
			ofDrawBitmapString("HIGH SCORE: " + std::to_string(highScore), -2, 14);
		}
		else {
			ofDrawBitmapString("HIGH SCORE: " + std::to_string(points), -2, 14);
		}
		//count_pontos = 0;
	}
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	switch (key) {
		case '1':
			view_mode = 1;
			break;
		case '2':
			view_mode = 2;
			break;
		case '3':
			view_mode = 3;
			break;
		case '4':
			view_mode = 4;
			break;
		case '5':
			view_mode = 5;
			break;
		case OF_KEY_LEFT:
			pieces[count].moveLeft(n, m, matrix);
            break;

        case OF_KEY_RIGHT:
			pieces[count].moveRight(n, m, matrix);
            break;
        case OF_KEY_DOWN:
			if (finish != 1) {
				points += 1;
				pieces[count].moveDown(n, m, matrix, red, green, blue);
			}
			break;
		case ' ':
			points = 0;
			dif = 0;
			pieces.clear();
			lines.clear();
			count = 0;
			clean_count = 0;
			orbitAngle = 0;
			raio = 0;

			cleanup = 0;
			need_clean = 0;
			clean_aux = 0;
			clean_aux1 = 0;

			for (int i = 0; i < n; i++) {
				for (int j = 0; j < m; j++) {
					matrix[i][j] = 0;
				}
			}

			count_f = 0;
			finish = 0;
			currentPiece = new TetrisPiece(random_number);
	
			pieces.push_back(*currentPiece);

			loadHighScore();
			scoreFont.load("verdana.ttf", 20);
			break;
		case 'r':
			/*
			rotator_angle -= 90;
			*/
			pieces[count].rotate(n, m, matrix);
			break;
		case '6':
			glDisable(GL_CULL_FACE);
			break;
		case '7':
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			break;
		case '8':
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			break;
		case '9':
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT_AND_BACK);
			break;
		case 'g':
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case 'f':
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case 'h':
			ambientOn = !ambientOn;
			break;
		case 'j':
			dirOn = !dirOn;
			break;
		case 'p':
			pointOn = !pointOn;
			break;
		case 'm':
			glShadeModel(GL_FLAT);
			break;
		case 'n':
			glShadeModel(GL_SMOOTH);
			break;
		case 'q':
			autoMove = !autoMove;
			break;
		case 'w':
			focoOn = !focoOn;
			break;
		case 'e':
			if (dirOn)
				dirAmbOn = !dirAmbOn;
			break;
		case 't':
			if (dirOn)
				dirDifOn = !dirDifOn;
			break;
		case 'y':
			if (dirOn)
				dirSpecOn = !dirSpecOn;
			break;
		case 'u':
			if (pointOn)
				pointAmbOn = !pointAmbOn;
			break;
		case 'i':
			if (pointOn)
				pointDifOn = !pointDifOn;
			break;
		case 'o':
			if (pointOn)
				pointSpecOn = !pointSpecOn;
			break;
		case 'a':
			if (focoOn)
				focoAmbOn = !focoAmbOn;
			break;
		case 's':
			if (focoOn)
				focoDifOn = !focoDifOn;
			break;
		case 'd':
			if (focoOn)
				focoSpecOn = !focoSpecOn;
			break;
		case 'z':
			if (dif == 0 && points < 1500) {
				dif = 1;
			}
			else if (dif == 1) {
				dif = 0;
			}
			break;
	}
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	cout << endl << x << " " << y;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	setup();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
