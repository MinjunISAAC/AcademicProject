//	Kyung Hee University Game Graphics Programming Class
// KHU ADVENTURE (Mini Rhythm Game)
// Game Class Using OpenGL & Glut (Header), ver. 0.6
//	Project Host : Park Min Jun( qkralswns931026@gmail.com )


#pragma warning(disable: 4996)

#include "GameMain.h"
#include "Bmp.h"
#include "LodePng.h"
#include "LoadObj.h"
#include "fmod.h"
#include "fmod_errors.h"
#include <cstdlib>

CGameMain::CGameMain(int nW, int nH, int nPosX, int nPosY)
{
	//
	m_nW = nW;
	m_nH = nH;

	m_nPosX = nPosX;
	m_nPosY = nPosY;

	m_bPause = false;
	m_bCreated = false;

	if (nInstanceCnt == 0) pInstance = this;

	m_HumanVertex = NULL;
	m_HumanIndex = NULL;

	nInstanceCnt++;
}

CGameMain::~CGameMain()
{
	if (m_bCreated)
		ShutdownApp();

	nInstanceCnt--;
}

void CGame::Create(int arg, char **argc, float *fBgColor, double(*dLookAt)[3], bool bVSync)
{
	glutInitWindowSize(m_nW, m_nH);
	glutInitWindowPosition(m_nPosX, m_nPosY);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInit(&arg, argc);
	glutCreateWindow("Types of Light");

	m_nFrame = 0;
	m_nBaseTime = glutGet(GLUT_ELAPSED_TIME);
	m_nPreviousTime = m_nBaseTime;

	glutDisplayFunc(RenderSceneStatic);
	glutReshapeFunc(ResizeStatic);
	glutKeyboardFunc(KeyDownStatic);
	glutSpecialFunc(SpecialInputStatic);
	glutMouseFunc(MouseStatic);

	atexit(ShutdownAppStatic);

	m_bVSync = bVSync;
	SetVSync(m_bVSync);



	const double m_pi = 3.1415926536;
	m_dDistace = 6.0;
	m_dAngleZ = m_pi / 2;

	int i;
	int r, c;
	for (i = 0; i < 4; i++)
		m_fBgColor[i] = fBgColor[i];

	for (r = 0; r < 3; r++)
		for (c = 0; c < 3; c++)
			m_dLookAt[r][c] = dLookAt[r][c];

	m_dFps = 0.;

	m_Hud.SetPosition(10.0f, 10.0f);
	m_Hud.SetColor(1.0f, 0.0f, 0.0f);


	m_bFullscreen = false;

	if (InitializeApp() == true)
	{
		m_bCreated = true;
		glutMainLoop();
	}
}

