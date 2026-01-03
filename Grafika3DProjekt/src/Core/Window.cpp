#include "Window.h"

/**
 * @brief Default constructor for Window.
 *
 * Initializes:
 * - Default resolution (1280x720)
 * - Key state array
 * - Mouse tracking variables
 * - Scroll offsets
 */
Window::Window()
{
    width = 1280;
    height = 720;

    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = false;
    }

    lastX = width / 2.0f;
    lastY = height / 2.0f;
    firstMouseMove = true;
    xChange = 0.0f;
    yChange = 0.0f;
    scrollX = 0.0;
    scrollY = 0.0;
}

/**
 * @brief Parameterized constructor for Window.
 *
 * @param windowWidth Width of the window.
 * @param windowHeight Height of the window.
 * @param full Whether the window should be fullscreen.
 */
Window::Window(GLint windowWidth, GLint windowHeight, bool full)
{
    width = windowWidth;
    height = windowHeight;
    fullscreen = full;

    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = false;
    }

    lastX = width / 2.0f;
    lastY = height / 2.0f;
    firstMouseMove = true;
    xChange = 0.0f;
    yChange = 0.0f;
}

/**
 * @brief Initializes the GLFW window, OpenGL context, and input callbacks.
 *
 * Handles:
 * - GLFW initialization
 * - Window creation (fullscreen or windowed)
 * - GLAD loading
 * - Depth testing, face culling, gamma correction
 * - Mouse capture
 * - Callback setup
 *
 * @return 0 on success, 1 on failure.
 */
int Window::Initialise()
{
    // Initialise GLFW
    if (!glfwInit())
    {
        printf("GLFW initialisation failed!");
        glfwTerminate();
        return 1;
    }

    // Set OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    // Set profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    if (fullscreen)
        mainWindow = glfwCreateWindow(width, height, "Gra", glfwGetPrimaryMonitor(), NULL);
    else
        mainWindow = glfwCreateWindow(width, height, "Gra", NULL, NULL);

    if (!mainWindow)
    {
        printf("GLFW window creation failed!");
        glfwTerminate();
        return 1;
    }

    // Get buffer size
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context
    glfwMakeContextCurrent(mainWindow);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("GLAD initialisation failed!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Enable face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Enable gamma correction
    glEnable(GL_FRAMEBUFFER_SRGB);

    // Set viewport
    glViewport(0, 0, bufferWidth, bufferHeight);

    // Capture mouse
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Store pointer to this instance
    glfwSetWindowUserPointer(mainWindow, this);

    // Create callbacks
    createCallbacks();

    return 0;
}

/**
 * @brief Keyboard input callback.
 *
 * Updates the internal key state array.
 *
 * @param window GLFW window pointer.
 * @param key Key code.
 * @param code Scan code (unused).
 * @param action Press/release action.
 * @param mode Modifier keys (unused).
 */
void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            theWindow->keys[key] = true;
        else if (action == GLFW_RELEASE)
            theWindow->keys[key] = false;
    }
}

/**
 * @brief Mouse movement callback.
 *
 * Tracks mouse delta movement for camera rotation.
 *
 * @param window GLFW window pointer.
 * @param xPos Current mouse X position.
 * @param yPos Current mouse Y position.
 */
void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (theWindow->firstMouseMove)
    {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->firstMouseMove = false;
    }

    theWindow->xChange = xPos - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - yPos;

    theWindow->lastX = xPos;
    theWindow->lastY = yPos;
}

/**
 * @brief Mouse scroll callback.
 *
 * Stores scroll offsets for zoom or UI interactions.
 *
 * @param window GLFW window pointer.
 * @param xOffset Horizontal scroll.
 * @param yOffset Vertical scroll.
 */
void Window::handleScroll(GLFWwindow* window, double xOffset, double yOffset)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    theWindow->scrollX = xOffset;
    theWindow->scrollY = yOffset;
}

/**
 * @brief Registers GLFW callbacks for keyboard, mouse movement, and scroll.
 */
void Window::createCallbacks()
{
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
    glfwSetScrollCallback(mainWindow, handleScroll);
}

/**
 * @brief Gets the horizontal mouse movement since last frame.
 *
 * @return X delta movement.
 */
GLfloat Window::getXChange()
{
    GLfloat theChange = xChange;
    xChange = 0.0f;
    return theChange;
}

/**
 * @brief Gets the vertical mouse movement since last frame.
 *
 * @return Y delta movement.
 */
GLfloat Window::getYChange()
{
    GLfloat theChange = yChange;
    yChange = 0.0f;
    return theChange;
}

/**
 * @brief Window destructor.
 *
 * Destroys the GLFW window and terminates GLFW.
 */
Window::~Window()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}

/**
 * @brief Gets the vertical scroll offset since last frame.
 *
 * @return Y scroll delta.
 */
double Window::getScrollY()
{
    double theScrollY = scrollY;
    scrollY = 0.0;
    return theScrollY;
}
