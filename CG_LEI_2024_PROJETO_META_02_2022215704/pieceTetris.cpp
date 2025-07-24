// pieceTetris.cpp
#include "pieceTetris.h"
#include "cg_drawing_extras.h"
#include <random>
#include "ofApp.h"
#include <cmath>

TetrisPiece::TetrisPiece(int type) : rotator_angle(0), type(type) {
    pos.z = 0;
    if (type == 3) {
        pos.x = 0;
        pos.y = n/2 - 1;
        pos1 = ofVec3f(pos.x + 0.5, pos.y +0.5, pos.z);
        pos2 = ofVec3f(pos.x + 0.5, pos.y - 0.5, pos.z);
        pos3 = ofVec3f(pos.x - 0.5, pos.y - 0.5, pos.z);
        pos4 = ofVec3f(pos.x - 0.5, pos.y + 0.5, pos.z);
    } else if (type == 2) {
        pos.x = -0.5;
        pos.y = n/2 - 0.5;
        pos1 = ofVec3f(pos.x - 1, pos.y, pos.z);
        pos2 = ofVec3f(pos.x, pos.y - 1, pos.z);
        pos3 = ofVec3f(pos.x + 1, pos.y, pos.z);
    }
    else if (type == 4) {
        pos.x = -0.5;
        pos.y = n/2 - 0.5;
        pos1 = ofVec3f(pos.x - 1, pos.y, pos.z);
        pos2 = ofVec3f(pos.x + 1, pos.y, pos.z);
        pos3 = ofVec3f(pos.x + 2, pos.y, pos.z);
    }
    else {
        pos.x = -0.5;
        pos.y = n/2 - 0.5;
        pos1 = ofVec3f(pos.x + 1, pos.y - 1, pos.z);
        pos2 = ofVec3f(pos.x + 1, pos.y, pos.z);
        pos3 = ofVec3f(pos.x - 1, pos.y, pos.z);
    }

    generateRandomColor(); // Gera uma cor aleatória
}

void TetrisPiece::generateRandomColor() {
    std::random_device rd;            // Semente
    std::mt19937 gen(rd());           // Mersenne Twister como gerador
    std::uniform_real_distribution<> distrib_real(0.0, 1.0);
    
    color[0] = distrib_real(gen);
    color[1] = distrib_real(gen);
    color[2] = distrib_real(gen);

}

void TetrisPiece::points_now(int points, int dif) {
    if (dif == 0) {
        if (points > 1500) {
            hard = 30;
        }
        else if (points > 1000) {
            hard = 50;
        }
        else if (points > 500) {
            hard = 75;
        }
        else {
            hard = 100;
        }
        hard_new = 0;
    }
    else if (dif == 1) {
        hard_new = 30;
    }
    
}

void TetrisPiece::update() {
    if (stop) {
        return;
    }
    
    if (hard_new != 0 && pos.y >= n/2 - 1) {
        hard = hard_new;
    }

    if (fps == hard) {
        pos.y -= 1;
        pos1.y -= 1;
        pos2.y -= 1;
        pos3.y -= 1;

        if (type == 3) {
            pos4.y -= 1;
        }

        fps = 0;
        updateExtremes();
        if (leftmost_x < -m / 2) {
            stop = true;
            std::cout << "STOP UPDATE 1" << std::endl;
        }
        if (rightmost_x > m / 2) {
            stop = true;
            std::cout << "STOP UPDATE 2" << std::endl;
        }
        if (lowest_y <= -n / 2 + 1) {
            stop = true;
            std::cout << "STOP UPDATE 3" << std::endl;
        }

    }
    fps++;
}

void TetrisPiece::draw() {
    //glColor3f(color[0], color[1], color[2]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPushMatrix();
    glTranslated(pos.x, pos.y, pos.z);
    glRotated(rotator_angle, 0, 0, 1);

    switch (type) {
        case 1:
            drawL();
            break;
        case 2:
            drawT();
            break;
        case 3:
            drawCube();
            break;
        case 4:
            drawLine();
            break;
        default:
            break;
    }

    glPopMatrix();
}

