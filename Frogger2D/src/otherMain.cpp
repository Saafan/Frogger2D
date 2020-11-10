#include <stdio.h>
#include <string.h>
#include <iostream>
#include <iostream>
#include <random>
#include <ctime>
#include <glut.h>

using namespace std;

int windW = 800;
int windH = 600;
int numLanes = 5; // only works for 5 or 6 lanes
int numBorderLanes = numLanes - 1;
int bridgeW = 100;
int laneborderH = 10;
int numCoinsGenerated = 4;
int randomX = rand() % (windW - 40) + 20;
int randomY = rand() % (windH - 40) + 20;
float angle = 0.0f;
float coinPos[8]; // please change when changing numCoinsGenerated
bool firstPosC = true;
bool firstPosP = true;
bool firstPosP2 = true;

float lanesPosStart[5]; //please change when changing numLanes too
float lanesPosEnd[5];  //please change when changing numLanes too

float lanesBorderPosStart[4];  //please change when changing numLanes too
float lanesBorderPosEnd[4];  //please change when changing numLanes too

int posX = 0;
int posY = 0;
int posUp = 0;
int posDown = 0;

int numPowerUpB = 3;
float powerUpPosB[6];

int numPowerUpC = 3;
float powerUpPosC[6];

void timer(int value) {
	// what we want to change by time
	glutTimerFunc(1000, timer, 0);
}

void SpecialK(int key, int mx, int my) {
	if (key == GLUT_KEY_RIGHT) {
		posX += 10;
	}
	else if (key == GLUT_KEY_LEFT) {
		posX += 10;
	}
	else if (key == GLUT_KEY_UP) {
		posY += 10;
	}
	else if (key == GLUT_KEY_DOWN) {
		posY -= 10;
	}
	glutPostRedisplay();
}

void Anim() {
	angle += 0.5;

	glutPostRedisplay();
}

