// Standart C/C++ includes
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

// GLM
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

typedef unsigned char uch;

GLFWwindow* window;
uint16_t w_width = 1000, w_height = 600;
float w_aspect = (float)w_width / (float)w_height;
double _time, _deltaTime, _lastTime;
char w_title[32];

int mouse_last_x = 0, mouse_last_y = 0;
double mouse_delta_x, mouse_delta_y;
glm::vec3 cam_pos{0};
float quad_scaling = 1.f;

// SwitchImage logic components
int current_image = 0;
enum ImageTypes {
	IDEFAULT = 0,
	IQUANTIZED,
	IDITHERED,
	INUM,
};

void window_key_callback(
	GLFWwindow* window, int key, int scancode, int action, int mods
)
{
	if (action == GLFW_PRESS)
	{
		// Handle close action
		if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, 1);
		// Handle switch image action
		if (key == GLFW_KEY_S) current_image = (current_image + 1) % INUM;			
	}
};

void window_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	quad_scaling += (float)yoffset / 10.f;
	quad_scaling = quad_scaling <= .1f ? .1f : quad_scaling;
};


void window_resize_callback (
	GLFWwindow* window, int width, int height
)
{
	w_width = width; w_height = height;
	w_aspect = (float)w_width / (float)w_height;
	glViewport(0, 0, width, height);
};

static void window_error_callback(int error, const char* desc)
{
    fprintf(stderr, "Error: %s\n", desc);
}

int init_window()
{
	if (!glfwInit())
	{
		fprintf(stderr, "Could not init glfw\n");
		return 0;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(w_width, w_height, "Dithering test", NULL, NULL);

	if (!window)
	{
		fprintf(stderr, "Could not create window\n");
		return 0;
	}

	glfwSwapInterval(1); // VSync

	glfwFocusWindow(window);

	glfwSetKeyCallback(window, window_key_callback);
	glfwSetErrorCallback(window_error_callback);
	glfwSetScrollCallback(window, window_scroll_callback);
	glfwSetFramebufferSizeCallback(window, window_resize_callback);

	glfwMakeContextCurrent(window);

	return 1;
};

void compute_mouse_delta()
{
	double mouse_x, mouse_y;
	glfwGetCursorPos(window, &mouse_x, &mouse_y);
	mouse_delta_x = mouse_delta_y = 0.0;
	mouse_delta_x = mouse_x - mouse_last_x;
	mouse_delta_y = mouse_y - mouse_last_y;
	mouse_last_x = mouse_x;
	mouse_last_y = mouse_y;
};

int init_context()
{
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Could not create window\n");
		return 0;
	}
	glewExperimental = true;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	return 1;
};

GLuint compile_shader(const char* source, GLenum type)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (!status)
	{
		char error_log[512];
		glGetShaderInfoLog(shader, 512, NULL, error_log);
		fprintf(stderr, "Shader compilation error:\n%s\n", error_log);
	};

	return shader;
};

void link_program(GLuint prog, GLuint vsh, GLuint fsh)
{
	glAttachShader(prog, vsh);
	glAttachShader(prog, fsh);
	glLinkProgram(prog);
	int status;
	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	if (!status)
	{
		char error_log[512];
		glGetProgramInfoLog(prog, 512, NULL, error_log);
		fprintf(stderr, "Program link error:\n%s\n", error_log);
	};
}

// Creates GL_RGB texture object
GLuint create_texture(unsigned char* data, uint16_t width, uint16_t height)
{
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	return tex;
}

// Ordered dither matrix
// 1/64
int dither_matrix[8][8] = {
	{0, 32, 8, 40, 2, 34, 10, 42 },
	{48, 16, 56, 24, 50, 18, 58, 26 },
	{12, 44, 4, 36, 14, 46, 6, 38 },
	{60, 28, 52, 20, 62, 30, 54, 22 },
	{3, 35, 11, 43, 1, 33, 9, 41 },
	{51, 19, 59, 27, 49, 17, 57, 25 },
	{15, 47, 7, 39, 13, 45, 5, 37 },
	{63, 31, 55, 23, 61, 29, 53, 21 }
};

