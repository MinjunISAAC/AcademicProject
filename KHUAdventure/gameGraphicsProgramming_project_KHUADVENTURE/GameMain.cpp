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

	// 우리의 .obj 파일을 읽습니다. 
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

	// 시선 처리
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

	// 바닥 입력
	glPushMatrix();
	glColor3f(0.03f, 0.2f, 0.03f);
	glTranslatef(0.0f, -0.1f, 0.0f);
	glScalef(100.0f, 0.2f, 120.0f);
	DrawCube();
	glPopMatrix();

	/*	// 배경입력 hdri
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
	// 발판 기준 구성
	double defaultLine = 0.0f;
	double turn = 2.2;

	// 발판 기준
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(defaultLine, defaultLine, defaultLine);
	glScalef(2.0f, 0.1f, 2.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
	DrawCube();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// 발판 1라인(총 10개)
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

	// 발판 2라인(총 12개)
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

	// 발판 3라인(총 21개)
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

	// 발판 4라인(총 23개)
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

	// 발판 5라인(총 25개)
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

	// 발판 6라인(총 27개)
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

	// 발판 7라인(총 29개)
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

	// 발판 8라인(총 31개)
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

	// 발판 9라인(총 33개)
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

	// 발판 10라인(총 35개)
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

	// 발판 11라인(총 37개)
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

	// 발판 12라인(총 39개)
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

	// 발판 13라인(총 42개)
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

	// 발판 14라인(총 48개)
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

	// 발판 15라인(총 50개)
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

	// 발판 16라인(총 52개)
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

	// 발판 17라인(총 54개)
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

	// 발판 18라인(총 58개)
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

	// 발판 19라인(총 65개)
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

	// 발판 20라인(총 67개)
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

	// 발판 21라인(총 72개)
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

	// 발판 22라인(총 74개)
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

	// 발판 23라인(총 77개)
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

	// 발판 24라인(총 79개)
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

	// 발판 25라인(총 84개)
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

	// 발판 26라인(총 88개)
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

	// 발판 27라인(총 90개)
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

	// 발판 28라인(총 96개)
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

	// 발판 29라인(총 99개)
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

	// 발판 30라인(총 100개)
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(defaultLine - turn * 3, defaultLine, defaultLine + turn * 9);
	glScalef(2.0f, 0.1f, 2.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
	DrawCube();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// 발판 31라인(총 102개)
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

	// 발판 32라인(총 103개)
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

	// 발판 33라인(총 105개)
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

	// 발판 34라인(총 107개)
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

	// 발판 35라인(총 109개)
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

	// 발판 36라인(총 111개)
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

	// 발판 37라인(총 113개)
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

	// 발판 37라인(총 113개)
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

	// 발판 38라인(총 115개)
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

	// 발판 39라인(총 117개)
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

	// 발판 40라인(총 119개)
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

	// 발판 41라인(총 123개)
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

	// 발판 42라인(총 127개)
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

	// 발판 43라인(총 132개)
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

	// 발판 44라인(총 134개)
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

	// 발판 45라인(총 136개)
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


	// 발판 46라인(총 138개)
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

	// 발판 48라인(총 140개)
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

	// 발판 49라인(총 142개)
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

	// 발판 50라인(총 146개)
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

	// 발판 51라인(총 150개)
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

	// 발판 52라인(총 152개)
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

	// 발판 53라인(총 153개)
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

	// 발판 54라인(총 154개)
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

	// 발판 55라인(총 156개)
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

	// 발판 56라인(총 157개)
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

	// 발판 57라인(총 159개)
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

	// 발판 58라인(총 160개)
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

	// 발판 59라인(총 162개)
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

	// 발판 60라인(총 170개)
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

	// 발판 61라인(총 179개)
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

	// 발판 62라인(총 186개)
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

	// 발판 63라인(총 192개)
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

	// 발판 64라인(총 196개)
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

	// 발판 65라인(총 200개)
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

	// 발판 66라인(보스라인)
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

	// 발판 67라인(보스라인)
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

	// 발판 68라인(보스라인)
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

	// 발판 69라인(총 204개)
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

	// 발판 70라인(총 206개)
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

	// 발판 71라인(총 208개)
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

	// 발판 72라인(총 214개)
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

	// 발판 73라인(총 216개)
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

	// 발판 74라인(총 221개)
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

	// 발판 75라인(총 224개)
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

	// 발판 76라인(총 226개)
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

	// 발판 77라인(총 232개)
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

	// 발판 78라인(총 237개)
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

	// 발판 79라인(총 239개)
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

	// 발판 80라인(총 245개)
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

	// 발판 81라인(총 247개)
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

	// 발판 82라인(총 249개)
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

	// 발판 83라인(총 259개)
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

	// 발판 84라인(총 266개)
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

	// 발판 85라인(총 269개)
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

	// 발판 86라인(총 275개)
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

	// 발판 87라인(총 281개)
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



	// 튜토리얼 캐릭터

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