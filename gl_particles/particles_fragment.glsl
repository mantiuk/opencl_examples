#version 330 core

in vec3 particle_color;
out vec4 color;

void main(void)
{
	color = vec4(particle_color.r, particle_color.g, particle_color.b, 1.0);
}
