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
unsigned int seed = static_cast<long int>(std::time(nullptr));
std::mt19937_64 RandomUtility::randomEngine = std::mt19937_64(size_t(seed));

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 1200;

unsigned int Shape::shaderId = -1;
unsigned int ShapeContainer::shaderId = -1;

int numberOfObstacles = 3;
float speedOfObstacles = 0.005;

float speedOfBullet = 0.05;

float timerTillNextShootingAllowed = 0.0;

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

bool shootClicked(GLFWwindow *window);

bool refreshArsenalClicked(GLFWwindow *window);

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

    Circle heroCircle = Circle::createCircle(0.1, Color::BLACK);

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

    RectangleShape obstacleRectangle = RectangleShape::createRectangle(0.02, 1.00, Color::BLACK);
    RectangleShape heroCanonRectangle = RectangleShape::createRectangle(0.2, 0.075, Color::GREY);

    std::vector<float> heroCanonEdgeVertices {
            0.1f,  0.25f, // top right
            0.1f, 0.2f, // bottom right
            -0.1f, 0.2f,// bottom left
            -0.1f,  0.25f,// top left
    };
    RectangleShape heroCanonEdgeRectangle = RectangleShape::createRectangle(heroCanonEdgeVertices, Color::DARK_GREY);

    std::vector<MovableObject> obstacles;
    for (int i = 0; i < numberOfObstacles; ++i) {
        float initX = RandomUtility::getRandomFloat(-0.5, 0.5);
        float initY = RandomUtility::getRandomFloat(-0.3, 0.7);
        float initWidth = RandomUtility::getRandomFloat(0.1, 0.4);

        obstacles.emplace_back(initX,  initY, speedOfObstacles, initWidth, 1);
    }

    MovableObject hero = MovableObject(0.0, -1.0, 0.01);
    MovableObject enemy = MovableObject(0.0, 1.0, 0.02, 0.05, 0.05);

    ShapeContainer heroContainer;
    heroContainer.add(&heroCanonEdgeRectangle);
    heroContainer.add(&heroCanonRectangle);
    heroContainer.add(&heroCircle);

    ourShader.use();
    Shape::setShaderId(ourShader.ID);
    ShapeContainer::setShaderId(ourShader.ID);

    std::vector<MovableObject> bullets;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // render
        // ------
        glm::vec3 whiteColor = Color::WHITE;
        glClearColor(whiteColor.x, whiteColor.y, whiteColor.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (enemy.health < 0) break;

        if(refreshArsenalClicked(window)) {
            hero.arsenal= 1.0;
        }

        enemyHealthBarRectangle.transform(glm::vec2(-1.0, 0.8), glm::vec2(enemy.health, 1.0), 0);
        enemyHealthBarRectangle.draw();

        heroArsenalBarRectangle.transform(glm::vec2(-1.0, -0.8), glm::vec2(hero.arsenal, 1.0), 0);
        heroArsenalBarRectangle.draw();

        // check if bullets collide with obstacle, current implementation o(n^2)
        for (auto & obstacle : obstacles) {
            obstacle.moveRandom(false);
            obstacleRectangle.transform(glm::vec2(obstacle.x , obstacle.y), glm::vec2(obstacle.width, obstacle.height), 0);
            obstacleRectangle.draw();

            for (int i = 0; i < bullets.size(); ++i) {
                if (bullets[i].collide(obstacle)) {
                    bullets.erase(bullets.begin()+i);
                }
            }
        }

        enemy.moveRandom(true);
        enemyCircle.move(glm::vec2(enemy.x, enemy.y));
        enemyCircle.draw();

        // animate bullets
        for (int i=0; i < bullets.size(); i++) {
            if (bullets[i].y > 1.0) {
                bullets.erase(bullets.begin()+i);
            }

            if (bullets[i].collide(enemy)) {
                bullets.erase(bullets.begin()+i);

                enemy.hit();
                greenCircle.move(glm::vec2(enemy.x, enemy.y));
                greenCircle.draw();
            }

            bullets[i].move_y(speedOfBullet);
            shootCircle.move(glm::vec2(bullets[i].x , bullets[i].y));
            shootCircle.draw();
        }

        // hero animation
        listenToMoveHero(window, hero);
        heroContainer.move(glm::vec2(hero.x, hero.y));
        heroContainer.draw();

        bool heroShoots = shootClicked(window);

        if (hero.canShoot()) {
            if (heroShoots) {
                hero.shoot();
                bullets.emplace_back(hero.x, hero.y, speedOfBullet);

                shootCircle.move(glm::vec2(hero.x, hero.y));
                shootCircle.draw();

                timerTillNextShootingAllowed = 0;
            }

            timerTillNextShootingAllowed += 0.0000000001;
        } else {
            //hero out of arsenal
            shootCircle.move(glm::vec2(hero.x, -0.8));
            shootCircle.draw();
        }

        std::cout << timerTillNextShootingAllowed << (timerTillNextShootingAllowed - 1.0 <= 0.01) << std::endl;

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    std::cout << "You've won. Congratulations!";

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    heroCircle.destructor();
    enemyCircle.destructor();
    greenCircle.destructor();
    enemyHealthBarRectangle.destructor();
    obstacleRectangle.destructor();

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
        object.move_x(-0.01);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        object.move_x(0.01);
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