void CGame::RenderScene()
{
	m_nCurrentTime = glutGet(GLUT_ELAPSED_TIME);
	int nElapsedTime = m_nCurrentTime - m_nPreviousTime;

	// �츮�� .obj ������ �н��ϴ�. 
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;
	bool res = LoadObj("Human.obj", vertices, uvs, normals);

	if (nElapsedTime < 16.67)
	{
		//glutPostRedisplay();
		//return;
	}

	m_nPreviousTime = m_nCurrentTime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// �ü� ó��
	m_dLookAt[0][0] = (m_dDistace)* sin(m_dAngleZ);
	m_dLookAt[0][1] = 15.0f;
	m_dLookAt[0][2] = (m_dDistace)* cos(m_dAngleZ);
	m_dLookAt[1][1] = 1;

	gluLookAt(m_dLookAt[0][0], m_dLookAt[0][1], m_dLookAt[0][2], m_dLookAt[1][0], m_dLookAt[1][1], m_dLookAt[1][2], m_dLookAt[2][0], m_dLookAt[2][1], m_dLookAt[2][2]);

	//DrawCoordinates();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glLightfv(GL_LIGHT0, GL_POSITION, m_LightPosition);


	//Draw Human 3Dobject
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glPushMatrix();

	glColor3f(0.8f, 0.8f, 0.9f);
	glRotatef(20, 0.0f, 0.0f, 1.0f);

	glTranslatef(0.0f, 0.7f, 0.0f);
	glScalef(0.03f, 0.03f, 0.03f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, m_HumanVertex);
	glDrawElements(GL_TRIANGLES, m_nHumanTriangleCnt, GL_UNSIGNED_INT, m_HumanIndex);
	printf("%d\n", m_nHumanTriangleCnt);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();

	// �ٴ� �Է�
	glPushMatrix();
	glColor3f(0.03f, 0.2f, 0.03f);
	glTranslatef(0.0f, -0.1f, 0.0f);
	glScalef(100.0f, 0.2f, 120.0f);
	DrawCube();
	glPopMatrix();

	/*	// ����Է� hdri
		glPushMatrix();
		glColor3f(0.03f, 0.2f, 0.03f);
		glTranslatef(0.0f, -0.1f, 0.0f);
		glScalef(100.0f, 0.2f, 120.0f);
		DrawCube();
		glPopMatrix();
		*/

	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);

	if (m_Sphere.m_fPosition[1] < 0.6f)
	{
		m_Sphere.m_fPosition[1] = 0.6f;
		m_nDirectionZ *= -1;

		if (m_nDirectionZ < 0) m_Sphere.m_fPosition[2] = 4.5;
		m_nElapsedTime = 0;

		printf("%d, %lf, %lf\n", m_nDirectionZ, m_Sphere.m_fPosition[1], m_Sphere.m_fPosition[2]);
	}
	// ���� ���� ����
	double defaultLine = 0.0f;
	double turn = 2.2;

	// ���� ����
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(defaultLine, defaultLine, defaultLine);
	glScalef(2.0f, 0.1f, 2.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
	DrawCube();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// ���� 1����(�� 10��)
	for (int i = 1; i < 11; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * i, defaultLine, defaultLine);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 2����(�� 12��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 10, defaultLine, defaultLine - turn * i);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 3����(�� 21��)
	for (int i = 1; i < 11; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (11 - i), defaultLine, defaultLine - turn * 2);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 4����(�� 23��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn, defaultLine, defaultLine - turn * (2 + i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 5����(�� 25��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - (turn * (i + 1)), defaultLine, defaultLine - turn * 4);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 6����(�� 27��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 3, defaultLine, defaultLine - turn * (4 + i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 7����(�� 29��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (3 + i), defaultLine, defaultLine - turn * 6);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 8����(�� 31��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 5, defaultLine, defaultLine - turn * (6 - i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 9����(�� 33��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (5 + i), defaultLine, defaultLine - turn * 4);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 10����(�� 35��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 7, defaultLine, defaultLine - turn * (4 + i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 11����(�� 37��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (7 + i), defaultLine, defaultLine - turn * 6);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 12����(�� 39��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 9, defaultLine, defaultLine - turn * (6 - i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 13����(�� 42��)
	for (int i = 1; i < 4; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (9 + i), defaultLine, defaultLine - turn * 4);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 14����(�� 48��)
	for (int i = 1; i < 7; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 12, defaultLine, defaultLine - turn * (4 - i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 15����(�� 50��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (12 + i), defaultLine, defaultLine - turn * -2);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 16����(�� 52��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 14, defaultLine, defaultLine - turn * (-2 + i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 17����(�� 54��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (14 + i), defaultLine, defaultLine);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 18����(�� 58��)
	for (int i = 1; i < 5; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 16, defaultLine, defaultLine + (turn * i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 19����(�� 65��)
	for (int i = 1; i < 8; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (16 - i), defaultLine, defaultLine + (turn * 4));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 20����(�� 67��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 9, defaultLine, defaultLine + turn * (4 - i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 21����(�� 72��)
	for (int i = 1; i < 6; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (9 - i), defaultLine, defaultLine + turn * 2);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 22����(�� 74��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 4, defaultLine, defaultLine + turn * (2 + i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 23����(�� 77��)
	for (int i = 1; i < 4; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (4 + i), defaultLine, defaultLine + turn * 4);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 24����(�� 79��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 7, defaultLine, defaultLine + turn * (4 + i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 25����(�� 84��)
	for (int i = 1; i < 6; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (7 - i), defaultLine, defaultLine + turn * 6);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 26����(�� 88��)
	for (int i = 1; i < 5; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 2, defaultLine, defaultLine + turn * (6 - i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 27����(�� 90��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (2 - i), defaultLine, defaultLine + turn * 2);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 28����(�� 96��)
	for (int i = 1; i < 7; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine, defaultLine, defaultLine + turn * (2 + i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 29����(�� 99��)
	for (int i = 1; i < 4; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * i, defaultLine, defaultLine + turn * 8);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 30����(�� 100��)
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(defaultLine - turn * 3, defaultLine, defaultLine + turn * 9);
	glScalef(2.0f, 0.1f, 2.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
	DrawCube();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// ���� 31����(�� 102��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (3 + i), defaultLine, defaultLine + turn * 9);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 32����(�� 103��)
	for (int i = 1; i < 2; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 5, defaultLine, defaultLine + turn * (9 - i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 33����(�� 105��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (5 + i), defaultLine, defaultLine + turn * 8);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 34����(�� 107��)
	for (int i = 1; i < 2; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 7, defaultLine, defaultLine + turn * (8 + i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 35����(�� 109��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (7 + i), defaultLine, defaultLine + turn * 9);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 36����(�� 111��)
	for (int i = 1; i < 4; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 9, defaultLine, defaultLine + turn * (9 - i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 37����(�� 113��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (9 + i), defaultLine, defaultLine + turn * 6);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 37����(�� 113��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (9 + i), defaultLine, defaultLine + turn * 6);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 38����(�� 115��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 11, defaultLine, defaultLine + turn * (6 + i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 39����(�� 117��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (11 + i), defaultLine, defaultLine + turn * 8);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 40����(�� 119��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 13, defaultLine, defaultLine + turn * (8 - i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 41����(�� 123��)
	for (int i = 1; i < 5; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (13 + i), defaultLine, defaultLine + turn * 6);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 42����(�� 127��)
	for (int i = 1; i < 5; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 17, defaultLine, defaultLine + turn * (6 + i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 43����(�� 132��)
	for (int i = 1; i < 6; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (17 - i), defaultLine, defaultLine + turn * 10);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 44����(�� 134��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 12, defaultLine, defaultLine + turn * (10 + i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 45����(�� 136��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (12 + i), defaultLine, defaultLine + turn * 12);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}


	// ���� 46����(�� 138��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 14, defaultLine, defaultLine + turn * (12 + i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 48����(�� 140��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (14 - i), defaultLine, defaultLine + turn * 14);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 49����(�� 142��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 12, defaultLine, defaultLine + turn * (14 + i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 50����(�� 146��)
	for (int i = 1; i < 5; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (12 + i), defaultLine, defaultLine + turn * 16);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 51����(�� 150��)
	for (int i = 1; i < 5; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 16, defaultLine, defaultLine + turn * (16 - i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 52����(�� 152��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (16 + i), defaultLine, defaultLine + turn * 12);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 53����(�� 153��)
	for (int i = 1; i < 2; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 18, defaultLine, defaultLine + turn * (12 + i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 54����(�� 154��)
	for (int i = 1; i < 2; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (18 + i), defaultLine, defaultLine + turn * 13);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 55����(�� 156��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 19, defaultLine, defaultLine + turn * (13 + i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 56����(�� 157��)
	for (int i = 1; i < 2; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (19 - i), defaultLine, defaultLine + turn * 15);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 57����(�� 159��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 18, defaultLine, defaultLine + turn * (15 + i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 58����(�� 160��)
	for (int i = 1; i < 2; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (18 + i), defaultLine, defaultLine + turn * 17);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 59����(�� 162��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 19, defaultLine, defaultLine + turn * (17 + i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 60����(�� 170��)
	for (int i = 1; i < 9; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (19 - i), defaultLine, defaultLine + turn * 19);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 61����(�� 179��)
	for (int i = 1; i < 10; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (18 - i), defaultLine, defaultLine + turn * 18);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 62����(�� 186��)
	for (int i = 1; i < 8; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 9, defaultLine, defaultLine + turn * (18 - i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 63����(�� 192��)
	for (int i = 1; i < 7; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 10, defaultLine, defaultLine + turn * (18 - i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 64����(�� 196��)
	for (int i = 1; i < 5; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (10 - i), defaultLine, defaultLine + turn * 11);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 65����(�� 200��)
	for (int i = 1; i < 5; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 6, defaultLine, defaultLine + turn * (11 + i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 66����(��������)
	for (int i = 1; i < 4; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 5, defaultLine, defaultLine + turn * (14 + i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 67����(��������)
	for (int i = 1; i < 4; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 7, defaultLine, defaultLine + turn * (14 + i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 68����(��������)
	for (int i = 1; i < 2; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 6, defaultLine, defaultLine + turn * 17);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 69����(�� 204��)
	for (int i = 1; i < 5; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 6, defaultLine, defaultLine + turn * (17 + i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 70����(�� 206��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 5, defaultLine, defaultLine + turn * (22 - i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 71����(�� 208��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (5 - i), defaultLine, defaultLine + turn * 20);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 72����(�� 214��)
	for (int i = 1; i < 7; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 3, defaultLine, defaultLine + turn * (20 - i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 73����(�� 216��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 4, defaultLine, defaultLine + turn * (19 - i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 74����(�� 221��)
	for (int i = 1; i < 6; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 2, defaultLine, defaultLine + turn * (17 - i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 75����(�� 224��)
	for (int i = 1; i < 4; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * 1, defaultLine, defaultLine + turn * (15 - i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 76����(�� 226��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (1 - i), defaultLine, defaultLine + turn * 12);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 77����(�� 232��)
	for (int i = 1; i < 7; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (-1), defaultLine, defaultLine + turn * (12 + i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 78����(�� 237��)
	for (int i = 1; i < 6; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (-2), defaultLine, defaultLine + turn * (13 + i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 79����(�� 239��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (-2 - i), defaultLine, defaultLine + turn * 18);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 80����(�� 245��)
	for (int i = 1; i < 7; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (-4), defaultLine, defaultLine + turn * (18 - i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 81����(�� 247��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (-4 - i), defaultLine, defaultLine + turn * 12);
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 82����(�� 249��)
	for (int i = 1; i < 3; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (-6), defaultLine, defaultLine + turn * (12 - i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 83����(�� 259��)
	for (int i = 1; i < 11; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (-5), defaultLine, defaultLine + turn * (11 - i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 84����(�� 266��)
	for (int i = 1; i < 8; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (-4), defaultLine, defaultLine + turn * (10 - i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 85����(�� 269��)
	for (int i = 1; i < 4; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (-3), defaultLine, defaultLine + turn * (6 - i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 86����(�� 275��)
	for (int i = 1; i < 7; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (-2), defaultLine, defaultLine + turn * (10 - i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// ���� 87����(�� 281��)
	for (int i = 1; i < 7; i++) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(defaultLine - turn * (-1), defaultLine, defaultLine + turn * (10 - i));
		glScalef(2.0f, 0.1f, 2.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
		DrawCube();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}



	// Ʃ�丮�� ĳ����

	m_nElapsedTime += nElapsedTime;

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glLightfv(GL_LIGHT0, GL_POSITION, m_LightPosition);

	float g = 9.8;
	float initial_v = 5.0f;
	double angle = 65.*3.14159 / 180.;

	m_Sphere.SetVelocity(0.0f, initial_v*sin(angle) - g * m_nElapsedTime / 1000, m_nDirectionZ * initial_v*cos(angle));
	m_Sphere.Move(nElapsedTime / 200.);
	m_Sphere.Render();

	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);


	if (m_Sphere.m_fPosition[1] < 0.6f)
	{
		m_Sphere.m_fPosition[1] = 0.6f;
		m_nDirectionZ *= -1;

		if (m_nDirectionZ < 0) m_Sphere.m_fPosition[2] = 4.5;
		m_nElapsedTime = 0;

		printf("%d, %lf, %lf\n", m_nDirectionZ, m_Sphere.m_fPosition[1], m_Sphere.m_fPosition[2]);
	}







	m_nFrame++;
	m_nCurrentTime = glutGet(GLUT_ELAPSED_TIME);
	if (m_nCurrentTime - m_nBaseTime > 1000)
	{
		m_dFps = m_nFrame * 1000.0 / (m_nCurrentTime - m_nBaseTime);
		m_nBaseTime = m_nCurrentTime;
		m_nFrame = 0;
	}

	char FpsText[100];
	sprintf(FpsText, "FPS: %4.2f", m_dFps);

	m_Hud.SetText(FpsText);
	m_Hud.DisplayHud(this);

	glutSwapBuffers();
	glutPostRedisplay();

}

void CGame::KeyDown(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;

	case 'v':
		m_bVSync = !m_bVSync;
		SetVSync(m_bVSync);
		break;

	case 'p':
		m_bPause = !m_bPause;
		break;
	}
}

void CGame::SpecialInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		m_dDistace += 0.1;
		break;

	case GLUT_KEY_DOWN:
		m_dDistace -= 0.1;
		break;

	case GLUT_KEY_RIGHT:
		m_dAngleZ += 0.1;
		break;

	case GLUT_KEY_LEFT:
		m_dAngleZ -= 0.1;
		break;

	case GLUT_KEY_F11:
		if (!m_bFullscreen) {
			glutFullScreen();

			m_bFullscreen = true;
		}
		else if (m_bFullscreen) {
			glutReshapeWindow(m_nW, m_nH);
			glutPositionWindow(m_nPosX, m_nPosY);

			m_bFullscreen = false;
		}

		break;
	}
}

void CGame::Resize(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (double)width / (double)height, 0.1, 50.0);
}

void CGame::Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

	}
}

bool CGame::InitializeApp()
{
	glClearColor(m_fBgColor[0], m_fBgColor[1], m_fBgColor[2], m_fBgColor[3]);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);


	unsigned char *Image;
	int nW, nH;
	Image = ReadBmp("bound.bmp", &nW, &nH);
	if (!Image) return false;
	glGenTextures(1, m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, nW, nH, GL_BGR, GL_UNSIGNED_BYTE, Image);
	delete[] Image;

	m_LightPosition[0] = 5.0f;
	m_LightPosition[1] = 10.0f;
	m_LightPosition[2] = 0.0f;
	m_LightPosition[3] = 0.0f;

	glEnable(GL_NORMALIZE);

	return true;
}

void CGame::ShutdownApp()
{
	if (m_HumanVertex) delete[] m_HumanVertex;
	if (m_HumanIndex) delete[] m_HumanIndex;
}