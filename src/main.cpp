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
void processInput(GLFWwindow *window, Shape &shape);

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

bool left = true;

float robotSteps(float prevStep) {
    if (prevStep - 0.01 <= -1.00 && left == true) {
        left = false;
        return prevStep;
    }
    else if (prevStep + 0.01 >= 1.00 && left == false) {
        left = true;
        return prevStep;
    }
    if (left) {
        return  prevStep - 0.01;
    } else {
        return  prevStep + 0.01;
    }
}

int x = 0;

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

    Circle whiteCircle = Circle::createCircle(0.2, glm::vec2(0.0, 0.0), glm::vec3(1.0, 1.0, 1.0));

    Circle redCircle = Circle::createCircle(0.1, glm::vec2(0.0, 0.8), glm::vec3(1.0, 0.0, 0.0));

    Circle blueCircle = Circle::createCircle(0.05, glm::vec2(0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));


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

    RectangleShape rectangle = RectangleShape::createRectangle(vertices, glm::vec3(0.0f,  0.0f, 0.0f), glm::vec3(0.5f,  0.5f, 0.0f));

    float prevStep = 0.0;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {


        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render the triangle
        ourShader.use();

        Shape::setShaderId(ourShader.ID);


        //rectangle.rotate((float)glfwGetTime());
        //whiteCircle.move(glm::vec2(0.0,-1.0));
        //rectangle.scale(glm::vec2(1.0));
        //whiteCircle.draw();

        //whiteCircle.move(glm::vec2(-0.5, 0.9));

        // input
        // -----
        Shape shape1 = whiteCircle;
        whiteCircle.move(glm::vec2(0.01*x,-1.0));
        //rectangle.scale(glm::vec2(1.0));
        processInput(window, shape1);
        whiteCircle.draw();

        whiteCircle.draw();

        prevStep = robotSteps(prevStep);
        redCircle.move(glm::vec2(prevStep, 1.0));
        redCircle.draw();




        //blueCircle.transform(glm::vec2(-0.5, -0.5), glm::vec2(5.0), 0);
        //blueCircle.draw();


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

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, Shape &shape)
{
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        shape.move(glm::vec2(0.0,0.1));
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        shape.move(glm::vec2(0.0,-0.1));
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        shape.move(glm::vec2(0.01*x,-1.0));
        x--;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        shape.move(glm::vec2(0.01*x,-1.0));
        x++;
    }
    shape.draw();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

