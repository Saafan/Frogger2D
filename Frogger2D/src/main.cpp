#include "Renderer.h"
#include "Model.h"
#include "Object.h"
#include <stdio.h>
#include "Randomize.h"
#include "Texture.h"
#include <string>

Texture bordersTex(".\\images\\borders.png");
Texture grass_areaTex(".\\images\\grass_area.png");

Texture coinsTex(".\\images\\coin.png");
Texture keyTex(".\\images\\key.png");

Texture frogTex(".\\images\\frog.png");
Texture frog_protectedTex(".\\images\\frog_protected.png");
Texture powerUpProtectTex(".\\images\\protect.png");

Texture streetTex(".\\images\\street.png");

Texture carsTex(".\\images\\car.png");

Texture rest_collisionTex(".\\images\\rest_collision.png");
Texture rest_normalTex(".\\images\\rest_normal.png");

Texture waterTex(".\\images\\water.png");
Texture logsTex(".\\images\\logs.png");

Texture goal_areaTex(".\\images\\goal_area.png");
Texture targetTex(".\\images\\target.png");

Object key(100.0f, 100.0f, 250, 250, 0, ModelType::Key, &keyTex);
Object protection(100.0f, 100.0f, 500, 500, 0, ModelType::Protection, &powerUpProtectTex);
Object target(100.0f, 100.0f, 8 * 110, 8 * 110 + 10, 0, ModelType::Key, &targetTex);

Model border(Primitive::Plane, WIDTH + 40, HEIGHT + 40, -20.0f, -20.0f, Color{ 0.5f, 0.5f, 0.5f }, ModelType::Null, & bordersTex);
Model grass(Primitive::Plane, WIDTH, 110, 0, 0, Color{ 0.07, 0.42, 0.08 }, ModelType::Null, & grass_areaTex);
Model frog(Primitive::Plane, 100.0f, 100.0f, 0.0f, 0.0f, Color{ 0.2f, 0.5f, 0.3f }, ModelType::Character, & frogTex);
Model street(Primitive::Plane, WIDTH, 330.0f, 0.0f, 110.0f, Color{ 0.3f, 0.3f, 0.3f }, ModelType::Environment, & streetTex);
Model rest(Primitive::Plane, WIDTH, 120.0f, 0.0f, 110.0f * 4, Color{ 0.8f, 0.3f, 0.3f }, ModelType::Null, & rest_collisionTex);
Model water(Primitive::Plane, WIDTH, 330.0f, 0.0f, 110.0f * 5, Color{ 0.2f, 0.5f, 0.8f }, ModelType::Null, & waterTex);
Model goal(Primitive::Plane, WIDTH, 120.0f, 0.0f, 110.0f * 8, Color{ 0.07, 0.42, 0.08 }, ModelType::Goal, & goal_areaTex);

Model borderLane(Primitive::Plane, WIDTH, 10, 0, 0, Color{ 0.3, 0.3, 0.2 }, ModelType::Lane);

struct Vector2
{
	float x;
	float y;
};


std::vector<Model*> models;
std::vector<Object> objects;
std::vector<Vector2>  characterCollision;
std::vector<Model*> bridges;


bool firstTime = true;
bool CheckStaticCollision(float nextX, float nextY);
bool CheckEnemyCollision();

int score = 0;
bool opened = false;
bool powerTaken = false;
bool protect = false;

int laneNum = 5;

int Approx(int x)
{
	x = x / 10;
	x *= 10;
	return x;
}

void GenerateCollisionPoints()
{
	characterCollision.clear();
	characterCollision.push_back(Vector2{ frog.GetX() - 1, frog.GetY() + frog.GetHeight() + 1 });					//Top Left
	characterCollision.push_back(Vector2{ frog.GetX() + frog.GetWidth() + 1, frog.GetY() + frog.GetHeight() + 1 }); // Top Right
	characterCollision.push_back(Vector2{ frog.GetX() - 1, frog.GetY() - 1 });										// Bottom Left
	characterCollision.push_back(Vector2{ frog.GetX() + frog.GetWidth() + 1, frog.GetY() - 1 });					// Bottom Right

	glPointSize(3);
	glBegin(GL_POINTS);
	glVertex2f(frog.GetX() - 1, frog.GetY() + frog.GetHeight() + 1);
	glVertex2f(frog.GetX() + frog.GetWidth() + 1, frog.GetY() + frog.GetHeight() + 1);
	glVertex2f(frog.GetX() - 1, frog.GetY() - 1);
	glVertex2f(frog.GetX() + frog.GetWidth() + 1, frog.GetY() - 1);
	glEnd();

}

