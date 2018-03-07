#include "obj.h"
#include"texture.h"
#include <fstream>
#include <iostream>
using namespace std;

obj::obj(string path)
{
	fstream f;
	string line;

	f.open(path, ios::in);

	while (!f.eof()) {
		getline(f, line);

		string word = "";
		vector<string>p;

		line = line + " ";
		for (int i = 0; i < line.length(); i++) {

			char ch = line[i];
			if (ch != ' ') {
				word += ch;
			}
			else {
				p.push_back(word);
				word = "";
			}

		}
		if (p.size() == 4) {

			if (p[0] == "v") {
				vector<GLfloat>point;
				for (int i = 1; i < 4; i++) {

					GLfloat xyz = atof(p[i].c_str());

					point.push_back(xyz);
				}
				vetex.push_back(point);
			}

			if (p[0] == "f") {

				vector<vector<GLint>>index1;

				for (int i = 1; i < 4; i++) {
					string x = p[i];
					string ans = "";
					vector<GLint>index2;
					for (int j = 0; j < x.length(); j++) {
						char ch = x[j];
						if (ch == '/' || j == x.length() - 1) {
							if (j == x.length() - 1)
								ans += ch;
							GLint index = atof(ans.c_str());
							index--;
							index2.push_back(index);
							ans = "";
						}
						else {
							ans += ch;
						}
					}
					if (index2.size() == 3)
						index1.push_back(index2);
				}
				if (index1.size() == 3)
					face.push_back(index1);
			}

			if (p[0] == "vn") {
				vector<GLfloat>VN;
				for (int i = 1; i < 4; i++) {
					GLfloat vnxyz = atof(p[i].c_str());
					VN.push_back(vnxyz);
				}
				vn.push_back(VN);
			}
		}

		if (p.size() == 3 && p[0] == "vt") {
			vector<GLfloat>VT;
			for (int i = 1; i < 3; i++) {
				GLfloat vtxyz = atof(p[i].c_str());
				VT.push_back(vtxyz);
			}
			vt.push_back(VT);

		}

	}
	f.close();
}

void obj::draw()
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < face.size(); i++) {

		GLfloat SV1[3];
		GLfloat	SV2[3];
		GLfloat SV3[3];  //点
		GLfloat VN1[3];
		GLfloat VN2[3];
		GLfloat VN3[3];   //法向量
		GLfloat VT1[2];
		GLfloat VT2[2];
		GLfloat VT3[2];   //贴图坐标


		GLint firstVertexIndex = ((face[i])[0])[0];
		GLint secondVertexIndex = ((face[i])[1])[0];
		GLint thirdVertexIndex = ((face[i])[2])[0];

		GLint firstvtIndex = ((face[i])[0])[1];
		GLint secondvtIndex = ((face[i])[1])[1];
		GLint thirdvtIndex = ((face[i])[2])[1];

		GLint firstvnIndex = ((face[i])[0])[2];
		GLint secondvnIndex = ((face[i])[1])[2];
		GLint thirdvnIndex = ((face[i])[2])[2];

		SV1[0] = (vetex[firstVertexIndex])[0];
		SV1[1] = (vetex[firstVertexIndex])[1];
		SV1[2] = (vetex[firstVertexIndex])[2];

		SV2[0] = (vetex[secondVertexIndex])[0];
		SV2[1] = (vetex[secondVertexIndex])[1];
		SV2[2] = (vetex[secondVertexIndex])[2];

		SV3[0] = (vetex[thirdVertexIndex])[0];
		SV3[1] = (vetex[thirdVertexIndex])[1];
		SV3[2] = (vetex[thirdVertexIndex])[2];

		VT1[0] = (vt[firstvtIndex])[0];
		VT1[1] = (vt[firstvtIndex])[1];

		VT2[0] = (vt[secondvtIndex])[0];
		VT2[1] = (vt[secondvtIndex])[1];

		VT3[0] = (vt[thirdvtIndex])[0];
		VT3[1] = (vt[thirdvtIndex])[1];

		VN1[0] = (vn[firstvnIndex])[0];
		VN2[0] = (vn[secondvnIndex])[0];
		VN3[0] = (vn[thirdvnIndex])[0];

		VN1[1] = (vn[firstvnIndex])[1];
		VN2[1] = (vn[secondvnIndex])[1];
		VN3[1] = (vn[thirdvnIndex])[1];

		VN1[2] = (vn[firstvnIndex])[2];
		VN2[2] = (vn[secondvnIndex])[2];
		VN3[2] = (vn[thirdvnIndex])[2];

		glNormal3f(VN1[0], VN1[1], VN1[2]);
		glTexCoord2f(VT1[0], VT1[1]);
		glVertex3f(SV1[0], SV1[1], SV1[2]);  //画点

		glNormal3f(VN2[0], VN2[1], VN2[2]);
		glTexCoord2f(VT2[0], VT2[1]);
		glVertex3f(SV2[0], SV2[1], SV2[2]);

		glNormal3f(VN3[0], VN3[1], VN3[2]);
		glTexCoord2f(VT3[0], VT3[1]);
		glVertex3f(SV3[0], SV3[1], SV3[2]);

	}
	glEnd();
}

