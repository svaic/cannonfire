#include <OpenGLPrj.hpp>

#include <GLFW/glfw3.h>

#include <Shader.hpp>

#include <iostream>
#include <cstring>
#include <vector>
#include "VaoModel.hpp"
#include "Shape.hpp"
#include "Circle.hpp"
#include "Rectangle.hpp"


const std::string program_name = ("GLSL Shader class example");

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 1200;

GLFWwindow* load() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, program_name.c_str(), nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    return window;
}

unsigned int Shape::shaderId = -1;

int main()
{
    GLFWwindow * window = load();
    if(window == nullptr) return -1;

    // build and compile our shader program
    // ------------------------------------
    Shader ourShader("../res/shaders/shader.vert",
                     "../res/shaders/shader.frag"
                     );

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    Circle whiteCircle = Circle::createCircle(0.2, glm::vec3(1.0, 1.0, 1.0));

    Circle redCircle = Circle::createCircle(0.1, glm::vec3(1.0, 0.0, 0.0));

    Circle blueCircle = Circle::createCircle(0.05, glm::vec3(0.0, 1.0, 0.0));


    std::vector<float> vertices {
            0.5f,  0.5f, // top right
            0.5f, -0.5f, // bottom right
            -0.5f, -0.5f,// bottom left
            -0.5f,  0.5f,// top left
    };

    std::vector<float> vertices2 {
            0.5f,  0.2f,
            0.5f, -0.2f,
            -0.1f, -0.2f,
    };

    std::vector<float> vertices3 {
            0.1f,  0.2f,
            0.1f, -0.2f,
            -0.1f, -0.2f,
    };

    RectangleShape rectangle = RectangleShape::createRectangle(vertices, glm::vec3(0.5f,  0.5f, 0.0f));
    RectangleShape rectangle2 = RectangleShape::createTriangle(vertices2, glm::vec3(1.0, 0.0, 0.0));
    RectangleShape rectangle3 = RectangleShape::createTriangle(vertices3, glm::vec3(1.0, 0.0, 1.0));


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render the triangle
        ourShader.use();

        Shape::setShaderId(ourShader.ID);

        //rectangle.transform(ourShader.ID, glm::vec2(0.5, -0.5), glm::vec2(1.0), (float)glfwGetTime());
        rectangle.move(glm::vec2(0.5, -0.5));
        rectangle.rotate((float)glfwGetTime());

        rectangle.draw();

        whiteCircle.move(glm::vec2(-0.5, 0.9));

        whiteCircle.draw();

        redCircle.transform(glm::vec2(0.9, 0.5), glm::vec2(0.2), 0);
        redCircle.draw();
        blueCircle.transform(glm::vec2(-0.5, -0.5), glm::vec2(5.0), 0);
        blueCircle.draw();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    whiteCircle.desctruct();
    redCircle.desctruct();
    blueCircle.desctruct();
    rectangle.desctruct();
    rectangle2.desctruct();

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

