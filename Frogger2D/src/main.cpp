#include "Renderer.h"
#include "Model.h"
#include "Object.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl2.h>
#include <imgui/imgui_impl_glut.h>
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
Model frog(Primitive::Plane, 100.0f, 100.0f, 0.0f, 0.0f, Color{ 0.2f, 0.5f, 0.3f }, ModelType::Null, & frogTex);
Model street(Primitive::Plane, WIDTH, 330.0f, 0.0f, 110.0f, Color{ 0.3f, 0.3f, 0.3f }, ModelType::Environment, & streetTex);
Model rest(Primitive::Plane, WIDTH, 120.0f, 0.0f, 110.0f * 4, Color{ 0.8f, 0.3f, 0.3f }, ModelType::Collision, & rest_collisionTex);
Model water(Primitive::Plane, WIDTH, 330.0f, 0.0f, 110.0f * 5, Color{ 0.2f, 0.5f, 0.8f }, ModelType::Water, & waterTex);
Model goal(Primitive::Plane, WIDTH, 120.0f, 0.0f, 110.0f * 8, Color{ 0.07, 0.42, 0.08 }, ModelType::Goal, & goal_areaTex);

std::vector<Model*> models;
std::vector<Object> objects;

bool firstTime = true;
bool CheckStaticCollision(float nextX, float nextY);
bool CheckEnemyCollision();

int score = 0;
bool opened = false;
bool powerTaken = false;
bool protect = false;



void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGLUT_NewFrame();

	std::string line;




	{
		ImGui::Begin("Frogger2D");

		ImGui::Text("This is your score: %i", score);

		ImGui::SliderFloat2("Street Tiling: ", street.GetTextureCoord(), 0.1f, 10.0f);
		ImGui::SliderFloat2("Border Tiling: ", border.GetTextureCoord(), 0.1f, 10.0f);
		ImGui::SliderFloat2("Grass Area Tiling: ", grass.GetTextureCoord(), 0.1f, 10.0f);
		ImGui::SliderFloat2("Rest Tiling: ", rest.GetTextureCoord(), 0.1f, 10.0f);
		ImGui::SliderFloat2("Water Tiling: ", water.GetTextureCoord(), 0.1f, 10.0f);
		ImGui::SliderFloat2("Goal Area Tiling: ", goal.GetTextureCoord(), 0.1f, 10.0f);


		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::End();
	}

	border.Render();

	glPushMatrix();


	//Environment
	street.Render();
	rest.Render();
	water.Render();
	goal.Render();
	grass.Render();
	target.GetModel()->Render();
	//Enemies
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i].empty)
			objects.erase(objects.begin() + i);

		objects[i].RenderObject();
	}

	frog.Render();

	glPopMatrix();

	ImGui::Render();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	glutSwapBuffers();
	CheckEnemyCollision();
	glutPostRedisplay();

}

void GenerateKeyProtection()
{
	float randX2 = 0.0f;
	float randX1 = 0.0f;
	float randY1 = 0.0f;
	float randY2 = 0.0f;
	bool repeated = false;
	do
	{
		randX2 = Randomize(0, 16);
		randX1 = Randomize(0, 16);
		randY1 = Randomize(1, 3);
		randY2 = Randomize(1, 3);
		repeated = false;
		for (auto& model : objects)
		{
			if (model.GetModel()->GetType() == ModelType::Coin)
			{
				float powerX = 110 * randX1;
				float powerY = 110 * randY1 + 5;

				if (model.GetModel()->GetX() == powerX && model.GetModel()->GetY() == powerY)
					repeated = true;
			}
		}
	} while (randX2 == randX1 || repeated);
	if (!opened)
		key.GetModel()->Translate(110 * randX1, 110 * randY1 + 5);
	else
		key.GetModel()->Translate(-600, -600);

	if (!powerTaken)
		protection.GetModel()->Translate(110 * randX2, 110 * randY2 + 5);
	else
		protection.GetModel()->Translate(-600, -600);
}

