#version 460 core
out vec4 FragColor;

uniform vec3 color; //holds rgb to modify lighting color

void main()
{
    FragColor = vec4(color, 1.0); // set alle 4 vector values to 1.0
}