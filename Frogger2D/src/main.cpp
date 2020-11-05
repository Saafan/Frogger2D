#include "Renderer.h"
#include "Model.h"
#include "glut.h"
#include "Enemy.h"

Model frog(Primitive::Plane, 100.0f, 100.0f, 0.0f, 0.0f, Color{ 0.2f, 0.5f, 0.3f });
Model coll(Primitive::Plane, 500.0f, 100.0f, 300.0f, 300.0f, Color{ 1.0f, 1.0f, 1.0f }, ModelType::Collision);
Model street(Primitive::Plane, WIDTH, 330.0f , 0.0f, 110.0f, Color{ 0.3f, 0.3f, 0.3f }, ModelType::Environment);
Model rest(Primitive::Plane, WIDTH, 120.0f , 0.0f, 110.0f * 4, Color{ 0.8f, 0.3f, 0.3f }, ModelType::Collision);
Model water(Primitive::Plane, WIDTH, 330.0f , 0.0f, 110.0f * 5, Color{ 0.2f, 0.5f, 0.8f }, ModelType::Water);
Model goal(Primitive::Plane, WIDTH, 120.0f , 0.0f, 110.0f * 8, Color{ 0.96f, 0.81, 0.00f }, ModelType::Environment);


std::vector<Model*> models;
std::vector<Enemy*> enemies;

bool onLog;

bool CheckStaticCollision(float nextX, float nextY);
bool CheckEnemyCollision();



void GenerateEnemies()
{
	
}


void renderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	
	//glTranslatef(0, -100, 0);

	//#TODO Make view System
	
	//Environment
	GenerateEnemies();
	street.Render();
	rest.Render();
	water.Render();
	goal.Render();

	frog.Render();

	coll.Render();


	glPopMatrix();

	glutSwapBuffers();

	CheckEnemyCollision();

	glutPostRedisplay();
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

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	models.push_back(&frog);
	models.push_back(&coll);
	models.push_back(&rest);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Frogger 2D");

	gluOrtho2D(0, WIDTH, 0, HEIGHT);

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);

	glutKeyboardFunc(ProcessInput);
	glutIgnoreKeyRepeat(1);

	glutMainLoop();
	return 1;
}


bool CheckStaticCollision(float nextX, float nextY)
{

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
	for (Model* model : models)
	{
		ModelType type = model->GetType();
		if (type == ModelType::Enemy|| (type == ModelType::Water && !onLog))
		{
			if (frog.GetX() >= model->GetX() && frog.GetX() <= model->GetX() + model->GetWidth() - 1)
			{
				if (frog.GetY() >= model->GetY() && frog.GetY() <= model->GetY() + model->GetHeight() - 1)
				{
					std::cout << "DEATH!!" << std::endl;
					return true;
				}
			}
		}
	}
}
