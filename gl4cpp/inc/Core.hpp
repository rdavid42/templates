
#ifndef CORE_HPP
# define CORE_HPP

# define GLFW_INCLUDE_GLCOREARB
# define GLFW_INCLUDE_GLEXT

# include <iostream>
# include <ostream>
# include <sstream>
# include <fstream>
# include <sys/stat.h>
# include <GLFW/glfw3.h>
# include "Vec3.hpp"
# include "Utils.hpp"
# include "Bmp.hpp"

# define BUFSIZE			8192

class Core
{
public:
	GLFWwindow				*window;
	int						windowWidth;
	int						windowHeight;
	float					projMatrix[16];
	float					viewMatrix[16];
	Vec3<float>				cameraPos;
	Vec3<float>				cameraLookAt;

	Core(void);
	~Core(void);

	int						init(void);
	int						printError(std::ostream &msg, int const &code);
	int						printError(std::string const &msg, int const &code);
	void *					printError(std::string const &msg);
	std::string				getFileContents(std::string const &filename);
	void					update(void);
	void					render(void);
	void					loop(void);
	void					getLocations(void);
	void					loadTextures(void);
	GLuint					loadTexture(char const *filename);
	char *					readFile(char const *filename);
	int						compileShader(GLuint shader, char const *filename);
	GLuint					loadShader(GLenum type, char const *filename);

	/* matrix stuff */
	void					multiplyMatrix(float *a, float const *b);
	void					setTranslationMatrix(float *translation, float const &x,
												float const &y, float const &z);
	void					setViewMatrix(float *view, Vec3<float> const &dir,
										Vec3<float> const &right, Vec3<float> const &up);
	void					setCamera(float *view, Vec3<float> const &pos, Vec3<float> const &lookAt);
	void					buildProjectionMatrix(float *proj, float const &fov,
												float const &near, float const &far);
	void					setIdentityMatrix(float *mat, int const &size);

	Core					&operator=(Core const &rhs);

private:
	Core(Core const &src);
};

std::ostream				&operator<<(std::ostream &o, Core const &i);

#endif
