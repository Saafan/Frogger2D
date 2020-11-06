#include "Model.h"

Model::Model(Primitive prim, float width, float height, float x, float y, Color color, ModelType type)
	: prim{ prim }, x{ x }, y{ y }, width{ width }, height{ height }, color{ color }, type{ type }
{
	
}


void Model::RenderTriangle()
{
	glPushMatrix();


	glTranslatef(-x, -y, 0.0f);
	glRotatef(13, 0, 0, 1);
	glTranslatef(x, y, 0.0f);

	glTranslatef(x, y, 0.0f);

	glBegin(GL_TRIANGLES);
	glColor3f(color.R, color.G, color.B);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(width, 0.0f, 0.0f);
	glVertex3f(width / 2, height, 0.0f);
	glEnd();

	glPopMatrix();
}


void Model::Render()
{
	if (GetPrimitive() == Primitive::Plane)
		RenderPlane();
	if (GetPrimitive() == Primitive::Triangle)
		RenderTriangle();
}

void Model::Translate(float f_x, float f_y)
{
	x = f_x;
	y = f_y;
}

void Model::TranslateAccum(float f_x, float f_y)
{
	x += f_x;
	y += f_y;
}

void Model::Rotate(float f_angle)
{
	angle = f_angle;
}

void Model::RotateAccum(float f_angle, float x, float y, float z)
{
	angle += f_angle;

	rotX = x;
	rotY = y;
	rotZ = z;
}

void Model::SetType(ModelType f_type)
{
	type = f_type;
}



void Model::RenderPlane()
{
	glPushMatrix();

	glTranslatef(x, y, 0.0f);
	glRotatef(angle, 0, 1, 0);

	glBegin(GL_QUADS);
	glColor3f(color.R, color.G, color.B);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, height, 0.0f);
	glVertex3f(width, height, 0.0f);
	glVertex3f(width, 0.0f, 0.0f);
	glEnd();

	glPopMatrix();
}


