#version 330

uniform mat4 MVP;

layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 vertex_color;

out vec3 fragment_color;

void main() {
  gl_Position = MVP * vec4(vertex_pos, 1.0);
  fragment_color = vertex_color;
}