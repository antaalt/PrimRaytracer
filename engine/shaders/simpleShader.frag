// FRAGMENT SHADER
#version 330 core

in vec4 fragmentColor;
in vec2 UV;

out vec4 color;

uniform sampler2D textureSampler0;
uniform sampler2D textureSampler1;
uniform sampler2D textureSampler2;
uniform sampler2D textureSampler3;

void main(){
  //color = fragmentColor;
  color = texture(textureSampler0, UV);
}