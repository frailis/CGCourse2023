#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include "..\common\debugging.h"
#include "..\common\renderable.h"
#include "..\common\shaders.h"

/* if you want you can use the support struct "renderable" */
renderable r;

/*	this function  sets up the buffers for rendering a box  made
	of nx by ny quadrilateral, each made of two triangles. 
*/
void create_box2d(int nx, int ny) {
    float px, py, passox, passoy;
    px = py = (float) - 0.1;
    passox = 0.2 / (float)nx;
    passoy = 0.2 / (float)ny;

    float colors[] = {
        1.0, 0.0, 0.0,
        0.0, 0.0, 1.0,
        0.0, 1.0, 0.0,
        0.0, 0.5, 0.5
    };

    std::vector<unsigned int> ind_vec;
    std::vector<float> pos_vec;
    int c = 0;
    for (int i = 0; i <= nx; ++i) {
        py = -0.1;

        for (int j = 0; j <= ny; ++j) {
            c = (c + 1) % 3;
            pos_vec.push_back(px);
            pos_vec.push_back(py);

            pos_vec.push_back(colors[c * 3]);
            pos_vec.push_back(colors[c * 3 + 1]);
            pos_vec.push_back(colors[c * 3 + 2]);
            py = py + passoy;
        }
        px = px + passox;
    }
    int padd = 0;
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {

            ind_vec.push_back(padd + j);
            ind_vec.push_back(padd + j + 1);
            ind_vec.push_back(padd + j + ny + 2);

            ind_vec.push_back(padd + j);
            ind_vec.push_back(padd + j + ny + 2);
            ind_vec.push_back(padd + j + ny + 1);
        }
        padd = padd + ny + 1;
    }

    for (int i = 0; i < pos_vec.size(); i++) {
        std::cout << pos_vec.at(i) << ' ';
    }

    for (int i = 0; i < ind_vec.size(); i++) {
        std::cout << ind_vec.at(i) << ' ';
    }
    

    
    int dim = ((ny + 1) * (nx + 1)) * 3;
    r.create();
    r.add_vertex_attribute<float>(&pos_vec[0], sizeof(float) * (dim), 0, 2, GL_FLOAT, 5 * sizeof(float), 0);
    r.add_vertex_attribute<float>(&pos_vec[0], sizeof(float) * dim, 1, 3, GL_FLOAT, 20, 8);

    std::cout << 12.0 * nx;
    r.add_indices(&ind_vec[0], sizeof(unsigned int) * (nx*ny*6), GL_TRIANGLES);
}

int main(void)
{
    GLFWwindow* window;
    int nx = 3;
    int ny = 3;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1000, 800, "code_3_box2d", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
   
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewInit();

    printout_opengl_glsl_info();
    
	create_box2d(nx, ny);
	/* uncomment if you used the support class renderable */
	r.bind();


    //shader
    shader s;
    s.create_program("../../src/code_2_wrapping/shaders/basic.vert", "../../src/code_2_wrapping/shaders/basic.frag");
    s.bind("uDelta");
    check_shader(s.vs);
    check_shader(s.fs);
    validate_shader_program(s.pr);

    check_gl_errors(__LINE__, __FILE__);



    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);



		/* here the call to render the box --*/
		// glDraw ....
        glUseProgram(s.pr);


        glDrawElements(GL_TRIANGLES, (nx*ny*6), GL_UNSIGNED_INT, NULL);
        glUseProgram(0);

		/* -------------------------------------*/
		
		/* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
