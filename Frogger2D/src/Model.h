#pragma once
#include "Renderer.h"

enum class Primitive
{
	Plane = 0,
	Triangle = 1,
};

enum class ModelType
{
	Character,
	Environment,
	Collision,
	Goal,
	Enemy,
	Key,
	Protection,
	Water,
	Log,
	Coin,
	Null,
	Lane,
	Bridge,
	DoubleCoin,
	Target
};

struct Color
{
	float R = 1.0f, G = 1.0f, B = 1.0f;
};



class Model
{

public:
	Model(Primitive prim = Primitive::Plane, float width = 0.0f, float height = 0.0f, float x = 0.0f, float y = 0.0f, Color color = { 1.0f, 1.0f, 1.0f }, ModelType type = ModelType::Null, Texture* tex = nullptr);

	void RenderPlane();
	void RenderTriangle();
	void Render();

	void Translate(float x, float y);
	void TranslateAccum(float x, float y);
	void Rotate(float f_angle);
	void RotateAccum(float f_angle, float x, float y, float z);

	void SetType(ModelType f_type);

	ModelType GetType() const { return type; }

	float GetWidth() const { return width; }
	float GetHeight() const { return height; }
	float GetX() const { return x; }
	float GetY() const { return y; }

	Primitive GetPrimitive() const { return prim; }
	void SetPrimitive(Primitive val) { prim = val; }

	void SetTextureCoord(float U, float W);
	float* GetTextureCoord();
	void SetColor(Color f_color);
	Texture* texture = nullptr;

private:
	Primitive prim;

	float x = 0.0f;
	float y = 0.0f;

	float width = 0.0f;
	float height = 0.0f;

	float coord[2] ={1.0f, 1.0f};

	float angle = 0.0f;

	float rotX = 0.0f;
	float rotY = 0.0f;
	float rotZ = 0.0f;


	Color color;
	ModelType type;
};