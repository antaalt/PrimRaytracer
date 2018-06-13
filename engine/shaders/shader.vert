#version 150

in vec3 in_position;
in vec3 in_normal;
in vec3 in_color;
in vec2 in_texcoord0;
in vec2 in_texcoord1;
in vec2 in_texcoord2;
in vec2 in_texcoord3;

uniform mat4 viewMatrix;
uniform mat3 normalMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;

#define NB_MAX_LIGHTS 8
struct lightParams {
	vec4 diffuse;
	vec4 position;
};

uniform int nbLights;
uniform lightParams light[NB_MAX_LIGHTS];

out block {
	vec3 normalView;
	vec3 positionView;
	vec2 texcoord;
} vs_out;


void main() {
	vs_out.positionView = (modelViewMatrix * vec4(in_position, 1.0)).xyz;
	vs_out.normalView = normalize((viewMatrix * vec4(normalMatrix * in_normal, 0.0)).xyz);;
	vs_out.texcoord = in_texcoord0;

    gl_Position = modelViewProjectionMatrix * vec4(in_position, 1.0);
}
