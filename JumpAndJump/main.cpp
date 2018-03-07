#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include "GLee/GLee.h"
#include <glut.h>
#include "Maths/Maths.h"
#include "TIMER.h"
#include "FPS_COUNTER.h"
#include "scene.h"
#include "Box.h"
#include "main.h"
#include "Texture.h"

#define FRAME 1000

double patrick_x = -1;
double patrick_y = 0.575;
double patrick_z = -1;
double patrick_angle = 0;
double patrick_rotation = 0;
int n = 0;
int score = 0;
double Box_y = 0.325f;

int state = 0;
int frame = 0;
int strength = 0;
double boxScale = 1;
static char GameOver[9] = "GameOver";
static char Restart[36] = "Push Enter to restart o(- -+)o";
static char Score[10];

std::vector <Box> Boxs;

//Frames per second counter
FPS_COUNTER fpsCounter;

//Camera & light positions
VECTOR3D cameraPosition(-2.5f, 3.5f, -2.5f);
VECTOR3D lightPosition(-2.0f, 4.0f, 2.0f);

//Size of shadow map
const int shadowMapSize = 512;

//Textures
GLuint shadowMapTexture;

//window size
int windowWidth, windowHeight;

//Matrices
MATRIX4X4 lightProjectionMatrix, lightViewMatrix;
MATRIX4X4 cameraProjectionMatrix, cameraViewMatrix;

texture* text;

int counter = 0;
int pre_counter = 0;
long c = 0;

void newBox()
{
	Box box = Box();
	Boxs.back().direction = rand() % 2;
	box.type = rand() % 4 + 1;
	box.length = (rand() % 500 + 250) / 1000.0;
	if (Boxs.back().direction == 0)
	{
		box.x = Boxs.back().x;
		box.y = Boxs.back().y + Boxs.back().length + box.length + rand() % 500 / 1000.0;
	}
	else
	{
		box.x = Boxs.back().x + Boxs.back().length + box.length + rand() % 500 / 1000.0;
		box.y = Boxs.back().y;
	}
	box.texture = rand() % 16;
	Boxs.push_back(box);
}

void initTexure()
{
	string path = "texture/";
	text = (texture*)malloc(sizeof(texture) * 18);
	for (unsigned int i = 0; i < 18; i++)
	{
		text[i] = texture((char *)((path + to_string(i + 1) + ".bmp")).c_str());
	}
}

//Called for initiation
bool Init(void)
{
	//Check for necessary extensions
	if (!GLEE_ARB_depth_texture || !GLEE_ARB_shadow)
	{
		printf("I require ARB_depth_texture and ARB_shadow extensionsn\n");
		return false;
	}

	//Load identity modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Shading states
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//Depth states
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	//We use glScale when drawing the scene
	glEnable(GL_NORMALIZE);

	//Create the shadow map texture
	glGenTextures(1, &shadowMapTexture);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapSize, shadowMapSize, 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	//Use the color as the ambient and diffuse material
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	//White specular material color, shininess 16
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 16.0f);

	//Calculate & save matrices
	glPushMatrix();

	glLoadIdentity();
	gluPerspective(45.0f, (float)windowWidth / windowHeight, 1.0f, 100.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraProjectionMatrix);

	glLoadIdentity();
	gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraViewMatrix);

	glLoadIdentity();
	gluPerspective(45.0f, 1.0f, 2.0f, 8.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, lightProjectionMatrix);

	glLoadIdentity();
	gluLookAt(lightPosition.x, lightPosition.y, lightPosition.z,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, lightViewMatrix);

	glPopMatrix();

	return true;
}

