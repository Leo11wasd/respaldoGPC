#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include "linmath.h"

#include <stb_image.h>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_s.h>

#include "Resorte_04_Normales_TEX_ejes.cpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderEjesSource ="#version 330 core\n"
    "uniform mat4 MVP;\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aCol;\n"
    "out vec3 color;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = MVP * vec4(aPos.x,aPos.y,aPos.z, 1.0);\n"
    "   color = aCol;\n"
    "}\0";

const char *fragmentShaderEjesSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "//uniform vec4 ourColor;\n"
    "in vec3 color;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(color,1.0);//ourColor;\n"
    "}\n\0";

const char *vertexShaderSource = "#version 330 core\n"
    "uniform mat4 MVP;\n"
    "uniform mat4 MW;\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "layout (location = 3) in vec3 aNorm;\n"
    "out vec3 ourColor;\n"
    "out vec2 TexCoord;\n"
    "out vec3 Normales;\n"
    "out vec3 Posicion;\n"
    "vec4 pospre;\n"
    "vec4 normpre;\n"
    "void main()\n"
    "{\n"
    "	gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   pospre = MW * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   Posicion= vec3(pospre.x,pospre.y,pospre.z);\n"
    "	ourColor = aColor;\n"
    "	TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
    "   normpre = MW * vec4(aNorm.x,aNorm.y,aNorm.z,0.0);\n"
    "   Normales= vec3(normpre.x,normpre.y,normpre.z);\n"
    "}\n\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "in vec2 TexCoord;\n"
    "in vec3 Normales;\n"
    "in vec3 Posicion;\n"
    "// texture sampler\n"
    "uniform sampler2D texture1;\n"
    "uniform vec4 ambiental;\n"
    "uniform vec3 Luzpos;\n"
    "uniform vec4 colorLuz;\n"
    "uniform vec3 posCamara;\n"
    "vec4 color_frag;\n"
    "void main()\n"
    "{\n"
    "float specularStrength=0.9;\n"
    "vec3 norm=normalize(Normales);\n"
    "vec3 lightDir= normalize(Luzpos-Posicion);\n"
	"float diff = max(dot(norm, lightDir), 0.0);\n"
    "vec4 Luzdifusa = diff * colorLuz;\n"

    "vec3 viewDir = normalize(posCamara - Posicion);\n"
    "vec3 reflectDir = reflect(-lightDir, norm);\n"
    "float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
    "vec4 specular = specularStrength * spec * colorLuz;\n"

	"     color_frag = texture(texture1, TexCoord);\n"
	"     FragColor = vec4(color_frag*(Luzdifusa+ambiental+specular));\n"
    "}\n\0";

const char *vertexShaderCuboLuzSource = "#version 330 core\n"
    "uniform mat4 MVP;\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position =  MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\n\0";

const char *fragmentShaderCuboLuzSource = "#version 330 core\n"
    "uniform vec4 ElColor;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(ElColor.x,ElColor.y,ElColor.z,ElColor.t); //vec4(1.0); // set all 4 vector values to 1.0\n"
    "}\n\0";

// ==================================================================
//                            Main
// ==================================================================

struct V3
{
    float x;
    float y;
    float z;
};

vec3 eye, target, up;

V3 eye0    = { 0.0f, 0.0f, -18.0f};   //{ 0.0f, 0.0f, -20.0f};
V3 target0 = { 0.f,  0.f,   -6.0f}; //{ 0.1f, 0.3f,  -6.0f};   //{ 0.0f, 0.0f, -0.1f};
V3 up0     = { 0.0f, 1.0f,   0.0f};   //{ 0.0f, 1.0f, 0.0f};

vec4 elcolor;
vec4 ambiental;
float radio_camara;

