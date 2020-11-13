
//Important Note: This was Originally a Frog Crossing Game, However since the Theme was a bit off, I had to recreate the game and refactor the code
//The Original Frog Game Project with Better Textures: https://github.com/saafan/Frogger2D
//If you want to Remove the Bounded Textures (Recommended). Just Delete the Images Folders 

#include "Renderer.h"
#include "Model.h"
#include "Object.h"
#include <stdio.h>
#include "Randomize.h"
#include "Texture.h"
#include <string>

Texture bordersTex(".\\images\\borders.png");
Texture laneTex(".\\images\\lane.png");
Texture chickenTex(".\\images\\chicken.png");


Object openUp(100.0f, 100.0f, 0, 0, 0, ModelType::Key);
Object coinDoubler(100.0f, 100.0f, 0, 0, 0, ModelType::DoubleCoin);

Object finalTarget(100, 100, WIDTH / 2, HEIGHT - 100, 0, ModelType::Target);

Model border(Primitive::Plane, WIDTH + 40, HEIGHT + 40, -20.0f, -20.0f, Color{ 0.5f, 0.5f, 0.5f }, ModelType::Null, & bordersTex);
Model grass(Primitive::Plane, WIDTH, 110, 0, 0, Color{ 0.07, 0.42, 0.08 }, ModelType::Null);
Model chicken(Primitive::Plane, 100.0f, 100.0f, 0.0f, 0.0f, Color{ 0.2f, 0.5f, 0.3f }, ModelType::Character, & chickenTex);
Model street(Primitive::Plane, WIDTH, 330.0f, 0.0f, 110.0f, Color{ 0.3f, 0.3f, 0.3f }, ModelType::Environment);
Model rest(Primitive::Plane, WIDTH, 120.0f, 0.0f, 110.0f * 4, Color{ 0.8f, 0.3f, 0.3f }, ModelType::Null);
Model water(Primitive::Plane, WIDTH, 330.0f, 0.0f, 110.0f * 5, Color{ 0.2f, 0.5f, 0.8f }, ModelType::Null);
Model goal(Primitive::Plane, WIDTH, 120.0f, 0.0f, 110.0f * 8, Color{ 0.07, 0.42, 0.08 }, ModelType::Goal);

Model borderLane(Primitive::Plane, WIDTH, 10, 0, 0, Color{ 0.3, 0.3, 0.2 }, ModelType::Lane);


std::vector<Model*> models;
std::vector<Object> objects;
std::vector<std::pair<float, float>>  characterCollision;
std::vector<Model*> bridges;


bool firstTime = true;
bool CheckStaticCollision(float nextX, float nextY);
bool CheckEnemyCollision();

int coinInc = 0;
int time = 40;
const static int startTime = time;

int score = 0;
bool openUpTaken = false;
bool powerTaken = false;
bool protect = false;
bool gameOver = false;
bool finished = false;



int laneNum = 4;


void print(int x, int y, char* string)
{

	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
	}
}

void GenerateCoins()
{

	for (size_t i = 0; i < 5; i++)
	{
		float randX = Randomize(50, WIDTH - 150);

		int lane = Randomize(0, laneNum - 3);

		int beginPoint = (int)bridges.at(lane)->GetY() + 57;

		float randY = Randomize(beginPoint, beginPoint + 50);

		Object coin(100, 100, 0, 0, 0, ModelType::Coin);

		coin.GetModel()->Translate(randX, randY);

		objects.emplace_back(coin);
	}

}


void GenerateCollisionPoints()
{
	characterCollision.clear();
	characterCollision.emplace_back(std::pair<float, float>{ chicken.GetX() - 1, chicken.GetY() + chicken.GetHeight() + 1 });					//Top Left
	characterCollision.emplace_back(std::pair<float, float>{ chicken.GetX() + chicken.GetWidth() + 1, chicken.GetY() + chicken.GetHeight() + 1 }); // Top Right
	characterCollision.emplace_back(std::pair<float, float>{ chicken.GetX() - 1, chicken.GetY() - 1 });										// Bottom Left
	characterCollision.emplace_back(std::pair<float, float>{ chicken.GetX() + chicken.GetWidth() + 1, chicken.GetY() - 1 });					// Bottom Right


}

int CheckBordersCollision()
{
	if (chicken.GetX() + 5 < 0)
		return 2;

	if (chicken.GetX() + chicken.GetWidth() > WIDTH - 10)
		return 1;

	if (chicken.GetY() + 5 < 0)
		return 2;

	if (chicken.GetY() + chicken.GetHeight() > HEIGHT - 10)
		return 0;

	return -1;
}



