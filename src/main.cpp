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
#define STB_IMAGE_IMPLEMENTATION
const std::string program_name = ("GLSL Shader class example");

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void listenToMoveHero(GLFWwindow *window, MovableObject &object);

// settings
unsigned int seed = static_cast<long int>(std::time(nullptr));
std::mt19937_64 RandomUtility::randomEngine = std::mt19937_64(size_t(seed));

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

unsigned int Shape::shaderId = -1;
unsigned int ShapeContainer::shaderId = -1;

RectangleShape* DefaultShape::greenObstacle = nullptr;
RectangleShape* DefaultShape::redObstacle = nullptr;
RectangleShape* DefaultShape::blackObstacle = nullptr;
RectangleShape* DefaultShape::asteroidField = nullptr;

int numberOfObstacles = 3;
float maxSpeedOfEnemy = 0.05;

float speedOfBullet = 0.05;

float timerTillNextShootingAllowed = 0.0;

unsigned int currentShaderUsed;

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

void changeShader(Shader* textureShader, Shader* colorShader);

bool refreshArsenalClicked(GLFWwindow *window);

int main()
{
    GLFWwindow * window = load();
    if(window == nullptr) return -1;

    // build and compile our shader program
    // ------------------------------------

    std::string shader_location("../res/shaders/");

    std::string used_shaders("shader");

    Shader textureShader(shader_location + used_shaders + std::string(".vert"),
                     shader_location + used_shaders + std::string(".frag")
                     );

    Shader colorShader(shader_location + used_shaders + std::string("2.vert"),
                     shader_location + used_shaders + std::string("2.frag")
    );


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    Circle heroCircle = Circle::createCircle(0.1, Color::BLACK);

    Circle enemyCircle = Circle::createCircle(0.1, Color::RED);

    Circle greenCircle = Circle::createCircle(0.05, Color::GREEN);

    Circle shootCircle = Circle::createCircle(1.0, Color::YELLOW);

    Circle outOfArsenal = Circle::createCircle(0.05, Color::YELLOW);

    Circle enemyShootCircle = Circle::createCircle(1.00, Color::BLUE);

    std::vector<float> healthBarVertices {
            0.5f,  0.01f, // top right
            0.5f, -0.0f, // bottom right
            -0.5f, -0.0f,// bottom left
            -0.5f,  0.01f,// top left
    };

    std::vector<float> rectangleFullSize {
            1.0f,  1.0f, // top right
            1.0f, -1.0f, // bottom right
            -1.0f, -1.0f,// bottom left
            -1.0f,  1.00f,// top left
    };

    RectangleShape heroShootIcon = RectangleShape::createRectangle(rectangleFullSize, Color::DARK_RED, "bullet.png");
    RectangleShape enemyShootIcon = RectangleShape::createRectangle(rectangleFullSize, Color::DARK_RED, "w-wing.png");
    RectangleShape backgroundIcon = RectangleShape::createRectangle(rectangleFullSize, Color::DARK_GREY, "bg.png");
    RectangleShape groguIcon = RectangleShape::createRectangle(rectangleFullSize, Color::WHITE, "grogu.png");
    RectangleShape r2d2Icon = RectangleShape::createRectangle(rectangleFullSize, Color::WHITE, "r2d2.png");

    MovableObject enemyShoot = MovableObject(0.0, 0.0, 0.01, 1.0, 1.0);
    enemyShoot.add(&enemyShootIcon);

    MovableObject backgroundObj = MovableObject(0.0, 0.0, 0.01, 1.0, 1.0);
    backgroundObj.add(&backgroundIcon);

    RectangleShape enemyHealthBarRectangle = RectangleShape::createRectangle(rectangleFullSize, Color::DARK_RED, "");
    RectangleShape enemyStatsBackground = RectangleShape::createRectangle(rectangleFullSize, Color::BLACK, "");
    RectangleShape heroHealthBarRectangle = RectangleShape::createRectangle(rectangleFullSize, Color::GREEN, "");
    RectangleShape heroArsenalBarRectangle = RectangleShape::createRectangle(rectangleFullSize, Color::YELLOW, "");

    RectangleShape obstacleGreenRectangle = RectangleShape::createRectangle(1.0, 1.0, Color::WHITE, "green_planet.png");
    RectangleShape obstacleYellowRectangle = RectangleShape::createRectangle(1.0, 1.0, Color::GREEN, "yellow_planet.png");
    RectangleShape obstacleRedRectangle = RectangleShape::createRectangle(1.0, 1.0, Color::YELLOW, "red_planet.png");
    RectangleShape obstacleAsteroidField = RectangleShape::createRectangle(1.0, 1.0, Color::YELLOW, "asteroids.png");

    DefaultShape::setYellowObstacle(obstacleYellowRectangle);
    DefaultShape::setGreenObstacle(obstacleGreenRectangle);
    DefaultShape::setRedObstacle(obstacleRedRectangle);
    DefaultShape::setAsteroidField(obstacleAsteroidField);

    RectangleShape heroCanonRectangle = RectangleShape::createRectangle(0.2, 0.075, Color::GREY, "");

    std::vector<float> heroCanonEdgeVertices {
            0.1f,  0.25f, // top right
            0.1f, 0.2f, // bottom right
            -0.1f, 0.2f,// bottom left
            -0.1f,  0.25f,// top left
    };

    RectangleShape heroCanonEdgeRectangle = RectangleShape::createRectangle(heroCanonEdgeVertices, Color::DARK_GREY, "");

    std::vector<Obstacle> obstacles;
    for (int i = 0; i < numberOfObstacles; ++i) {
        obstacles.emplace_back();
    }

    MovableObject hero = MovableObject(0.0, -0.80, 0.01, 0.2, 0.3);
    //hero.add(&heroCanonEdgeRectangle);
    //hero.add(&heroCanonRectangle)
    hero.add(&groguIcon);
    hero.add(&heroCircle);

    MovableObject enemy = MovableObject(0.0, 0.8, maxSpeedOfEnemy * 0.1f, 0.10, 0.15);
    enemy.add(&r2d2Icon);

    MovableObject heroArsenalBar = MovableObject(-1.0, -0.95, 0, 1.0, 0.05);
    heroArsenalBar.add(&heroArsenalBarRectangle);

    MovableObject heroHealthBar = MovableObject(-1.0, -1.0, 0, 1.0, 0.05);
    heroHealthBar.add(&heroHealthBarRectangle);

    MovableObject enemyHealthBar = MovableObject(-1.0, 0.95, 0, 1.0, 0.05);
    enemyHealthBar.add(&enemyHealthBarRectangle);

    textureShader.use();
    Shape::setShaderId(textureShader.ID);
    ShapeContainer::setShaderId(textureShader.ID);
    currentShaderUsed = textureShader.ID;

    std::vector<MovableObject> bulletsOfHero;
    std::vector<MovableObject> bulletsOfEnemy;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // render
        // ------
        glm::vec3 whiteColor = Color::WHITE;
        glClearColor(whiteColor.x, whiteColor.y, whiteColor.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        backgroundObj.draw();

        for (int i = 0; i < bulletsOfEnemy.size(); ++i) {
            bulletsOfEnemy[i].draw();
        }

        if (enemy.health < 0) break;
        if (hero.health < 0) return 1;

        if(refreshArsenalClicked(window)) {
            hero.arsenal= 1.0;
        }

        // check if bulletsOfHero collide with obstacle, current implementation o(n^2)
        for (int i = 0; i < obstacles.size(); i++) {
            obstacles[i].moveRandomX(false);

            for (int j = 0; j < bulletsOfHero.size(); ++j) {

                if (bulletsOfHero[j].collide((obstacles[i]))) {

                    bulletsOfHero.erase(bulletsOfHero.begin() + j);

                    if (obstacles[i].reduceHealth()) {
                        obstacles.erase(obstacles.begin()+i);
                    }
                }
            }
        }

        if(obstacles.size() < numberOfObstacles) {
            obstacles.emplace_back();
        }

        //changeShader(&textureShader, &colorShader);

        enemy.moveRandomX(true);

        // animate bulletsOfHero
        for (int i=0; i < bulletsOfHero.size(); i++) {
            if (bulletsOfHero[i].y > 1.0) {
                bulletsOfHero.erase(bulletsOfHero.begin() + i);
            }

            if (enemy.collide(bulletsOfHero[i])) {
                bulletsOfHero.erase(bulletsOfHero.begin() + i);

                //enemy.add(&greenCircle);
                enemy.reduceHealth();
                enemy.changeSpeed(1/enemy.health * maxSpeedOfEnemy);

                enemy.draw();

                //enemy.remove(1);

                //enemy shoots bullet
                MovableObject bullet = MovableObject(enemy.x, enemy.y, speedOfBullet*1.5f, 0.15, 0.25);
                bullet.add(&enemyShootIcon);
                bulletsOfEnemy.push_back(bullet);
            }

            bulletsOfHero[i].moveY();
        }

        // animate bulletsOfEnemy
        for (int i = 0; i < bulletsOfEnemy.size(); ++i) {

            bulletsOfEnemy[i].moveY(-0.01);

            if (hero.collide(bulletsOfEnemy[i])) {
                bulletsOfEnemy[i].add(&heroCircle);

                /*textureShader.use();
                ShapeContainer::setShaderId(textureShader.ID);
                Shape::setShaderId(textureShader.ID);

                bulletsOfEnemy[i].draw();

                colorShader.use();
                ShapeContainer::setShaderId(colorShader.ID);*/
                hero.health -=0.02f;
            }

            if (bulletsOfEnemy[i].y <= -1.0) {
                bulletsOfEnemy.erase(bulletsOfEnemy.begin() + i);
            }
        }

        // hero animation
        listenToMoveHero(window, hero);
        hero.draw();

        // hero shoots
        if (hero.canShoot()) {
            if (shootClicked(window) && timerTillNextShootingAllowed >= 1.0) {
                hero.shoot();

                MovableObject bullet = MovableObject(hero.x, hero.y, speedOfBullet, 0.2, 0.2);
                bullet.add(&heroShootIcon);

                bullet.draw();

                bulletsOfHero.push_back(bullet);

                timerTillNextShootingAllowed = 0;
            }

            timerTillNextShootingAllowed += 0.1;

        } else {
            //hero out of arsenal
            hero.add(&outOfArsenal);

            hero.draw();

            hero.remove(3);
        }

        changeShader(&textureShader, &colorShader);

        enemyStatsBackground.transform(glm::vec2(0.0, 0.95), glm::vec2(1.0, 0.05),0);
        enemyStatsBackground.draw();


        enemyStatsBackground.transform(glm::vec2(0.0, -0.95), glm::vec2(1.0, 0.05),0);
        enemyStatsBackground.draw();

        enemyHealthBar.changeWidth(enemy.health * 2);
        heroArsenalBar.changeWidth(hero.arsenal * 2);
        heroHealthBar.changeWidth(hero.health * 2);

        changeShader(&textureShader, &colorShader);

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
    obstacleYellowRectangle.destructor();

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

void changeShader(Shader* textureShader, Shader* colorShader) {
    if (currentShaderUsed != textureShader->ID) {
        textureShader->use();
        ShapeContainer::setShaderId(textureShader->ID);
        Shape::setShaderId(textureShader->ID);

        currentShaderUsed = textureShader->ID;
    } else {
        colorShader->use();
        ShapeContainer::setShaderId(colorShader->ID);
        Shape::setShaderId(colorShader->ID);

        currentShaderUsed = colorShader->ID;
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