//Called to draw scene
void Display(void)
{
	if (c + 300  <= clock())
	{
		c = clock();
		if (counter != 0)
		{
			if (state == 0 && counter > 1)
			{
				printf("%lf %lf %lf %lf\n", patrick_x, patrick_y, patrick_z, patrick_angle);
				printf("state1\n");
				state = 1;
				frame = 0;
			}
			if (state == 1 && counter == pre_counter)
			{
				printf("state2\n");
				state = 2;
				strength = counter;
				if (strength > 50)
					strength = 50;
				//printf("%d\n", strength);
				counter = 0;
				frame = 0;
			}
		}
		pre_counter = counter;
	}

	if (state == 1)//Push Space,compress
	{
		strength = counter;
		if (strength > 50)
			strength = 50;
		boxScale = 1.0 - strength / 100.0;
	}
	else if (state == 2)//Realease Space,rebound
	{
		boxScale += 0.01;
		if (boxScale >= 1)
		{
			boxScale = 1;
			frame = FRAME;
			//printf("frame:%d\n",frame);
			state = 3;
			//printf("state3\n");
		}
	}
	else if(state == 3)//Jump
	{
		if (Boxs[n].direction == 0)
		{
			patrick_x += (Boxs[n + 1].x - patrick_x) / frame;
			patrick_y += (frame * 2 - 1001) * 5.0 / FRAME / FRAME;
			patrick_z += 0.00006 * strength;
			patrick_angle += 360.0 / FRAME;
		}
		else
		{
			patrick_x += 0.00006 * strength;
			patrick_y += (frame * 2 - 1001) * 5.0 / FRAME / FRAME;
			patrick_z += (Boxs[n + 1].y - patrick_z) / frame;
			patrick_angle += 360.0 / FRAME;
		}
		frame--;
		if (frame == 0)
		{
			//printf("%d\n", Boxs[n].direction);
			//printf("%lf %lf %lf %lf\n", patrick_x, patrick_y, patrick_z, patrick_angle);
			patrick_angle = 0;
			if (Boxs[n].direction == 0)
			{
				if (patrick_z <= Boxs[n].y + Boxs[n].length / 2)
				{
					state = 0;
				}
				else if (patrick_z < Boxs[n].y + Boxs[n].length / 2 + 0.1)
				{
					patrick_angle = 1;
					state = 5;
				}
				else if (patrick_z <= Boxs[n + 1].y - Boxs[n + 1].length / 2 - 0.1)
				{
					state = 6;
				}
				else if (patrick_z < Boxs[n + 1].y - Boxs[n + 1].length / 2)
				{
					patrick_angle = -1;
					state = 5;
				}
				else if (patrick_z <= Boxs[n + 1].y + Boxs[n + 1].length / 2)
				{
					state = 4;
					newBox();
					n++;
					score++;
					frame = 3 * FRAME / 2;
					Box_y = 0.325 + 0.001 * FRAME / 2;
					//printf("%d %d %lf\n", n, Boxs[n].direction, patrick_rotation);
				}
				else if (patrick_z < Boxs[n + 1].y + Boxs[n + 1].length / 2 + 0.1)
				{
					patrick_angle = 1;
					state = 5;
				}
				else
				{
					state = 6;
				}
			}
			else
			{
				if (patrick_x <= Boxs[n].x + Boxs[n].length / 2)
				{
					state = 0;
				}
				else if (patrick_x < Boxs[n].x + Boxs[n].length / 2 + 0.05)
				{
					patrick_angle = -1;
					state = 5;
				}
				else if (patrick_x <= Boxs[n + 1].x - Boxs[n + 1].length / 2 - 0.1)
				{
					state = 6;
				}
				else if (patrick_x < Boxs[n + 1].x - Boxs[n + 1].length / 2)
				{
					patrick_angle = 1;
					state = 5;
				}
				else if (patrick_x <= Boxs[n + 1].x + Boxs[n + 1].length / 2)
				{
					state = 4;
					newBox();
					n++;
					score++;
					frame = 3 * FRAME / 2;
					Box_y = 0.325 + 0.001 * FRAME / 2;
					//printf("%d %d %lf\n", n, Boxs[n].direction, patrick_rotation);
				}
				else if (patrick_x < Boxs[n + 1].x + Boxs[n + 1].length / 2 + 0.1)
				{
					patrick_angle = -1;
					state = 5;
				}
				else
				{
					state = 6;
				}
				//printf("%d\n", state);
			}
		}
	}
	else if (state == 4)//arrive next box
	{
		if (frame > FRAME)
			Box_y -= 1.0 / FRAME;
		else if(frame > FRAME / 2)
			Box_y += 1.0 / FRAME;
		else if (frame > 0)
			Box_y -= 1.0 / FRAME;
		else
		{
			if (Boxs[n].direction == 0)
				patrick_rotation = 0;
			else
				patrick_rotation = 90;
			state = 7;
		}
		if (Boxs[n].direction == 0 && patrick_rotation != 0)
			patrick_rotation -= 60.0 / FRAME;
		else if (Boxs[n].direction == 1 && patrick_rotation != 90)
			patrick_rotation += 60.0 / FRAME;
		frame--;
	}
	else if (state == 5)//rotation
	{
		if (patrick_angle > 0)
			patrick_angle += 0.5;
		else if (patrick_angle < 0)
			patrick_angle -= 0.5;
		if (patrick_angle >= 90 || patrick_angle <= -90)
			state = 6;
	}
	else if (state == 6)//drop
	{
		//printf("%lf\n", patrick_y);
		if (patrick_y > 0.1)
			patrick_y -= 1.0 / FRAME;
		else
			state = 8;
	}
	else if (state == 7)
	{
		if (Boxs[n].x > -0.5)
		{
			for (unsigned int i = 0; i < Boxs.size(); i++)
			{
				Boxs[i].x -= 1.0 / FRAME;
			}
			patrick_x -= 1.0 / FRAME;
		}
		else if (Boxs[n].y > -0.5)
		{
			for (unsigned int i = 0; i < Boxs.size(); i++)
			{
				Boxs[i].y -= 1.0 / FRAME;
			}
			patrick_z -= 1.0 / FRAME;
		}
		else
		{
			while (Boxs[0].x > 2.0 && Boxs[0].y > 2.0)
			{
				Boxs.erase(Boxs.begin());
			}
			state = 0;
		}
	}
	//First pass - from light's point of view
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(lightProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(lightViewMatrix);

	//Use viewport the same size as the shadow map
	glViewport(0, 0, shadowMapSize, shadowMapSize);

	//Draw back faces into the shadow map
	glCullFace(GL_FRONT);

	//Disable color writes, and use flat shading for speed
	glShadeModel(GL_FLAT);
	glColorMask(0, 0, 0, 0);

	//Draw the scene
	DrawScene(0);

	//Read the depth buffer into the shadow map texture
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, shadowMapSize, shadowMapSize);

	//restore states
	glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);
	glColorMask(1, 1, 1, 1);

	//2nd pass - Draw from camera's point of view
	glClear(GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(cameraProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(cameraViewMatrix);

	glViewport(0, 0, windowWidth, windowHeight);

	//Use dim light to represent shadowed areas
	glLightfv(GL_LIGHT1, GL_POSITION, VECTOR4D(lightPosition));
	glLightfv(GL_LIGHT1, GL_AMBIENT, white*0.2f);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white*0.2f);
	glLightfv(GL_LIGHT1, GL_SPECULAR, black);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);

	DrawScene(0);

	//3rd pass
	//Draw with bright light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white);

	//Calculate texture matrix for projection
	//This matrix takes us from eye space to the light's clip space
	//It is postmultiplied by the inverse of the current view matrix when specifying texgen
	static MATRIX4X4 biasMatrix(0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f);	//bias from [-1, 1] to [0, 1]
	MATRIX4X4 textureMatrix = biasMatrix * lightProjectionMatrix*lightViewMatrix;

	//Set up texture coordinate generation.
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_S, GL_EYE_PLANE, textureMatrix.GetRow(0));
	glEnable(GL_TEXTURE_GEN_S);

	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_T, GL_EYE_PLANE, textureMatrix.GetRow(1));
	glEnable(GL_TEXTURE_GEN_T);

	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_R, GL_EYE_PLANE, textureMatrix.GetRow(2));
	glEnable(GL_TEXTURE_GEN_R);

	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_Q, GL_EYE_PLANE, textureMatrix.GetRow(3));
	glEnable(GL_TEXTURE_GEN_Q);

	//Bind & enable shadow map texture
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glEnable(GL_TEXTURE_2D);

	//Enable shadow comparison
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE);

	//Shadow comparison should be true (ie not in shadow) if r<=texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);

	//Shadow comparison should generate an INTENSITY result
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);

	//Set alpha test to discard false comparisons
	glAlphaFunc(GL_GEQUAL, 0.99f);
	glEnable(GL_ALPHA_TEST);

	DrawScene(1);

	//Disable textures and texgen
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);

	//Restore other states
	glDisable(GL_LIGHTING);
	glDisable(GL_ALPHA_TEST);



	//Update frames per second counter
	fpsCounter.Update();

	//Print fps
	static char fpsString[32];
	sprintf(fpsString, "FPS: %.2f", fpsCounter.GetFps());
	sprintf(Score, "Score: %d", score);

	//Set matrices for ortho
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//Print text
	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos2f(-1.0f, 0.9f);
	for (unsigned int i = 0; i<strlen(fpsString); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, fpsString[i]);

	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos2f(0.5f, 0.9f);
	for (unsigned int i = 0; i<strlen(Score); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, Score[i]);

	if (state == 8)
	{
		glRasterPos2f(-0.25f, 0.1f);
		for (unsigned int i = 0; i<strlen(GameOver); i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, GameOver[i]);
		glRasterPos2f(-0.5f, 0.0f);
		for (unsigned int i = 0; i<strlen(Restart); i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, Restart[i]);
	}

	//reset matrices
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glFinish();
	glutSwapBuffers();
	glutPostRedisplay();
}

