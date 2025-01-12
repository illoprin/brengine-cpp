#include "game_handler.h"

using namespace b_Game;

GameHandler::GameHandler(std::string filename)
{
	// Load game data
	GameDataFromBGD(filename, game_data);
	build_texture_storage();
	build_palette();
	game_data.print();
};

void GameHandler::installLevel(uint16_t index)
{
	std::unique_ptr<b_Level::Level> lvl_ptr{
		new b_Level::Level{game_data.levels[index], &game_data}
	};

	level_up = std::move(lvl_ptr);
};

void GameHandler::releaseLevel()
{
	if (this->level_up.get() != nullptr)
		this->level_up.reset();
};

void GameHandler::update()
{
	p_player.update();
	level_up->update();
};

// -- Getters
b_Level::Level* GameHandler::getCurrentLevel() const
{
	return level_up.get();
};

void GameHandler::build_texture_storage()
{

};

void GameHandler::build_palette()
{

};

GameHandler::~GameHandler()
{
	releaseLevel();
};