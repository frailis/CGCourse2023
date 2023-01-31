#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include "..\common\debugging.h"
 

//check_gl_errors(__LINE__, __FILE__)

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1000, 800, "code_1_my_first_triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewInit();

    printout_opengl_glsl_info();

	//--------------------------------------
	//            MIA PARTE
	//--------------------------------------

	//creo i dati da renderizzare da buttare nella ram
	GLuint positionAttribIndex = 0;
	GLuint colorAttribIndex = 1;
	float pos_colour[] = {
		0.0, 0.0, //1st vertex
		1.0, 0.0, 0.0, //1st vertex colour
		0.5, 0.0, //2nd vertex
		0.0, 1.0, 0.0, //2nd vertex colour
		0.5, 0.5, //3rd vertex 
		0.0, 0.0, 1.0 //3rd vertex colour
	};

	//creo il buffer per i dati da buttare in ram
	GLuint posColourBuffer;
	glCreateBuffers(1, &posColourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, posColourBuffer);

	//specifico che dati sto buttando nel buffer ram
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 15, pos_colour, GL_STATIC_DRAW);
	glEnableVertexAttribArray(positionAttribIndex);
	glEnableVertexAttribArray(colorAttribIndex);

	//specifico il formato dei dati

	glVertexAttribPointer(positionAttribIndex, 2, GL_FLOAT, false, 20, 0);
	glVertexAttribPointer(colorAttribIndex, 3, GL_FLOAT, false, 20, (GLvoid*)8);

	//--------------------------------------
	//           FINE MIA PARTE
	//--------------------------------------

	/*std::vector<float> pos_col_buf;  // programmazione a template (tipo generics)
	for (int i = 0; i < 3; ++i) {
		pos_col_buf.push_back(positions[2 * i]);
		pos_col_buf.push_back(positions[2 * i + 1]);

		pos_col_buf.push_back(colors[3 * i]);
		pos_col_buf.push_back(colors[3 * i + 1]);
		pos_col_buf.push_back(colors[3 * i + 2]);
	}*/

	// VVV  BLOCCO SOSTITUITO  VVV
	/*

	//create render data in RAM 
	GLuint positionAttribIndex = 0;
	float positions[] = {	0.0, 0.0,  // 1st vertex
							0.5, 0.0,  // 2nd vertex
							0.5, 0.5   // 3rd vertex
	};
	//create a buffer for the render data in video RAM 
	GLuint positionsBuffer;
	glCreateBuffers(1, &positionsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, positionsBuffer);

	//declare what data in RAM are filling the bufferin video RAM 
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, positions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(positionAttribIndex);

	// specify the data format 
	glVertexAttribPointer(positionAttribIndex, 2, GL_FLOAT, false, 0, 0);

	GLuint colorAttribIndex = 1;
	float colors[] = {	1.0, 0.0, 0.0,  // 1st vertex
						0.0, 1.0, 0.0,  // 2nd vertex
						0.0, 0.0, 1.0   // 3rd vertex
	};
	//create a buffer for the render data in video RAM 
	GLuint colorsBuffer;
	glCreateBuffers(1, &colorsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorsBuffer);

	//declare what data in RAM are filling the bufferin video RAM 
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(colorAttribIndex);

	//specify the data format
	glVertexAttribPointer(colorAttribIndex, 3, GL_FLOAT, false, 0, 0);
	*/
	// ^^^  BLOCCO SOSTITUITO  ^^^


	/* create a vertex shader */
	std::string  vertex_shader_src = "#version 330\n \
        in vec2 aPosition;\
        in vec3 aColor;\
        out vec3 vColor;\
        void main(void)\
        {\
         gl_Position = vec4(aPosition, 0.0, 1.0);\
         vColor = aColor;\
        }\
       ";
	const GLchar* vs_source = (const GLchar*)vertex_shader_src.c_str();
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vs_source, NULL);
	glCompileShader(vertex_shader);
	check_shader(vertex_shader);

	/* create a fragment shader */
	std::string   fragment_shader_src = "#version 330 \n \
        out vec4 color;\
        in vec3 vColor;\
        void main(void)\
        {\
            color = vec4(vColor, 1.0);\
        }";
	const GLchar* fs_source = (const GLchar*)fragment_shader_src.c_str();
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fs_source, NULL);
	glCompileShader(fragment_shader);
	check_shader(fragment_shader);

	GLuint program_shader = glCreateProgram();
	glAttachShader(program_shader, vertex_shader);
	glAttachShader(program_shader, fragment_shader);
	glBindAttribLocation(program_shader, positionAttribIndex, "aPosition");
	glBindAttribLocation(program_shader, colorAttribIndex, "aColor");
	glLinkProgram(program_shader);
/*  \END IGNORATE  */


	/* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program_shader);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