void TetrisPiece::matrix_update(int n, int m, int (*matrix)[10], float (*red)[10], float (*green)[10], float (*blue)[10]) {
    int offsetX = m / 2 - 1; // Offset para alinhar o centro do grid
    int offsetY = n / 2;

    
    // Verifica se a peça precisa parar (colisão com outra peça ou limite da matriz)
    // Atualiza a matriz apenas se a peça tiver parado
    if (stop) {
        // Atualiza a posição da peça principal e das sub-posições
        if (type == 1 || type == 2 || type == 4) {
            matrix[(int)(pos.y + offsetY - 0.5)][(int)(pos.x + offsetX + 0.5)] = 1;

            matrix[(int)(pos1.y + offsetY - 0.5)][(int)(pos1.x + offsetX + 0.5)] = 1;

            matrix[(int)(pos2.y + offsetY - 0.5)][(int)(pos2.x + offsetX + 0.5)] = 1;
            

            matrix[(int)(pos3.y + offsetY - 0.5)][(int)(pos3.x + offsetX + 0.5)] = 1;
        } else if (type == 3) { // Peça Cube

            matrix[(int)(pos1.y + offsetY - 0.5)][(int)(pos1.x + offsetX + 0.5)] = 1;

            matrix[(int)(pos2.y + offsetY - 0.5)][(int)(pos2.x + offsetX + 0.5)] = 1;

            matrix[(int)(pos3.y + offsetY - 0.5)][(int)(pos3.x + offsetX + 0.5)] = 1;

            matrix[(int)(pos4.y + offsetY - 0.5)][(int)(pos4.x + offsetX + 0.5)] = 1;
    
        }
        return;
    }

    if (type == 1 || type == 2 || type == 4) {
        if (pos.y + offsetY - 1.5 < 0 && pos1.y + offsetY - 1.5 < 0 && pos2.y + offsetY - 1.5 < 0 && pos3.y + offsetY - 1.5 < 0) {
            stop = true;
            matrix_update(n, m, matrix, red, green, blue);
            std::cout << "STOP MATRIX 1" << std::endl;
        }
        else {
            if (matrix[(int)(pos.y + offsetY - 1.5)][(int)(pos.x + offsetX + 0.5)] == 1 ||
                matrix[(int)(pos1.y + offsetY - 1.5)][(int)(pos1.x + offsetX + 0.5)] == 1 ||
                matrix[(int)(pos2.y + offsetY - 1.5)][(int)(pos2.x + offsetX + 0.5)] == 1 ||
                matrix[(int)(pos3.y + offsetY - 1.5)][(int)(pos3.x + offsetX + 0.5)] == 1) {
                stop = true;
                matrix_update(n, m, matrix, red, green, blue);
                std::cout << "STOP MATRIX 2" << std::endl;
            }
        }
    } else if (type == 3) { // Peça Cube
        if (matrix[(int)(pos1.y + offsetY - 1.5)][(int)(pos1.x + offsetX + 0.5)] == 1 ||
            matrix[(int)(pos2.y + offsetY - 1.5)][(int)(pos2.x + offsetX + 0.5)] == 1 ||
            matrix[(int)(pos3.y + offsetY - 1.5)][(int)(pos3.x + offsetX + 0.5)] == 1 ||
            matrix[(int)(pos4.y + offsetY - 1.5)][(int)(pos4.x + offsetX + 0.5)] == 1) {
            stop = true;
            matrix_update(n, m, matrix, red, green, blue);
            std::cout << "STOP MATRIX 3" << std::endl;
        }
    }
}





