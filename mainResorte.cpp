// Ejemplo_02_TetraedroAlambre.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//
//
// Resorte de alambre
//
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

#include "linmath.h"

#include <stdlib.h>
#include <stdio.h>

float a = 0.5f;

#include"Resorte_01.cpp"

static const char* vertex_shader_text =
"#version 110\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec3 vPos; // vec2 \n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 1.0); // gl_Position = MVP * vec4(vPos, 0.0, 1.0);  \n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 110\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}
//FUNCION GENERADORA PARA EL SOLIDO DE REVOLUCION
float func(float x){
return .5;
}

int main(void)
{
//GENERA VERTICES Y ARREGLO DE INDICES DE UNION "CARAS"
    int num_vertices;

    int M_puntos_rodaja   = 5;
    int N_rodajas         = 3;
    int num_caras=(2* M_puntos_rodaja * (N_rodajas - 1))+ 2*M_puntos_rodaja;

    VERTICE *vertices = creaSolRev2(-a,a,N_rodajas,M_puntos_rodaja,func);
    num_vertices = (M_puntos_rodaja*N_rodajas)+2;

    int** caras=new int*[num_caras];
    for (int i = 0; i < a; i++) {
        caras[i] = new int[3]; // Crear un arreglo de int de tamaño 3 para cada fila
    }

    caras=generaCarasArr(N_rodajas,M_puntos_rodaja,num_caras);


    //SETTINGS PUERTO DE VISION Y GLFW
	GLFWwindow* window;
	GLuint vertex_buffer, vertex_shader, fragment_shader, program,EBO,VAO;
	GLint mvp_location, vpos_location, vcol_location;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(640, 480, "Resorte-ronte", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);

	//DECLARACION Y BINDING DE VERTEX ARRAY OBJECT, VERTEX BUFFER Y ELEMENT BUFFER
    glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &vertex_buffer);
	glGenBuffers(1,&EBO);
    //VAO
    glBindVertexArray(VAO);
    //VBO; VERTICES
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, num_vertices*sizeof(vertices), vertices, GL_STATIC_DRAW);

    //EBO; INDICES
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,3*num_caras*sizeof(caras[0]),caras,GL_STATIC_DRAW);


    //VAO POINTERS
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);


    //SHADERS Y VARIABLES "UNIFORMS"
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
	glCompileShader(vertex_shader);

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
	glCompileShader(fragment_shader);

	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
    // A PARTIR DE AQUI YA NO HAGO COSAS CON LOS SHADERS
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

	mvp_location = glGetUniformLocation(program, "MVP");
	vpos_location = glGetAttribLocation(program, "vPos");
	vcol_location = glGetAttribLocation(program, "vCol");

	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*)0);
	glEnableVertexAttribArray(vcol_location);
	glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,	sizeof(vertices[0]), (void*)(sizeof(float) * 3)); // decia 2



    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

//OCULTAMIENTO DE CARAS CON CULLFACE

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CW);


//VALIDACIÓN CORRECTO RECIBIMIENTO DE ARREGLO DE VERTICES Y DE INDICES
/*
int c=0;
    for (int i=0;i<num_caras_t;i++){
             if(i%3==0){
                std::cout <<"\n"<<c<<": ";
                c++;
            }
            std::cout << caras[i] << " ";
    }


*/
    for(int k = 0; k < (M_puntos_rodaja*N_rodajas)+2; k++)
      printf("%d %10.6f %10.6f %10.6f , %f %f %f\n",k,vertices[k].x,vertices[k].y,vertices[k].z,vertices[k].r,vertices[k].g,vertices[k].b);

for (int i = 0; i < num_caras; i++) {
        for (int j = 0; j < 3; j++) {
            std::cout << caras[i][j] << " ";
        }
        std::cout << std::endl; // Cambiar de línea después de cada fila de 3 elementos
    }


      //RENDER LOOP
	while (!glfwWindowShouldClose(window))
	{
	    float ratio;
		int width, height;
		mat4x4 m, p, mvp;
		//CONFIG VIEWPORT
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);



		mat4x4_identity(m);
		//mat4x4_rotate_X(m, m, (float)glfwGetTime());
		mat4x4_rotate_Y(m, m, (float)glfwGetTime()/2);
		mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		mat4x4_mul(mvp, p, m);
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);

		//COSAS PA DIBUJAR XDXD
		glUseProgram(program);
		glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, num_caras*3, GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0);

		//glDrawArrays(GL_LINE_STRIP , 0, num_vertices);//glDrawArrays(GL_LINE_STRIP , 0, M); //glDrawArrays(GL_LINE_LOOP , 0, 12); // glDrawArrays(GL_TRIANGLES, 0, 3);

        //glDrawArrays(GL_POINTS,0,num_vertices);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