void print(int x, int y, char* string)
{
	int len, i;

	glRasterPos2f(x, y);

	len = (int)strlen(string);

	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

void drawGoal(int x, int y, float r) {
	glPushMatrix();
	glColor3f(0.6, 0.0f, 0.0f);

	glBegin(GL_LINES);
	glVertex3f(x, y, 0);
	glVertex3f(x + r + 20, y + r + 20, 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(x, y, 0);
	glVertex3f(x, y + r + 30, 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(x, y, 0);
	glVertex3f(x - r - 20, y + r + 20, 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(x, y, 0);
	glVertex3f(x + r + 30, y, 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(x, y, 0);
	glVertex3f(x - r - 30, y, 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(x, y, 0);
	glVertex3f(x + r + 20, y - r - 20, 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(x, y, 0);
	glVertex3f(x - r - 30, y - r - 20, 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(x, y, 0);
	glVertex3f(x, y - r - 30, 0);
	glEnd();

	glPushMatrix();
	glTranslatef(x, y, 0);
	GLUquadric* quadObj = gluNewQuadric();
	gluDisk(quadObj, 0, r, 50, 50);
	glPopMatrix();

	glBegin(GL_TRIANGLES);
	glColor3f(0.5, 0.5, 0.5);
	glVertex3f(x - 5, y - 5, 0);
	glVertex3f(x + 5, y - 5, 0);
	glVertex3f(x, y + 10, 0);
	glEnd();
	glPopMatrix();
}

void generateBorderLanes(int numLanes) {
	glPushMatrix();
	int laneW = ((windH - 40) - 10 * numBorderLanes) / numLanes;
	int n = 1;
	int s;
	s = (laneW);
	for (int i = 0; i <= numBorderLanes; i++) {
		if (i < numBorderLanes) {
			glColor3f(0, 0.0f, 0.0f);
			glBegin(GL_QUADS);
			glVertex3f(20, laneW + 20, 0);
			glVertex3f(windW - 20, laneW + 20, 0);
			glVertex3f(windW - 20, laneW + 30, 0);
			glVertex3f(20, laneW + 30, 0);
			glEnd();

			glColor3f(0.5, 0.8, 0.5);
			glBegin(GL_LINES);
			glVertex3f(20, laneW + 20, 0);
			glVertex3f(windW - 20, laneW + 20, 0);
			glVertex3f(windW - 20, laneW + 30, 0);
			glVertex3f(20, laneW + 30, 0);
			glEnd();

			lanesBorderPosStart[i] = laneW + 20;
			lanesBorderPosEnd[i] = laneW + 30;


			if (i == 0) {
				lanesPosStart[i] = 20;
				lanesPosEnd[i] = laneW + 20;

			}
			else {
				lanesPosStart[i] = laneW - s + 20;
				lanesPosEnd[i] = laneW + (20);
			}

			n++;
			laneW = (s * n) + (10 * (n - 1));
		}
		else {
			laneW = (s * n) + (10 * (n - 1));
			lanesPosStart[i] = laneW - s + 20;
			lanesPosEnd[i] = laneW + (20);
		}

	}
	glPopMatrix();
}

void generatePowerUpbridge(int x, int y, int l, int l2, int h, int h2) {
	glPushMatrix();
	glColor3f(0, 0.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	glVertex3f(x, y, 0);
	glVertex3f(x + l, y, 0);
	glVertex3f((2 * x + l) / 2, y + h, 0);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0, 0.0f, 0.0f);
	glVertex3f(((2 * x + l) / 2) - 3, y, 0);
	glVertex3f(((2 * x + l) / 2) - 3, y - h2, 0);
	glVertex3f(((2 * x + l) / 2) + 3, y - h2, 0);
	glVertex3f(((2 * x + l) / 2) + 3, y, 0);

	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3f(0.5, 0.8, 0.5);
	glVertex3f(x, y, 0);
	glVertex3f(((2 * x + l) / 2) - 3, y, 0);
	glVertex3f(((2 * x + l) / 2) - 3, y - h2, 0);
	glVertex3f(((2 * x + l) / 2) + 3, y - h2, 0);
	glVertex3f(((2 * x + l) / 2) + 3, y, 0);
	glVertex3f(x + l, y, 0);
	glVertex3f((2 * x + l) / 2, y + h, 0);

	glEnd();
	glPopMatrix();
	glutPostRedisplay();
}

void generatePowerUpcoin(int x, int y, int l) {
	glPushMatrix();

	glBegin(GL_QUADS);
	glColor3f(0.5, 0.5, 0.5);
	glVertex3f(x, y, 0);
	glVertex3f(x + l, y, 0);
	glVertex3f(x + l, y + l, 0);
	glVertex3f(x, y + l, 0);

	glEnd();

	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex3f(x + (l / 8), y + (l / 4), 0);
	glVertex3f(x + (l / 3), y + (3 * l / 4), 0);

	glEnd();


	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex3f(x + (l / 3), y + (l / 4), 0);
	glVertex3f(x + (l / 8), y + (3 * l / 4), 0);

	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(x + (5 * l / 8), y + (5 * l / 8) + 2, 0);
	glVertex3f(x + (3 * l / 4), y + (5 * l / 8) + 2, 0);

	glVertex3f(x + (3 * l / 4), y + (l / 4) + 2, 0);
	glVertex3f(x + (7 * l / 8), y + (l / 4) + 2, 0);

	glEnd();
	glPopMatrix();
}

void drawCoinsStars(int x, int y, int h1, int h2, int w) {
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(x + w / 2, y + h1 / 2, 0);
	glRotated(angle, 0, 0, 1);
	glTranslatef(-x - w / 2, -y - h1 / 2, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glVertex2f((2 * x + w) / 2, y + h2);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(x, y + h1);
	glVertex2f(x + w, y + h1);
	glVertex2f((2 * x + w) / 2, y + h1 - h2);
	glEnd();
	glPopMatrix();

}

void generateCoins(float x, float y) {
	for (int i = 0; i < numCoinsGenerated; i++) {
		drawCoinsStars(x, y, 20, 30, 20);
	}
	glutPostRedisplay();
}

void Display() {
	// printf("%f,%f", lanesBorderPosStart[0]);
	//printf("%f,%f", lanesBorderPosEnd[0]);

	glClearColor(0.2, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	// drawing coins and powerups

	generateBorderLanes(numLanes);

	//std::cout << rand() % (windH / numLanes - 80) + 20 << std::endl;
	
	if (firstPosC == true) {

		for (size_t i = 0; i < numCoinsGenerated * 2; i++)
		{
			int randomX = rand() % (windW - 100) + 100;
			int randomY = lanesBorderPosEnd[i] + rand() % (windH / numLanes - 50) + 10;

			if (randomY < 20)
				randomY += 20;

			coinPos[i] = randomX;
			coinPos[i + 1] = randomY;
			i++;
		}
		firstPosC = false;
	}


// 	if (firstPosC == true) {
// 		int j = 0;
// 		for (int i = 0; i < (numCoinsGenerated) * 2; i++) {
// 			int randomX = rand() % (windW - 20) + 20;
// 			int randomY = rand() % (windH - 20) + 20;
// 
// 
// 			if (randomY > lanesPosStart[j] && randomY < lanesPosEnd[j]) {
// 				coinPos[i] = randomX;
// 				coinPos[i + 1] = randomY;
// 
// 				i++;
// 				j++;
// 				
// 			}
// 			else {
// 				if (i == 0) {
// 					i = 0;
// 				}
// 				else {
// 					i--;
// 				}
// 			}
// 
// 		}
// 		firstPosC = false;
// 	}

	for (int i = 0; i < (numCoinsGenerated) * 2; i++) {
		generateCoins(coinPos[i], coinPos[i + 1]);
		i++;
	}

	if (firstPosP == true) {

		int j = 0;
		for (int i = 0; i < (numPowerUpB) * 2; i++) {
			int randomX = rand() % (windW - 20) + 20;
			int randomY = rand() % (windH - 20) + 20;

			//	if (randomY > lanesPosStart[j] && randomY < lanesPosEnd[j]) {
			powerUpPosB[i] = randomX;
			powerUpPosB[i + 1] = randomY;
			i++;
			j++;
			/*	}
				else {
					if (i == 0 ) {
						i = 0;
					}
					else {
						i--;
					}
				}*/

		}
		firstPosP = false;
	}
	for (int i = 0; i < (numPowerUpB) * 2; i++) {
		generatePowerUpbridge(powerUpPosB[i], powerUpPosB[i + 1], 25, 10, 25, 15);
		i++;
	}

	if (firstPosP2 == true) {

		int j = 0;
		for (int i = 0; i < (numPowerUpC) * 2; i++) {
			int randomX = rand() % (windW - 20) + 20;
			int randomY = rand() % (windH - 20) + 20;

			//	if (randomY > lanesPosStart[j] && randomY < lanesPosEnd[j]) {
			powerUpPosC[i] = randomX;
			powerUpPosC[i + 1] = randomY;
			i++;
			j++;
			/*	}
				else {
					if (i == 0 ) {
						i = 0;
					}
					else {
						i--;
					}
				}*/

		}
		firstPosP2 = false;
	}
	for (int i = 0; i < (numPowerUpC) * 2; i++) {
		generatePowerUpcoin(powerUpPosC[i], powerUpPosC[i + 1], 25);
		i++;
	}


	//Code Borders Here
	glBegin(GL_QUADS);
	glColor3f(0.5, 0.5, 0.5);
	glVertex3f(0, 0, 0);
	glVertex3f(20, 0, 0);
	glVertex3f(20, windH, 0);
	glVertex3f(0, windH, 0);

	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.5, 0.5, 0.5);
	glVertex3f(0, 0, 0);
	glVertex3f(windW, 0, 0);
	glVertex3f(windW, 20, 0);
	glVertex3f(0, 20, 0);

	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.5, 0.5, 0.5);
	glVertex3f(windW - 20, 0, 0);
	glVertex3f(windW, 0, 0);
	glVertex3f(windW, windH, 0);
	glVertex3f(windW - 20, windH, 0);

	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.5, 0.5, 0.5);
	glVertex3f(0, windH - 20, 0);
	glVertex3f(windW, windH - 20, 0);
	glVertex3f(windW, windH, 0);
	glVertex3f(0, windH, 0);

	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0, 0.0f, 0.0f);
	glVertex3f(5, 5, 0);
	glVertex3f(20, 5, 0);
	glVertex3f(12.5, 20, 0);

	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0, 0.0f, 0.0f);
	glVertex3f(windW - 20, 5, 0);
	glVertex3f(windW - 5, 5, 0);
	glVertex3f(((windW - 5 + windW - 20) / 2), 20, 0);

	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0, 0.0f, 0.0f);
	glVertex3f(windW - 5, windH - 5, 0);
	glVertex3f(windW - 20, windH - 5, 0);
	glVertex3f(((windW - 5 + windW - 20) / 2), windH - 20, 0);

	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0, 0.0f, 0.0f);
	glVertex3f(5, windH - 5, 0);
	glVertex3f(20, windH - 5, 0);
	glVertex3f(12.5, windH - 20, 0);

	glEnd();

	// Code Charachter Here
	glPushMatrix();
	glTranslatef(posX, posY, 0);

	glBegin(GL_LINES);
	glColor3f(0, 0.0f, 0.0f);
	glVertex3f(47.5, 40, 0);
	glVertex3f(70, 70, 0);

	glEnd();

	glBegin(GL_LINES);
	glColor3f(0, 0.0f, 0.0f);
	glVertex3f(47.5, 40, 0);
	glVertex3f(25, 70, 0);

	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.5, 0.5, 0.5);
	glVertex3f(35.75, 25, 0);
	glVertex3f(41.75, 25, 0);
	glVertex3f(41.75, 40, 0);
	glVertex3f(35.75, 40, 0);

	glEnd();


	glBegin(GL_QUADS);
	glColor3f(0.5, 0.5, 0.5);
	glVertex3f(53.25, 25, 0);
	glVertex3f(59.25, 25, 0);
	glVertex3f(59.25, 40, 0);
	glVertex3f(53.25, 40, 0);

	glEnd();

	glPointSize(3);
	glBegin(GL_TRIANGLES);
	glColor3f(0.5, 0.5, 0.5);
	glVertex3f(30, 40, 0);
	glVertex3f(65, 40, 0);
	glVertex3f(47.5, 80, 0);

	glEnd();


	glBegin(GL_POINTS);
	glColor3f(0, 0.0f, 0.0f);
	glPointSize(300);
	glVertex3f(40.75, 60, 0);
	glVertex3f(54.25, 60, 0);

	glEnd();


	glBegin(GL_LINES);
	glColor3f(0, 0.0f, 0.0f);
	glVertex3f(40, 50, 0);
	glVertex3f(55, 50, 0);

	glEnd();
	glPopMatrix();

	//GOAL
	drawGoal(650, 530, 20);

	//generatePowerUpbridge(200, 200, 25, 10, 25, 15);
	//generatePowerUpcoin(200,150,25);




	glColor3f(0.6, 0, 0);
	char* p0s[20];

	//printf((char*)p0s, "Score: %d", 50);

	print(windW - 130, windH - 20, (char*)p0s);

	glutSwapBuffers();
	glFlush();

}

void main(int argc, char** argr) {

	srand(time(0));

	if (numLanes > 6) {
		printf("number of lanes can't exceede 6");
		exit(1);
	}

	glutInit(&argc, argr);
	glutInitWindowSize(windW, windH);
	glutInitWindowPosition(150, 150);

	glutCreateWindow("OpenGL - 2D Template");
	glutDisplayFunc(Display);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	gluOrtho2D(0.0, windW, 0.0, windH);

	glutSpecialFunc(SpecialK);
	glutIdleFunc(Anim);
	glutTimerFunc(1000, timer, 0);
	glutMainLoop();
}

