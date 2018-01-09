#include "glad.h" 
#include "shader_s.h" 
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height); 
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
		glfwSetWindowShouldClose(window, true); 
}

float vertices[] = {
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f 
};

int verticesSize = sizeof(vertices);  

unsigned int getShader(const char* shaderSource, GLenum shaderType)
{	
	//compile shader
	unsigned int shader = glCreateShader(shaderType); 
	glShaderSource(shader, 1, &shaderSource, NULL); 
	glCompileShader(shader); 

	//check compilation status
	int shaderSuccess; 
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderSuccess);
	if (!shaderSuccess) 
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog); 
		std::cout << "ERROR::SHADER::COMPILATION_FAILED:" << shaderType << infoLog << std::endl; 
	}
	
	return shader; 
}


unsigned int triangleVAO(float vertices[], int verticesSize) 
{
	unsigned int VAO; 
	glGenVertexArrays(1, &VAO); 
	glBindVertexArray(VAO); 
	unsigned int VBO; 
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0); 
	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float))); 
	glEnableVertexAttribArray(1); 

	return VAO; 
}

int main()
{
	glfwInit(); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl; 
		glfwTerminate(); 
		return -1; 
	}
	glfwMakeContextCurrent(window); 

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
	{
		std::cout << "Failed to initailize GLAD" << std::endl; 
		return -1; 
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 

	Shader ourShader("thing.vs", "thing.fs");  
	
	//create variable array objects 
	unsigned int VAO{triangleVAO(vertices, verticesSize)}; 

	while (!glfwWindowShouldClose(window))
	{
		processInput(window); 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT); 
		
		ourShader.use();   
	
		glBindVertexArray(VAO); 
		glDrawArrays(GL_TRIANGLES, 0, 3); 

		glfwSwapBuffers(window);
		glfwPollEvents(); 
	}

	glfwTerminate(); 
	return 0; 
} 
 
