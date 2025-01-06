#include "utils.h"

void b_Utils::read_file_lines(const char* filepath, std::string& buffer)
{
	FILE* src = fopen(filepath, "r");
	if (!src)
	{
		fprintf(stderr, "Error: Could not open file with path %s\n", filepath);
		fclose(src);
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

std::string b_Utils::current_time_s()
{
	time_t current_time = time(NULL);
	char c_time_str[256];
	strftime(
		c_time_str, 256, 
		"%d-%m-%Y %H:%M:%S",
		localtime(&current_time)
	);
	std::string time_s{c_time_str};
	return time_s;
}

void b_ImageIO::GenBytes(std::vector<unsigned char>& bytes, unsigned width, unsigned height)
{
	unsigned char r, g, b;
	for (unsigned i = 0; i < width * height; i++)
	{
		float factor = (float)i / (float)(width * height);
		
		// Generate gradient
		r = (unsigned char) floor(255.f * factor);
		g = (unsigned char) floor(255.f * factor); 
		b = (unsigned char) floor(255.f * factor); 

		// Set values to vector
		// RGB - stride is 3
		bytes[i * 3 + 0] = r; bytes[i * 3 + 1] = g; bytes[i * 3 + 2] = b;
	}
};

void b_ImageIO::WriteBytes(
	std::vector<unsigned char>& byteData,
	int width, 
	int height, 
	int components, 
	const char* path
)
{
	if (byteData.size() < components)
	{
		fprintf(stderr, "ImageIO - Could not write, byte data size is less then %d\n", components);
		return;
	}

	if (strlen(path) < 2)
	{
		fprintf(stderr, "ImageIO - Could not write, file path field is empty\n");
		return;
	}
	
	if (!stbi_write_png(path, width, height, components, &byteData[0], components * width))
	{
		fprintf(stderr, "ImageIO - ERROR Could not write\n");
		return;
	}
	printf("ImageIO - png file with path %s writed!\n", path);
};

void b_Files::InitFilesystem()
{
	if (!fs::is_directory(FS_TEMP_PATH))
	{
		fs::create_directory(fs::path(FS_TEMP_PATH));
		printf("b_Files::InitFilesystem - %s directory created\n", FS_TEMP_PATH);
	};

	if (!fs::is_directory(FS_SCREENSHOTS_PATH))
	{
		fs::create_directory(fs::path(FS_SCREENSHOTS_PATH));
		printf("b_Files::InitFilesystem - %s directory created\n", FS_SCREENSHOTS_PATH);
	};

	fs::path log_dir = fs::path(FS_TEMP_PATH) / fs::path(FS_LOG_PATH);
	if (!fs::is_directory(log_dir))
	{
		fs::create_directory(log_dir);
	};

	if (!fs::is_directory(FS_ASSETS_PATH))
	{
		fs::create_directory(fs::path(FS_ASSETS_PATH));
	};
};