int CheckBridgeCollision()
{

	// Top Left     --> 0
	// Top Right    -->	1
	// Bottom Left  -->	2
	// Bottom Right --> 3


	for (size_t i = 0; i < bridges.size(); i++)
	{
		if (bridges.at(i)->GetX() < characterCollision.at(0).first && bridges.at(i)->GetX() + bridges.at(i)->GetWidth() > characterCollision.at(0).first)
		{
			if (bridges.at(i)->GetX() < characterCollision.at(1).first && bridges.at(i)->GetX() + bridges.at(i)->GetWidth() > characterCollision.at(1).first)
			{
				if (bridges.at(i)->GetY() < characterCollision.at(0).second && bridges.at(i)->GetY() + bridges.at(i)->GetHeight() > characterCollision.at(0).second)
				{
					if (bridges.at(i)->GetY() < characterCollision.at(1).second && bridges.at(i)->GetY() + bridges.at(i)->GetHeight() > characterCollision.at(1).second)
					{
						return i;
					}
				}
			}
		}

		if (bridges.at(i)->GetX() < characterCollision.at(2).first && bridges.at(i)->GetX() + bridges.at(i)->GetWidth() > characterCollision.at(2).first)
		{
			if (bridges.at(i)->GetX() < characterCollision.at(3).first && bridges.at(i)->GetX() + bridges.at(i)->GetWidth() > characterCollision.at(3).first)
			{
				if (bridges.at(i)->GetY() < characterCollision.at(2).second && bridges.at(i)->GetY() + bridges.at(i)->GetHeight() > characterCollision.at(2).second)
				{
					if (bridges.at(i)->GetY() < characterCollision.at(3).second && bridges.at(i)->GetY() + bridges.at(i)->GetHeight() > characterCollision.at(3).second)
					{
						return i;
					}
				}
			}
		}
	}

	return -1;

}


int CheckLaneCollision()
{
	int bridge = CheckBridgeCollision();

	for (size_t i = 0; i < models.size(); i++)
	{
		for (size_t j = 0; j < characterCollision.size(); j++)
		{
			if (models.at(i)->GetX() < characterCollision.at(j).first && models.at(i)->GetX() + models.at(i)->GetWidth() > characterCollision.at(j).first)
			{
				if (models.at(i)->GetY() - 15 < characterCollision.at(j).second && models.at(i)->GetY() - 15 + models.at(i)->GetHeight() > characterCollision.at(j).second)
				{

					if (models.at(i)->GetType() == ModelType::Lane)
					{
						return j;
					}
				}
			}
		}
	}
	glutPostRedisplay();
	return -1;
}

void RenderBorders()
{
	Model leftBorder(Primitive::Plane, 20.0f, HEIGHT + 40, -20.0f, -20, Color{}, ModelType::Collision, &bordersTex);
	Model rightBorder(Primitive::Plane, 20.0f, HEIGHT + 40, WIDTH, -20, Color{}, ModelType::Collision, &bordersTex);
	Model upBorder(Primitive::Plane, WIDTH, 20, 0, HEIGHT, Color{}, ModelType::Collision, &bordersTex);
	Model downBorder(Primitive::Plane, WIDTH, 20, 0, -20, Color{}, ModelType::Collision, &bordersTex);
	Model triangle1(Primitive::Triangle, 20, 20, -20, -20, Color{ 0.5, 0.5, 0.5 }, ModelType::Collision);
	Model triangle2(Primitive::Triangle, 20, 20, -20, HEIGHT, Color{ 0.5, 0.5, 0.5 }, ModelType::Collision);
	Model triangle3(Primitive::Triangle, 20, 20, WIDTH, -20, Color{ 0.5, 0.5, 0.5 }, ModelType::Collision);
	Model triangle4(Primitive::Triangle, 20, 20, WIDTH, HEIGHT, Color{ 0.5, 0.5, 0.5 }, ModelType::Collision);

	leftBorder.Render();
	rightBorder.Render();
	upBorder.Render();
	downBorder.Render();

	GenerateCollisionPoints();

	triangle1.Render();
	triangle2.Render();
	triangle3.Render();
	triangle4.Render();

}




void RenderLanes()
{

	for (size_t i = 0; i < laneNum - 1; i++)
	{
		models.push_back(new Model(Primitive::Plane, WIDTH - 10, 20, 0, (HEIGHT / laneNum) * (i + 1), Color{ 0.3, 0.3, 0.2 }, ModelType::Lane, &laneTex));
		Model* Bridge = new Model(Primitive::Plane, 200, 110, Randomize(150, WIDTH - 180), ((HEIGHT / laneNum) * (i + 1)) - 55, Color{ 0.2, 0.5, 0.8 }, ModelType::Bridge);

		models.push_back(Bridge);
		bridges.push_back(Bridge);
	}
}