int CheckBordersCollision()
{
	if (frog.GetX() + 5 < 0)
		return 2;

	if (frog.GetX() + frog.GetWidth() > WIDTH - 10)
		return 1;

	if (frog.GetY() + 5 < 0)
		return 2;

	if (frog.GetY() + frog.GetHeight() > HEIGHT - 10)
		return 0;

	return -1;
}

bool CheckBridgeCollision()
{

	// Top Left     --> 0
	// Top Right    -->	1
	// Bottom Left  -->	2
	// Bottom Right --> 3

	bool top = false;
	bool bottom = false;
	for (size_t i = 0; i < bridges.size(); i++)
	{
		if (bridges.at(i)->GetX() < characterCollision.at(0).x && bridges.at(i)->GetX() + bridges.at(i)->GetWidth() > characterCollision.at(0).x)
		{
			if (bridges.at(i)->GetX() < characterCollision.at(1).x && bridges.at(i)->GetX() + bridges.at(i)->GetWidth() > characterCollision.at(1).x)
			{
				if (bridges.at(i)->GetY() < characterCollision.at(0).y && bridges.at(i)->GetY() + bridges.at(i)->GetHeight() > characterCollision.at(0).y)
				{
					if (bridges.at(i)->GetY() < characterCollision.at(1).y && bridges.at(i)->GetY() + bridges.at(i)->GetHeight() > characterCollision.at(1).y)
					{
						std::cout << "TOP ISNIDE BRIDGEEE" << std::endl;
						top = true;
					}
				}
			}
		}

		if (bridges.at(i)->GetX() < characterCollision.at(2).x && bridges.at(i)->GetX() + bridges.at(i)->GetWidth() > characterCollision.at(2).x)
		{
			if (bridges.at(i)->GetX() < characterCollision.at(3).x && bridges.at(i)->GetX() + bridges.at(i)->GetWidth() > characterCollision.at(3).x)
			{
				if (bridges.at(i)->GetY() < characterCollision.at(2).y && bridges.at(i)->GetY() + bridges.at(i)->GetHeight() > characterCollision.at(2).y)
				{
					if (bridges.at(i)->GetY() < characterCollision.at(3).y && bridges.at(i)->GetY() + bridges.at(i)->GetHeight() > characterCollision.at(3).y)
					{
						std::cout << " BOTTOM ISNIDE BRIDGEEE" << std::endl;
						bottom = true;
					}
				}
			}
		}
	}

	if (top || bottom)
		return true;
	else
		return false;

}


