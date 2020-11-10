#include "Model.h"
#include "Texture.h"

Model::Model(Primitive prim, float width, float height, float x, float y, Color color, ModelType type, Texture* f_tex)
	: prim{ prim }, x{ x }, y{ y }, width{ width }, height{ height }, color{ color }, type{ type }, texture{ f_tex }
{

}


void Model::RenderTriangle()
{
	glPushMatrix();


	glBegin(GL_TRIANGLES);
	glColor3f(color.R, color.G, color.B);
	glVertex3f(x, y, 0.0f);
	glVertex3f(width + x, y, 0.0f);
	glVertex3f(width / 2 + x, height + y, 0.0f);
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

	if (type == ModelType::Character)
	{
		glPushMatrix();

		glBegin(GL_QUADS);
		glColor3f(1.0f, 0.2f, 0.2f);
		glVertex2f(0.0f + x, 0.0f + y);
		glVertex2f(0.0f + x, 100.0f + y);
		glVertex2f(100.0f + x, 100.0f + y);
		glVertex2f(100.0f + x, 0.0f + y);
		glEnd();

		glBegin(GL_TRIANGLES);
		glColor3f(0.98f, 0.93f, 0.67f);
		glVertex2f(50.0f + x, 60.0f + y);
		glVertex2f(30.0f + x, 40.0f + y);
		glVertex2f(70.0f + x, 40.0f + y);
		glEnd();

		glPointSize(10);

		glBegin(GL_POINTS);
		glVertex2f(6.0f * 5 + x, 16.0f * 5 + y);
		glVertex2f(14.0f * 5 + x, 16.0f * 5 + y);
		glEnd();

		glLineWidth(3);

		glBegin(GL_LINES);
		glVertex2f(4 * 5 + x, 4 * 5 + y);
		glVertex2f(4 * 5 + x, 2 * 5 + y);
		glEnd();

		glBegin(GL_LINES);
		glVertex2f(4 * 5 + x, 4 * 5 + y);
		glVertex2f(6 * 5 + x, 2 * 5 + y);
		glEnd();

		glBegin(GL_LINES);
		glVertex2f(4 * 5 + x, 4 * 5 + y);
		glVertex2f(2 * 5 + x, 2 * 5 + y);
		glEnd();


		glBegin(GL_LINES);
		glVertex2f(4 * 5 + x, 4 * 5 + y);
		glEnd();

		glBegin(GL_LINES);
		glVertex2f(4 * 5 + x, 4 * 5 + y);
		glVertex2f(4 * 5 + x, 2 * 5 + y);
		glEnd();

		glBegin(GL_LINES);
		glVertex2f(4 * 5 + x, 4 * 5 + y);
		glVertex2f(6 * 5 + x, 2 * 5 + y);
		glEnd();

		glBegin(GL_LINES);
		glVertex2f(4 * 5 + x, 4 * 5 + y);
		glVertex2f(2 * 5 + x, 2 * 5 + y);
		glEnd();

		glBegin(GL_LINES);
		glVertex2f(16 * 5 + x, 4 * 5 + y);
		glVertex2f(16 * 5 + x, 2 * 5 + y);
		glEnd();

		glBegin(GL_LINES);
		glVertex2f(16 * 5 + x, 4 * 5 + y);
		glVertex2f(18 * 5 + x, 2 * 5 + y);
		glEnd();

		glBegin(GL_LINES);
		glVertex2f(16 * 5 + x, 4 * 5 + y);
		glVertex2f(14 * 5 + x, 2 * 5 + y);
		glEnd();
	}
	else if (type == ModelType::Lane)
	{
		glBegin(GL_POLYGON);
		glColor3f(color.R, color.G, color.B);
		glVertex2f(0, y);
		glVertex2f(10, 10 + y);
		glVertex2f(width - 10, 10 + y);

		glVertex2f(width, y);
		glVertex2f(width - 10, -10 + y);
		glVertex2f(10, -10 + y);
		glEnd();
	}
	else
	{
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
}



