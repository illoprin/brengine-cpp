#pragma once

#include "../util/deps.h"
#include "level_data.h"
#include "level_model.h"
#include "../game/game_data.h"
#include "../mesh/base_mesh.h"
#include "../entity/entity.h"
#include "../texture/texture_storage.h"

namespace b_Level
{
	class Level
	{
	public:
		Level(std::string filename, b_Game::GameData* gd);

		void useAtlas(b_Texture::TextureStorage3D*);
		void usePalette(b_Texture::TextureStorage3D*);
		
		b_Texture::TextureStorage3D* getTextureStorage() const;
		b_Texture::TextureStorage3D* getPalette() const;

		const b_Level::LevelData& getLevelData() const;

		BaseMesh* getMesh();
		void update();
		~Level() = default;
	private:
		b_Game::GameData* game_data;
		b_Level::LevelData level_data;
		b_Texture::TextureStorage3D* textures = nullptr;
		b_Texture::TextureStorage3D* palette = nullptr;
		
		
		BaseMesh m_level{"LevelMesh"};
		std::vector<b_GameObject::Entity> objects;
	};
};