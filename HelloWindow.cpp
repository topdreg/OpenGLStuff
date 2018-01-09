#include "glad.h" 
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

float vertices1[] = {
	-1.0f, -0.5f, 0.0f, 
	0.0f, -0.5f, 0.0f, 
	-0.5f, 0.5f, 0.0f, 
};

float vertices2[] = {
	1.0f, -0.5f, 0.0f, 
	0.0f, -0.5f, 0.0f, 
	0.5f, 0.5f, 0.0f 
};

int verticesOneSize = sizeof(vertices1);

int verticesTwoSize = sizeof(vertices2);  

const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n" 
	"void main()\n" 
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" 
	"}\0"; 

const char* fragmentShaderSource = "#version 330 core\n" 
	"out vec4 FragColor;\n" 
	"void main()\n" 
	"{\n" 
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n" 
	"}\n\0"; 

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

}

unsigned int getTriangleShaders() 
{
	
	//compile shaders here
	unsigned int vertexShader; 
	vertexShader = glCreateShader(GL_VERTEX_SHADER); 
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); 
	glCompileShader(vertexShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); 
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); 
	glCompileShader(fragmentShader); 

	int vertexSuccess;
	int fragmentSuccess; 
	int linkerSuccess; 
	char infoLog[512]; 

	//check compilation status
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexSuccess); 
	if (!vertexSuccess)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); 
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl; 
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentSuccess); 
	if (!fragmentSuccess)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog); 
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl; 
	}

	//now link shaders to shader program object
	unsigned int shaderProgram; 
	shaderProgram = glCreateProgram(); 
	glAttachShader(shaderProgram, vertexShader); 
	glAttachShader(shaderProgram, fragmentShader); 
	glLinkProgram(shaderProgram); 

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkerSuccess); 
	if (!linkerSuccess) 
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog); 
		std::cout << "ERROR:SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl; 
	}
	
	//delete shaders
	glDeleteShader(vertexShader); 
	glDeleteShader(fragmentShader);

	return shaderProgram; 
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0); 
	glEnableVertexAttribArray(0); 

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

	unsigned int shaderProgram{getTriangleShaders()};

	//create variable array objects 
	unsigned int VAO1{triangleVAO(vertices1, verticesOneSize)};
	unsigned int VAO2{triangleVAO(vertices2, verticesTwoSize)};  

	while (!glfwWindowShouldClose(window))
	{
		processInput(window); 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT); 
		
		glUseProgram(shaderProgram); 
		
		glBindVertexArray(VAO1); 
		glDrawArrays(GL_TRIANGLES, 0, 3); 
 		
		glBindVertexArray(VAO2); 
		glDrawArrays(GL_TRIANGLES, 0, 3); 

		glfwSwapBuffers(window);
		glfwPollEvents(); 
	}

	glfwTerminate(); 
	return 0; 
} 
 