void renderScene(void)
{



	std::string line;

	//Environment
	//RenderCharacter();

	if (time <= 0)
	{
		gameOver = true;
	}

	if (finished)
	{
		static int elapsedTime = startTime - time;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.5f, 0.8f, 1.0f);

		glColor3f(1, 1, 1);
		char* p0s[20];
		sprintf((char*)p0s, "You Finished the Game. Score: %d  In: %i Seconds", score, elapsedTime);
		print(WIDTH / 2 - 200, HEIGHT / 2, (char*)p0s);
	}
	else if (gameOver)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1.00f, 0.20f, 0.20f, 1.0f);

		static int elapsedTime = startTime - time;
		glColor3f(0, 0, 0);
		char* p0s[20];
		sprintf((char*)p0s, "You Lost the Game. Score: %d", score);
		print(WIDTH / 2 - 120, HEIGHT / 2, (char*)p0s);
	}
	else
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.5f, 0.8f, 1.0f);

		glColor3f(1, 0, 0);
		char* p0s[20];
		sprintf((char*)p0s, "Score: %d, Time: %i", score, time);
		print(WIDTH - 300, HEIGHT - 50, (char*)p0s);

		finalTarget.RenderObject();


		RenderBorders();
		CheckLaneCollision();

		for (size_t i = 0; i < models.size(); i++)
		{
			models[i]->Render();
		}

		for (size_t i = 0; i < objects.size(); i++)
		{
			objects.at(i).RenderObject();
		}

		openUp.RenderObject();
		coinDoubler.RenderObject();

		chicken.Render();


		CheckEnemyCollision();
	}
	glPointSize(10);
	glBegin(GL_POINTS);
	glColor3f(1, 0, 0);
	glEnd();

	glutSwapBuffers();
	glutPostRedisplay();

}

void ScatterKeyDoubleCoins()
{

	float randX1 = Randomize(50, WIDTH - 150);
	float randX2 = Randomize(50, WIDTH - 150);

	int lane1 = Randomize(0, (laneNum - 3));
	int lane2 = Randomize(0, (laneNum - 3));

	int beginPoint1 = (int)bridges.at(lane1)->GetY() + 57;
	int beginPoint2 = (int)bridges.at(lane2)->GetY() + 57;

	float randY1 = Randomize(beginPoint1, beginPoint1 + 20);
	float randY2 = Randomize(beginPoint2, beginPoint2 + 20);


	if (!openUpTaken)
		openUp.GetModel()->Translate(randX1, randY1);
	if (!powerTaken)
		coinDoubler.GetModel()->Translate(randX2, randY2);
}


void Tick(int value)
{

	glutTimerFunc(3000, Tick, 0);
}


void ScatterBridges(int value)
{
	for (size_t i = 0; i < bridges.size(); i++)
	{
		bridges[i]->Translate(Randomize(180, WIDTH - 250), bridges[i]->GetY());
	}

	glutTimerFunc(3000, ScatterBridges, 0);
}

void TickSpeed(int value)
{
	glBindTexture(GL_TEXTURE, 0);
	for (Object obj : objects)
	{
		if (obj.GetModel()->GetType() == ModelType::Enemy || obj.GetModel()->GetType() == ModelType::Log)
			obj.MoveObject(value);
	}

	glutTimerFunc(10, TickSpeed, 10);
}

void TickKeyProtection(int value)
{
	ScatterKeyDoubleCoins();
	glutTimerFunc(4000, TickKeyProtection, 0);
}


void ConsumeTime(int value)
{
	if (time > 0)
	{
		if (!finished)
			time--;
	}
	else
		gameOver = true;
	glutTimerFunc(1000, ConsumeTime, 0);
}

void DoubleCoin()
{
	coinInc++;
}


