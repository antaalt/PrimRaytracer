#pragma once

#include "Ray.h"
#include "Hitable.h"
#include "Geometry.h"
#include "Texture.h"
#include "BSDF.h"
#include "Intersection.h"

namespace prim {

enum TextureType {
	COLOR_TEXTURE,
	NORMAL_TEXTURE,
	BUMP_TEXTURE,
	ROUGHNESS_TEXTURE,
	EMISSION_TEXTURE,
	NB_TEXTURES_TYPE
};

enum BSDFType {
	BSDF_REFLECTION = 1 << 0,
	BSDF_TRANSMISSION = 1 << 1,
	BSDF_GLOSSY = 1 << 2,
	BSDF_SPECULAR = 1 << 3,
	BSDF_DIFFUSE = 1 << 4,
	BSDF_ALL = BSDF_REFLECTION | BSDF_TRANSMISSION | BSDF_SPECULAR | BSDF_DIFFUSE | BSDF_GLOSSY
};

class Material
{
public:
	Material(Texture4f *color, BSDFType type) : m_type(type), m_texture(color) {}
	virtual color4f sample(const Intersection &intersection, const vec3f &wi, vec3f *wo, float *pdf, BSDFType *type) const = 0;
	vec3f scatter() const;
	BSDFType getType() const { return m_type; }
	Texture4f* getTexture() { return m_texture; }
protected:
	Texture4f *m_texture;
	BSDFType m_type;
};

}