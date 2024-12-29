#include "utils.h"


void b_Utils::read_file_lines(const char* filepath, std::string& buffer)
{
	FILE* src = fopen(filepath, "r");
	if (!src)
	{
		fprintf(stderr, "Error: Could not open file with path %s\n", filepath);
		return;
	}

	char line_buffer[MAX_FILE_LINE];
	unsigned long line_counter = 0;
	while (fgets(line_buffer, sizeof(line_buffer), src) != NULL)
	{
		buffer += line_buffer;
		++line_counter;
	}
	printf("File with path %s readed, line number is %lu\n", filepath, line_counter);

	fclose(src);

	if (buffer.length() < 2)
	{
		printf("Warning: File with path %s, string length is less than 2\n", filepath);
	}
};