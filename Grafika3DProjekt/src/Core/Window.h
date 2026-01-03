#pragma once
// Standard Library Includes
#include "stdio.h"

// GL includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>

/**
 * @class Window
 * @brief Manages the creation of the GLFW window, OpenGL context, and user input.
 *
 * The Window class handles:
 * - Window creation (fullscreen or windowed)
 * - GLAD initialization
 * - Input handling (keyboard, mouse movement, scroll)
 * - Mouse delta tracking for camera control
 * - Buffer swapping and window close state
 */
class Window
{
public:

    /**
     * @brief Default constructor.
     *
     * Initializes:
     * - Default resolution (1280x720)
     * - Key state array
     * - Mouse tracking variables
     * - Scroll offsets
     */
    Window();

    /**
     * @brief Parameterized constructor.
     *
     * @param windowWidth Width of the window.
     * @param windowHeight Height of the window.
     * @param full Whether the window should be fullscreen.
     */
    Window(GLint windowWidth, GLint windowHeight, bool full = false);

    /**
     * @brief Initializes GLFW, GLAD, the OpenGL context, and input callbacks.
     *
     * @return 0 on success, 1 on failure.
     */
    int Initialise();

    /**
     * @brief Gets the framebuffer width.
     *
     * @return Width of the framebuffer.
     */
    GLint getBufferWidth() { return bufferWidth; }

    /**
     * @brief Gets the framebuffer height.
     *
     * @return Height of the framebuffer.
     */
    GLint getBufferHeight() { return bufferHeight; }

    /**
     * @brief Checks whether the window should close.
     *
     * @return True if the window is closing.
     */
    bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

    /**
     * @brief Sets the window close flag.
     *
     * @param close True to close the window.
     */
    void setShouldClose(bool close) { glfwSetWindowShouldClose(mainWindow, close); }

    /**
     * @brief Gets the array of key states.
     *
     * @return Pointer to the key state array.
     */
    bool* getKeys() { return keys; }

    /**
     * @brief Gets the horizontal mouse movement since last frame.
     *
     * @return X delta movement.
     */
    GLfloat getXChange();

    /**
     * @brief Gets the vertical mouse movement since last frame.
     *
     * @return Y delta movement.
     */
    GLfloat getYChange();

    /**
     * @brief Gets the vertical scroll offset since last frame.
     *
     * @return Y scroll delta.
     */
    double getScrollY();

    /**
     * @brief Gets the GLFW window pointer.
     *
     * @return Pointer to GLFWwindow.
     */
    GLFWwindow* getWindow() { return mainWindow; }

    /**
     * @brief Swaps the front and back buffers.
     */
    void swapBuffers() { glfwSwapBuffers(mainWindow); }

    /**
     * @brief Destructor.
     *
     * Destroys the GLFW window and terminates GLFW.
     */
    ~Window();

private:

    /** @brief Pointer to the GLFW window. */
    GLFWwindow* mainWindow;

    /** @brief Window width and height. */
    GLint width, height;

    /** @brief Whether the window is fullscreen. */
    bool fullscreen;

    /** @brief Framebuffer width and height. */
    GLint bufferWidth, bufferHeight;

    /** @brief Last recorded mouse X position. */
    GLfloat lastX;

    /** @brief Last recorded mouse Y position. */
    GLfloat lastY;

    /** @brief Mouse X delta since last frame. */
    GLfloat xChange;

    /** @brief Mouse Y delta since last frame. */
    GLfloat yChange;

    /** @brief Scroll X offset. */
    double scrollX;

    /** @brief Scroll Y offset. */
    double scrollY;

    /** @brief Whether this is the first mouse movement event. */
    bool firstMouseMove;

    /** @brief Array storing the state of 1024 possible keys. */
    bool keys[1024];

    /**
     * @brief GLFW keyboard callback.
     *
     * @param window GLFW window pointer.
     * @param key Key code.
     * @param code Scan code.
     * @param action Press/release action.
     * @param mode Modifier keys.
     */
    static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);

    /**
     * @brief GLFW mouse movement callback.
     *
     * @param window GLFW window pointer.
     * @param xPos Current mouse X position.
     * @param yPos Current mouse Y position.
     */
    static void handleMouse(GLFWwindow* window, double xPos, double yPos);

    /**
     * @brief GLFW scroll callback.
     *
     * @param window GLFW window pointer.
     * @param xOffset Horizontal scroll.
     * @param yOffset Vertical scroll.
     */
    static void handleScroll(GLFWwindow* window, double xOffset, double yOffset);

    /**
     * @brief Registers GLFW callbacks for keyboard, mouse, and scroll.
     */
    void createCallbacks();
};