void ProcessInput(unsigned char key, int x_f, int y_f)
{

	float x = chicken.GetX();
	float y = chicken.GetY();
	int point = CheckLaneCollision();

	if (point == -1)
		point = CheckBordersCollision();

	int currentBridge = CheckBridgeCollision();

	if (currentBridge != -1)
		point = -1;

	// Top Left     ==> 0
	// Top Right    ==>	1
	// Bottom Left  ==>	2
	// Bottom Right ==> 3

	if (key == 's')
		if (point == 2 || point == 3)
			return;
		else
			chicken.TranslateAccum(0, -10);

	if (key == 'w')
		if (point == 0 || point == 1)
			return;
		else
			chicken.TranslateAccum(0, 10);

	if (key == 'a')
		if (point == 0 || point == 2)
			return;
		else
			chicken.TranslateAccum(-10, 0);

	if (key == 'd')
		if (point == 1 || point == 3)
			return;
		else
			chicken.TranslateAccum(10, 0);

	if (key == '`')
		exit(1);

	glutPostRedisplay();
}

void ProcessSpecialInput(int key, int x_f, int y_f)
{
	if (key == GLUT_KEY_DOWN)
		ProcessInput('s', 0, 0);
	if (key == GLUT_KEY_UP)
		ProcessInput('w', 0, 0);
	if (key == GLUT_KEY_LEFT)
		ProcessInput('a', 0, 0);
	if (key == GLUT_KEY_RIGHT)
		ProcessInput('d', 0, 0);

}

int main(int argc, char** argv)
{


	if (firstTime)
	{
		RenderLanes();
		firstTime = false;
		GenerateCoins();
	}


	if (laneNum > (HEIGHT / chicken.GetHeight()))
	{
		std::cout << "You Exceeded the Maximum Number of Lanes :(" << std::endl;
		exit(1);
	}

	models.push_back(&chicken);
	objects.push_back(finalTarget);
	objects.push_back(openUp);
	objects.push_back(coinDoubler);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Frogger 2D");

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);


	laneTex.LoadTexture();
	bordersTex.LoadTexture();
	chickenTex.LoadTexture();

	glutTimerFunc(0, Tick, 500);
	glutTimerFunc(0, TickKeyProtection, 100);
	glutTimerFunc(1000, ConsumeTime, 0);
	glutTimerFunc(10, TickSpeed, 10);
	glutTimerFunc(3000, ScatterBridges, 10);
	gluOrtho2D(-20, WIDTH + 20, -20, HEIGHT + 20);

	glutDisplayFunc(renderScene);
	//glutIdleFunc(renderScene);

	glutKeyboardFunc(ProcessInput);
	glutSpecialFunc(ProcessSpecialInput);
	//glutIgnoreKeyRepeat(1);
	glutMainLoop();


	return 1;
}


bool CheckStaticCollision(float nextX, float nextY)
{
	if (nextX > WIDTH - 70 || nextX < 0 || nextY > HEIGHT - 70 || nextY < 0)
		return true;

	for (Model* model : models)
	{
		if (model->GetType() == ModelType::Collision)
		{
			if (nextX >= model->GetX() && nextX <= model->GetX() + model->GetWidth() - 1)
			{
				if (nextY >= model->GetY() && nextY <= model->GetY() + model->GetHeight() - 1)
				{
					std::cout << "Collision Detected!!" << std::endl;
					return true;
				}
			}
		}
	}

	return false;
}

int Lane()
{
	for (size_t i = 0; i < laneNum; i++)
	{
		if (characterCollision[0].second < bridges.at(i)->GetY() + bridges.at(i)->GetHeight() / 2)
		{
			std::cout << i << std::endl;
			return i;
		}
	}
}

bool CheckEnemyCollision()
{

	bool onLog = false;

	for (Object model : objects)
	{
		for (const auto& point : characterCollision)
		{
			if (point.first >= model.GetModel()->GetX() && point.first - 10 <= model.GetModel()->GetX() + model.GetModel()->GetWidth())
			{
				if (point.second + 5 >= model.GetModel()->GetY() && point.second <= model.GetModel()->GetY() + model.GetModel()->GetHeight())
				{

					if (model.GetModel()->GetType() == ModelType::Coin)
					{
						score += 1 + coinInc;
						model.GetModel()->Translate(-600, -600);
					}

					else if (model.GetModel()->GetType() == ModelType::Protection)
					{

					}
					else if (model.GetModel()->GetType() == ModelType::Target)
					{
						finished = true;
					}
					else if (model.GetModel()->GetType() == ModelType::Key)
					{
						int curLane = Lane();
						openUpTaken = true;
						bridges.at(curLane)->Translate(characterCollision[0].first - 20, bridges.at(curLane)->GetY());
						model.GetModel()->Translate(-600, -600);
					}
					else if (model.GetModel()->GetType() == ModelType::DoubleCoin)
					{
						DoubleCoin();
						model.GetModel()->Translate(-600, -600);
						powerTaken = true;
					}

				}
			}
		}
	}

	return false;
}