double palette_r = 255, palette_g = 217, palette_b = 168;
double exposure = 1.12;

/*
	Creates quantized or dithered image
	bpp - bits per pixel
	dither - dither image ?
*/
unsigned char* image_ordered_dither(
	unsigned char* src, uint16_t width, uint16_t height, uch bpp, bool use_palette
)
{
	unsigned char* dithered = (unsigned char*)malloc(width * height * 3);
	memcpy(dithered, src, width * height * 3);

	auto nearest_pallete_color = [](double grayscale, uch bits)
	{
		return round(((double)grayscale / 255.0) * (double)bits) * (255.0 / (double)bits); 
	};

	unsigned mod_x, mod_y;
	double grayscale, dithered_gr;
	double dr, dg, db;
	double r = 255.0 / (double)bpp;
	double old_r, old_g, old_b;
	for (unsigned y = 0; y < height; y++)
		for (unsigned x = 0; x < width; x++)
		{
			int bo = x + y * width;
			old_r = dithered[bo * 3 + 0];
			old_g = dithered[bo * 3 + 1];
			old_b = dithered[bo * 3 + 2];


			// Ordered dithering
			mod_x = x % 8;
			mod_y = y % 8;

			// Set new pixel to image
			if (use_palette)
			{
				// Get grayscaled pixel
				grayscale = 0.299 * old_r + 0.587 * old_g + 0.114 * old_b;
				dithered_gr = nearest_pallete_color(grayscale + r * ((double)dither_matrix[mod_y][mod_x] / 64.0), bpp);
				dithered[bo * 3 + 0] = (uch)std::clamp((dithered_gr / 255.0) * palette_r * exposure, 0.0, 255.0);
				dithered[bo * 3 + 1] = (uch)std::clamp((dithered_gr / 255.0) * palette_g * exposure, 0.0, 255.0);
				dithered[bo * 3 + 2] = (uch)std::clamp((dithered_gr / 255.0) * palette_b * exposure, 0.0, 255.0);
			}
			else
			{
				dr = nearest_pallete_color(old_r + r * ((double) dither_matrix[mod_y][mod_x] / 64.0), bpp);
				dg = nearest_pallete_color(old_g + r * ((double) dither_matrix[mod_y][mod_x] / 64.0), bpp);
				db = nearest_pallete_color(old_b + r * ((double) dither_matrix[mod_y][mod_x] / 64.0), bpp);
				dithered[bo * 3 + 0] = (uch)dr;
				dithered[bo * 3 + 1] = (uch)dg;
				dithered[bo * 3 + 2] = (uch)db;
			}

		};
	return dithered;
};


const char* vertex_shader_s =
"#version 430\n"

"in vec2 in_position;\n"
"in vec2 in_texcoord;\n"

"uniform mat4 u_projection;\n"
"uniform mat4 u_view;\n"
"uniform mat4 u_model;\n"

"out vec2 out_uv;"
"void main() {\n"
"	out_uv = in_texcoord;\n"
"	gl_Position = u_projection * u_view * u_model * vec4(in_position, 0.0, 1.0);\n"
"}\n";

const char* fragment_shader_s =
"#version 430\n"

"in vec2 out_uv;\n"

"uniform sampler2D u_texture;\n"

"out vec4 frag_color;\n"
"void main() {\n"
"	frag_color = texture(u_texture, out_uv);\n"
"}\n";

glm::mat4 projection{1.f};
glm::mat4 view{1.f};
glm::mat4 model{1.f};

