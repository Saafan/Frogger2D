#include "Renderer.h"
#include "Model.h"
#include "Object.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl2.h>
#include <imgui/imgui_impl_glut.h>


Model frog(Primitive::Plane, 100.0f, 100.0f, 0.0f, 0.0f, Color{ 0.2f, 0.5f, 0.3f });
Model street(Primitive::Plane, WIDTH, 330.0f, 0.0f, 110.0f, Color{ 0.3f, 0.3f, 0.3f }, ModelType::Environment);
Model rest(Primitive::Plane, WIDTH, 120.0f, 0.0f, 110.0f * 4, Color{ 0.8f, 0.3f, 0.3f }, ModelType::null);
Model water(Primitive::Plane, WIDTH, 330.0f, 0.0f, 110.0f * 5, Color{ 0.2f, 0.5f, 0.8f }, ModelType::Water);
Model goal(Primitive::Plane, WIDTH, 120.0f, 0.0f, 110.0f * 8, Color{ 0.96f, 0.81, 0.0f }, ModelType::Environment);
Model coin(Primitive::Plane, 100.0f, 100.0f, 0.0f, 0.0f, Color{ 0.2f, 0.5f, 0.3f });


std::vector<Model*> models;
std::vector<Object> objects;

bool firstTime = true;
bool CheckStaticCollision(float nextX, float nextY);
bool CheckEnemyCollision();


void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	//Environment
	street.Render();
	rest.Render();
	water.Render();
	goal.Render();


	//Enemies
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i].empty)
			objects.erase(objects.begin() + i);

		objects[i].RenderObject();
	}

	frog.Render();

	glPopMatrix();

	glutSwapBuffers();

	CheckEnemyCollision();

	glutPostRedisplay();
}

void Tick(int value)
{
	if (firstTime)
	{

		objects.emplace_back(Object(200, 100, rand() % 1000 + 400, 115, 0.6));
		objects.emplace_back(Object(200, 100, rand() % 1000 + 400, 110 * 2 + 5, 0.6));
		objects.emplace_back(Object(200, 100, rand() % 1000 + 400, 110 * 3 + 5, 0.6));

		objects.emplace_back(Object(200, 100, rand() % 300, 115, 0.6));
		objects.emplace_back(Object(200, 100, rand() % 300, 110 * 2 + 5, 0.6));
		objects.emplace_back(Object(200, 100, rand() % 300, 110 * 3 + 5, 0.6));

		firstTime = false;
	}

	objects.emplace_back(Object(200, 100, -(rand() % 500 + 250), 115, 0.6));
	objects.emplace_back(Object(200, 100, -(rand() % 500 + 250), 110 * 2 + 5, 0.6));
	objects.emplace_back(Object(200, 100, -(rand() % 500 + 250), 110 * 3 + 5, 0.6));

	objects.emplace_back(Object(400, 100, -(rand() % 700 + 450), 110 * 5 + 5, 0.4, ModelType::Log));
	objects.emplace_back(Object(400, 100, -(rand() % 700 + 450), 110 * 6 + 5, 0.4, ModelType::Log));
	objects.emplace_back(Object(400, 100, -(rand() % 700 + 450), 110 * 7 + 5, 0.4, ModelType::Log));

	glutTimerFunc(1500, Tick, 0);
}

void TickSpeed(int value)
{
	for (Object enemy : objects)
	{
		enemy.MoveObject(value);
	}
	glutTimerFunc(10, TickSpeed, 10);
}

void ProcessInput(unsigned char key, int x_f, int y_f)
{
	float x = frog.GetX();
	float y = frog.GetY();

	if (key == 's')
		if (CheckStaticCollision(x, y - 110))
			return;
		else
			frog.TranslateAccum(0, -110);

	if (key == 'w')
		if (CheckStaticCollision(x, y + 110))
			return;
		else
			frog.TranslateAccum(0, 110);

	if (key == 'a')
		if (CheckStaticCollision(x - 110, y))
			return;
		else
			frog.TranslateAccum(-110, 0);

	if (key == 'd')
		if (CheckStaticCollision(x + 110, y))
			return;
		else
			frog.TranslateAccum(110, 0);

	if (key == '`')
		exit(1);

	glutPostRedisplay();
}

int main(int argc, char** argv)
{

	models.push_back(&frog);
	models.push_back(&rest);
	models.push_back(&water);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Frogger 2D");

	glutTimerFunc(0, Tick, 500);
	glutTimerFunc(10, TickSpeed, 10);

	gluOrtho2D(0, WIDTH, 0, HEIGHT);

	glutDisplayFunc(renderScene);
	//glutIdleFunc(renderScene);

	glutKeyboardFunc(ProcessInput);
	glutIgnoreKeyRepeat(1);
	glutMainLoop();


	return 1;
}


bool CheckStaticCollision(float nextX, float nextY)
{
	for (Model* model : models)
	{
		if (model->GetType() == ModelType::Enemy)
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
				if (model.GetModel()->GetType() == ModelType::Enemy)
				{
					std::cout << "DEATH!!" << std::endl;
					frog.Translate(0, 0);
					return true;
				}
				else if (model.GetModel()->GetType() == ModelType::Log && frog.GetX() - model.GetModel()->GetX() > -5 && frog.GetX() - model.GetModel()->GetX() + frog.GetWidth()  < model.GetModel()->GetWidth())
					onLog = true;
				
			}
		}
	}

	if (frog.GetX() >= water.GetX() && frog.GetX() - 10 <= water.GetX() + water.GetWidth())
	{
		if (frog.GetY() + 5 >= water.GetY() && frog.GetY() + 10 <= water.GetY() + water.GetHeight())
		{
			if (!onLog)
			{
				std::cout << "Water" << std::endl;
 				frog.Translate(0, 0);
				return true;
			}
		}
	}

	return false;
}
