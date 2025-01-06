#include "font.h"

using namespace b_Font;

void BufferFromTTF(
	const char*      fp,
	unsigned char**  buffer,
	size_t*          size
)
{
	if (*buffer != NULL)
	{
		free(*buffer);
		*buffer = NULL;
	}

	FILE* ttf_src = fopen(fp, "rb"); // Open TTF file in read bytes mode
	if (!ttf_src)
	{
		printf("b_Font::BufferFromTTF - Could not open file with path %s\n", fp);
		return;
	}

	fseek(ttf_src, 0, SEEK_END); // Move caret in end of file
	*size = ftell(ttf_src); // Get position of caret in bytes
	fseek(ttf_src, 0, SEEK_SET); // Reset caret position

	*buffer = (unsigned char*)malloc(*size); // Allocate memory for ttf bytes
	fread(*buffer, *size, 1, ttf_src); // Read bytes from file
	fclose(ttf_src); // Close file
	
	printf("b_Font::LoadTTFBytes - Opened file %s, size of bytes is %lu\n", fp, *size);

};