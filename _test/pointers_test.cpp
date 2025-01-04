#include <stdio.h>
#include <vector>
#include <time.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

int main()
{
	srand(time(NULL));
	std::vector<glm::mat4> matrices;

	for (int i = 0; i < 5; i++)
	{
		glm::mat4 model{1.f};
		model = glm::translate(model, 
			glm::vec3(rand() % 30,
					  rand() % 30,
					  rand() % 30)
		);
		matrices.push_back(model);
	};

	float* matrices_ptr = (float*)&matrices.data()[0][0][0];
	int index = 1;
	for (float* i = matrices_ptr; i < (matrices_ptr + matrices.size() * 16); i++)
	{
		printf("%.2f ", *i);
		if (index != 0)
		{
			if (index % 4 == 0)
				printf("\n");
			if (index % 16 == 0)
				printf("\n");
		}
		index++;
	}
	printf("Floats: %d\n", index - 1);
	printf("Size of vector: %lu\n", matrices.size());

	return 0;
}