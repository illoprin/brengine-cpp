#pragma once

#include "../util/deps.h"
#include "../mesh/base_mesh.h"
#include "transform.h"
#include "entity_view.h"
#include "entity_view.h"

namespace b_GameObject
{
	/*
		Entity - it is any object that exists in 3D world of game

		It can be one point or textured mesh or something else
	*/
	class Entity
	{

	public:
		Entity(std::string);
		Entity(std::string, b_GameObject::Transform);
		

		virtual ~Entity();
		const std::string& getName() const;
		b_GameObject::Transform transform;
		
		// --- Methods
		virtual void update();
		virtual bool hasView();

		// -- Override
		const EntityAppearance& getAppearance() const;
		
		BaseMesh* getMesh() const;
		
		// -- Setters
		void setMesh(BaseMesh*);
		void setAppearance(const EntityAppearance&);
	protected:
		// Base
		std::string name;

		// Rendering
		BaseMesh* mesh;
		EntityAppearance ap;

		virtual void init_view();
	};
}
