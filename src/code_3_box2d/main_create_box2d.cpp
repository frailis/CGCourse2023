#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include "..\common\debugging.h"
#include "..\common\renderable.h"
#include "..\common\shaders.h"
using namespace std; // permette di omettere std:: durante la dichiarazione delle variabili
renderable r;

/*  this function  sets up the buffers for rendering a box  made
    of nx by ny quadrilateral, each made of two triangles.
*/
//int nvert = 1;
//quadrato fatto da nx triangoli in larghezza, e ny triangoli in lunghezza
void create_box2d(int nx, int ny) {

    int nvert = (nx + 1) * (ny + 1);
    int ntriangles = 2 * nx * ny; //base per altezza = numero quadrati, moltpilicato per 2 = numero triangoli

    std::vector<float> pos;
    float xadd = 0.2 / nx;
    float yadd = 0.2 / ny;

    float x = -0.1; float y = -0.1;

    //genero le posizioni, a partire dalla + "negativa" fino a quella + "positiva"
    //from the most negative to the most positive
    /*
    ordine di generazione degli indici: linea guida

                1----3----5
                |  / |  / |
                | /  | /  |
                0----2----4

    */
    for (int i = 0; i <= nx; i++) {
        y = -0.1;
        for (int j = 0; j <= ny; j++) {
            pos.push_back(x); pos.push_back(y);
            y += yadd;

        }
        x += xadd;
    }

    /*float positions[] = {
        -0.1, -0.1,  // 1st vertex
        0.1, -0.1,  // 2nd vertex
        0.1, 0.1,  // 3nd vertex
        -0.1, 0.1    // 4th vertex
    };*/

    //unsigned int ind[] = { 0,1,2, 0,2,3 };
    /*unsigned int ind[] = {
        0,1,3,
        0,2,3,
        2,3,5,
        2,4,5
    };*/

    std::vector<unsigned int> indici;
    // indici dei 3 vertici del triangolino alto del singolo quadratino
    int a = 0;
    int b = 1;
    int c = ny + 2;
    for (int index = 0; index < nx * ny; index++) {

        //if this is true, that means ive reached the top of the row and need to change row
        if (b % (ny + 1) == 0) {
            a++;
            b++;
            c++;
        }

        //triangolino alto
        indici.push_back(a);
        indici.push_back(b);
        indici.push_back(c);

        //triangolino basso
        indici.push_back(a);
        indici.push_back(b + ny);
        indici.push_back(c);

        // incremento dei vertici
        a++; b++; c++;
    }
    //for debugging purposes i am trying to color the triangles
    float rainbow[] = {
        1.0, 0.0, 0.0, //red
        1.0, 0.5, 0.0, //orange
        1.0, 1.0, 0.0, //yellow
        0.0, 1.0, 0.0, //green
        0.0, 0.5, 0.5, //cyan
        0.0, 0.0, 1.0, //blue
        0.5, 0.0, 1.0, //purple
        1.0, 0.0, 1.0, //magenta
        //1.0, 1.0, 1.0,  //white
    };

    std::vector<float> colors;
    float red = 1.0;
    float green = 1.0;
    float blue = 1.0;
    int d = 0; //index for the rainbow array

    for (int counter = 0; counter < nvert; counter++) {
        if (d > 21) d = 0;
        red = rainbow[d];
        green = rainbow[d + 1];
        blue = rainbow[d + 2];
        colors.push_back(red);
        colors.push_back(green);
        colors.push_back(blue);
        d += 3;
    }
    r.create();
    r.add_vertex_attribute<float>(&pos[0], sizeof(float) * pos.size(), 0, 2); //pos.size() was nvert

    r.add_vertex_attribute<float>(&colors[0], sizeof(float) * colors.size(), 1, 3);
    r.add_indices(&indici[0], sizeof(unsigned int) * indici.size(), GL_TRIANGLES);
    check_gl_errors(__LINE__, __FILE__);
    //check_shader
}

int main(void)
{


    GLFWwindow* window;

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

    int nx = 2;
    int ny = 2;
    int nvert_render = 6 * nx * ny; //numero di vertici da renderizzrare


    create_box2d(nx, ny);
    r.bind();

    //shader

    shader s;
    s.create_program("../../src/code_2_wrapping/shaders/basic.vert", "../../src/code_2_wrapping/shaders/basic.frag");
    s.bind("uDelta");
    check_shader(s.vs);
    check_shader(s.fs);
    validate_shader_program(s.pr);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(s.pr);

        /* here the call to render the box --*/
        glDrawElements(GL_TRIANGLES, (GLsizei)nvert_render, GL_UNSIGNED_INT, 0); // nverts replaces 4
        check_gl_errors(__LINE__, __FILE__);
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