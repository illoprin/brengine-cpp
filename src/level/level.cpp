#include "level.h"

#include "../core/engine.h"
#include "../core/context.h"

using namespace b_Level;

Level::Level(std::string fn, b_Game::GameData* gd)
{
	game_data = gd;
	b_Level::FromBLF(fn, level_data);
	if (level_data.game_name != gd->file_name)
	{
		printf("b_Level::Level - Warning - Level named %s is from another game named %s\n",
			level_data.name.c_str(), game_data->file_name.c_str());
		return;
	}

	// Build level mesh
	std::vector<b_Level::LevelVertex> vertex_list;
	b_Level::LevelDataToVertices(level_data, *gd, vertex_list);
	b_Level::printLevelVertices(vertex_list);

	// Store data in VBO object
	GLuint lb = m_level.AddBuffer(vertex_list.data(), vertex_list.size() * sizeof(LevelVertex));
	
	// Add attributes

	// 1. In position
	m_level.SetDataPointer
		(lb, 3, sizeof(LevelVertex), offsetof(LevelVertex, position));
	// 2. In texcoord
	m_level.SetDataPointer
		(lb, 2, sizeof(LevelVertex), offsetof(LevelVertex, texcoord));
	// 3. In normal
	m_level.SetDataPointer
		(lb, 3, sizeof(LevelVertex), offsetof(LevelVertex, normal));
	
	// 4. Texture id
	m_level.SetDataIntegerPointer
		(lb, 1, sizeof(LevelVertex), offsetof(LevelVertex, textureId));
	// 5. Texture type
	m_level.SetDataIntegerPointer
		(lb, 1, sizeof(LevelVertex), offsetof(LevelVertex, textureType));
	// 6. Glow intensity
	m_level.SetDataPointer
		(lb, 1, sizeof(LevelVertex), offsetof(LevelVertex, glow_intensity));

	m_level.setTotal(vertex_list.size());

	b_log->logf("[INFO] Level %s - Mesh created!\n", level_data.name.c_str());
};

void Level::update()
{

};

void b_Level::Level::usePalette(b_Texture::TextureStorage3D* palette)
{
	this->palette = palette;
};
void b_Level::Level::useAtlas(b_Texture::TextureStorage3D* atlas)
{
	this->textures = atlas;
};

// -- Getters
b_Texture::TextureStorage3D* Level::getTextureStorage() const
{
	return this->textures;
};
b_Texture::TextureStorage3D* Level::getPalette() const
{
	return this->palette;
};
const b_Level::LevelData& b_Level::Level::getLevelData() const
{
	return this->level_data;
};
BaseMesh* Level::getMesh()
{
	return &m_level;
};