void TetrisPiece::updateExtremes() {
    switch (type) {
        case 1: // Peça L
            if (type_rotate == 0) {
                leftmost_x = pos.x - 1;
                rightmost_x = pos.x + 1;
                lowest_y = pos.y - 1;
                pos1 = ofVec3f(pos.x + 1, pos.y - 1, pos.z);
                pos2 = ofVec3f(pos.x + 1, pos.y, pos.z);
                pos3 = ofVec3f(pos.x - 1, pos.y, pos.z);
            }
            else if (type_rotate == 1) {
                leftmost_x = pos.x - 1;
                rightmost_x = pos.x;
                lowest_y = pos.y - 1;
                pos1 = ofVec3f(pos.x - 1, pos.y - 1, pos.z);
                pos2 = ofVec3f(pos.x, pos.y - 1, pos.z);
                pos3 = ofVec3f(pos.x, pos.y + 1, pos.z);
            }
            else if (type_rotate == 2) {
                leftmost_x = pos.x - 1;
                rightmost_x = pos.x + 1;
                lowest_y = pos.y;
                pos1 = ofVec3f(pos.x - 1, pos.y + 1, pos.z);
                pos2 = ofVec3f(pos.x - 1, pos.y, pos.z);
                pos3 = ofVec3f(pos.x + 1, pos.y, pos.z);
            }
            else if (type_rotate == 3) {
                leftmost_x = pos.x;
                rightmost_x = pos.x + 1;
                lowest_y = pos.y - 1;
                pos1 = ofVec3f(pos.x + 1, pos.y + 1, pos.z);
                pos2 = ofVec3f(pos.x, pos.y + 1, pos.z);
                pos3 = ofVec3f(pos.x, pos.y - 1, pos.z);
            }
            break;
        case 2: // Peça T
            if (type_rotate == 0) {
                leftmost_x = pos.x - 1;
                rightmost_x = pos.x + 1;
                lowest_y = pos.y - 1;
                pos1 = ofVec3f(pos.x - 1, pos.y, pos.z);
                pos2 = ofVec3f(pos.x, pos.y - 1, pos.z);
                pos3 = ofVec3f(pos.x + 1, pos.y, pos.z);
            }
            else if (type_rotate == 1) {
                leftmost_x = pos.x - 1;
                rightmost_x = pos.x;
                lowest_y = pos.y - 1;
                pos1 = ofVec3f(pos.x, pos.y + 1, pos.z);
                pos2 = ofVec3f(pos.x - 1, pos.y, pos.z);
                pos3 = ofVec3f(pos.x, pos.y - 1, pos.z);
            }
            else if (type_rotate == 2) {
                leftmost_x = pos.x - 1;
                rightmost_x = pos.x + 1;
                lowest_y = pos.y;
                pos1 = ofVec3f(pos.x + 1, pos.y, pos.z);
                pos2 = ofVec3f(pos.x, pos.y + 1, pos.z);
                pos3 = ofVec3f(pos.x - 1, pos.y, pos.z);
            }
            else if (type_rotate == 3) {
                leftmost_x = pos.x;
                rightmost_x = pos.x + 1;
                lowest_y = pos.y - 1;
                pos1 = ofVec3f(pos.x, pos.y - 1, pos.z);
                pos2 = ofVec3f(pos.x + 1, pos.y, pos.z);
                pos3 = ofVec3f(pos.x, pos.y + 1, pos.z);
            }
            break;
        case 3: // Peça Cube
            leftmost_x = pos.x - 0.5;
            rightmost_x = pos.x + 0.5;
            lowest_y = pos.y - 0.5;
            pos1 = ofVec3f(pos.x + 0.5, pos.y +0.5, pos.z);
            pos2 = ofVec3f(pos.x + 0.5, pos.y - 0.5, pos.z);
            pos3 = ofVec3f(pos.x - 0.5, pos.y - 0.5, pos.z);
            pos4 = ofVec3f(pos.x - 0.5, pos.y + 0.5, pos.z);
            break;
        case 4:
            if (type_rotate == 0) {
                leftmost_x = pos.x - 1;
                rightmost_x = pos.x + 2;
                lowest_y = pos.y;
                pos1 = ofVec3f(pos.x - 1, pos.y, pos.z);
                pos2 = ofVec3f(pos.x + 1, pos.y, pos.z);
                pos3 = ofVec3f(pos.x + 2, pos.y, pos.z);
            }
            else if (type_rotate == 1) {
                leftmost_x = pos.x;
                rightmost_x = pos.x;
                lowest_y = pos.y - 2;
                pos1 = ofVec3f(pos.x, pos.y + 1, pos.z);
                pos2 = ofVec3f(pos.x, pos.y - 1, pos.z);
                pos3 = ofVec3f(pos.x, pos.y - 2, pos.z);
            }
            else if (type_rotate == 2) {
                leftmost_x = pos.x - 2;
                rightmost_x = pos.x + 1;
                lowest_y = pos.y;
                pos1 = ofVec3f(pos.x + 1, pos.y, pos.z);
                pos2 = ofVec3f(pos.x - 1, pos.y, pos.z);
                pos3 = ofVec3f(pos.x - 2, pos.y, pos.z);
            }
            else if (type_rotate == 3) {
                leftmost_x = pos.x;
                rightmost_x = pos.x;
                lowest_y = pos.y - 1;
                pos1 = ofVec3f(pos.x, pos.y - 1, pos.z);
                pos2 = ofVec3f(pos.x, pos.y + 1, pos.z);
                pos3 = ofVec3f(pos.x, pos.y + 2, pos.z);
            }
            break;
        default:
            break;
    }
}

