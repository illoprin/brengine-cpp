#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace b_GameObject
{
	struct Transform
	{
	private:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		glm::mat4 model;
		void init_zero();
	public:
		Transform();
		Transform(glm::vec3, glm::vec3, glm::vec3);
		Transform& operator=(Transform&);
		Transform(const Transform&) = delete;
		Transform(Transform&);
		~Transform();

		void UpdateModel();
		/* Morph */
		void move(glm::vec3);
		void rotate(glm::vec3);
		void resize(glm::vec3);
		/* Setters */
		void setPosition(glm::vec3);
		void setRotation(glm::vec3);
		void setScale(glm::vec3);

		/* Getters */
		glm::vec3 getPosition() const;
		glm::vec3 getRotation() const;
		glm::vec3 getScale() const;
		glm::mat4 getModel() const;
	};
};