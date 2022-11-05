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

const std::string program_name = ("GLSL Shader class example");

const unsigned int seed = time(0);

std::mt19937_64 rng(seed);

float getRandomFloat(float min, float max) {

    std::uniform_real_distribution<float> unif(min, max);
    return unif(rng);
}

int getRandomInt(int min, int max) {

    std::uniform_int_distribution<int> unii(min, max);
    return unii(rng);
}

class MovableObject {
private:

    float speed;
    bool left;
    int iteration;
public:
    float x;
    float y;
    float width;
    float height;
    float health;
    float arsenal;

    MovableObject(float initX, float initY, float initSpeed, float initWidth = 1, float initHeight = 1) {
        x = initX;
        y = initY;
        left = true;
        speed = initSpeed;
        health = 1.0;
        width = initWidth;
        height = initHeight;
        iteration = 0;
        arsenal = 1;
    }

    void moveRandom(bool changeSide) {
        if (changeSide) changePosition();
        if (x - speed <= -1.00 && left == true) {
            left = false;
        }
        else if (x + speed >= 1.00 && left == false) {
            left = true;
        }
        if (left) {
            x = x - speed;
        } else {
            x = x + speed;
        }
    }

    void move(float offset) {
        x += offset;
    }

    void changePosition() {
        if (iteration == 10) {
            iteration = 0;

            speed = getRandomFloat(0.05, 0.01);

            int shouldChangeDirectionCoinFlip = getRandomInt(0,10);
            if (shouldChangeDirectionCoinFlip > 7)
                left = !left;
        }
        iteration++;
    }

    bool collide(MovableObject & other, bool xAxis) {
        if (xAxis) return abs(this->x - other.x) < 0.1;
        else return abs(this->y - other.y) < 0.1;
    }

    bool inside(MovableObject & other, bool xAxis) {
        if (xAxis) return this->x <= other.x + other.width && this->x >= other.x - other.width;
        return false;
    }

    void hit() {
        health -=0.01;
    }

    void shoot() {
        arsenal -=0.01;
        if (arsenal < 0) {
            arsenal = 0;
        }
    }

    bool canShoot() {
        return arsenal > 0;
    }
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, MovableObject &shape);

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

bool shootClicked(GLFWwindow *window);

bool refreshArsenalClicked(GLFWwindow *window);

int numberOfObstacles = 3;

float speedOfObstacles = 0.005;

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

    Circle heroCircle = Circle::createCircle(0.2, glm::vec2(0.0, 0.0), glm::vec3(1.0, 1.0, 1.0));

    Circle enemyCircle = Circle::createCircle(0.1, glm::vec2(0.0, 0.8), glm::vec3(1.0, 0.0, 0.0));

    Circle greenCircle = Circle::createCircle(0.05, glm::vec2(0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

    Circle shootCircle = Circle::createCircle(0.05, glm::vec2(0.0, -0.8), glm::vec3(1.0, 1.0, 0.0));

    std::vector<float> healthBarVertices {
            0.5f,  0.01f, // top right
            0.5f, -0.0f, // bottom right
            -0.5f, -0.0f,// bottom left
            -0.5f,  0.01f,// top left
    };

    std::vector<float> obstacleVertices {
            1.0f,  0.04f, // top right
            1.0f, -0.0f, // bottom right
            -1.0f, -0.0f,// bottom left
            -1.0f,  0.04f,// top left
    };

    RectangleShape enemyHealthBarRectangle = RectangleShape::createRectangle(healthBarVertices, glm::vec2(-0.8, 0.8), glm::vec3(0.95, 0.1, 0.0));
    RectangleShape heroArsenalBarRectangle = RectangleShape::createRectangle(healthBarVertices, glm::vec2(-0.8, 0.7), glm::vec3(1.0, 1.0, 0.0));

    RectangleShape obstacleRectangle = RectangleShape::createRectangle(obstacleVertices, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.0f));

    std::vector<MovableObject> obstacles;
    for (int i = 0; i < numberOfObstacles; ++i) {
        float initX = getRandomFloat(-0.5, 0.5);
        float initY = getRandomFloat(-0.3, 0.7);
        float initWidth = getRandomFloat(0.1, 0.4);

        obstacles.emplace_back(initX,  initY, speedOfObstacles, initWidth, 1);
    }

    MovableObject hero = MovableObject(0.0, -1.0, 0.01);
    MovableObject enemy = MovableObject(0.0, 1.0, 0.02);

    ourShader.use();
    Shape::setShaderId(ourShader.ID);

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

        bool obstaclePreventsShoot = false;
        for (auto & obstacle : obstacles) {
            obstacle.moveRandom(false);
            obstacleRectangle.transform(glm::vec2(obstacle.x , obstacle.y), glm::vec2(obstacle.width, obstacle.height), 0);
            obstacleRectangle.draw();
        }

        processInput(window, hero);
        heroCircle.move(glm::vec2(hero.x, hero.y));
        heroCircle.draw();

        enemy.moveRandom(true);
        enemyCircle.move(glm::vec2(enemy.x, enemy.y));
        enemyCircle.draw();

        bool heroShoots = shootClicked(window);

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
            }

            if (heroShoots && hero.collide(enemy, true) && !obstaclePreventsShoot) {
                greenCircle.move(glm::vec2(enemy.x, enemy.y));
                greenCircle.draw();
                enemy.hit();
            }
        } else {
            shootCircle.move(glm::vec2(hero.x, -0.8));
            shootCircle.draw();
        }

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
void processInput(GLFWwindow *window, MovableObject &object)
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

