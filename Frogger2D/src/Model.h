#pragma once
#include "Renderer.h"

enum class Primitive
{
	Plane = 0,
	Triangle = 1,
};

enum class ModelType
{
	Environment,
	Collision,
	Enemy,
	Key,
	Protection,
	Water,
	Log,
	null
};

struct Color
{
	float R = 1.0f, G = 1.0f, B = 1.0f;
};



class Model
{

public:
	Model(Primitive prim = Primitive::Plane, float width = 0.0f, float height = 0.0f, float x = 0.0f, float y = 0.0f, Color color = { 1.0f, 1.0f, 1.0f }, ModelType type = ModelType::null);

	void RenderPlane();
	void RenderTriangle();
	void Render();

	void Translate(float x, float y);
	void TranslateAccum(float x, float y);
	void Rotate(float f_angle);
	void RotateAccum(float f_angle);

	void SetType(ModelType f_type);

	ModelType GetType() const { return type; }

	float GetWidth() const { return width; }
	float GetHeight() const { return height; }
	float GetX() const { return x; }
	float GetY() const { return y; }

private:
	Primitive prim;

	float x = 0.0f;
	float y = 0.0f;
	
	float width = 0.0f;
	float height = 0.0f;

	float angle = 0.0f;

	Color color;
	ModelType type;
};