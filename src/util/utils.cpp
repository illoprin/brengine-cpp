#include "utils.h"



void b_Utils::ReadFileLines(const char* filepath, std::string& buffer)
{
	FILE* src = fopen(filepath, "r");
	if (!src)
	{
		fprintf(stderr, "Error: Could not open file with path %s\n", filepath);
		return;
	}

	char line_buffer[MAX_BUFFER];
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
		printf("Warning - File with path %s, string length is less than 2\n", filepath);
	}
};

std::string b_Utils::TimeString()
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
};

void b_Utils::SplitFilename(
	std::string  src,
	std::string& filename,
	std::string& ext
)
{
	filename = {src.substr(0, src.find('.'))};
	ext = {src.substr(src.find('.') + 1)};
};

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

void b_ImageIO::FlipY(unsigned char* b, unsigned c, unsigned w, unsigned h)
{
	unsigned char* tmp = new unsigned char[w * h * c];
	for (unsigned y = 0; y < h; ++y)
    {
        for (unsigned x = 0; x < w; ++x)
        {
            for (unsigned ch = 0; ch < c; ++ch)
            {
                tmp[(h - 1 - y) * w * c + x * c + ch] = b[y * w * c + x * c + ch];
            }
        }
    }
	memcpy(b, tmp, w * h * c);
	delete tmp;
};