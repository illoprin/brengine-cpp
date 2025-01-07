#include <cstdio>
#include <cstring>
#include <vector>
#include <time.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

struct SimpleVertex2D
{
	glm::vec2 position;
	glm::vec2 texcoord;
};

void MutateVertices(std::vector<SimpleVertex2D>& v)
{
	SimpleVertex2D v1, v2;
	v1 = { {3, 8}, {0.4, 0.2} };
	v2 = { {4, 5}, {0.1, 0.7} };
	v[3] = v1;
	v[2] = v2;
};

int main()
{
	srand(time(NULL));

	std::vector<SimpleVertex2D> vertices(4);

	MutateVertices(vertices);

	for (SimpleVertex2D& v : vertices)
	{
		printf("X: %f Y: %f U: %f V: %f\n",
			v.position.x, v.position.y, v.texcoord.x, v.texcoord.y); 
	}

	return 0;
}