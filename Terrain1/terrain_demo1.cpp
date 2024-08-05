#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/glew.h>

#include "ogldev_util.h"
#include "ogldev_basic_glfw_camera.h"
#include "ogldev_glfw.h"
#include "terrain.h"

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void CursorPosCallback(GLFWwindow* window, double x, double y);
static void MouseButtonCallback(GLFWwindow* window, int Button, int Action, int Mode);


class TerrainDemo1
{
public:

	TerrainDemo1()
	{
	}

	virtual ~TerrainDemo1()
	{
		SAFE_DELETE(m_pGameCamera);
	}


	void Init()
	{
		CreateWindow();

		InitCallbacks();

		InitCamera();

		InitTerrain();
	}


	void Run()
	{
		while (!glfwWindowShouldClose(window)) {
			RenderScene();
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}


	void RenderScene()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	}


	void PassiveMouseCB(int x, int y)
	{
		m_pGameCamera->OnMouse(x, y);
	}

	void KeyboardCB(uint key, int state)
	{
		if (state == GLFW_PRESS) {

			switch (key) {

			case GLFW_KEY_ESCAPE:
			case GLFW_KEY_Q:
				glfwDestroyWindow(window);
				glfwTerminate();
				exit(0);

			case GLFW_KEY_C:
				m_pGameCamera->Print();
				break;

			case GLFW_KEY_W:
				m_isWireframe = !m_isWireframe;

				if (m_isWireframe) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}
				else {
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}

				break;
			}
		}

		m_pGameCamera->OnKeyboard(key);
	}


	void MouseCB(int button, int action, int x, int y)
	{
	}


private:

	void CreateWindow()
	{
		int major_ver = 0;
		int minor_ver = 0;
		bool is_full_screen = false;
		window = glfw_init(major_ver, minor_ver, WINDOW_WIDTH, WINDOW_HEIGHT, is_full_screen, "Terrain Rendering - Demo 1");

		glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	}


	void InitCallbacks()
	{
		glfwSetKeyCallback(window, KeyCallback);
		glfwSetCursorPosCallback(window, CursorPosCallback);
		glfwSetMouseButtonCallback(window, MouseButtonCallback);
	}


	void InitCamera()
	{
		Vector3f Pos(100.0f, 220.0f, -400.0f);
		Vector3f Target(0.0f, -0.25f, 1.0f);
		Vector3f Up(0.0, 1.0f, 0.0f);

		float FOV = 45.0f;
		float zNear = 0.1f;
		float zFar = 2000.0f;
		PersProjInfo persProjInfo = { FOV, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, zNear, zFar };

		m_pGameCamera = new BasicCamera(persProjInfo, Pos, Target, Up);
	}


	void InitTerrain()
	{

	}

	GLFWwindow* window = NULL;
	BasicCamera* m_pGameCamera = NULL;
	bool m_isWireframe = false;
};

TerrainDemo1* app = NULL;


static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	app->KeyboardCB(key, action);
}


static void CursorPosCallback(GLFWwindow* window, double x, double y)
{
	app->PassiveMouseCB((int)x, (int)y);
}


static void MouseButtonCallback(GLFWwindow* window, int Button, int Action, int Mode)
{
	double x, y;

	glfwGetCursorPos(window, &x, &y);

	app->MouseCB(Button, Action, (int)x, (int)y);
}


int main(int argc, char** argv)
{
	app = new TerrainDemo1();
	app->Init();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	app->Run();
	delete app;

	return 0;
}
