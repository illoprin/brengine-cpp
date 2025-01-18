#pragma once

#include <glm/glm.hpp>
#include <texture/texture.h>

namespace b_GameObject
{
	struct EntityAppearance
	{
	private:
		glm::vec4 color{1.f};
		b_Texture::Texture* texture = nullptr;
		bool use_face_culling = true;
		float uv_scaling = 1.f;
	public:
		b_Texture::Texture* getTexture() const;
		const glm::vec4& getColor() const;
		bool getFaceCullingUsing() const;
		float getUVScaling() const;

		bool hasTexture() const;
		
		void setTexture(b_Texture::Texture*);
		void setUseFaceCulling(bool);
		void setUVScaling(float);
		void setColor(glm::vec3);
		void setAlpha(float);
	};
}
