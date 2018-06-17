// VERTEX SHADER
#version 330 core

uniform mat4 matrix_model;
uniform mat4 matrix_view;
uniform mat4 matrix_projection;
uniform mat4 matrix_mvp;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec4 vertexColor;
layout(location = 3) in vec2 vertexTexCoord[4];

out vec4 fragmentColor;
out vec2 UV;

void main(){
  gl_Position = matrix_mvp * vec4(vertexPosition, 1);
  fragmentColor = vertexColor;
  UV = vertexTexCoord[0];
}