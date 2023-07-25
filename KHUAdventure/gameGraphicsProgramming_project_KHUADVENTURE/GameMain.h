//	Kyung Hee University Game Graphics Programming Class
// KHU ADVENTURE (Mini Rhythm Game)
// Game Class Using OpenGL & Glut (Header), ver. 0.6
//	Project Host : Park Min Jun( qkralswns931026@gmail.com )

#pragma once

#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <windows.h>

#include "Hud.h"
#include "Sphere.h"

#include "GLSLProgram.h"
#include <glm/gtx/transform.hpp>

class CGame
{
public:
	static int nInstanceCnt;
	static CGame *pInstance;

	static GLfloat Red[4], Green[4], Blue[4], White[4], Black[4];

	GLfloat m_LightPosition[4];

	int m_nElapsedTime;
	int m_nDirectionZ;

	float m_dDistace;
	float m_dAngleZ;

	int m_nW, m_nH;
	int m_nPosX, m_nPosY;
	bool m_bCreated;

	float m_fBgColor[4];
	double m_dLookAt[3][3];

	int m_nFrame;
	int m_nCurrentTime, m_nBaseTime, m_nPreviousTime;
	bool m_bVSync, m_bFullscreen;
	double m_dFps;

	bool m_bPause;

	GLfloat *m_HumanVertex;
	unsigned int *m_HumanIndex;
	int m_nHumanVertexCnt, m_nHumanTriangleCnt;

	GLuint m_Texture[1];

	CHud m_Hud;
	CSphere m_Sphere;

	CGame(int nW, int nH, int nPosX, int nPosY);
	~CGame();

	static void SetVSync(bool bSync);
	static void DrawBitmapText(char *string, GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, int nW, int nH, bool bFixed2D);

	static void DrawCube();
	static void DrawCoordinates();

	static void RenderSceneStatic();
	static void KeyDownStatic(unsigned char key, int x, int y);
	static void SpecialInputStatic(int key, int x, int y);
	static void ResizeStatic(int width, int height);
	static void MouseStatic(int button, int state, int x, int y);
	static void ShutdownAppStatic();

	void Create(int arg, char **argc, float *fBgColor, double(*dLookAt)[3], bool bVSync);

	void RenderScene();
	void KeyDown(unsigned char key, int x, int y);
	void SpecialInput(int key, int x, int y);
	void Resize(int width, int height);
	void Mouse(int button, int state, int x, int y);

	bool InitializeApp();
	void ShutdownApp();
};