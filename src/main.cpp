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
#include "Obstacle.hpp"
#include "DefaultShape.hpp"

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

RectangleShape* DefaultShape::greenObstacle = nullptr;
RectangleShape* DefaultShape::redObstacle = nullptr;
RectangleShape* DefaultShape::blackObstacle = nullptr;

int numberOfObstacles = 3;
float maxSpeedOfEnemy = 0.05;

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

    RectangleShape obstacleBlackRectangle = RectangleShape::createRectangle(0.02, 1.00, Color::BLACK);
    RectangleShape obstacleGreenRectangle = RectangleShape::createRectangle(0.02, 1.00, Color::GREEN);
    RectangleShape obstacleRedRectangle = RectangleShape::createRectangle(0.02, 1.00, Color::DARK_RED);

    DefaultShape::setBlackObstacle(obstacleBlackRectangle);
    DefaultShape::setGreenObstacle(obstacleGreenRectangle);
    DefaultShape::setRedObstacle(obstacleRedRectangle);

    RectangleShape heroCanonRectangle = RectangleShape::createRectangle(0.2, 0.075, Color::GREY);

    std::vector<float> heroCanonEdgeVertices {
            0.1f,  0.25f, // top right
            0.1f, 0.2f, // bottom right
            -0.1f, 0.2f,// bottom left
            -0.1f,  0.25f,// top left
    };

    RectangleShape heroCanonEdgeRectangle = RectangleShape::createRectangle(heroCanonEdgeVertices, Color::DARK_GREY);

    std::vector<Obstacle> obstacles;
    for (int i = 0; i < numberOfObstacles; ++i) {
        obstacles.emplace_back();
    }

    MovableObject hero = MovableObject(0.0, -1.0, 0.01);
    hero.add(&heroCanonEdgeRectangle);
    hero.add(&heroCanonRectangle);
    hero.add(&heroCircle);

    MovableObject enemy = MovableObject(0.0, 1.0, maxSpeedOfEnemy * 0.1f, 1.0, 1.0);
    enemy.add(&enemyCircle);

    MovableObject heroArsenalBar = MovableObject(-1.0, -0.8, 0, 1.0, 1.0);
    heroArsenalBar.add(&heroArsenalBarRectangle);

    MovableObject enemyHealthBar = MovableObject(-1.0, 0.8, 0, 1.0, 1.0);
    enemyHealthBar.add(&enemyHealthBarRectangle);

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

        enemyHealthBar.changeWidth(enemy.health);

        heroArsenalBar.changeWidth(hero.arsenal);

        // check if bullets collide with obstacle, current implementation o(n^2)
        for (int i = 0; i < obstacles.size(); i++) {
            obstacles[i].moveRandomX(false);

            for (int j = 0; j < bullets.size(); ++j) {

                if (bullets[j].collide((obstacles[i]))) {

                    bullets.erase(bullets.begin()+i);

                    if (obstacles[j].reduceHealth()) {
                        obstacles.erase(obstacles.begin()+j);
                    }
                }
            }
        }

        if(obstacles.size() < numberOfObstacles) {
            obstacles.emplace_back();
        }

        enemy.moveRandomX(true);

        // animate bullets
        for (int i=0; i < bullets.size(); i++) {
            if (bullets[i].y > 1.0) {
                bullets.erase(bullets.begin()+i);
            }

            if (bullets[i].collide(enemy)) {
                bullets.erase(bullets.begin()+i);

                enemy.add(&greenCircle);
                enemy.reduceHealth();
                enemy.changeSpeed(1/enemy.health * maxSpeedOfEnemy);
                enemy.draw();
                enemy.remove(1);
            }

            bullets[i].moveY();
        }

        // hero animation
        listenToMoveHero(window, hero);
        hero.draw();

        // hero shoots
        if (hero.canShoot()) {
            if (shootClicked(window) && timerTillNextShootingAllowed >= 1.0) {
                hero.shoot();

                MovableObject bullet = MovableObject(hero.x, hero.y, speedOfBullet);
                bullet.add(&shootCircle);
                bullet.draw();

                bullets.push_back(bullet);

                timerTillNextShootingAllowed = 0;
            }

            timerTillNextShootingAllowed += 0.1;

        } else {
            //hero out of arsenal
            hero.add(&shootCircle);
            hero.draw();
            hero.remove(1);
        }

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
    obstacleBlackRectangle.destructor();

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
        object.moveX(-0.01);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        object.moveX(0.01);
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