// Função para limpar linhas completas e "deixar cair" as linhas de cima
int TetrisPiece::clean_line(int n, int m, int (*matrix)[10], float (*red)[10], float (*green)[10], float (*blue)[10]) {
    int lineCleared = 0;

    for (int j = 0; j < n; j++) {
        int count = 0;

        // Conta o número de células ocupadas na linha `j`
        for (int i = 0; i < m; i++) {
            if (matrix[j][i] == 1) {
                count++;
            }
        }

        // Se a linha estiver cheia, limpa-a e "deixa cair" as linhas de cima
        if (count == m) {
            lineCleared += 1;
            
            // Mover todas as linhas acima de `j` uma posição para baixo
            for (int k = j; k < n - 1; k++) {
                for (int i = 0; i < m; i++) {
                    matrix[k][i] = matrix[k + 1][i];
                    //matrix[k][i] = 0;
                }
            }
        }
    }
    
    
    for (int j = 0; j < n; j++) {
        int count = 0;

        // Conta o número de células ocupadas na linha `j`
        for (int i = 0; i < m; i++) {
            if (matrix[j][i] == 1) {
                count++;
            }
        }

        // Se a linha estiver cheia, limpa-a e "deixa cair" as linhas de cima
        if (count == m) {
            //lineCleared += 1;
            
            // Mover todas as linhas acima de `j` uma posição para baixo
            for (int k = j; k < n - 1; k++) {
                for (int i = 0; i < m; i++) {
                    matrix[k][i] = matrix[k + 1][i];
                    //matrix[k][i] = 0;
                }
            }
        }
    }

    for (int j = 0; j < n; j++) {
        int count = 0;

        // Conta o número de células ocupadas na linha `j`
        for (int i = 0; i < m; i++) {
            if (matrix[j][i] == 1) {
                count++;
            }
        }

        // Se a linha estiver cheia, limpa-a e "deixa cair" as linhas de cima
        if (count == m) {
            //lineCleared += 1;
            
            // Mover todas as linhas acima de `j` uma posição para baixo
            for (int k = j; k < n - 1; k++) {
                for (int i = 0; i < m; i++) {
                    matrix[k][i] = matrix[k + 1][i];
                    //matrix[k][i] = 0;
                }
            }
        }
    }
    
    return lineCleared; // Retorna 1 se uma linha foi limpa, senão 0
}


void TetrisPiece::draw_lines(int n, int m, int (*matrix)[10], float (*red)[10], float (*green)[10], float (*blue)[10]) {
    int offsetX = m / 2 - 1; // Offset para alinhar o centro do grid
    int offsetY = n / 2;

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

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            glPushMatrix(); // Salva a matriz de transformação para cada célula

            // Calcula a posição correta do cubo
            glTranslated(j - offsetX - 0.5, i - offsetY + 0.5, 0);

            if (matrix[i][j] == 1) { // Se a posição está ocupada
                // Desenha o cubo preenchido
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                cube_unit();

                // Desenha o contorno do cubo em preto
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                cube_unit();
            }

            glPopMatrix(); // Restaura a matriz de transformação original
        }
    }
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}





