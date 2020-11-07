#include "Model.h"
#include "Texture.h"

Model::Model(Primitive prim, float width, float height, float x, float y, Color color, ModelType type, Texture* f_tex)
	: prim{ prim }, x{ x }, y{ y }, width{ width }, height{ height }, color{ color }, type{ type }, texture{ f_tex }
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



void Model::SetTextureCoord(float f_U, float f_W)
{
	coord[0] = f_U;
	coord[1] = f_W;
}

float* Model::GetTextureCoord()
{
	return coord;
}

void Model::SetColor(Color f_color)
{
	color = f_color;
}


void Model::RenderPlane()
{
	if (texture != NULL)
	{
		if (!texture->empty)
		{
			texture->Bind();
			glColor3f(1.0f, 1.0f, 1.0f);
		}
		else
			glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
		glBindTexture(GL_TEXTURE_2D, 0);

		glPushMatrix();

	glTranslatef(x, y, 0.0f);

	glRotatef(angle, rotX, rotY, rotZ);

	glBegin(GL_QUADS);

	if (texture == NULL || texture->empty)
	{
		glColor3f(color.R, color.G, color.B);
	}

	glTexCoord2f(0.0f, 0.0f);	glVertex2f(0.0f, 0.0f);

	glTexCoord2f(0.0f, coord[1]);	glVertex2f(0.0f, height);

	glTexCoord2f(coord[0], coord[1]);	glVertex2f(width, height);

	glTexCoord2f(coord[0], 0.0f);	glVertex2f(width, 0.0f);
	glEnd();

	glPopMatrix();
}


