#include <OpenGLPrj.hpp>

#include <GLFW/glfw3.h>

#include <Shader.hpp>

#include <iostream>
#include <vector>
#include <random>
#include "VaoModel.hpp"
#include "Shape.hpp"
#include "Circle.hpp"
#include "Rectangle.hpp"
#include "RandomUtility.hpp"
#include "MovableObj.hpp"
#include "Color.hpp"
#include "ShapeContainer.hpp"

const std::string program_name = ("GLSL Shader class example");

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void listenToMoveHero(GLFWwindow *window, MovableObject &object);

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
unsigned int ShapeContainer::shaderId = -1;

bool shootClicked(GLFWwindow *window);

bool refreshArsenalClicked(GLFWwindow *window);

int numberOfObstacles = 3;

float speedOfObstacles = 0.005;

unsigned int seed = static_cast<long int>(std::time(nullptr));
std::mt19937_64 RandomUtility::randomEngine = std::mt19937_64(size_t(seed));

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

    Circle heroCircle = Circle::createCircle(0.2, Color::WHITE);

    Circle enemyCircle = Circle::createCircle(0.1, Color::RED);

    Circle greenCircle = Circle::createCircle(0.05, Color::GREEN);

    Circle shootCircle = Circle::createCircle(0.05, Color::YELLOW);

    std::vector<float> healthBarVertices {
            0.5f,  0.01f, // top right
            0.5f, -0.0f, // bottom right
            -0.5f, -0.0f,// bottom left
            -0.5f,  0.01f,// top left
    };

    RectangleShape enemyHealthBarRectangle = RectangleShape::createRectangle(healthBarVertices, Color::DARK_RED);
    RectangleShape heroArsenalBarRectangle = RectangleShape::createRectangle(healthBarVertices, Color::YELLOW);

    RectangleShape obstacleRectangle = RectangleShape::createRectangle(0.02, 1.00, Color::GREEN_YELLOW);

    std::vector<MovableObject> obstacles;
    for (int i = 0; i < numberOfObstacles; ++i) {
        float initX = RandomUtility::getRandomFloat(-0.5, 0.5);
        float initY = RandomUtility::getRandomFloat(-0.3, 0.7);
        float initWidth = RandomUtility::getRandomFloat(0.1, 0.4);

        obstacles.emplace_back(initX,  initY, speedOfObstacles, initWidth, 1);
    }

    MovableObject hero = MovableObject(0.0, -1.0, 0.01);
    MovableObject enemy = MovableObject(0.0, 1.0, 0.02);

    ourShader.use();
    Shape::setShaderId(ourShader.ID);
    ShapeContainer::shaderId = ourShader.ID;

    ShapeContainer obstacleWithCircle;
    obstacleWithCircle.add(&obstacleRectangle);
    obstacleWithCircle.add(&enemyCircle);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (enemy.health < 0) break;

        if(refreshArsenalClicked(window)) {
            hero.arsenal= 1.0;
        }

        enemyHealthBarRectangle.transform(glm::vec2(-1.0, 0.8), glm::vec2(enemy.health, 1.0), 0);
        enemyHealthBarRectangle.draw();

        heroArsenalBarRectangle.transform(glm::vec2(-1.0, -0.8), glm::vec2(hero.arsenal, 1.0), 0);
        heroArsenalBarRectangle.draw();

        for (auto & obstacle : obstacles) {
            obstacle.moveRandom(false);
            obstacleRectangle.transform(glm::vec2(obstacle.x , obstacle.y), glm::vec2(obstacle.width, obstacle.height), 0);
            obstacleRectangle.draw();
        }

        listenToMoveHero(window, hero);
        heroCircle.move(glm::vec2(hero.x, hero.y));
        heroCircle.draw();

        enemy.moveRandom(true);
        enemyCircle.move(glm::vec2(enemy.x, enemy.y));
        enemyCircle.draw();

        bool heroShoots = shootClicked(window);

        bool obstaclePreventsShoot = false;
        if (hero.canShoot()) {
            if (heroShoots) {
                hero.shoot();

                shootCircle.move(glm::vec2(hero.x, -0.7f));
                shootCircle.draw();

                float minY = 1.0;

                for (auto & obstacle : obstacles) {
                    if (hero.inside(obstacle, true)) {
                        minY = std::min(minY, obstacle.y);
                        obstaclePreventsShoot = true;
                    }
                }

                if (obstaclePreventsShoot) {
                    shootCircle.move(glm::vec2(hero.x ,minY + 0.01));
                    shootCircle.draw();
                }
                else if (hero.collide(enemy, true)) {
                    greenCircle.move(glm::vec2(enemy.x, enemy.y));
                    greenCircle.draw();
                    enemy.hit();
                }
            }
        } else {
            //hero out of arsenal
            shootCircle.move(glm::vec2(hero.x, -0.8));
            shootCircle.draw();
        }

        obstacleWithCircle.transform(glm::vec2(0.0, 0.0), glm::vec2(1.0, 1.0), static_cast<float>(glfwGetTime()*20));

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    std::cout << "You've won. Congratulations!";

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    heroCircle.desctruct();
    enemyCircle.desctruct();
    greenCircle.desctruct();
    enemyHealthBarRectangle.desctruct();
    obstacleRectangle.desctruct();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void listenToMoveHero(GLFWwindow *window, MovableObject &object)
{
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        object.move(-0.01);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        object.move(0.01);
    }
}

bool shootClicked(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        return true;
    }
    return false;
}

bool refreshArsenalClicked(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
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

