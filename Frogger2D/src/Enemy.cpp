#include "Enemy.h"

Enemy::Enemy(float width, float height, float x, float y, float m_speed)
{
	Model instanceModel(Primitive::Plane, width, height, x, y, Color{0.8f, 0.2f, 0.0f}, ModelType::Enemy);
	model = instanceModel;
	speed = m_speed;
}

void Enemy::RenderEnemy()
{
	model.Render();
	model.TranslateAccum(speed, 0);
}