//Called on window resize
void Reshape(int w, int h)
{
	//Save new window size
	windowWidth = w, windowHeight = h;

	//Update the camera's projection matrix
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(45.0f, (float)windowWidth / windowHeight, 1.0f, 100.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraProjectionMatrix);
	glPopMatrix();
}

//Called when a key is pressed
void Keyboard(unsigned char key, int x, int y)
{
	//If escape is pressed, exit
	//printf("%d", key);
	if (key == 27)
		exit(0);
	if (key == 13 && state == 8)
	{
		state = 0;
		patrick_x = -1;
		patrick_y = 0.575;
		patrick_z = -1;
		patrick_angle = 0;
		n = 0;
		score = 0;
		Box_y = 0.325f;
		frame = 0;
		strength = 0;
		boxScale = 1;
		counter = 0;
		pre_counter = 0;
		c = 0;

		Boxs.clear();
		Box box = Box();
		box.type = rand() % 4 + 1;
		box.length = (rand() % 500 + 250) / 1000.0;
		box.texture = rand() % 16;
		box.x = -1;
		box.y = -1;
		Boxs.push_back(box);
		newBox();
		newBox();

		if (Boxs[0].direction == 0)
			patrick_rotation = 0;
		else
			patrick_rotation = 90;
	}
	if (key == ' ')
	{
		counter++;
		//printf("%d %d\n", counter, pre_counter);
	}
}

int main(int argc, char** argv)
{
	srand((unsigned)time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 512);
	glutCreateWindow("Jump and Jump");

	Box box = Box();
	box.type = rand() % 4 + 1;
	box.length = (rand() % 500 + 250) / 1000.0;
	box.texture = rand() % 16;
	box.x = -1;
	box.y = -1;
	Boxs.push_back(box);
	newBox();
	newBox();
	//printf("%d\n", Boxs[0].direction);

	if (Boxs[0].direction == 0)
		patrick_rotation = 0;
	else
		patrick_rotation = 90;
	
	initTexure();

	if (!Init())
		return 0;

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	c = clock();
	glutMainLoop();
	return 0;
}