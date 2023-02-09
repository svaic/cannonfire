#include <OpenGLPrj.hpp>

#include <GLFW/glfw3.h>

#include <Shader.hpp>

#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <vector>
#include <iostream>

const std::string program_name = ("Transformation basics");

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;



// static const char *vertexShaderSource = "#version 330 core"
// "layout (location = 0) in vec3 aPos;"
// "layout (location = 1) in vec3 aColor;"
// "uniform mat4 model;"
// "uniform mat4 view;"
// "uniform mat4 projection;"
// "out vec3 ourColor;"
// "void main()"
// "{"
//         "gl_Position = projection * view * model * vec4(aPos, 1.0);"
//         "ourColor = aColor; "
// "}";
// static const char *fragmentShaderSource = "#version 330 core"
// "out vec4 FragColor;"
// "uniform sampler2D texture1;"
// "uniform sampler2D texture2;"
// "in vec3 ourColor;"
// "void main()"
// "{"
//         "FragColor = vec4(ourColor, 1.0);"
// "}";


int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(
      GLFW_OPENGL_FORWARD_COMPAT,
      GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
                                          program_name.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------

    std::string shader_location("../res/shaders/");

    std::string used_shaders("shader");

    Shader ourShader(shader_location + used_shaders + std::string(".vert"),
                     shader_location + used_shaders + std::string(".frag"));

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------


    std::vector<float> vertices;

    vertices.push_back(-0.05f);
    vertices.push_back(-0.5f);
    vertices.push_back(-0.05f);



    vertices.push_back(0.05f);
    vertices.push_back(-0.5f);
    vertices.push_back(-0.05f);


    vertices.push_back(0.05f);
    vertices.push_back(0.5f);
    vertices.push_back(-0.05f);


    vertices.push_back(0.05f);
    vertices.push_back(0.5f);
    vertices.push_back(-0.05f);


    vertices.push_back(0.05f);
    vertices.push_back(-0.5f);
    vertices.push_back(0.05f);


    vertices.push_back(-0.05f);
    vertices.push_back(-0.5f);
    vertices.push_back(-0.05f);





////////////////         1




    vertices.push_back(-0.05f);
    vertices.push_back(-0.5f);
    vertices.push_back(0.05f);



    vertices.push_back(0.05f);
    vertices.push_back(-0.5f);
    vertices.push_back(0.05f);



    vertices.push_back(0.05f);
    vertices.push_back(0.5f);
    vertices.push_back(0.05f);



    vertices.push_back(0.05f);
    vertices.push_back(0.5f);
    vertices.push_back(0.05f);



    vertices.push_back(-0.05f);
    vertices.push_back(0.5f);
    vertices.push_back(0.05f);



    vertices.push_back(-0.05f);
    vertices.push_back(-0.5f);
    vertices.push_back(0.05f);




/////////////////////////////           2

    vertices.push_back(-0.05f);
    vertices.push_back(0.5f);
    vertices.push_back(0.05f);



    vertices.push_back(-0.05f);
    vertices.push_back(0.5f);
    vertices.push_back(-0.05f);



    vertices.push_back(-0.05f);
    vertices.push_back(-0.5f);
    vertices.push_back(-0.05f);



    vertices.push_back(-0.05f);
    vertices.push_back(-0.5f);
    vertices.push_back(-0.05f);



    vertices.push_back(-0.05f);
    vertices.push_back(-0.5f);
    vertices.push_back(0.05f);


    vertices.push_back(-0.05f);
    vertices.push_back(0.5f);
    vertices.push_back(0.05f);




///////////////////         3


    vertices.push_back(0.05f);
    vertices.push_back(0.5f);
    vertices.push_back(0.05f);



    vertices.push_back(0.05f);
    vertices.push_back(0.5f);
    vertices.push_back(-0.05f);



    vertices.push_back(0.05f);
    vertices.push_back(-0.5f);
    vertices.push_back(-0.05f);



    vertices.push_back(0.05f);
    vertices.push_back(-0.5f);
    vertices.push_back(-0.05f);



    vertices.push_back(0.05f);
    vertices.push_back(-0.5f);
    vertices.push_back(0.05f);



    vertices.push_back(0.05f);
    vertices.push_back(0.5f);
    vertices.push_back(0.05f);







    /////////////              4


    vertices.push_back(-0.05f);
    vertices.push_back(-0.5f);
    vertices.push_back(-0.05f);



    vertices.push_back(0.05f);
    vertices.push_back(-0.5f);
    vertices.push_back(-0.05f);


    vertices.push_back(0.05f);
    vertices.push_back(-0.5f);
    vertices.push_back(0.05f);


    vertices.push_back(0.05f);
    vertices.push_back(-0.5f);
    vertices.push_back(0.05f);


    vertices.push_back(-0.05f);
    vertices.push_back(-0.5f);
    vertices.push_back(0.05f);



    vertices.push_back(-0.05f);
    vertices.push_back(-0.5f);
    vertices.push_back(-0.05f);


//////////////             5

    vertices.push_back(-0.05f);
    vertices.push_back(0.5f);
    vertices.push_back(-0.05f);



    vertices.push_back(0.05f);
    vertices.push_back(0.5f);
    vertices.push_back(-0.05f);



    vertices.push_back(0.05f);
    vertices.push_back(0.5f);
    vertices.push_back(0.05f);



    vertices.push_back(0.05f);
    vertices.push_back(0.5f);
    vertices.push_back(0.05f);


    vertices.push_back(-0.05f);
    vertices.push_back(0.5f);
    vertices.push_back(0.05f);



    vertices.push_back(-0.05f);
    vertices.push_back(0.5f);
    vertices.push_back(-0.05f);


    std::vector<float> vertices2;
    const float PI_OVER_3 = glm::pi<float>() / 3.0f;
    float angle = 0.0f;
    float angle2 = 0.0f;

    for (int i = 0; i < 80; i++) {
        // Add vertices for the top layer
        vertices2.push_back(0.8f + glm::cos(angle) / 1.9f);
        vertices2.push_back(glm::sin(angle) / 1.9f);
        vertices2.push_back(0.05f);

        vertices2.push_back(0.8f + glm::cos(angle) / 1.6f);
        vertices2.push_back(glm::sin(angle) / 1.6f);
        vertices2.push_back(0.05f);

        angle += PI_OVER_3 / 6.0f;

        vertices2.push_back(0.8f + glm::cos(angle) / 1.6f);
        vertices2.push_back(glm::sin(angle) / 1.6f);
        vertices2.push_back(0.05f);

        vertices2.push_back(0.8f + glm::cos(angle - PI_OVER_3 / 6.0f) / 1.9f);
        vertices2.push_back(glm::sin(angle - PI_OVER_3 / 6.0f) / 1.9f);
        vertices2.push_back(0.05f);

        vertices2.push_back(0.8f + glm::cos(angle) / 1.6f);
        vertices2.push_back(glm::sin(angle) / 1.6f);
        vertices2.push_back(0.05f);

        vertices2.push_back(0.8f + glm::cos(angle) / 1.9f);
        vertices2.push_back(glm::sin(angle) / 1.9f);
        vertices2.push_back(0.05f);

        // Add vertices for the bottom layer
        vertices2.push_back(0.8f + glm::cos(angle2) / 1.9f);
        vertices2.push_back(glm::sin(angle2) / 1.9f);
        vertices2.push_back(-0.05f);

        vertices2.push_back(0.8f + glm::cos(angle2) / 1.6f);
        vertices2.push_back(glm::sin(angle2) / 1.6f);
        vertices2.push_back(-0.05f);

        angle2 += PI_OVER_3 / 6.0f;

        vertices2.push_back(0.8f + glm::cos(angle2) / 1.6f);
        vertices2.push_back(glm::sin(angle2) / 1.6f);
        vertices2.push_back(-0.05f);

        vertices2.push_back(0.8f + glm::cos(angle2 - PI_OVER_3 / 6.0f) / 1.9f);
        vertices2.push_back(glm::sin(angle2 - PI_OVER_3 / 6.0f) / 1.9f);
        vertices2.push_back(-0.05f);

        vertices2.push_back(0.8f + glm::cos(angle2) / 1.6f);
        vertices2.push_back(glm::sin(angle2) / 1.6f);
        vertices2.push_back(-0.05f);

        vertices2.push_back(0.8f + glm::cos(angle2) / 1.9f);
        vertices2.push_back(glm::sin(angle2) / 1.9f);
        vertices2.push_back(-0.05f);
    }


    unsigned int VBO, VBO2, VAO, VAO2;
    glGenVertexArrays(1, &VAO);
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO2);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO2);


    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, vertices2.size() * sizeof(float), &vertices2[0], GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // -------------------------
    unsigned int texture;
    // // texture 1
    // // ---------
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glEnable(GL_DEPTH_TEST);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // render
        // ------
        glClearColor(0.05f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activate shader
        ourShader.use();

        // create transformations
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        model = model =
                glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f),
                            glm::vec3(0.5f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection =
                glm::perspective(glm::radians(45.0f),
                                 (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.05f, 100.0f);

        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // note: currently we set the projection matrix each frame, but since the
        // projection matrix rarely changes it's often best practice to set it
        // outside the main loop only once.
        ourShader.setMat4("projection", projection);

        // render container
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 50);

        glBindVertexArray(VAO2);

        glDrawArrays(GL_TRIANGLES, 0, 700);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
        // etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