int main()
{
	if (init_window())
	{	
		init_context();

		// -- Create shader program
		GLuint p1 = glCreateProgram();
		GLuint p1_vs = compile_shader(vertex_shader_s, GL_VERTEX_SHADER);
		GLuint p1_fs = compile_shader(fragment_shader_s, GL_FRAGMENT_SHADER);
		link_program(p1, p1_vs, p1_fs);
		GLint p1_model_loc = glGetUniformLocation(p1, "u_model");
		GLint p1_projection_loc = glGetUniformLocation(p1, "u_projection");
		GLint p1_view_loc = glGetUniformLocation(p1, "u_view");
		GLint p1_texture_loc = glGetUniformLocation(p1, "u_texture");

		// -- Init image to dither
		GLuint image_tex, quantized_tex, dithered_tex;
		int iwidth, iheight, icomponents;
		float iaspect = 1.f;

		stbi_set_flip_vertically_on_load(1);
		unsigned char* iimage = stbi_load("tmp/image_to_dither_2.bmp", &iwidth, &iheight, &icomponents, 3);
		if (iimage != NULL)
		{
			printf("Image loaded!\n");

			unsigned char* iquantized = image_ordered_dither(iimage, iwidth, iheight, 6, false);
			unsigned char* idithered = image_ordered_dither(iimage, iwidth, iheight, 4, true);
			quantized_tex = create_texture(iquantized, iwidth, iheight);
			dithered_tex = create_texture(idithered, iwidth, iheight);
			image_tex = create_texture(iimage, iwidth, iheight);
			
			free(iimage);
			free(iquantized);
			free(idithered);
			iaspect = (float)iwidth / (float)iheight;
		}
		else
		{
			fprintf(stderr, "Could not load image\n");
			exit(EXIT_FAILURE);
		}


		// -- Create quad mesh
		GLuint quad_vao;
		GLuint quad_buffer;

		static float quad_vertex_data[] = {
			 iaspect, -1.f, 1.f, 0.f,   // Left bottom
			-iaspect, -1.f, 0.f, 0.f,   // Right bottom
			 iaspect, 1.f, 1.f, 1.f, 	// Left top
			-iaspect, -1.f, 0.f, 0.f, 	// Right bottom
			-iaspect, 1.f, 0.f, 1.f, 	// Right top
			 iaspect, 1.f, 1.f, 1.f, 	// Left top
		};

		glCreateVertexArrays(1, &quad_vao);
		glBindVertexArray(quad_vao);

		glGenBuffers(1, &quad_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, quad_buffer);
		glBufferData(GL_ARRAY_BUFFER, 4 * 6 * sizeof(float), &quad_vertex_data[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(sizeof(float) * 2));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);


		// Game loop
		while(!glfwWindowShouldClose(window))
		{
			_time = glfwGetTime();
			glfwPollEvents();

			compute_mouse_delta();

			// -- Camera position update
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
			{
				cam_pos += glm::vec3{
					(float)-mouse_delta_x / (float)w_width * w_aspect,
					(float)mouse_delta_y / (float)w_height, 0.f};
			};

			// -- Matrices update
			projection = glm::ortho(
				-w_aspect, w_aspect, -1.f, 1.f, 0.f, 1.f
			);
			view = glm::lookAt(cam_pos, cam_pos + glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
			model = glm::mat4{1.f};
			model = glm::scale(model, glm::vec3{quad_scaling});
			// Set origin to center
			model = glm::translate(model, {-.5f, -.5f, 0.f});

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.1, 0.1, 0.1, 1.f);

			// -- Rendering
			glUseProgram(p1);
			glUniformMatrix4fv(p1_projection_loc, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(p1_view_loc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(p1_model_loc, 1, GL_FALSE, glm::value_ptr(model));

			glActiveTexture(GL_TEXTURE0);
			switch (current_image)
			{
				case ImageTypes::IDEFAULT:
					glBindTexture(GL_TEXTURE_2D, image_tex);
				break;
				case ImageTypes::IDITHERED:
					glBindTexture(GL_TEXTURE_2D, dithered_tex);
				break;
				case ImageTypes::IQUANTIZED:
					glBindTexture(GL_TEXTURE_2D, quantized_tex);
				break;
			}
			glUniform1i(p1_texture_loc, 0);

			glBindVertexArray(quad_vao);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			glDrawArrays(GL_TRIANGLES, 0, 6);
			
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glBindVertexArray(0);

			glfwSwapBuffers(window);

			// Compute fps
			_deltaTime = (_time - _lastTime) * 1000.0;
			_lastTime = _time;
			sprintf(w_title, "FPS %.0f", 1.f / (float)_deltaTime);
		}

		glfwDestroyWindow(window);
		glfwTerminate();
	}
	else
	{
		exit(EXIT_FAILURE);
	}

	return 0;
}