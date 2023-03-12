﻿

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>



// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;

//******************
// Η LoadShaders είναι black box για σας

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1000, 1000, "Εργασία 1Α", NULL, NULL); //the window has dimensions 1000x1000.We changed the title of the window to Εργασία 1Α 


	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark green background
	glClearColor(0.0f, 1.0f, 0.0f, 0.0f); //this command takes as parameter R,G,B,alpha. We want to have dark green background, so we set G=1.0f and the other parameters=0.0f

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//***********************************************
	// Οι shaders σας είναι οι 
	// ProjectVertexShader.vertexshader
	// ProjectFragmentShader.fragmentshader

	GLuint programID = LoadShaders("ProjectVertexShader.vertexshader", "ProjectFragmentShader.fragmentshader");

	///////////////////////////////////////////////////////////////////////////////////////	
		/**Το παρακάτω το αγνοείτε - είναι τοποθέτηση κάμερας ***/
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(30.0f), 4.0f / 4.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 30), // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	///////////////////////////////////////////////////////////////////////////////////////
		//**************************************************
		/// Για βοήθεια το πρόγραμμα αναπαριστά ενα τυχαίο τρίγωνο - εσείς θα πρέπει να βάλετε κορυφές κατάλληλες 
		//  για το δικό σας τρίγωνο.
		//Στην άσκηση αυτή δουλεύετε στις 2 διαστάσεις x,y οπότε η z συνιστώσα θα ειναι πάντα 0.0f

	static const GLfloat shape_1_buffer[] = { //buffer with coordinates of triangles.A rhombus will be made from 2 triangles 
		-1.73f, -1.0f, 0.0f, //x,y,z of left apex of the main triangle in the middle
		 0.0f, 2.0f, 0.0f, //x,y,z of middle apex of the main triangle in the middle
		 1.73f,  -1.0f, 0.0f, //x,y,z of right apex of the main triangle in the middle
		 -1.73f,  -1.0f, 0.0f, //x,y,z of right apex of the upper left triangle used to make the left rhombus
		 -4.23f, 1.5f, 0.0f, //x,y,z of middle apex of the upper left triangle used to make the left rhombus
		 -6.73f,  -1.0f, 0.0f, //x,y,z of left apex of the upper left triangle used to make the left rhombus
		 -1.73f,  -1.0f, 0.0f, //x,y,z of right apex of the lower left triangle used to make the left rhombus
		 -6.73f,  -1.0f, 0.0f, //x,y,z of left apex of the lower left triangle used to make the left rhombus
		 -4.23f, -3.5f, 0.0f, //x,y,z of middle apex of the lower left triangle used to make the left rhombus
		 0.0f, 2.0f, 0.0f, //x,y,z of middle apex of the lower triangle used to make the rhombus which is on top of the main triangle
		 -2.5f, 4.5f, 0.0f, //x,y,z of left apex of the lower triangle used to make the rhombus which is on top of the main triangle
		 2.5f, 4.5f, 0.0f, //x,y,z of right apex of the lower triangle used to make the rhombus which is on top of the main triangle
		 0.0f, 7.0f, 0.0f, //x,y,z of middle apex of the upper triangle used to make the rhombus which is on top of the main triangle
		 -2.5f, 4.5f, 0.0f, //x,y,z of left apex of the upper triangle used to make the rhombus which is on top of the main triangle
		 2.5f, 4.5f, 0.0f, //x,y,z of right apex of the upper triangle used to make the rhombus which is on top of the main triangle
		 1.73f,  -1.0f, 0.0f, //x,y,z of left apex of the upper right triangle used to make the right rhombus
		 4.23f, 1.5f, 0.0f, //x,y,z of middle apex of the upper right triangle used to make the right rhombus
		 6.73f,  -1.0f, 0.0f, //x,y,z of right apex of the upper right triangle used to make the right rhombus
		 1.73f,  -1.0f, 0.0f, //x,y,z of left apex of the lower right triangle used to make the right rhombus
		 6.73f,  -1.0f, 0.0f, //x,y,z of right apex of the lower right triangle used to make the right rhombus
		 4.23f, -3.5f, 0.0f //x,y,z of middle apex of the lower right triangle used to make the right rhombus
	};
	
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape_1_buffer), shape_1_buffer, GL_STATIC_DRAW);
	int counter = 3; //initialization of counter to 3, used to hide and show the rhombuses
	int secondsCounter = 0; //initialization of a second counter, used to hide and show the rhombuses

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);  /// Αυτό αφορά την κάμερα  - το αγνοείτε

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
		glDrawArrays(GL_TRIANGLES, counter, 6); //takes 6 apexes from triangles to draw a rhombus every time
		counter += 6; //increase the counter in order to draw a different rhombus
		while (secondsCounter != 517999900) //we make the program wait in order not to change rhombuses quickly
		{
			secondsCounter++; //increase the second counter if it has not the value 517999900
		}
		secondsCounter = 0; // we set it again to 0 to use it in the next loop
		if (counter == 21) //when it shows the last rhombus we se the counter back to 3 to show again the first rhombus 
		{                  //and continue the loop showing all the rhombuses
			counter = 3;
		}
		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0); //terminate if space is pressed

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