int main()
{
    elcolor[0] = 1.f;
    elcolor[1] = 1.f;
    elcolor[2] = 1.f;
    elcolor[3] = 0.9f;

    ambiental[0] = 1.f;
    ambiental[1] = 1.f;
    ambiental[2] = 0.3f;
    ambiental[3] = 0.9f;

    eye[0] = eye0.x;
    eye[1] = eye0.y;
    eye[2] = eye0.z;

    target[0] = target0.x;
    target[1] = target0.y;
    target[2] = target0.z;

    up[0] = up0.x;
    up[1] = up0.y;
    up[2] = up0.z;

    vec3 radio_vector;
    vec3_sub(radio_vector,eye,target);
    radio_camara = vec3_len(radio_vector);

    GLint mvp_location,posCamara_location,mw_location, mvp_ejes_location, mvp_cuboLuz_location,
          el_color_location, vpos_location, vcol_location,ambiental_location, luzpos_location,colorluz_location;

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "10_Camara Depth y Ejes", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // ===============================================================
    //              Uso del buffer de profundidd
    // ===============================================================
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); // GL_ALWAYS, GL_LESS no,GL_LEQUAL,GL_EQUAL, GL_GEQUAL, GL_GREATER no,GL_NOTEQUAL no , , , , ,
    //glDepthRange(0.85, 1.0);

    // ========================================================
    //                    SHADERS
    // ========================================================
    // ========================================================
    //                    SHADER para las figuras
    // ========================================================
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    mvp_location = glGetUniformLocation(shaderProgram, "MVP");
    mw_location = glGetUniformLocation(shaderProgram, "MW");
    ambiental_location =glGetUniformLocation(shaderProgram, "ambiental");
    luzpos_location=glGetUniformLocation(shaderProgram,"Luzpos");
    posCamara_location=glGetUniformLocation(shaderProgram,"posCamara");
    colorluz_location=glGetUniformLocation(shaderProgram,"colorLuz");
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // ========================================================
    //               fin del Shader para las figuras
    // ========================================================

    // ========================================================
    //                    SHADER para los ejes
    // ========================================================
        // vertex shader
    unsigned int vertexEjesShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexEjesShader, 1, &vertexShaderEjesSource, NULL);
    glCompileShader(vertexEjesShader);
    // check for shader compile errors
    glGetShaderiv(vertexEjesShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexEjesShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX Ejes::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentEjesShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentEjesShader, 1, &fragmentShaderEjesSource, NULL);
    glCompileShader(fragmentEjesShader);
    // check for shader compile errors
    glGetShaderiv(fragmentEjesShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentEjesShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT Ejes::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderEjesProgram = glCreateProgram();
    glAttachShader(shaderEjesProgram, vertexEjesShader);
    glAttachShader(shaderEjesProgram, fragmentEjesShader);
    glLinkProgram(shaderEjesProgram);
    // check for linking errors
    glGetProgramiv(shaderEjesProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderEjesProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER Ejes::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    mvp_ejes_location = glGetUniformLocation(shaderEjesProgram, "MVP");

    glDeleteShader(vertexEjesShader);
    glDeleteShader(fragmentEjesShader);
    // ========================================================
    //               fin del shader para los ejes
    // ========================================================
    // ========================================================
    //               SHADER para el cubo de luz
    // ========================================================
            // vertex shader
    unsigned int vertexCuboLuzShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexCuboLuzShader, 1, &vertexShaderCuboLuzSource, NULL);
    glCompileShader(vertexCuboLuzShader);
    // check for shader compile errors
    glGetShaderiv(vertexCuboLuzShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexCuboLuzShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX CuboLuz::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentCuboLuzShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentCuboLuzShader, 1, &fragmentShaderCuboLuzSource, NULL);
    glCompileShader(fragmentCuboLuzShader);
    // check for shader compile errors
    glGetShaderiv(fragmentCuboLuzShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentCuboLuzShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT CuboLuz::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderCuboLuzProgram = glCreateProgram();
    glAttachShader(shaderCuboLuzProgram, vertexCuboLuzShader);
    glAttachShader(shaderCuboLuzProgram, fragmentCuboLuzShader);
    glLinkProgram(shaderCuboLuzProgram);
    // check for linking errors
    glGetProgramiv(shaderCuboLuzProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderCuboLuzProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER CuboLuz::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    mvp_cuboLuz_location = glGetUniformLocation(shaderCuboLuzProgram, "MVP");
    el_color_location     = glGetUniformLocation(shaderCuboLuzProgram, "ElColor");

    glDeleteShader(vertexCuboLuzShader);
    glDeleteShader(fragmentCuboLuzShader);
    // ========================================================
    //              fin del shader para el cubo de luz
    // ========================================================
    // =================================================================================
    //     FIN DE SHADERS
    // =================================================================================

    // build and compile our shader program
    // ------------------------------------
    //Shader ourShader("4.1.texture.vs", "4.1.texture.fs");

    mvp_location      = glGetUniformLocation(shaderProgram, "MVP");


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    int num_indices;

    int              num_vertices_prisma;
    int              num_indices_prisma;
    VERTICE_TEX_NOR *vertices_prisma;
    unsigned int    *indices_prisma;

    int              num_vertices_esfera;
    int              num_indices_esfera;
    VERTICE_TEX_NOR *vertices_esfera;
    unsigned int    *indices_esfera;

    int              num_vertices_cilindro;
    int              num_indices_cilindro;
    VERTICE_TEX_NOR *vertices_cilindro;
    unsigned int    *indices_cilindro;

    int              num_vertices_cubo;
    int              num_indices_cubo;
    VERTICE_TEX_NOR *vertices_cubo;
    unsigned int    *indices_cubo;



    int PRISMA   = 1;
    int ESFERA   = 2;
    int CILINDRO = 3;
    int CUBO     = 4;

    int TIPO;

    TIPO = PRISMA;

    creaSolRevCaras(PRISMA,
                    &num_vertices_prisma, &vertices_prisma, // notense los & para obtener el lugar de memoria (referencia) de las variables
                    &num_indices_prisma,  &indices_prisma);

    creaSolRevCaras(ESFERA,
                    &num_vertices_esfera, &vertices_esfera, // notense los & para obtener el lugar de memoria (referencia) de las variables
                    &num_indices_esfera,  &indices_esfera);

    creaSolRevCaras(CILINDRO,
                    &num_vertices_cilindro, &vertices_cilindro, // notense los & para obtener el lugar de memoria (referencia) de las variables
                    &num_indices_cilindro,  &indices_cilindro);

    creaSolRevCaras(CUBO,
                    &num_vertices_cubo, &vertices_cubo, // notense los & para obtener el lugar de memoria (referencia) de las variables
                    &num_indices_cubo,  &indices_cubo);


    // ================================================================================
    //                            EJES
    // ================================================================================

    int num_vertices_ejes;
    VERTICE_TEX_NOR *vertices_ejes = ejes(&num_vertices_ejes);

    unsigned int VBO_ejes, VAO_ejes;

    //if(TIPO == EJES)
    {
      glGenVertexArrays(1, &VAO_ejes);
      glGenBuffers(1, &VBO_ejes);

      // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
      glBindVertexArray(VAO_ejes);

      glBindBuffer(GL_ARRAY_BUFFER, VBO_ejes);
      glBufferData(GL_ARRAY_BUFFER, num_vertices_ejes*sizeof(VERTICE_TEX_NOR), vertices_ejes, GL_STATIC_DRAW);

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VERTICE_TEX_NOR), (void*)0);
      glEnableVertexAttribArray(0);

      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VERTICE_TEX_NOR), (void*)(3*sizeof(float)));
      glEnableVertexAttribArray(1);

    }

    // ================================================================================
    //                               PRISMA
    // ================================================================================

    unsigned int VBO_prisma, VAO_prisma, EBO_prisma;


    //if(TIPO == PRISMA)
    {
      glGenVertexArrays(1, &VAO_prisma);
      glGenBuffers(1, &VBO_prisma);
      glGenBuffers(1, &EBO_prisma);

      num_indices = num_indices_prisma;

      // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
      glBindVertexArray(VAO_prisma);

      glBindBuffer(GL_ARRAY_BUFFER, VBO_prisma);
      glBufferData(GL_ARRAY_BUFFER, num_vertices_prisma*sizeof(VERTICE_TEX_NOR), vertices_prisma, GL_STATIC_DRAW);

      //posición del vertice(x,y,z)
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VERTICE_TEX_NOR), (void*)0);
      glEnableVertexAttribArray(0);

      // color (r,g,b)
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VERTICE_TEX_NOR), (void*)(3*sizeof(float)));
      glEnableVertexAttribArray(1);
        //textura (s.t)
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VERTICE_TEX_NOR), (void*)(6 * sizeof(float)));
      glEnableVertexAttribArray(2);
        //normales (nx,ny,nz)
      glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VERTICE_TEX_NOR), (void*)(8 * sizeof(float)));
      glEnableVertexAttribArray(3);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_prisma);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices_prisma * sizeof(unsigned int), indices_prisma, GL_STATIC_DRAW);

    }
    // ================================================================================
    //                               FIN DE PRISMA
    // ================================================================================

    // ================================================================================
    //                                   CUBO
    // ================================================================================

    unsigned int VBO_cubo, VAO_cubo, EBO_cubo;


    //if(TIPO == 4) // CUBO
    {
      glGenVertexArrays(1, &VAO_cubo);
      glGenBuffers(1, &VBO_cubo);
      glGenBuffers(1, &EBO_cubo);

      num_indices = num_indices_cubo;

      // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
      glBindVertexArray(VAO_cubo);

      glBindBuffer(GL_ARRAY_BUFFER, VBO_cubo);
      glBufferData(GL_ARRAY_BUFFER, num_vertices_cubo*sizeof(VERTICE_TEX_NOR), vertices_cubo, GL_STATIC_DRAW);

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VERTICE_TEX_NOR), (void*)0);
      glEnableVertexAttribArray(0);

      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VERTICE_TEX_NOR), (void*)(3*sizeof(float)));
      glEnableVertexAttribArray(1);

      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VERTICE_TEX_NOR), (void*)(6 * sizeof(float)));
      glEnableVertexAttribArray(2);

      glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VERTICE_TEX_NOR), (void*)(8 * sizeof(float)));
      glEnableVertexAttribArray(3);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_cubo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices_cubo * sizeof(unsigned int), indices_cubo, GL_STATIC_DRAW);

    }
    // ================================================================================
    //                               FIN DE CUBO
    // ================================================================================

    // ================================================================================
    //                               ESFERA
    // ================================================================================

    unsigned int VBO_esfera, VAO_esfera, EBO_esfera;

    //if(TIPO == ESFERA)
    {
      glGenVertexArrays(1, &VAO_esfera);
      glGenBuffers(1,      &VBO_esfera);
      glGenBuffers(1,      &EBO_esfera);

      num_indices = num_indices_esfera;

      // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
      glBindVertexArray(VAO_esfera);

      glBindBuffer(GL_ARRAY_BUFFER, VBO_esfera);
      glBufferData(GL_ARRAY_BUFFER, num_vertices_esfera*sizeof(VERTICE_TEX_NOR), vertices_esfera, GL_STATIC_DRAW);

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VERTICE_TEX_NOR), (void*)0);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VERTICE_TEX_NOR), (void*)(3*sizeof(float)));
      glEnableVertexAttribArray(1);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_esfera);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices_esfera * sizeof(unsigned int), indices_esfera, GL_STATIC_DRAW);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VERTICE_TEX_NOR), (void*)(6 * sizeof(float)));
      glEnableVertexAttribArray(2);
      glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VERTICE_TEX_NOR), (void*)(8 * sizeof(float)));
      glEnableVertexAttribArray(3);

      //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO_esfera);
    }
    // ================================================================================
    //                              FIN DE ESFERA
    // ================================================================================

    // ================================================================================
    //                               CILINDRO
    // ================================================================================

    unsigned int VBO_cilindro, VAO_cilindro, EBO_cilindro;

    //if(TIPO == CILINDRO)
    {
      glGenVertexArrays(1, &VAO_cilindro);
      glGenBuffers(1,      &VBO_cilindro);
      glGenBuffers(1,      &EBO_cilindro);

      num_indices = num_indices_cilindro;

      // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
      glBindVertexArray(VAO_cilindro);

      glBindBuffer(GL_ARRAY_BUFFER, VBO_cilindro);
      glBufferData(GL_ARRAY_BUFFER, num_vertices_cilindro*sizeof(VERTICE_TEX_NOR), vertices_cilindro, GL_STATIC_DRAW);

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VERTICE_TEX_NOR), (void*)0);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VERTICE_TEX_NOR), (void*)(3*sizeof(float)));
      glEnableVertexAttribArray(1);

      // texture coord attribute
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VERTICE_TEX_NOR), (void*)(6 * sizeof(float)));
      glEnableVertexAttribArray(2);

      glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VERTICE_TEX_NOR), (void*)(8 * sizeof(float)));
      glEnableVertexAttribArray(3);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_cilindro);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices_cilindro * sizeof(unsigned int), indices_cilindro, GL_STATIC_DRAW);
    }
    // ================================================================================
    //                              FIN DE CILINDRO
    // ================================================================================



    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);


    // bind the VAO (it was already bound, but just to demonstrate): seeing as we only have a single VAO we can
    // just bind it beforehand before rendering the respective triangle; this is another approach.

    //glBindVertexArray(VAO);


    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);


    // load and create a texture
    // -------------------------
    //             texture1
    // ------------------------_
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load(FileSystem::getPath("resources/textures/earthCylindric.jpg").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // ----------------------------
    //            texture2
    // ----------------------------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    data = stbi_load(FileSystem::getPath("resources/textures/brickwall.jpg").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // ----------------------------
    //      fin de carga y definición de texturas
    // ----------------------------

    // render loop
    // -----------
    unsigned int usar_textura;
    //glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
        //glClearColor(0.f, 0.f, 0.f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate the shader before any calls to glUniform
        //glUseProgram(shaderProgram);

        // bind Texture
        //glBindTexture(GL_TEXTURE_2D, texture2);

        // render container
        //ourShader.use();

		float ratio;
		int width, height;
		mat4x4 m, p, mvp,mw,pers,mk,mtr,mcam;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);
		//glClear(GL_COLOR_BUFFER_BIT);

		//mat4x4_identity(m);
		//mat4x4_rotate_X(m, m, (float)glfwGetTime());
		//mat4x4_rotate_Y(m, m, (float)glfwGetTime());

		//mat4x4_ortho( p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		//mat4x4_mul(mvp, p, m);

		float tr_x[4] = { 0.3f, -0.3f,   0.5f,  5.f}; // el último es el cubo de luz
		float tr_y[4] = { 0.3f, -0.3f,   0.5f,  0.f};
		float tr_z[4] = {-10.0f, -5.0f, -8.0f, -5.f};
        float posluz[3];
        posluz[0]=tr_x[3];
        posluz[1]=tr_y[3];
        posluz[2]=tr_z[3];
		mat4x4_perspective(pers,30.0*atan(1.0)/45.0,800.0/600.0, 1.0,40.0);

		eye[0] = target[0] + radio_camara * cosf((float)glfwGetTime());
		eye[2] = target[2] - radio_camara * sinf((float)glfwGetTime());

        mat4x4_look_at( mcam, eye, target, up );

        //mat4x4_identity(mvp);
        //mat4x4_mul(mvp,mcam,mvp);
        mat4x4_mul(mvp,pers,mcam);

        glBindVertexArray(VAO_ejes);
        glUseProgram(shaderEjesProgram);
        glUniformMatrix4fv(mvp_ejes_location, 1, GL_FALSE, (const GLfloat*)mvp);
        glDrawArrays(GL_LINES,0,num_vertices_ejes);

		for(int t = 0; t < 4; t++)
        {
          mat4x4_identity(m);
          if( t < 3)
          {
		    mat4x4_rotate_X(m, m, (t == 1?-1.0:1.0) * (float)glfwGetTime());
		    mat4x4_rotate_Y(m, m, (t == 1?-1.0:1.0) * (float)glfwGetTime());
            mat4x4_rotate_Z(m, m, (t == 1?-1.0:1.0) * (float)glfwGetTime());

            mat4x4_dup(mk,m);
		    mat4x4_translate(mtr,tr_x[t],tr_y[t],tr_z[t]);

            mat4x4_mul(mk,mtr,mk);

            mat4x4_dup(mw,mk);
            glUseProgram(shaderProgram);
          }
          else
          {
            mat4x4_dup(mk,m);
            glUseProgram(shaderCuboLuzProgram);
          }
		  //mat4x4_mul(mvp, pers, mk);

		  mat4x4_identity(mvp);
          mat4x4_mul(mvp,mcam,mk);
          mat4x4_mul(mvp,pers,mvp);



          switch(t)
          {
            case 0:
              num_indices = num_indices_esfera;
              usar_textura = texture1;
              glBindTexture(GL_TEXTURE_2D, usar_textura);
              glUniformMatrix4fv(mw_location, 1, GL_FALSE, (const GLfloat*)mw);
              glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);
              glUniform4fv(ambiental_location, 1, (const GLfloat*)ambiental);
              glUniform4fv(luzpos_location,1,(const GLfloat*) posluz);
              glUniform4fv(posCamara_location,1,(const GLfloat*)eye);
              glUniform4fv(colorluz_location,1,(const GLfloat*)elcolor);


              glBindVertexArray(VAO_esfera);
              break;

            case 1:
              num_indices = num_indices_cilindro;
              usar_textura = texture2;
              glBindTexture(GL_TEXTURE_2D, usar_textura);
              glUniformMatrix4fv(mw_location, 1, GL_FALSE, (const GLfloat*)mw);
              glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);
              glUniform4fv(ambiental_location, 1, (const GLfloat*)elcolor);

              glBindVertexArray(VAO_cilindro);
              break;

            case 2:
              num_indices = num_indices_prisma;
              usar_textura = texture1;
              glBindTexture(GL_TEXTURE_2D, usar_textura);
              glUniformMatrix4fv(mw_location, 1, GL_FALSE, (const GLfloat*)mw);

              glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);
              glUniform4fv(ambiental_location, 1, (const GLfloat*)elcolor);

              glBindVertexArray(VAO_prisma);
              break;

            case 3:
              num_indices = num_indices_cubo;
              glUniformMatrix4fv(mvp_cuboLuz_location, 1, GL_FALSE, (const GLfloat*)mvp);
              glUniform4fv(el_color_location, 1, (const GLfloat*)elcolor);
              glBindVertexArray(VAO_cubo);
              break;

          }

          glDrawElements(GL_TRIANGLES,num_indices,GL_UNSIGNED_INT,0);

        }
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

    {
      glDeleteVertexArrays(1, &VAO_prisma);
      glDeleteBuffers(1,      &VBO_prisma);
      glDeleteBuffers(1,      &EBO_prisma);
    }
    {
      glDeleteVertexArrays(1, &VAO_esfera);
      glDeleteBuffers(1,      &VBO_esfera);
      glDeleteBuffers(1,      &EBO_esfera);
    }
    {
      glDeleteVertexArrays(1, &VAO_cilindro);
      glDeleteBuffers(1,      &VBO_cilindro);
      glDeleteBuffers(1,      &EBO_cilindro);
    }

    //glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
