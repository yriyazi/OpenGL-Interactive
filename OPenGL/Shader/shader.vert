#version 330 core                              
layout (location = 0) in vec3 position;

//outputing color
out vec4 vColor;

//for tranlation,rotation and scale
uniform mat4 model;
    
//for projection
uniform mat4 projection;

//camera
//uniform mat4 view;

void main()
{

    gl_Position = projection * model * vec4(position, 1.0);

    //
    //vColor = vec4(clamp(vec3(gl_Position.x,gl_Position.y,gl_Position.z),0.0f,1.0f),1.0f);
    vColor = vec4(clamp(position,0.0f,1.0f),1.0f);

}