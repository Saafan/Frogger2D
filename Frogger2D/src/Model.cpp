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

void drawCircle(int x, int y, float r) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	glColor3f(1.0f, 0.5f, 0.2f);
	GLUquadric* quadObj = gluNewQuadric();
	gluDisk(quadObj, 0, r, 50, 50);
	glPopMatrix();
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
		if (!texture)
			glColor3f(1.0f, 0.2f, 0.2f);
		glTexCoord2f(0, 0);
		glVertex2f(20.0f + x, 18.0f + y);
		glTexCoord2f(0, 1);
		glVertex2f(0.0f + x, 100.0f + y);
		glTexCoord2f(1, 1);
		glVertex2f(100.0f + x, 100.0f + y);
		glTexCoord2f(1, 0);
		glVertex2f(80.0f + x, 18.0f + y);
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
	else if (type == ModelType::Coin)
	{
		glPushMatrix();

		static float angle = 0.0f;

		glTranslatef(x + 50, y + 50, 0);
		glRotatef(angle, 0, 0, 1);
		angle += 1.0f;
		glTranslatef(-x - 50, -y - 50, 0);

		glBegin(GL_POLYGON);
		glColor3f(.95, .95, .95);
		glVertex2f(50 + x, 100 + y);
		glVertex2f(46.3566238516744 + x, 99.1960393892769 + y);
		glVertex2f(40.8960412738185 + x, 94.8991875247347 + y);
		glVertex2f(34.719316718539 + x, 88.9014984638111 + y);
		glVertex2f(32.6604085334458 + x, 84.6941643464468 + y);
		glVertex2f(29.1692163935052 + x, 80.1287592403706 + y);
		glVertex2f(27.4683791971239 + x, 76.1004606173622 + y);
		glVertex2f(26.2151307366324 + x, 72.6987862245996 + y);
		glVertex2f(23.5295983212934 + x, 69.3866295790149 + y);
		glVertex2f(20.6650304115986 + x, 65.0897777144727 + y);
		glVertex2f(18.3375689849715 + x, 60.4348548612185 + y);
		glVertex2f(16.5472140414122 + x, 57.3912514571678 + y);
		glVertex2f(13.1455396486496 + x, 50.1403139357527 + y);
		glVertex2f(12.2503621768699 + x, 48.8870654752612 + y);
		glVertex2f(10.7285604748445 + x, 42.5313054256257 + y);
		glVertex2f(10.2809717389547 + x, 33.5795307078293 + y);
		glVertex2f(11.8922911881581 + x, 25.5229334618126 + y);
		glVertex2f(14.8463768450309 + x, 19.2566911593551 + y);
		glVertex2f(18.6061222265054 + x, 16.4816409968382 + y);
		glVertex2f(21.2916546418443 + x, 13.4380375927875 + y);
		glVertex2f(26.5732017253442 + x, 11.0210584189824 + y);
		glVertex2f(31.0490890842425 + x, 8.3355260036435 + y);
		glVertex2f(35.0773877072509 + x, 7.4403485318639 + y);
		glVertex2f(38.5685798471915 + x, 6.2766178185503 + y);

		glVertex2f(41.0750767681745 + x, 4.2177096334572 + y);
		glVertex2f(44.0291624250473 + x, 2.5168724370759 + y);
		glVertex2f(47.6993900593438 + x, 1.7112127124742 + y);
		glVertex2f(49.4002272557252 + x, 1.0845884822284 + y);
		glVertex2f(52.443830659776 + x, 0.6369997463386 + y);
		glVertex2f(63.9916200457334 + x, 1.5321772181182 + y);
		glVertex2f(73.1224302578858 + x, 5.7395113354826 + y);
		glVertex2f(79.5677080546992 + x, 10.8420229246265 + y);
		glVertex2f(85.6549148628008 + x, 16.2130877553043 + y);
		 glVertex2f(90 + x, 20 + y);
		glVertex2f(92.9058523842159 + x, 25.5229334618126 + y);
		glVertex2f(94.8752428221311 + x, 32.057729005804 + y);
		glVertex2f(93.0848878785718 + x, 40.1143262518207 + y);
		glVertex2f(92.8163346370379 + x, 41.7256457010241 + y);
		glVertex2f(92.4582636483261 + x, 45.8434620712104 + y);
		glVertex2f(89.8622489801651 + x, 53.6315060756933 + y);
		glVertex2f(87.624305300716 + x, 58.1969111817694 + y);
		 glVertex2f(82.8798647002839 + x, 66.701097163676 + y);
		glVertex2f(78.9410838244535 + x, 73.4149282020233 + y);
		glVertex2f(75.091820695801 + x, 77.6222623193876 + y);
		glVertex2f(70.8844865784367 + x, 82.9933271500655 + y);
		glVertex2f(65.9610104836486 + x, 88.5434274750992 + y);
		 glVertex2f(63.0069248267758 + x, 92.2136551093957 + y);
		glVertex2f(60 + x, 95 + y);
		glVertex2f(57.6358599960979 + x, 96.7790602154719 + y);
			glVertex2f(53.6075613730895 + x, 98.8379684005651 + y);
		glEnd();

		glBegin(GL_QUADS);
		glColor3f(0.91, 0.07, 0.14);
		glVertex2f(60 + x - 5, 90 + y - 10);
		glVertex2f(60 + x - 5, 80 + y - 10);
		glVertex2f(70 + x - 5, 80 + y - 10);
		glVertex2f(70 + x - 5, 90 + y - 10);

		glEnd();

		glPopMatrix();
	}
	else if (type == ModelType::Lane)
	{
		glBegin(GL_POLYGON);
		glColor3f(color.R, color.G, color.B);
		glTexCoord2f(0, 0); glVertex2f(0, y);
		glVertex2f(10, 10 + y);
		glTexCoord2f(1, 1); glVertex2f(width, 10 + y);

		glVertex2f(width + 10, y);
		glTexCoord2f(1, 0); glVertex2f(width, -10 + y);
		glVertex2f(10, -10 + y);
		glEnd();

		glPointSize(3);
		glBegin(GL_POINTS);
		glColor3f(0.9, 0.9, 0.9);
		glVertex2f(20, y);
		glVertex2f(width - 10, y);
		glEnd();
	}
	else if (type == ModelType::Key)
	{
		glBegin(GL_QUADS);
		glColor3f(1.0f, 0.2f, 0.2f);
		glVertex2f(0.0f + x, 20.0f + y);
		glVertex2f(0.0f + x, 100.0f + y);
		glVertex2f(100.0f + x, 100.0f + y);
		glVertex2f(100.0f + x, 20.0f + y);
		glEnd();

		glBegin(GL_LINES);
		glColor3f(0.05, 0.26, 0.50);
		glVertex2f(50 + x - 10, 0 + y + 25);
		glVertex2f(50 + x - 10, 50 + y + 25);
		glVertex2f(50 + x - 10, 50 + y + 25);
		glVertex2f(40 + x - 10, 50 + y + 25);
		glVertex2f(40 + x - 10, 50 + y + 25);
		glVertex2f(60 + x - 10, 70 + y + 25);
		glVertex2f(60 + x - 10, 70 + y + 25);
		glVertex2f(80 + x - 10, 50 + y + 25);
		glVertex2f(80 + x - 10, 50 + y + 25);
		glVertex2f(70 + x - 10, 50 + y + 25);
		glVertex2f(70 + x - 10, 50 + y + 25);
		glVertex2f(70 + x - 10, 0 + y + 25);
		glVertex2f(70 + x - 10, 0 + y + 25);
		glVertex2f(50 + x - 10, 0 + y + 25);
		glEnd();

		glBegin(GL_TRIANGLES);
		glColor3f(0.03f, 0.17f, 0.33f);
		glVertex2f(80 + x, 80 + y);
		glVertex2f(100 + x, 100 + y);
		glVertex2f(80 + x, 100 + y);
		glEnd();


	}
	else if (type == ModelType::DoubleCoin)
	{

		glBegin(GL_QUADS);
		glColor3f(1.0f, 0.2f, 0.2f);
		glVertex2f(0.0f + x, 25.0f + y);
		glVertex2f(0.0f + x, 100.0f + y);
		glVertex2f(100.0f + x, 100.0f + y);
		glVertex2f(100.0f + x, 25.0f + y);
		glEnd();

		glPushMatrix();
		glLineWidth(10);
		glBegin(GL_LINES);
		glColor3f(0.05, 0.26, 0.50);
		glVertex2f(20 + x, 90 + y);
		glVertex2f(60 + x, 90 + y);
		glVertex2f(60 + x, 90 + y);
		glVertex2f(60 + x, 70 + y);
		glVertex2f(60 + x, 70 + y);
		glVertex2f(10 + x, 70 + y);
		glVertex2f(10 + x, 70 + y);
		glVertex2f(10 + x, 40 + y);
		glVertex2f(10 + x, 40 + y);
		glVertex2f(60 + x, 40 + y);
		glVertex2f(60 + x, 40 + y);
		glEnd();

		glBegin(GL_LINES);
		glColor3f(0.05, 0.26, 0.50);
		glVertex2f(60 + x, 90 + y - 20);
		glVertex2f(90 + x, 55 + y - 20);
		glVertex2f(90 + x, 90 + y - 20);
		glVertex2f(60 + x, 55 + y - 20);
		glEnd();

		glBegin(GL_TRIANGLES);
		glColor3f(0.03f, 0.17f, 0.33f);
		glVertex2f(80 + x, 80 + y);
		glVertex2f(100 + x, 100 + y);
		glVertex2f(80 + x, 100 + y);
		glEnd();

		glPopMatrix();

		glLineWidth(4);

	}
	else if (type == ModelType::Target)
	{

		glBegin(GL_QUADS);
		glColor3f(1.0f, 0.2f, 0.2f);
		glVertex2f(0.0f + x, 0.0f + y);
		glVertex2f(0.0f + x, 100.0f + y);
		glVertex2f(100.0f + x, 100.0f + y);
		glVertex2f(100.0f + x, 0.0f + y);
		glEnd();


		glBegin(GL_LINES);
		glColor3f(0.07, 0.16, 0.27);
		glVertex2f(55 + x - 5, 100 + y - 5);
		glVertex2f(45 + x - 5, 75 + y - 5);
		glVertex2f(45 + x - 5, 75 + y - 5);
		glVertex2f(15 + x - 5, 75 + y - 5);
		glVertex2f(15 + x - 5, 75 + y - 5);
		glVertex2f(35 + x - 5, 55 + y - 5);
		glVertex2f(35 + x - 5, 55 + y - 5);
		glVertex2f(15 + x - 5, 15 + y - 5);
		glVertex2f(15 + x - 5, 15 + y - 5);
		glVertex2f(55 + x - 5, 35 + y - 5);
		glVertex2f(55 + x - 5, 35 + y - 5);
		glVertex2f(95 + x - 5, 15 + y - 5);
		glVertex2f(95 + x - 5, 15 + y - 5);
		glVertex2f(75 + x - 5, 55 + y - 5);
		glVertex2f(75 + x - 5, 55 + y - 5);
		glVertex2f(95 + x - 5, 75 + y - 5);
		glVertex2f(95 + x - 5, 75 + y - 5);
		glVertex2f(65 + x - 5, 75 + y - 5);
		glVertex2f(65 + x - 5, 75 + y - 5);
		glVertex2f(55 + x - 5, 100 + y - 5);
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