int TetrisPiece::colision_left(int n, int m, int (*matrix)[10]){
    if (type == 1 || type == 2 || type == 4){
        if (matrix[(int)(pos.y + n / 2 - 0.5)][(int)(pos.x + m / 2 - 1.5)] == 1 ||
            matrix[(int)(pos1.y + n / 2 - 0.5)][(int)(pos1.x + m / 2 - 1.5)] == 1 ||
            matrix[(int)(pos2.y + n / 2 - 0.5)][(int)(pos2.x + m / 2 - 1.5)] == 1 ||
            matrix[(int)(pos3.y + n / 2 - 0.5)][(int)(pos3.x + m / 2 - 1.5)] == 1) {
            std::cout << "COLISION LEFT" << std::endl;
            return 1;
        }
    }
    else {
        if (pos4.x + m / 2 - 0.5 < 0 || pos1.x + m/2 - 0.5 < 0 || pos2.x + m/2 - 0.5 < 0 || pos3.x + m/2 - 0.5 < 0) {
            std::cout << "COLISION LEFT BARRIER" << std::endl;
            return 1;

        }
        if (matrix[(int)(pos4.y + n / 2 - 0.5)][(int)(pos4.x + m / 2 - 1.5)] == 1 ||
            matrix[(int)(pos1.y + n / 2 - 0.5)][(int)(pos1.x + m / 2 - 1.5)] == 1 ||
            matrix[(int)(pos2.y + n / 2 - 0.5)][(int)(pos2.x + m / 2 - 1.5)] == 1 ||
            matrix[(int)(pos3.y + n / 2 - 0.5)][(int)(pos3.x + m / 2 - 1.5)] == 1) {
            std::cout << "COLISION LEFT" << std::endl;
            return 1;
        }
    }
     return 0;   
}

int TetrisPiece::colision_right(int n, int m, int (*matrix)[10]){
    if (type == 1 || type == 2 || type == 4){
        if (matrix[(int)(pos.y + n / 2 - 0.5)][(int)(pos.x + m / 2 + 0.5)] == 1 ||
            matrix[(int)(pos1.y + n / 2 - 0.5)][(int)(pos1.x + m / 2 + 0.5)] == 1 ||
            matrix[(int)(pos2.y + n / 2 - 0.5)][(int)(pos2.x + m / 2 + 0.5)] == 1 ||
            matrix[(int)(pos3.y + n / 2 - 0.5)][(int)(pos3.x + m / 2 + 0.5)] == 1) {
            std::cout << "COLISION RIGHT" << std::endl;
            return 1;
        }
    }
    else {
        if (matrix[(int)(pos4.y + n / 2 - 0.5)][(int)(pos4.x + m / 2 + 0.5)] == 1 ||
            matrix[(int)(pos1.y + n / 2 - 0.5)][(int)(pos1.x + m / 2 + 0.5)] == 1 ||
            matrix[(int)(pos2.y + n / 2 - 0.5)][(int)(pos2.x + m / 2 + 0.5)] == 1 ||
            matrix[(int)(pos3.y + n / 2 - 0.5)][(int)(pos3.x + m / 2 + 0.5)] == 1) {
            std::cout << "COLISION RIGHT" << std::endl;
            return 1;
        }
    }
     return 0;   
}

