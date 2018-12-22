#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Shader.h"


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

float lastTick = (float)glfwGetTime();
float currentTick = (float)glfwGetTime();

void processInput(GLFWwindow* window);
void updateTick();
void endTick();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
float deltaTime();

// Helpers - move later!
float fInterpTo(float current, float target, float speed);
float* fInterpVec(float* current, float* target, float speed, int size);


int main()
{
	/* GLFW init and config */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Enable compat for Mac OS
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


	/* GLFW main window */
	GLFWwindow* window = glfwCreateWindow(800, 600, "gltp - Main Window", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	/* GLAD init */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!" << std::endl;
		return -1;
	}

	Shader colorShader("shaders/colorShader.vs", "shaders/colorShader.fs");
	Shader greenShader("shaders/colorShader.vs", "shaders/customGreenShader.fs");

	float triangleVerts[] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f  // bottom left
		//-0.5f,  0.5f, 0.0f   // top left 
	};
	float triangleColors[] = {
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f
	};
	float secondTriVerts[] = {
		-0.5f, 0.5f, 0.0f,
		-0.1f, 0.5f, 0.0f,
		-0.5f, 0.1f, 0.0f
	};
	unsigned int indices[] = {
		0, 1, 3,	// first triangle
		1, 2, 3		// second triangle
	};
	
	/* Buffers */
	unsigned int VBO, VBO2, VBOC, VAO, VAO2;
	//unsigned int EBO;
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBOC);
	glGenBuffers(1, &VBO2);
	//glGenBuffers(1, &EBO);


	/* Colored left triangle */
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVerts), triangleVerts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBOC);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleColors), triangleColors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAO2);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriVerts), secondTriVerts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); // CHECK IF NECESSARY

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Unbind objects
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// Fun stuff
	float greenValue = (sin(currentTick) / 2) + 0.5f;
	int customColorLocation = glGetUniformLocation(greenShader.ID, "customColor");
	glUniform4f(customColorLocation, 0.f, 1.f, 0.f, 1.f);


	// DEBUG
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	/* ~~~ Render Loop ~~~ */

	while (!glfwWindowShouldClose(window))
	{
		// Input
		processInput(window);
		updateTick();


		// Rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		float greenValue = (sin(currentTick) / 2) + 0.5f;
		int customColorLocation = glGetUniformLocation(greenShader.ID, "customColor");
		glUniform4f(customColorLocation, 0.3f, greenValue, 0.2f, 1.f);

		colorShader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		greenShader.use();
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		// Finalize frame
		glfwSwapBuffers(window);
		glfwPollEvents();
		endTick();
	}

	// END
	glfwTerminate();
	return 0;
} 


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
}

void updateTick()
{
	currentTick = (float)glfwGetTime();
}

void endTick()
{
	lastTick = currentTick;
}


/* Window resize callback */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

/* Get time since last tick */
float deltaTime()
{
	return currentTick - lastTick;
}

// Linear interpolatino
float fInterpTo(float current, float target, float speed)
{
	float result = (current + speed) * ((target > current) ? 1 : -1); // Add speed in the direction of target
	return ((abs(result) > abs(target)) ? target : result); // Don't return result if we overshoot target
}

// Linear interp thru array
float* fInterpVec(float * current, float * target, float speed, int size)
{
	float* result = new float[size];
	for (int i = 0; i < size; i++)
	{
		result[i] = fInterpTo(current[i], target[i], speed);
	}
	return result;
}
