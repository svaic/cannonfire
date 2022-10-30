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

class Obstacle {
public:
    float x;
    float y;
    float speedOfEnemy;
    bool left;

    Obstacle(float initX, float initY, float speed) {
        x = initX;
        y = initY;
        left = true;
        speedOfEnemy = speed;
    }

    float move() {
        if (x - speedOfEnemy <= -1.50 && left == true) {
            left = false;
            //return x;
        }
        else if (x + speedOfEnemy >= 1.50 && left == false) {
            left = true;
            //return x;
        }
        if (left) {
            x = x - speedOfEnemy;
        } else {
            x = x + speedOfEnemy;
        }
        std::cout << x << " " << left << std::endl;

    }
};

unsigned int Shape::shaderId = -1;

bool left = true;

float speedOfEnemy = 0.02;
int timeToChangeSpeed = 0;

float robotSteps(float prevStep) {
    timeToChangeSpeed++;

    if (timeToChangeSpeed == 10) {
        timeToChangeSpeed = 0;
        int newNumber = (rand() % (50 - 0)) + 0;
        speedOfEnemy = newNumber / 1000.0;

        int newNumber2 = (rand() % (10 - 0)) + 0;
        if (newNumber2 > 7)
            left = !left;
    }

    if (prevStep - speedOfEnemy <= -1.00 && left == true) {
        left = false;
        return prevStep;
    }
    else if (prevStep + speedOfEnemy >= 1.00 && left == false) {
        left = true;
        return prevStep;
    }
    if (left) {
        return  prevStep - speedOfEnemy;
    } else {
        return  prevStep + speedOfEnemy;
    }
}

bool shootClicked(GLFWwindow *window);

int x = 0;

int health = 100;

float getHeroX() {
    return x * 0.01;
}

std::vector<glm::vec2> shootings;

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

    Circle greenCircle = Circle::createCircle(0.05, glm::vec2(0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

    Circle shootCircle = Circle::createCircle(0.05, glm::vec2(0.0, -0.8), glm::vec3(1.0, 1.0, 0.0));



    std::vector<float> vertices {
            0.5f,  0.01f, // top right
            0.5f, -0.0f, // bottom right
            -0.5f, -0.0f,// bottom left
            -0.5f,  0.01f,// top left
    };

    RectangleShape HealthCircle = RectangleShape::createRectangle(vertices, glm::vec2(-0.8, 0.8), glm::vec3(0.95, 0.1, 0.0));

    RectangleShape rectangle = RectangleShape::createRectangle(vertices, glm::vec3(0.0f,  0.0f, 0.0f), glm::vec3(0.5f,  0.5f, 0.0f));

    float offsetOfObstacles = 0.1;

    std::vector<Obstacle> obstacles;
    for (int i = 0; i < 5; ++i) {
        int random = (rand() % (100- 0)) - 50;

        obstacles.push_back(Obstacle(random / 100.0f,  offsetOfObstacles * ((float)i), random / 1000.0f));
    }

    float prevStep = 0.0;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (health == 0) return 0;

        // render the triangle
        ourShader.use();

        Shape::setShaderId(ourShader.ID);

        HealthCircle.transform(glm::vec2(-1.0,0.8), glm::vec2(health / 100.0f, 1), 0);
        HealthCircle.draw();

/*        for (int i = 0; i < shootings.size(); ++i) {
            glm::vec2 newCords = glm::vec2(shootings[i].x, shootings[i].y + 0.032);
            Circle t1 = Circle::createCircle(0.05, newCords , glm::vec3(1.0, 0.0, 0.0));
            t1.draw();
            //shootings[i] = newCords;
        }*/

        //rectangle.rotate((float)glfwGetTime());
        //whiteCircle.move(glm::vec2(0.0,-1.0));
        //rectangle.scale(glm::vec2(1.0));
        //whiteCircle.draw();

        //whiteCircle.move(glm::vec2(-0.5, 0.9));

        // input
        // -----
        Shape shape1 = whiteCircle;
        whiteCircle.move(glm::vec2(getHeroX(),-1.0));
        //rectangle.scale(glm::vec2(1.0));
        processInput(window, shape1);
        whiteCircle.draw();

        whiteCircle.draw();

        prevStep = robotSteps(prevStep);
        redCircle.move(glm::vec2(prevStep, 1.0));
        redCircle.draw();

        bool xPressed = shootClicked(window);

        bool obstaclePreventsShoot = false;
        for (int i = 0; i < obstacles.size(); ++i) {
            obstacles[i].move();
            HealthCircle.transform(glm::vec2(obstacles[i].x ,obstacles[i].y), glm::vec2(1, 1), 0);
            HealthCircle.draw();

/*            if(abs(obstacles[i].x - prevStep) < 0.5) {
                obstaclePreventsShoot = true;
            }*/
        }

        if (abs(getHeroX() - prevStep) < 0.1 && xPressed) {
            greenCircle.draw();
            health -=1;
        }

        if (xPressed) {
            shootCircle.move(glm::vec2(getHeroX(), -0.7f));
            shootCircle.draw();
            //shootings.push_back(glm::vec2(getHeroX(), 0.2));
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    whiteCircle.desctruct();
    redCircle.desctruct();
    greenCircle.desctruct();
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
/*    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        shape.move(glm::vec2(0.0,0.1));
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        shape.move(glm::vec2(0.0,-0.1));
    }*/
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

bool shootClicked(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        return true;
    }
    return false;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

