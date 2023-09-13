#include "Window.h"

/*Window::Window()
{
    //windows size
    width   = 800;
    height  = 600;

    xChange = 0.0f;
    yChange = 0.0f;

    // because we know how many keys are we dow keep the array size and just
    // use 104
    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }
}*/

Window::Window(GLint windowWidth, GLint windowHeight)
{
    width = windowWidth;
    height = windowHeight;

    xChange = 0.0f;
    yChange = 0.0f;

    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }
}

Window::~Window()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}

int Window::Initialise()
{
    // --------------------------------------
    // -----------IMPORTANT------------------
    // --------------------------------------
    //          Initialize GLFW
    if (!glfwInit())
    {
        printf("GLFW Initialization failed!");
        glfwTerminate();
        return 1;
    }

    // Set GLFW options
    // in next two following line we define the version 3.3 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // GLFW_OPENGL_PROFILE          : define how should code that past to it interpered
   // GLFW_OPENGL_CORE_PROFILE     : not be backward compatible (we dont want use deprecated features , feature thta removed or going to be removed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // allow foreward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a GLFW window
    // we actually create a pointer to window
    mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
    /////////GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", nullptr, nullptr);
    
    //checking that windows is created successfuly
    if (!mainWindow)
    {
        printf("GLFW window creation failed!");
        glfwTerminate();
        return 1;
    }
    //set contex for GLEW to use
    //we can stitch windows and draw on them
    glfwMakeContextCurrent(mainWindow);

    //get buffer size information
    ///////////int bufferwidth, bufferheight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    

    //handle Key + Mouse input
    createCallbacks();
    
    //this lock the mouse to thw window
    //glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //allow  modern extention features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLFW Initialization failed!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Set viewport size

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, bufferWidth, bufferHeight);

    glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
    // when  a key prees on main window then call back "handleKeys" function
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
    // getting th window
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    
    //if user press the escape key 
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        //manually close the window
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    //key preesed
    if (key >= 0 && key < 1024)
    {
        /*if (action == GLFW_PRESS) theWindow->keys[key] = true;
        else if (action == GLFW_RELEASE) theWindow->keys[key] = false;*/

        if (action == GLFW_PRESS)
        {
            theWindow->keys[key] = true;
            printf("pressed : %d", key);
        }
        else if (action == GLFW_RELEASE)
        {
            theWindow->keys[key] = false;
            printf("released : %d", key);
        }
    }
}

GLfloat Window::getXChange()
{
    // we set the change to zero becasue we are lazy in real game we
    // must define a bool to say after last modificTION didnt apply or not
    GLfloat theChange = xChange;
    xChange = 0.0f;
    return theChange;
}

GLfloat Window::getYChange()
{
    GLfloat theChange = yChange;
    yChange = 0.0f;
    return theChange;
}





void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    // check is it the forst movment
    //if (theWindow->mouseFirstMoved)
    //{
    //    theWindow->lastX = xPos;
     //   theWindow->lastY = yPos;
     //   theWindow->mouseFirstMoved = false;
   // }

    theWindow->xChange = xPos - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - yPos;

    theWindow->lastX = xPos;
    theWindow->lastY = yPos;

    printf("x:%.6f , y:%.6f\n", theWindow->xChange, theWindow->yChange);
}
