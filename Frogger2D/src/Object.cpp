#include "Object.h"



Object::Object(float width, float height, float x, float y, float speed, ModelType type, Texture* texture)
{
	Color color = Color{ 0.8f, 0.2f, 0.0f };
	if (type == ModelType::Log)
	{
		color = Color{ 0.39, 0.21, 0.12 };
	}
	if (type == ModelType::Coin)
	{
		color = Color{ 0.96f, 0.81, 0.0f };
	}
	if (type == ModelType::Key)
	{
		color = Color{ 0.7f, 0.7f, 0.7f };
	}
	if (type == ModelType::Protection)
	{
		color = Color{ 0.2f, 0.6, 0.7f };
	}

	model = new Model(Primitive::Plane, width, height, x, y, color, type, texture);
	
	speed = speed;
}

void Object::RenderObject()
{
	if (model->GetX() > WIDTH)
	{
		empty = true;
		return;
	}
	model->Render();
}

void Object::MoveObject(float value)
{
	model->TranslateAccum(value, 0);
}

Model* Object::GetModel()
{
	return model;
}
