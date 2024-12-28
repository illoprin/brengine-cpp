#include "log.h"

Log::Log(const char* path)
{
	// Create file name
	time_t current_time = time(NULL);
	char file_name[MAX_FILENAME];
	strftime(
		file_name, 
		MAX_FILENAME, 
		"%d-%m-%Y %H:%M:%S.log",
		localtime(&current_time)
	);

	// Open new file
	std::string file_path = std::string(path) + "/" + std::string(file_name);
	this->file = fopen(file_path.c_str(), "w");
	if (this->file == NULL)
	{
		fprintf(stderr, "Logger: Could not open new log file with path: %s!\n", file_path.c_str());
		exit(EXIT_FAILURE);
	}
}

int Log::logf(const char* format, ...)
{
	// va_list can be passed once to func that receives va_list
	va_list args, args2;
	va_start(args, format);
	va_copy(args2, args);

	// Print to console
	vprintf(format, args); 
	
	va_end(args);

	// Print to file
	vfprintf(this->file, format, args2); 
	
	va_end(args2);

	// Write buffer to file
	fflush(this->file);

	return 0;
}

Log::~Log()
{
	fclose(this->file);
	fprintf(stderr, "Logger: File writed!\n");
}
