#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;

GLint xRotate = 0;
GLint yRotate = 0;
GLint zRotate = 0;

GLint clickDown = 0;
GLint fixX = 0;
GLint fixY = 0;

class CPoint2f {
public:
	vector <float> d{ 0, 0 };
};

class CPoint3f {
public:
	vector <float> d{ 0, 0, 0 };      // v, vn ó�� �Ǽ��� x,y,z��ǥ�� ������ ����   �Ѹ���� ��
};

class CPoint2i {
public:
	vector <int> d{ 0, 0 };        // f�� ������ 2���� ������ ����      
};

class CFace {
public:
	vector <CPoint2i> v_pairs;    // �ؿ� f ���ε� ��) 2//1 �̷��� ��� �ϳ��� ������ ��  
};

class CObj {                           // v, vn, f �� ������ ���͵��� ��Ƶ� ������Ʈ
public:
	
	vector <CPoint3f> v;
	vector <CPoint3f> vn;
	vector <CFace> f;
};

class CModel {
public:
	CObj objs;                  // CObj�� ������ ����      objs[0], objs[1], objs[2] .....

	vector<float> my_strtok_f(char* str, char* delimeter) {             //����(���⼱ �Ǽ�) str���� delimeter�� �������� ������ vector���·� ����
		vector <float> v;
		char* context;
		char* tok = strtok_s(str, delimeter, &context);                   //���� str���� delimeter�� ������ �� �պκ� tok�� ���� �� ������ context�� ����

		while (tok != NULL) {                                                         // �� �̻� ���� �� ������ �� ���� v�� tok ���� �� ������ 
			v.push_back(atof(tok));
			tok = strtok_s(context, delimeter, &context);
		}
		return v;
	}

	vector<string> my_strtok_s(char* str, char* delimeter) {
		vector <string> v;
		char* context;
		char* tok = strtok_s(str, delimeter, &context);

		while (tok != NULL) {
			v.push_back(tok);
			tok = strtok_s(context, delimeter, &context);
		}
		return v;
	}

	vector<int> my_strtok_i(char* str, char* delimeter) {
		vector <int> v;
		char* context;
		char* tok = strtok_s(str, delimeter, &context);

		while (tok != NULL) {
			v.push_back(atoi(tok));
			tok = strtok_s(context, delimeter, &context);
		}
		return v;
	}

	void loadObj(ifstream& fin) {
		string line;                              // ���Ͽ��� �� �� �о���� ���� ����
	                                                      //������Ʈ �ϳ� �ҷ��� �ӽ÷� ����
				                             // �� �� �ҷ��Դ��� üũ
		while (getline(fin, line)) {       // line�� �� �� �� ����
			int len = line.length();   // len�� �� ���� ����
			vector<float> vf;
			vector<string> s;	
			vector<int> vi;
			CPoint3f p3;
			CPoint2f p2;
			CPoint2i p2i;
			

			if (line[0] == 'v' && line[1] == ' ') {
				vf = my_strtok_f((char*)line.substr(2).c_str(), (char*)" ");
				p3.d = { vf[0], vf[1], vf[2] };
				objs.v.push_back(p3);
			}

			else if (line[0] == 'v' && line[1] == 'n') {
				vf = my_strtok_f((char*)line.substr(3).c_str(), (char*)" ");
				p3.d = { vf[0], vf[1], vf[2] };
				objs.vn.push_back(p3);
			}

			else if (line[0] == 'f' && line[1] == ' ') {
				s = my_strtok_s((char*)line.substr(2).c_str(), (char*)" ");    // s�� 3��° ���ں��� ������ �������� ������ vector���·� ����
				int nVertexes = s.size();                                                                 // s ������ ���� -- ���⼱ vectorũ��
				CFace face_tmp;                                                                           
				for (int i = 0; i < nVertexes; i++) {                                                  // ���� ������ŭ �ݺ�, /�� �������� ������ vector����
					vi = my_strtok_i((char*)s[i].c_str(), (char*)"//");
					p2i.d = { vi[0], vi[1] };
					face_tmp.v_pairs.push_back(p2i);
				}
				objs.f.push_back(face_tmp);
			}
		}
	}
};

