
#include "engine/brengine.h"

int main()
{
	Engine engine;

	std::vector<Triangle> triangle_mesh_data
	{
		{
			{
				{0.f, .5f, 0.f, 0, 0, 0, 0, 0},
				{-.5f, -.5f, 0.f, 0, 0, 0, 0, 0},
				{.5f, -.5f, 0.f, 0, 0, 0, 0, 0},
			}
		},
	};
	
	BaseMesh tm(engine.getLogger(), "basic_triangle");
	tm.simpleInit(&triangle_mesh_data);

	Entity tri{ &tm, engine.getDefaultProgram() };

	Scene demo;
	demo.append(&tri);

	while (!glfwWindowShouldClose(engine.getWindow()))
	{
		engine.prepare();
		engine.update(demo);
		engine.render(demo);
		engine.close();
	}
	
	return 0;
}