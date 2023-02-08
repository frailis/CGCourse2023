#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include "..\common\debugging.h"
#include "..\common\renderable.h"
#include "..\common\shaders.h"
#include "..\common\simple_shapes.h"

/* 
GLM library for math  https://github.com/g-truc/glm 
it's a header-only library. You can just copy the folder glm into 3dparty
and set the path properly. 
*/
#include <glm/glm.hpp>  
#include <glm/ext.hpp>  

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1000, 800, "code_4_my_first_car", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewInit();

    printout_opengl_glsl_info();

	shader basic_shader; //../../src/code_2_wrapping/shaders/basic.vert
    basic_shader.create_program("../../src/code_4_my_first_car/shaders/basic.vert", "../../src/code_4_my_first_car/shaders/basic.frag");
	basic_shader.bind("uP");
	basic_shader.bind("uV");
	basic_shader.bind("uT");
	check_shader(basic_shader.vs);
	check_shader(basic_shader.fs);
    validate_shader_program(basic_shader.pr);

	/* Set the uT matrix to Identity */
	glUseProgram(basic_shader.pr);
	glUniformMatrix4fv(basic_shader["uT"], 1, GL_FALSE, &glm::mat4(1.0)[0][0]);
	glUseProgram(0);

	check_gl_errors(__LINE__, __FILE__);

	/* create a  cube   centered at the origin with side 2*/
	renderable r_cube	= shape_maker::cube(0.5,0.6,0.0);
	renderable r_cube_window = shape_maker::cube(0, 0.6, 0.6);

	/* create a  cylinder with base on the XZ plane, and height=2*/
	renderable r_cyl	= shape_maker::cylinder(30,0.2,0.1,0.5);

	/* create 3 lines showing the reference frame*/
	renderable r_frame	= shape_maker::frame(4.0);

	check_gl_errors(__LINE__, __FILE__);

	/* Transformation to setup the point of view on the scene */
	glm::mat4 proj = glm::perspective(glm::radians(45.f), 1.33f, 0.1f, 100.f);
	glm::mat4 view = glm::lookAt(glm::vec3(0, 5, 10.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	glEnable(GL_DEPTH_TEST);

	glm::mat4 transform_matrix_carbody(1.0f);
	transform_matrix_carbody[2].z = 3.0;
	transform_matrix_carbody[1].y = 0.5;

	glm::mat4 transform_matrix_carbody2(1.0f);
	transform_matrix_carbody2[2].z = 1.0;
	transform_matrix_carbody2[1].y = 0.5;
	transform_matrix_carbody2[3].y = 0.5;

	glm::mat4 transform_window(1.0f);
	transform_window[2].z = 0.5;
	transform_window[1].y = 0.5;
	transform_window[0].x = 0.8;
	transform_window[3].x = 0;
	transform_window[3].y = 0.4;
	transform_window[3].z = 0.51;



	//avrei potuto fare matrici parametriche e far fare i calcoli al pc ma ho poca voglia
	glm::mat4 transform_matrix_wheel(1.0f);
	transform_matrix_wheel[0].x = 0;
	transform_matrix_wheel[0].y = 0.5;
	transform_matrix_wheel[1].x = -0.2;
	transform_matrix_wheel[1].y = 0;
	transform_matrix_wheel[2].z = 0.5;
	transform_matrix_wheel[3].z = 1.3;
	transform_matrix_wheel[3].y = -0.5;
	transform_matrix_wheel[3].x = 1.2;

	/*Initialize the matrix to implement the continuos rotation aroun the y axis*/
	glm::mat4 R = glm::mat4(1.f);

	int it = 0;
	/* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
		it++;
		/*incremente the rotation by 0.01 radians*/
		R = glm::rotate(R, 0.001f, glm::vec3(0.f, 1.f, 0.f));

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		
		glm::mat4 M = view*R;
        glUseProgram(basic_shader.pr);
		glUniformMatrix4fv(basic_shader["uP"], 1, GL_FALSE, &proj[0][0]);
		glUniformMatrix4fv(basic_shader["uV"], 1, GL_FALSE, &M[0][0]);
		check_gl_errors(__LINE__, __FILE__);
		

		/* render box and cylinders so that the look like a car */
		/* instructions: you need to set up the matrix to set as the uniform variable
		uT in the vertex shader (file basic.vert):
		glUniformMatrix4fv(basic_shader["uT"], 1, GL_FALSE, YOUR_MATRIX_GOES_HERE);
		before rendering every shape so that the shape is transformed as needed.
		You will have to render at least once the cube and four times the cylinder.
		Then again, you can free you immagination and make a more inventive drawing.
		*/

		transform_matrix_wheel[3].z = 1.3;
		transform_matrix_wheel[3].x = 1.2;
		glUniformMatrix4fv(basic_shader["uT"], 1, GL_FALSE, &transform_matrix_wheel[0][0]);
	 	r_cyl.bind();
	 	glDrawElements(GL_TRIANGLES,r_cyl.in, GL_UNSIGNED_INT, 0);

		transform_matrix_wheel[3].z = -1.3;
		transform_matrix_wheel[3].x = 1.2;
		glUniformMatrix4fv(basic_shader["uT"], 1, GL_FALSE, &transform_matrix_wheel[0][0]);
		r_cyl.bind();
		glDrawElements(GL_TRIANGLES, r_cyl.in, GL_UNSIGNED_INT, 0);

		transform_matrix_wheel[3].z = 1.3;
		transform_matrix_wheel[3].x = -0.8;
		glUniformMatrix4fv(basic_shader["uT"], 1, GL_FALSE, &transform_matrix_wheel[0][0]);
		r_cyl.bind();
		glDrawElements(GL_TRIANGLES, r_cyl.in, GL_UNSIGNED_INT, 0);

		transform_matrix_wheel[3].z = -1.3;
		transform_matrix_wheel[3].x = -0.8;
		glUniformMatrix4fv(basic_shader["uT"], 1, GL_FALSE, &transform_matrix_wheel[0][0]);
		r_cyl.bind();
		glDrawElements(GL_TRIANGLES, r_cyl.in, GL_UNSIGNED_INT, 0);


		glUniformMatrix4fv(basic_shader["uT"], 1, GL_FALSE, &transform_matrix_carbody2[0][0]);
		r_cube.bind();
		glDrawElements(GL_TRIANGLES, r_cube.in, GL_UNSIGNED_INT, 0);

		glUniformMatrix4fv(basic_shader["uT"], 1, GL_FALSE, &transform_matrix_carbody[0][0]);
		r_cube.bind();
		glDrawElements(GL_TRIANGLES, r_cube.in, GL_UNSIGNED_INT, 0);


		glUniformMatrix4fv(basic_shader["uT"], 1, GL_FALSE, &transform_window[0][0]);
		r_cube_window.bind();
		glDrawElements(GL_TRIANGLES, r_cube.in, GL_UNSIGNED_INT, 0);
		/* ******************************************************/


		r_frame.bind();
		glDrawArrays(GL_LINES, 0, 6);

        check_gl_errors(__LINE__,__FILE__);
        glUseProgram(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
