#include "Object.h"



Object::Object(float width, float height, float x, float y, float speed, ModelType type)
{
	Color color = Color{ 0.8f, 0.2f, 0.0f };
	if (type == ModelType::Log)
	{
		color = Color{ 0.39, 0.21, 0.12 };
	}

	model = new Model(Primitive::Plane, width, height, x, y, color, type);
	
	speed = speed;
}

void Object::RenderObject()
{
	if (model->GetX() > WIDTH)
	{
 		delete model;
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