int CheckLaneCollision()
{
	static bool onBridge = false;
	for (size_t i = models.size() - 1; i > 0; i--)
	{
		glBegin(GL_POINTS);
		glVertex2f(models.at(i)->GetX(), models.at(i)->GetY() - 15);
		glVertex2f(models.at(i)->GetX() + models.at(i)->GetWidth(), models.at(i)->GetY() + models.at(i)->GetHeight());
		glEnd();
	}

	for (size_t i = 0; i < bridges.size(); i++)
	{

	}

	for (size_t i = 0; i < models.size(); i++)
	{
		for (size_t j = 0; j < characterCollision.size(); j++)
		{
			if (models.at(i)->GetX() < characterCollision.at(j).x && models.at(i)->GetX() + models.at(i)->GetWidth() > characterCollision.at(j).x)
			{
				if (models.at(i)->GetY() - 15 < characterCollision.at(j).y && models.at(i)->GetY() - 15 + models.at(i)->GetHeight() > characterCollision.at(j).y)
				{

					if (models.at(i)->GetType() == ModelType::Lane)
					{
						std::cout << "Collided WITH SOMETHING" << std::endl;
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
	Model leftBorder(Primitive::Plane, 20.0f, HEIGHT + 40, -20.0f, -20, Color{}, ModelType::Collision, nullptr);
	Model rightBorder(Primitive::Plane, 20.0f, HEIGHT + 40, WIDTH, -20, Color{}, ModelType::Collision, nullptr);
	Model upBorder(Primitive::Plane, WIDTH, 20, 0, HEIGHT, Color{}, ModelType::Collision, nullptr);
	Model downBorder(Primitive::Plane, WIDTH, 20, 0, -20, Color{}, ModelType::Collision, nullptr);
	Model triangle1(Primitive::Triangle, 20, 20, -20, -20, Color{ 0.5, 0.5, 0.5 });
	Model triangle2(Primitive::Triangle, 20, 20, -20, HEIGHT, Color{ 0.5, 0.5, 0.5 });
	Model triangle3(Primitive::Triangle, 20, 20, WIDTH, -20, Color{ 0.5, 0.5, 0.5 });
	Model triangle4(Primitive::Triangle, 20, 20, WIDTH, HEIGHT, Color{ 0.5, 0.5, 0.5 });

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
		models.push_back(new Model(Primitive::Plane, WIDTH - 10, 20, 0, Approx((HEIGHT / laneNum) * (i + 1)), Color{ 0.3, 0.3, 0.2 }, ModelType::Lane));
		Model* Bridge = new Model(Primitive::Plane, 200, 110, WIDTH / 2 + Randomize(-400, 400), ((HEIGHT / laneNum) * (i + 1)) - 55, Color{ 0.9, 0.2, 0.2 }, ModelType::Bridge);

		models.push_back(Bridge);
		bridges.push_back(Bridge);
	}
}

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.5f, 0.8f, 1.0f);

	std::string line;

	if (firstTime)
	{
		RenderLanes();
		firstTime = false;
	}

	//Environment
	//RenderCharacter();
	RenderBorders();

	CheckLaneCollision();

	for (size_t i = 0; i < models.size(); i++)
	{
		models[i]->Render();
	}
	frog.Render();

	glutSwapBuffers();
	CheckEnemyCollision();
	glutPostRedisplay();

}

void GenerateKeyProtection()
{

}

void GenerateCoins()
{
	std::vector<float> prev;

	for (size_t i = 0; i < 25; i += 2)
	{
		float randX = Randomize(0, 16);
		float randY = Randomize(1, 3);
		for (size_t i = 0; i < prev.size(); i++)
		{
			if (prev.at(i) == randX)
			{
				i--;
				continue;
			}
			else
				prev.push_back(randX);
		}
		Object coin(100, 100, 0, 0, 0, ModelType::Coin, &coinsTex);

		if (i < 12)
			coin.GetModel()->Translate(110 * randX, 110 * randY + 5);
		else
			coin.GetModel()->Translate(110 * randX, 110 * (randY + 4) + 5);

		objects.emplace_back(coin);

	}
}

void Restart()
{
	// 	for (auto& object : objects)
	// 	{
	// 		if (object.GetModel()->GetType() == ModelType::Coin)
	// 			object.empty = true;
	// 	}
	// 
	// 
	// 	score = 0;
	// 	rest.SetType(ModelType::Collision);
	// 	opened = false;
	// 	powerTaken = false;
	// 	frog.SetColor(Color{ 0.2f, 0.5f, 0.3f });
	// 
	// 	rest.texture = &rest_collisionTex;
	// 	frog.texture = &frogTex;
	// 
	// 	GenerateCoins();
	// 	GenerateKeyProtection();
}

void Tick(int value)
{

	glutTimerFunc(1500, Tick, 0);
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
	GenerateKeyProtection();
	glutTimerFunc(5000, TickKeyProtection, 0);
}

void Protect()
{
	frog.SetColor(Color{ 0.7, 0.05, 0.05 });
	protect = true;
	frog.texture = &frog_protectedTex;
}

void UnProtect(int value)
{
	frog.SetColor(Color{ 0.2f, 0.5f, 0.3f });
	protect = false;
	frog.texture = &frogTex;
}


void ProcessInput(unsigned char key, int x_f, int y_f)
{

	float x = frog.GetX();
	float y = frog.GetY();
	int point = CheckLaneCollision();

	if (point == -1)
		point = CheckBordersCollision();

	// Top Left     ==> 0
	// Top Right    ==>	1
	// Bottom Left  ==>	2
	// Bottom Right ==> 3

	bool onBridge = CheckBridgeCollision();

	if (onBridge)
		point = -1;

	if (key == 's')
		if (point == 2 || point == 3)
			return;
		else
			frog.TranslateAccum(0, -10);

	if (key == 'w')
		if (point == 0 || point == 1)
			return;
		else
			frog.TranslateAccum(0, 10);

	if (key == 'a')
		if (point == 0 || point == 2)
			return;
		else
			frog.TranslateAccum(-10, 0);

	if (key == 'd')
		if (point == 1 || point == 3)
			return;
		else
			frog.TranslateAccum(10, 0);

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

	if (laneNum > (HEIGHT / frog.GetHeight()))
	{
		std::cout << "You Exceeded the Maximum Number of Lanes :(" << std::endl;
		exit(1);
	}

	models.push_back(&frog);



	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Frogger 2D");

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);


	coinsTex.LoadTexture();
	keyTex.LoadTexture();
	powerUpProtectTex.LoadTexture();
	frogTex.LoadTexture();
	streetTex.LoadTexture();
	carsTex.LoadTexture();
	rest_collisionTex.LoadTexture();
	rest_normalTex.LoadTexture();
	frog_protectedTex.LoadTexture();
	targetTex.LoadTexture();
	waterTex.LoadTexture();
	logsTex.LoadTexture();
	goal_areaTex.LoadTexture();
	grass_areaTex.LoadTexture();
	bordersTex.LoadTexture();

	glutTimerFunc(0, Tick, 500);
	glutTimerFunc(0, TickKeyProtection, 10);
	glutTimerFunc(10, TickSpeed, 10);

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





bool CheckEnemyCollision()
{

	bool onLog = false;

	for (Object model : objects)
	{
		if (frog.GetX() + frog.GetWidth() >= model.GetModel()->GetX() && frog.GetX() - 10 <= model.GetModel()->GetX() + model.GetModel()->GetWidth())
		{
			if (frog.GetY() + 5 >= model.GetModel()->GetY() && frog.GetY() <= model.GetModel()->GetY() + model.GetModel()->GetHeight())
			{
				if (model.GetModel()->GetType() == ModelType::Lane)
				{
					if (!protect)
					{
						std::cout << "DEATH!!" << std::endl;
						frog.Translate(0, 0);
						Restart();
						return true;
					}
				}
				else if (model.GetModel()->GetType() == ModelType::Log && frog.GetX() - model.GetModel()->GetX() > -5 && frog.GetX() - model.GetModel()->GetX() + frog.GetWidth() < model.GetModel()->GetWidth())
					onLog = true;
				else if (model.GetModel()->GetType() == ModelType::Coin)
				{
					score++;
					model.GetModel()->Translate(-600, -600);
				}
				else if (model.GetModel()->GetType() == ModelType::Key)
				{
					model.GetModel()->Translate(-600, -600);
					rest.SetType(ModelType::Null);
					rest.texture = &rest_normalTex;
					opened = true;
				}
				else if (model.GetModel()->GetType() == ModelType::Protection)
				{
					model.GetModel()->Translate(-600, -600);
					powerTaken = true;
					Protect();
					glutTimerFunc(3000, UnProtect, 0);
				}

			}
		}
	}

	// 	if (frog.GetX() >= water.GetX() && frog.GetX() - 10 <= water.GetX() + water.GetWidth())
	// 	{
	// 		if (frog.GetY() + 5 >= water.GetY() && frog.GetY() + 10 <= water.GetY() + water.GetHeight())
	// 		{
	// 			if (!onLog)
	// 			{
	// 				std::cout << "Water" << std::endl;
	// 				frog.Translate(0, 0);
	// 				Restart();
	// 				return true;
	// 			}
	// 		}
	// 	}

	return false;
}