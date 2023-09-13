#version 330 core                              
out vec4 color;

//inputting the color
in vec4 vColor;

void main()
{
	color = vColor;// vec4(1.0,1.0,0.0, 1.0);
}