#version 330 core
out vec4 FragColor;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec4 col;

void main()
{
        // linearly interpolate between both textures (80% container, 20% awesomeface)
        FragColor = vec4(0, 2/3, 1.0, 1.0);
}