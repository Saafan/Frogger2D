#pragma once
#include "Model.h"

class Object
{
public:
	Object(float width, float height, float x, float y, float speed, ModelType type = ModelType::Enemy, Texture* texture = nullptr);
	void RenderObject();

	void MoveObject(float value);
	Model* GetModel();
	bool empty = false;
private:
	Model* model;
	Texture* texture = nullptr;
	float speed = 0.0f;
};