void TetrisPiece::rotate(int n, int m, int (*matrix)[10]) {
    int offsetX = m / 2 - 1; // Offset para alinhar o centro do grid
    int offsetY = n / 2;

    if (stop) {
        std::cout << "STOP 1" << std::endl;
        return;
    }
    
    // Salva valores atuais de posição e rotação
    float prev_left = leftmost_x, prev_right = rightmost_x, prev_lowest = lowest_y;
    ofVec3f prev_pos = pos;
    ofVec3f prev_pos1 = pos1;
    ofVec3f prev_pos2 = pos2;
    ofVec3f prev_pos3 = pos3;
    ofVec3f prev_pos4 = pos4;
    int prev_type_rotate = type_rotate;
    int prev_rotator_angle = rotator_angle;

    // Tenta rotacionar
    type_rotate = (type_rotate + 1) % 4;
    rotator_angle -= 90;
    updateExtremes();

   if (leftmost_x < -m / 2 || rightmost_x > m / 2 || lowest_y <= -n / 2 + 1){
        // Reverte rotação se sair dos limites
        leftmost_x = prev_left;
        rightmost_x = prev_right;
        lowest_y = prev_lowest;
        type_rotate = prev_type_rotate;
        rotator_angle = prev_rotator_angle;
        pos = prev_pos;
        pos1 = prev_pos1;
        pos2 = prev_pos2;
        pos3 = prev_pos3;

        if (type == 3) {
            pos4 = prev_pos4;
        }
    }

    if (type == 1 || type == 2 || type == 4) {
        if (matrix[(int)(pos.y + offsetY - 0.5)][(int)(pos.x + offsetX + 0.5)] == 1 ||
            matrix[(int)(pos1.y + offsetY - 0.5)][(int)(pos1.x + offsetX + 0.5)] == 1 ||
            matrix[(int)(pos2.y + offsetY - 0.5)][(int)(pos2.x + offsetX + 0.5)] == 1 ||
            matrix[(int)(pos3.y + offsetY - 0.5)][(int)(pos3.x + offsetX + 0.5)] == 1) {
                leftmost_x = prev_left;
                rightmost_x = prev_right;
                lowest_y = prev_lowest;
                type_rotate = prev_type_rotate;
                rotator_angle = prev_rotator_angle;
                pos = prev_pos;
                pos1 = prev_pos1;
                pos2 = prev_pos2;
                pos3 = prev_pos3;
            }
    }
    else {
        if (matrix[(int)(pos.y + offsetY - 0.5)][(int)(pos.x + offsetX + 0.5)] == 1 ||
            matrix[(int)(pos1.y + offsetY - 0.5)][(int)(pos1.x + offsetX + 0.5)] == 1 ||
            matrix[(int)(pos2.y + offsetY - 0.5)][(int)(pos2.x + offsetX + 0.5)] == 1 ||
            matrix[(int)(pos3.y + offsetY - 0.5)][(int)(pos3.x + offsetX + 0.5)] == 1 ||
            matrix[(int)(pos4.y + offsetY - 0.5)][(int)(pos4.x + offsetX + 0.5)] == 1) {
                leftmost_x = prev_left;
                rightmost_x = prev_right;
                lowest_y = prev_lowest;
                type_rotate = prev_type_rotate;
                rotator_angle = prev_rotator_angle;
                pos1 = prev_pos1;
                pos2 = prev_pos2;
                pos3 = prev_pos3;
                pos4 = prev_pos4;
            }
    }
    
}

void TetrisPiece::moveLeft(int n, int m, int (*matrix)[10]) {
    int colision;
    if (stop) {
        std::cout << "STOP 2" << std::endl;
        return;
    }
    colision = colision_left(n, m, matrix);
    lim_esq = -m / 2 + 1.5;
    if (leftmost_x >= lim_esq && colision == 0) { // Verifica limite esquerdo
        pos.x--;   
    }
    updateExtremes();
}

void TetrisPiece::moveRight(int n, int m, int (*matrix)[10]) {
    int colision;

    if (stop) {
        std::cout << "STOP 3" << std::endl;
        return;
    }
    colision = colision_right(n, m, matrix);

    lim_dir = m / 2 - 1.5;
    if (rightmost_x <= lim_dir && colision == 0) { // Verifica limite direito
        pos.x++;
    }
    updateExtremes();
}

void TetrisPiece::moveDown(int n, int m, int (*matrix)[10], float (*red)[10], float (*green)[10], float (*blue)[10]) {
    if (stop) {
        std::cout << "STOP 4" << std::endl;
        return;
    }
    //lim_baixo = -n / 2 + 1.5;
    lim_baixo = -n / 2 + 1;
    if (lowest_y > lim_baixo) { // Verifica limite inferior
        pos.y--;
    } 
    updateExtremes();

    if (lowest_y <= lim_baixo) {
        stop = true;
        matrix_update(n, m, matrix, red, green, blue);
        std::cout << "STOP movedown" << std::endl;
    }
}


ofVec3f TetrisPiece::getPosition() {
    return pos;
}



