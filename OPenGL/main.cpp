# include "all_includes.h"

const GLchar* vShader = R"(
    #version 330 core                              
    layout (location = 0) in vec3 position;

    //outputing color
    out vec4 vColor;

    //for tranlation,rotation and scale
    uniform mat4 model;
    
    //for projection
    uniform mat4 projection;

    uniform mat4 view;

    //camera
    //uniform mat4 view;

    void main()
    {

        gl_Position = projection * view * model * vec4(position, 1.0);

        //
        vColor = vec4(clamp(vec3(gl_Position.x,gl_Position.y,gl_Position.z),0.0f,1.0f),1.0f);
        //vColor = vec4(clamp(position,0.0f,1.0f),1.0f);
    }
)";

const GLchar* fShader = R"(
    #version 330 core                              
    out vec4 color;

    //inputting the color
    in vec4 vColor;

    void main()
    {
        color = vColor;// vec4(1.0,1.0,0.0, 1.0);
    }
)";





GLuint VAO, VBO , IBO , shader;

float   triOffset = 0.0f;
bool    direction = true;

float TriMaxOffset = 0.7f;
float triIncrement = 0.005f; 


void CreateTriangle()
{
    // Define triangle3D vertices
    unsigned int indices[] =    {
                                0,3,1,
                                1,3,2,
                                2,3,0,
                                0,1,2
                                };

    GLfloat vertices[] =    {
                            -1.0f, -1.0f, 0.0f, //index = 0
                             0.0f, -1.0f, 1.0f, //index = 1
                             1.0f, -1.0f, 0.0f, //index = 2
                             0.0f,  5.0f, 0.0f, //index = 3
                            };
    // Creating a VAO
    // glGenVertexArrays(how many,id Of that)
    glGenVertexArrays(1, & VAO);
    // because the array is on GPu and there is no direct acess we need to pass
    // the id and then conncet that id to that shader
    glBindVertexArray(VAO);

    //create buffer for IBO
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,GL_STATIC_DRAW);
        //we need to make a buffer to it go inside that
        // 
        // create a buffer object inside the VAO and pass the id
            glGenBuffers(1,& VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

                ///////////// GL_STATIC_DRAW dosent allow you to change the values
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,GL_STATIC_DRAW);
                // glVertexAttribPointer( 0 , row lenght , data type , do normalize ,stride in array , start from);
                glVertexAttribPointer( 0 , 3 , GL_FLOAT , GL_FALSE , 0 , 0);
                glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void addShader(GLuint theProgram , const GLchar* shaderCode , GLenum    shaderType)
{
    //creatr empty shader for this type
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLenght[1];
    codeLenght[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLenght);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar elog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(theShader, sizeof(elog), NULL, elog);
        printf("Error in Compiling the %d shader program : '%s \n", shaderType, elog);

        return;
    }

    glAttachShader(theProgram, theShader);

    return;
}

void CompileShader() 
{
    shader = glCreateProgram();

    if (!shader) {
        printf("Error in shader Compiling");

        return ;
    }

    addShader(shader, vShader, GL_VERTEX_SHADER);
    addShader(shader, fShader, GL_FRAGMENT_SHADER);
    
    // for debuging
    GLint result = 0;
    GLchar elog[1024] = { 0 };

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, & result);
    if (!result) {
        glGetProgramInfoLog(shader, sizeof(elog),NULL, elog);
        printf("Error in Linking program : '%s \n",elog);

        return;
    }

    //validate program (if opengl is set up correctly for this shader)
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
        printf("Error in Validating program : '%s \n", elog);

        return;
    }

    // uniform xMOve
    //--------------------------------------------------
    // this id will be use to bind tha variable to value
    //--------------------------------------------------
    GLint uniformModel = glGetUniformLocation(shader, "model");

    
}



int main()
{

    Window window = Window(1280, 720);
    window.Initialise();
    
  

    CreateTriangle();
    CompileShader();
    
    // defining the id of the model and projection and in future view
    GLint uniformModel      = glGetUniformLocation(shader, "model");
    GLint uniformprojection = glGetUniformLocation(shader, "projection");
    GLint uniformview       = glGetUniformLocation(shader, "view");

    Camera camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.50f);

    GLfloat deltaTime = 0.0f;
    GLfloat lastTime = 0.0f;

    // Render loop
    while (!window.getShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        window.update_viewport();
        // Poll events (get and handle user input events)
        glfwPollEvents();

        if (direction)
        {
            triOffset += triIncrement;
        }
        else
        {
            triOffset -= triIncrement;
        }

        if (abs(triOffset)>=TriMaxOffset)
        {
            direction = !direction;
        }
        //printf(" % f",triOffset);
        window.screen_Clear();

        // assing and unassing shader
        glUseProgram(shader);


        float angle = static_cast<float>(glfwGetTime()) * glm::radians(50.0f) * 2;

        glm::mat4 model = glm::mat4(1.0f);  // Identity matrix
        glm::vec3 translation(triOffset, triOffset,-2.50f);
        //model = glm::scale(model    , glm::vec3(0.5f, 0.5f, 1.0f));        
        model = glm::translate(model, translation);  // Apply translation
        model = glm::rotate(model   , angle, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Z-axis
        
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));


        glm::mat4 projection = glm::mat4(1.0f);  // Identity matrix
        projection = glm::perspective(45.0f,(GLfloat)window.getBufferWidth() /(GLfloat)window.getBufferHeight(),0.1f,100.0f);
        glUniformMatrix4fv(uniformprojection, 1, GL_FALSE, glm::value_ptr(projection));

        camera.mouseControl(window.getXChange(), window.getYChange());
        camera.keyControl(window.getsKeys(), deltaTime);
        glUniformMatrix4fv(uniformview, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        
        
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

        //here we decise how to draw
        //glDrawArrays(GL_TRIANGLES,0,3);
        glDrawElements(GL_TRIANGLES,12,GL_UNSIGNED_INT,0);

        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glUseProgram(0);

        // Swap buffers (we have 2 scene 1 one draw and a for show and we change them
        window.swapBuffers();
    }

    // Terminate GLFW
    glfwTerminate();

    return 0;
}