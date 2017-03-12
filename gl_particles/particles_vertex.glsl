#version 330 core

uniform mat4 mv;

layout(location = 0) in vec3 pos;
layout(location = 3) in vec3 in_particle_color;

out vec3 particle_color;

void main(void)
{
	gl_Position = mv * vec4(pos, 1.0);
	particle_color = in_particle_color;
}