CModel m;



void MyDisplay() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();   //  ���� ��ȯ ��� �ʱ�ȭ �� ũ�� 1.5�� ����
	glScalef(0.8f, 0.8f, 0.8f);

	glRotatef(xRotate, 1.0, 0.0, 0.0);
	glRotatef(yRotate, 0.0, 1.0, 0.0);
	glRotatef(zRotate, 0.0, 0.0, 1.0);

	GLfloat x, y, z, nx, ny, nz;
	int v_id, vn_id;
	int nFaces = m.objs.f.size();

	for (int k = 0; k < nFaces; k++) {                  //k�� �� ��° f������ �ǹ�
		int nPoints = m.objs.f[k].v_pairs.size();                //objs.f[f������ŭ].v_pairs : f �ϳ� �� ���� ���� (���⼱ 3��), �� ����� nPoints�� 3
		glBegin(GL_POLYGON);
			for (int i = 0; i < nPoints; i++) {
				v_id = m.objs.f[k].v_pairs[i].d[0];              // �� ��° v���� ����
				vn_id = m.objs.f[k].v_pairs[i].d[1];
				x = m.objs.v[v_id - 1].d[0];                       // �ε����� 0���� �����ϰ� v_id�� 1���� �����ϹǷ� 1 ����
				y = m.objs.v[v_id - 1].d[1];
				z = m.objs.v[v_id - 1].d[2];

				nx = m.objs.vn[vn_id - 1].d[0];
				ny = m.objs.vn[vn_id - 1].d[1];
				nz = m.objs.vn[vn_id - 1].d[2];
				glNormal3f(nx, ny, nz);
				glVertex3f(x, y, z);
			}
		glEnd();
	}
	glFlush();
}

void MyKeyboard(unsigned char KeyPressed, int X, int Y)
{
	switch (KeyPressed) {
	case 'Z':
	case 'z':
		xRotate += 2;
		if (xRotate > 360)
			xRotate -= 360;
		break;
	case 'X':
	case 'x':
		yRotate += 2;
		if (yRotate > 360)
			yRotate -= 360;
		break;
	case 'C':
	case 'c':
		zRotate += 2;
		if (zRotate > 360)
			zRotate -= 360;
		break;
	case 'Q':
	case 'q':
		exit(0); break;
	case 32:  //space key
		xRotate = 0;
		yRotate = 0;
		zRotate = 0;
		break;
	case 27: //esc key
		exit(0);
		break;
	case 3: //ctrl + c
		exit(0);
		break;
	}

	glutPostRedisplay();
}

void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y)
{
	if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN)
	{
		fixX = X;
		fixY = Y;
		clickDown = 1;
	}
	if (Button == GLUT_LEFT_BUTTON && State == GLUT_UP)
	{
		clickDown = 0;
	}
}

void MyMouseMove(GLint X, GLint Y)
{
	if (clickDown == 1)
	{
		xRotate = xRotate + (fixX - X);
		yRotate = yRotate + (fixY - Y);
		fixX = X;
		fixX = Y;
	}
	glutPostRedisplay();
}

GLfloat light_position[] = { 4.0, 10.0, 1.0, 0.0 };

GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };

GLfloat light0ambient[] = { 0.8, 0.8, 0.8, 1.0 };

GLfloat light0specular[] = { 0.8, 0.8, 0.8, 1.0 };

int main(int argc, char** argv)
{

	ifstream fin("C:/Users/JeongJungu/Desktop/obj models/Armadillo.obj");

	m.loadObj(fin);

	fin.close();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(900, 900);
	glutCreateWindow("OBJ File Display");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0specular);



	glEnable(GL_DEPTH_TEST);

	glShadeModel(GL_SMOOTH);

	glutDisplayFunc(MyDisplay);
	glutKeyboardFunc(MyKeyboard);
	glutMouseFunc(MyMouseClick);
	glutMotionFunc(MyMouseMove);
	

	glutMainLoop();
	

	return 0;
}