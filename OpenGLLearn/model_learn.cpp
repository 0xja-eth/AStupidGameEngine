//#define MODEL_STYLE
#ifdef MODEL_STYLE

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Entity.h"

#include <filesystem>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lightVertices[] = {
	//     ---- λ�� ----       ---- ��ɫ ----     - �������� -    --- ������ ---
		 0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,  0.0f,  0.0f, -1.0f,  // ����
		 0.5f, -0.5f, -0.5f,   0.5f, 0.5f, 0.5f,   1.0f, 0.0f,  0.0f,  0.0f, -1.0f,  // ����
		-0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,  0.0f,  0.0f, -1.0f,  // ����
		-0.5f,  0.5f, -0.5f,   0.5f, 0.5f, 0.5f,   0.0f, 1.0f,  0.0f,  0.0f, -1.0f,  // ����
	//     ---- λ�� ----       ---- ��ɫ ----     - �������� -    --- ������ ---
		 0.5f,  0.5f, 0.5f,   0.5f, 0.5f, 0.5f,   1.0f, 1.0f,   0.0f,  0.0f,  1.0f,  // ����
		 0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   0.0f,  0.0f,  1.0f,  // ����
		-0.5f, -0.5f, 0.5f,   0.5f, 0.5f, 0.5f,   0.0f, 0.0f,   0.0f,  0.0f,  1.0f,  // ����
		-0.5f,  0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   0.0f,  0.0f,  1.0f,  // ����
	//     ---- λ�� ----       ---- ��ɫ ----     - �������� -    --- ������ ---
		-0.5f,  0.5f, -0.5f,   0.5f, 0.5f, 0.5f,   1.0f, 1.0f, -1.0f,  0.0f,  0.0f,  // ����
		-0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f, -1.0f,  0.0f,  0.0f,  // ����
		-0.5f, -0.5f,  0.5f,   0.5f, 0.5f, 0.5f,   0.0f, 0.0f, -1.0f,  0.0f,  0.0f,  // ǰ��
		-0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, -1.0f,  0.0f,  0.0f,  // ǰ��
	//     ---- λ�� ----       ---- ��ɫ ----     - �������� -    --- ������ ---
		0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   1.0f,  0.0f,  0.0f,  // ����
		0.5f, -0.5f, -0.5f,   0.5f, 0.5f, 0.5f,   0.0f, 1.0f,   1.0f,  0.0f,  0.0f,  // ����
		0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,   1.0f,  0.0f,  0.0f,  // ǰ��
		0.5f,  0.5f,  0.5f,   0.5f, 0.5f, 0.5f,   1.0f, 0.0f,   1.0f,  0.0f,  0.0f,  // ǰ��
	//     ---- λ�� ----       ---- ��ɫ ----     - �������� -    --- ������ ---
		 0.5f, -0.5f, -0.5f,   0.5f, 0.5f, 0.5f,   1.0f, 1.0f,  0.0f, -1.0f,  0.0f,  // �Һ�
		 0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,  0.0f, -1.0f,  0.0f,  // ��ǰ
		-0.5f, -0.5f,  0.5f,   0.5f, 0.5f, 0.5f,   0.0f, 0.0f,  0.0f, -1.0f,  0.0f,  // ��ǰ
		-0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,  0.0f, -1.0f,  0.0f,  // ���
	//     ---- λ�� ----       ---- ��ɫ ----     - �������� -    --- ������ ---
		 0.5f, 0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,  0.0f,   // �Һ�
		 0.5f, 0.5f,  0.5f,   0.5f, 0.5f, 0.5f,   1.0f, 0.0f,   0.0f, 1.0f,  0.0f,   // ��ǰ
		-0.5f, 0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   0.0f, 1.0f,  0.0f,   // ��ǰ
		-0.5f, 0.5f, -0.5f,   0.5f, 0.5f, 0.5f,   0.0f, 1.0f,   0.0f, 1.0f,  0.0f,   // ���
};

unsigned int lightIndices[] = { // ע��������0��ʼ! 
	0, 1, 3,  // ��1��������
	1, 2, 3,  // ��2��������

	4, 6, 7,  // ��3��������
	4, 6, 5,  // ��4��������

	8, 10,11, // ��5��������
	8, 10,9, // ��6��������

	12,13,15, // ��7��������
	13,14,15, // ��8��������

	16,18,19, // ��9��������
	16,18,17, // ��10��������

	20,21,23, // ��11��������
	21,22,23, // ��12��������
};

unsigned int lightVAO, VBO, EBO;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// ���ù�Դ
	// -----------------------------
	glGenVertexArrays(1, &lightVAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// �󶨶�������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);

	// ����������
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lightIndices), lightIndices, GL_STATIC_DRAW);

	// �󶨵ƶ����������
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// ���ù�Դ����
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// build and compile shaders
	// -------------------------
	Shader ourShader("mvShader.sd", "mfShader.sd");
	Shader lightShader("vShader.sd", "lShader.sd");

	// load models
	// -----------
	Entity ourModel("nanosuit/nanosuit.obj");


	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		ourShader.activate();

		float radius = 2.0f;
		float lx = sin(glfwGetTime()*1.2) * radius;
		float lz = cos(glfwGetTime()*1.2) * radius;
		glm::vec3 lightPos = glm::vec3(lx, 0.0, lz);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		ourShader.setVec3("lightPos", lightPos);
		ourShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

		ourShader.setVec3("material.ambient", 0.5f, 0.5f, 0.5f);
		ourShader.setVec3("material.diffuse", 1.0f, 1.0f, 1.0f);
		ourShader.setVec3("material.specular", 2.0f, 2.0f, 2.0f);
		ourShader.setFloat("material.shininess", 32.0f);

		ourShader.setVec3("viewPos.diffuse", camera.Position);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);

		// ���ƹ�Դ
		
		lightShader.activate();

		lightShader.setMat4("view", glm::value_ptr(view));
		lightShader.setMat4("projection", glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));

		lightShader.setVec3("color", 1.0f, 1.0f, 1.0f);

		lightShader.setMat4("model", glm::value_ptr(model));
		
		// ���Ƶ����������
		glBindVertexArray(lightVAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.updateCameraMove(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.updateCameraMove(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.updateCameraMove(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.updateCameraMove(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.updateCameraRotate(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.updateScrollEvents(yoffset);
}

#endif