void GenerateCoins()
{
	std::vector<float> prev;

	for (size_t i = 0; i < 25; i++)
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
	for (auto& object : objects)
	{
		if (object.GetModel()->GetType() == ModelType::Coin)
			object.empty = true;
	}


	score = 0;
	rest.SetType(ModelType::Collision);
	opened = false;
	powerTaken = false;
	frog.SetColor(Color{ 0.2f, 0.5f, 0.3f });

	rest.texture = &rest_collisionTex;
	frog.texture = &frogTex;

	GenerateCoins();
	GenerateKeyProtection();
}

void Tick(int value)
{
	if (firstTime)
	{
		GenerateCoins();
		GenerateKeyProtection();
		objects.emplace_back(key);
		objects.emplace_back(protection);

		objects.emplace_back(Object(200, 100, rand() % 1000 + 400, 115, 0.6, ModelType::Enemy, &carsTex));
		objects.emplace_back(Object(200, 100, rand() % 1000 + 400, 110 * 2 + 5, 0.6, ModelType::Enemy, &carsTex));
		objects.emplace_back(Object(200, 100, rand() % 1000 + 400, 110 * 3 + 5, 0.6, ModelType::Enemy, &carsTex));

		objects.emplace_back(Object(200, 100, rand() % 300, 115, 0.6, ModelType::Enemy, &carsTex));
		objects.emplace_back(Object(200, 100, rand() % 300, 110 * 2 + 5, 0.6, ModelType::Enemy, &carsTex));
		objects.emplace_back(Object(200, 100, rand() % 300, 110 * 3 + 5, 0.6, ModelType::Enemy, &carsTex));

		firstTime = false;
	}

	objects.emplace_back(Object(200, 100, -(rand() % 500 + 250), 115, 0.6, ModelType::Enemy, &carsTex));
	objects.emplace_back(Object(200, 100, -(rand() % 500 + 250), 110 * 2 + 5, 0.6, ModelType::Enemy, &carsTex));
	objects.emplace_back(Object(200, 100, -(rand() % 500 + 250), 110 * 3 + 5, 0.6, ModelType::Enemy, &carsTex));

	objects.insert(objects.begin(), Object(400, 100, -(rand() % 700 + 450), 110 * 5 + 5, 0.4, ModelType::Log, &logsTex));
	objects.insert(objects.begin(), Object(400, 100, -(rand() % 700 + 450), 110 * 6 + 5, 0.4, ModelType::Log, &logsTex));
	objects.insert(objects.begin(), Object(400, 100, -(rand() % 700 + 450), 110 * 7 + 5, 0.4, ModelType::Log, &logsTex));

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
	frog.SetColor(Color{ 0.7,0.05, 0.05 });
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

	models.push_back(&frog);
	models.push_back(&rest);
	models.push_back(&water);


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Frogger 2D");

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

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

	// Setup Platform/Renderer bindings
	ImGui_ImplGLUT_Init();
	ImGui_ImplGLUT_InstallFuncs();
	ImGui_ImplOpenGL2_Init();

	glutTimerFunc(0, Tick, 500);
	glutTimerFunc(0, TickKeyProtection, 10);
	glutTimerFunc(10, TickSpeed, 10);

	gluOrtho2D(-20, WIDTH + 20, -20, HEIGHT + 20);

	glutDisplayFunc(renderScene);
	//glutIdleFunc(renderScene);

	glutKeyboardFunc(ProcessInput);
	glutSpecialFunc(ProcessSpecialInput);
	glutIgnoreKeyRepeat(1);
	glutMainLoop();



	return 1;
}


bool CheckStaticCollision(float nextX, float nextY)
{
	if (nextX > WIDTH - 100 || nextX < 0 || nextY > HEIGHT - 100 || nextY < 0)
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
				if (model.GetModel()->GetType() == ModelType::Enemy)
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

	if (frog.GetX() >= water.GetX() && frog.GetX() - 10 <= water.GetX() + water.GetWidth())
	{
		if (frog.GetY() + 5 >= water.GetY() && frog.GetY() + 10 <= water.GetY() + water.GetHeight())
		{
			if (!onLog)
			{
				std::cout << "Water" << std::endl;
				frog.Translate(0, 0);
				Restart();
				return true;
			}
		}
	}

	return false;
}