#version 150

in block {
	vec3 normalView;
	vec3 positionView;
	vec2 texcoord;
} fs_in;

uniform sampler2D texture0;
uniform mat4 modelViewMatrix;
uniform mat4 viewMatrix;
uniform mat3 normalMatrix;

#define NB_MAX_LIGHTS 8
struct lightParams {
	vec4 diffuse;
	vec4 position;
};

struct materialParams {
    vec4 diffuse;
    vec4 emissive;
    vec4 ambient;
    vec4 specular;
    float power_specular;
};

uniform materialParams material;

uniform int nbLights;
uniform lightParams light[NB_MAX_LIGHTS];

out vec4 out_color;

void main() {
    // BLINN-PHONG model
    // I = ka + kd.(L.N) + ks.(H.Ns)^s;
    // H = E + L / ||E + L ||

    vec3 e = normalize(-fs_in.positionView);
	vec3 l = normalize((viewMatrix*light[0].position).xyz - fs_in.positionView);
	vec3 n = normalize(fs_in.normalView);

    out_color = material.ambient + material.emissive;
    vec4 dif = material.diffuse * clamp(dot(l, n), 0.0, 1.0) * texture(texture0, fs_in.texcoord);
    out_color += dif;

    if(material.power_specular!=0.0f)
	{
		vec3 h = normalize(e + l);
		vec4 spec = material.specular * pow( clamp(dot(h,n),0.0,1.0), material.power_specular );
		out_color += spec;
	}
}
