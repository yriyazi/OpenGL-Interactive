#pragma once

#include <stdio.h>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Window
{
public:
    //Window();
    Window(GLint windowWidth, GLint windowHeight);

    int Initialise();

    GLint getBufferWidth()  { return bufferWidth ; }
    GLint getBufferHeight() { return bufferHeight; }

    bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

    bool* getsKeys() { return keys; }
    GLfloat getXChange();
    GLfloat getYChange();

    void swapBuffers() { glfwSwapBuffers(mainWindow); }
    void updateBuffer(){ glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight); }
    void update_viewport()
    {
        updateBuffer();
        // Adjust the viewport to the new window size
        glViewport(0, 0, bufferWidth, bufferHeight);
    }

    void screen_Clear()
    {
        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    ~Window();
private:
    GLFWwindow* mainWindow;
    GLint width, height;
    GLint bufferWidth, bufferHeight;

    //array of bollain with lenght of ascii
    //basically this boo bool type will be true as long as key is hold and the !true
    bool keys[1024];

    GLfloat lastX;
    GLfloat lastY;
    GLfloat xChange;
    GLfloat yChange;
    bool mouseFirstMoved;

    // create all call backs 
    void createCallbacks();
    //we use static here because normal class member cant get opengl callback
    // code and mode not applicanle for our purpose but can be come in handy
    static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
    static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};