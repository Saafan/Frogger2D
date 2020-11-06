#pragma once
#include "Model.h"

class Object
{
public:
	Object(float width, float height, float x, float y, float speed, ModelType type = ModelType::Enemy);
	void RenderObject();

	void MoveObject(float value);
	Model* GetModel();
	bool empty = false;
private:
	Model* model;
	float speed = 0.0f;
};