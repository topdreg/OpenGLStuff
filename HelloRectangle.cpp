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

float vertices[] = {
	0.5f, 0.5f, 0.0f, 
	0.5f, -0.5f, 0.0f, 
	-0.5f, -0.5f, 0.0f, 
	-0.5f, 0.5f, 0.0f 
};

unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

int verticesSize = sizeof(vertices); 
int indicesSize = sizeof(indices); 

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


unsigned int getRectangleShaders() 
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


unsigned int rectangleVAO(float vertices[], int verticesSize, unsigned int indices[], int indicesSize) 
{
	unsigned int VAO; 
	glGenVertexArrays(1, &VAO); 
	glBindVertexArray(VAO); 
	
	unsigned int VBO; 
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);
	
	unsigned int EBO;
	glGenBuffers(1, &EBO);  
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);  
	
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

	unsigned int shaderProgram{getRectangleShaders()};

	unsigned int VAO{rectangleVAO(vertices, verticesSize, indices, indicesSize)}; 

	while (!glfwWindowShouldClose(window))
	{
		processInput(window); 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT); 
		
		glUseProgram(shaderProgram); 
		glBindVertexArray(VAO); 
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  

		glfwSwapBuffers(window);
		glfwPollEvents(); 
	}

	glfwTerminate(); 
	return 0; 
} 
 
