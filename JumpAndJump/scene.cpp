#define WIN32_LEAN_AND_MEAN
#include <vector>
#include <windows.h>
#include <stdio.h>
#include <glut.h>
#include "Box.h"
#include "Maths/Maths.h"
#include "scene.h"
#include "Texture.h"
#include "obj.h"

obj patrick = obj("obj/patrick.obj");
obj cube = obj("obj/cube.obj");
obj cone = obj("obj/cone.obj");
obj cylinder = obj("obj/cylinder.obj");
obj ear = obj("obj/ear.obj");
extern std::vector <Box> Boxs;
extern texture* text;
extern double patrick_x;
extern double patrick_y;
extern double patrick_z;
extern double patrick_angle;
extern double patrick_rotation;
extern int n;
extern double boxScale;
extern double Box_y;
//extern double rotation;

void DrawScene(int flag)
{
	static GLuint baseList = 0, patrickList = 0;

	//Create base if necessary
	if (!baseList)
	{
		baseList = glGenLists(1);
		glNewList(baseList, GL_COMPILE);
		{
			glColor3f(1.0f, 1.0f, 1.0f);
			glPushMatrix();

			glScalef(5.0f, 0.05f, 5.0f);
			glutSolidCube(3.0f);

			glPopMatrix();
		}
		glEndList();
	}
	glCallList(baseList);
	if (!patrickList)
	{
		patrickList = glGenLists(1);
		glNewList(patrickList, GL_COMPILE);
		{
			patrick.draw();
		}
		glEndList();
	}
	unsigned int i;
	for (i = 0; i < Boxs.size() - 2; i++)
	{
		if (Boxs[i].list == -1)
		{
			
			Boxs[i].list = glGenLists(1);
			Boxs[i].list = Boxs[i].list;
			glNewList(Boxs[i].list, GL_COMPILE);
			{
				glRotatef(-90, 1.0f, 0.0f, 0.0f);
				glScalef(Boxs[i].length / 2, Boxs[i].length / 2, 0.25f);
				if (Boxs[i].type == 1)
					cube.draw();
				else if (Boxs[i].type == 2)
				{
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					cylinder.draw();
				}
				else if (Boxs[i].type == 3)
				{
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					cone.draw();
				}
				else
				{
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
					ear.draw();
				}
			}
			glEndList();
		}
		glPushMatrix();
		if (i == n)
		{
			glScalef(1.0f, boxScale, 1.0f);
			glTranslatef(Boxs[i].x, 0.075f + 0.25f * boxScale, Boxs[i].y);
		}
		else
			glTranslatef(Boxs[i].x, 0.325f, Boxs[i].y);
		if (flag == 1)
		{
			glEnable(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, text[Boxs[i].texture].tex[0]);
		}
		glCallList(Boxs[i].list);
		glPopMatrix();
	}

	if (Boxs[i].list == -1)
	{

		Boxs[i].list = glGenLists(1);
		Boxs[i].list = Boxs[i].list;
		glNewList(Boxs[i].list, GL_COMPILE);
		{
			glRotatef(-90, 1.0f, 0.0f, 0.0f);
			glScalef(Boxs[i].length / 2, Boxs[i].length / 2, 0.25f);
			if (Boxs[i].type == 1)
				cube.draw();
			else if (Boxs[i].type == 2)
			{
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				cylinder.draw();
			}
			else if (Boxs[i].type == 3)
			{
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				cone.draw();
			}
			else
			{
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
				ear.draw();
			}
		}
		glEndList();
	}
	glPushMatrix();
	glTranslatef(Boxs[i].x, Box_y, Boxs[i].y);
	if (flag == 1)
	{
		glEnable(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, text[Boxs[i].texture].tex[0]);
	}
	glCallList(Boxs[i].list);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(patrick_x, (patrick_y - 0.075) * boxScale + 0.075f, patrick_z);
	glScalef(1.0f, boxScale, 1.0f);
	if(Boxs[n].direction == 0)
		glRotatef(patrick_angle, 1.0f, 0.0f, 0.0f);
	else
		glRotatef(patrick_angle, 0.0f, 0.0f, 1.0f);
	glRotatef(patrick_rotation, 0.0f, 1.0f, 0.0f);
	glScalef(0.2f, 0.2f, 0.2f);
	if (flag == 1)
	{
		glEnable(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, text[16].tex[0]);
	}
	glCallList(patrickList);
	glPopMatrix();
}