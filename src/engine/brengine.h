#ifndef BRENGINE_3D
#define BRENGINE_3D

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "utils/deps.h"

#include "engine.h"

// Model
#include "mesh/model.h"
#include "mesh/base_mesh.h"
#include "mesh/simple_mesh.h"

// Level
#include "level/level_data.h"

// World
#include "camera.h"
#include "render/texture.h"
#include "render/program.h"
#include "game_object/transform.h"
#include "game_object/entity.h"

// GUI
#include "ui/gui_item.h"
#include "ui/gui_image.h"
#include "ui/gui_text.h"
#include "ui/gui_scene.h"

// Scene
#include "scene_3d.h"

// Engine
#include "render/renderer.h"

// Utils
#include "utils/utils.h"

// Game
#include "editor_controller.h"
#include "assets.h"

#endif