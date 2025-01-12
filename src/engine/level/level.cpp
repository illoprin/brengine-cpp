#include "level.h"

#include "../core/engine.h"

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
	std::vector<b_Level::LevelVertex> vl;
	b_Level::LevelDataToVertices(level_data, *gd, vl);

	m_level = new BaseMesh{"LevelMesh"};
	// Store data in VBO object
	GLuint lb = m_level->AddBuffer(vl.data(), vl.size() * sizeof(b_Level::LevelVertex));
	// Add attributes
	m_level->SetDataPointer(lb, 3, sizeof(LevelVertex), offsetof(LevelVertex, position.x));
	m_level->SetDataPointer(lb, 2, sizeof(LevelVertex), offsetof(LevelVertex, texcoord.x));
	m_level->SetDataPointer(lb, 3, sizeof(LevelVertex), offsetof(LevelVertex, normal.x));
	m_level->SetDataIntegerPointer(lb, 1, sizeof(LevelVertex), offsetof(LevelVertex, textureId));
	m_level->SetDataIntegerPointer(lb, 1, sizeof(LevelVertex), offsetof(LevelVertex, textureType));
	m_level->SetDataPointer(lb, 1, sizeof(LevelVertex), offsetof(LevelVertex, glow_intensity));

	b_log->logf("[INFO] Level %s - Mesh created!\n", level_data.name.c_str());
};

BaseMesh* Level::getMesh() const
{
	return m_level;
};
b_Texture::TextureStorage3D* Level::getTextureStorage() const
{
	return this->textures;
};
b_Texture::TextureStorage3D* Level::getPalette() const
{
	return this->palette;
};


void Level::update()
{

};

Level::~Level()
{
	// Release VAO object
	delete m_level;
};