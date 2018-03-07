#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include "GLee\GLee.h"
#include "glut.h"

using namespace std;

class texture {
public:
	texture();
	texture(char *);
	void texload(char *);
	GLuint tex[1];
private:
};