void TetrisPiece::drawL() {
    glDisable(GL_COLOR_MATERIAL);
    // Configura o material para a peça L
    GLfloat mat_ambient[] = { 1.2f, 0.2f, 0.2f, 1.0f };
    GLfloat mat_diffuse[] = { .8f, .0f, .0f, 1.0f }; // Cor vermelha
    GLfloat mat_specular[] = { .5f, .5f, .5f, .0f };
    GLfloat mat_shininess[] = { 128.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // Desenha os cubos sólidos
    cube_unit();

    glPushMatrix();
    glTranslated(1, 0, 0);
    cube_unit();
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(-1, 0, 0);
    cube_unit();
    glPopMatrix();

    glPushMatrix();
    glTranslated(1, -1, 0);
    cube_unit();
    glPopMatrix();

    // Desenha as linhas dos cubos
    //glColor3f(0, 0, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    cube_unit();

    glPushMatrix();
    glTranslated(1, 0, 0);
    cube_unit();
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(-1, 0, 0);
    cube_unit();
    glPopMatrix();

    glPushMatrix();
    glTranslated(1, -1, 0);
    cube_unit();
    glPopMatrix();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Retorna ao modo de preenchimento padrão

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    /*
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
    */
}

void TetrisPiece::drawT() {
    glDisable(GL_COLOR_MATERIAL);
    // Configura o material para a peça T
    GLfloat mat_ambient[] = { .2f, 0.2f, 1.2f, 1.0f };
    GLfloat mat_diffuse[] = { .0f, .0f, 0.8f, 1.0f }; // Cor azul
    GLfloat mat_specular[] = { .0f, .0f, .0f, 1.0f };
    GLfloat mat_shininess[] = { 128.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // Desenha os cubos sólidos
    cube_unit();

    glPushMatrix();
    glTranslated(1, 0, 0);
    cube_unit();
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(-1, 0, 0);
    cube_unit();
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, -1, 0);
    cube_unit();
    glPopMatrix();

    // Desenha as linhas dos cubos
    //glColor3f(0, 0, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    cube_unit();

    glPushMatrix();
    glTranslated(1, 0, 0);
    cube_unit();
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(-1, 0, 0);
    cube_unit();
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, -1, 0);
    cube_unit();
    glPopMatrix();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Retorna ao modo de preenchimento padrão

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    /*
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
    */
}

void TetrisPiece::drawCube() {
    glDisable(GL_COLOR_MATERIAL);
    // Configura o material para a peça Cube
    GLfloat mat_ambient[] = { .2f, 1.2f, .2f, 1.0f };
    GLfloat mat_diffuse[] = { .0f, 0.8f, .0f, 1.0f }; // Cor verde
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[] = { 0.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // Desenha os cubos sólidos
    glTranslated(-0.5, -0.5, 0);
    cube_unit();

    glPushMatrix();
    glTranslated(1, 0, 0);
    cube_unit();
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, 1, 0);
    cube_unit();
    glPopMatrix();

    glPushMatrix();
    glTranslated(1, 1, 0);
    cube_unit();
    glPopMatrix();

    // Desenha as linhas dos cubos
    //glColor3f(0, 0, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    cube_unit();

    glPushMatrix();
    glTranslated(1, 0, 0);
    cube_unit();
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, 1, 0);
    cube_unit();
    glPopMatrix();

    glPushMatrix();
    glTranslated(1, 1, 0);
    cube_unit();
    glPopMatrix();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Retorna ao modo de preenchimento padrão

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    /*
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
    */
}



void TetrisPiece::drawLine() {
    glDisable(GL_COLOR_MATERIAL);
    // Configura o material para a peça Cube
    GLfloat mat_ambient[] = { .2f, 1.2f, .2f, 1.0f };
    GLfloat mat_diffuse[] = { .0f, 0.8f, .0f, 1.0f }; // Cor verde
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[] = { 0.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // Desenha os cubos sólidos
    cube_unit();

    glPushMatrix();
    glTranslated(-1, 0, 0);
    cube_unit();
    glPopMatrix();

    glPushMatrix();
    glTranslated(1, 0, 0);
    cube_unit();
    glPopMatrix();    

    glPushMatrix();
    glTranslated(2, 0, 0);
    cube_unit();
    glPopMatrix();

    // Desenha as linhas dos cubos
    //glColor3f(0, 0, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    cube_unit();

    glPushMatrix();
    glTranslated(-1, 0, 0);
    cube_unit();
    glPopMatrix();

    glPushMatrix();
    glTranslated(1, 0, 0);
    cube_unit();
    glPopMatrix();    

    glPushMatrix();
    glTranslated(2, 0, 0);
    cube_unit();
    glPopMatrix();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Retorna ao modo de preenchimento padrão

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    /*
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
    */
}