#ifndef _obj_H_
#define _obj_H_

#include <fstream>
#include <string>
#include <vector>
#include <glut.h>
using namespace std;

class obj {

private:
	vector<vector<GLfloat>>vetex;
	vector<vector<vector<GLint>>>face;
	vector<vector<GLfloat>>vn;
	vector<vector<GLfloat>>vt;
public:
	obj() {}
	obj(string path);
	void draw();
};

#endif // !_obj_H_