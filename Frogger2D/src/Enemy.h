#pragma once
#include "Model.h"

class Enemy
{
public:
	Enemy(float width, float height, float x, float y, float speed);
	void RenderEnemy();
private: 
	float speed = 0.0f;
	Model model;

};