#pragma once

#include "../utils/deps.h"
#include "base_mesh.h"
#include "../program.h"
#include "../texture.h"
#include "transform.h"

namespace b_GameObject
{
	class Entity
	{

	public:
		Entity(std::string);
		Entity(std::string, b_GameObject::Transform);
		~Entity();

		// --- Methods
		virtual void update();
		bool hasView();
		
		// -- Getters
		TextureImage2D* getTexture() const;
		glm::vec4 getColor() const;
		virtual BaseMesh* getMesh() const;
		virtual Program* getProgram() const;
		std::string getName() const;
		float getUVScaling() const;
		bool getFaceCulling() const;

		// -- Setters
		void setColor(glm::vec3);
		void setAlpha(float);
		void setMesh(BaseMesh*);
		void setProgram(Program*);
		void setTexture(TextureImage2D*);
		void setUVScaling(float);
		void setFaceCullingUsing(bool);

		b_GameObject::Transform transform;
	protected:
		// Base
		std::string name{"default"};

		// Rendering
		TextureImage2D* texture;
		float uv_scaling = 1.f;
		bool cull_faces = true;
		glm::vec4 color;
		BaseMesh* mesh;
		Program* program;

		// -- Private Methods
		virtual void init_view();
	};
}
