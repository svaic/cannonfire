#include <OpenGLPrj.hpp>

#include <GLFW/glfw3.h>

#include <Shader.hpp>

#include <iostream>
#include <string>
#include <vector>

const std::string program_name = ("Transformation basics");

class Pacman {
private:
    int pos_x;
    int pos_y;

    bool isInBorder(int newPos_x, int newPos_y) {
        float x = newPos_x * 0.05f;
        float y = newPos_y * 0.05f;
        if (x <= 0.85 && x >= -0.85 && y <= 0.85 && y >= -0.85) return true;
        return false;
    }

    void calcNewPosition() {
        next_position = {pos_x * 0.05, pos_y * 0.05, 0.0};
    }
public:
    float angle;
    glm::vec3 next_position;

    Pacman() {
        pos_x = 0;
        pos_y = 0;
        calcNewPosition();
        angle = 0;
    }

    void up() {
        if (!isInBorder(pos_x, pos_y + 1))return;
        pos_y++;
        calcNewPosition();
        angle = 90.0;
    }

    void down() {
        if (!isInBorder(pos_x, pos_y - 1))return;
        pos_y--;
        calcNewPosition();
        angle = -90.0;
    }

    void left() {
        if (!isInBorder(pos_x - 1, pos_y))return;
        pos_x--;
        calcNewPosition();
        angle = -180.0;
    }

    void right() {
        if (!isInBorder(pos_x + 1, pos_y))return;
        pos_x++;
        calcNewPosition();
        angle = 0;
    }
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

void move(GLFWwindow *window, Pacman &pacman);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;


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

    float const PI_OVER_4 = glm::quarter_pi<float>();

    // Starting angle is not 0, but PI/8
    float angle = PI_OVER_4 / 2.0f;

    for (auto i = 0; i < 3; ++i)
        vertices.push_back(0.0f);

    for (auto i = 0; i < 9; ++i) {
        vertices.push_back(0.25f * glm::cos(angle));
        vertices.push_back(0.25f * glm::sin(angle));
        vertices.push_back(0.0f);
        angle += PI_OVER_4;
    }

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices), &vertices[0], GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // ---------

    // tell opengl for each sampler to which texture unit it belongs to (only has
    // to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use(); // don't forget to activate/use the shader before setting
    // uniforms!
    // either set it manually like so:
    //glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    // or set it via the texture class
    //ourShader.setInt("texture2", 1);

    // render loop
    // -----------

    Pacman pacman;

    while (!glfwWindowShouldClose(window)) {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // create transformations

        move(window, pacman);

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, pacman.next_position);
        transform = glm::rotate(transform, glm::radians(pacman.angle),
                                glm::vec3(0.0f, 0.0f, 1.0f));

        // render container
        ourShader.use();

        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // render container
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 9);

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

void move(GLFWwindow *window, Pacman &pacman) {
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        pacman.up();
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        pacman.down();
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        pacman.left();
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        pacman.right();
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
