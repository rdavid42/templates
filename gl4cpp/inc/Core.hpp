
#ifndef CORE_HPP
# define CORE_HPP

#ifndef GLFW_INCLUDE_GLCOREARB
# define GLFW_INCLUDE_GLCOREARB
#endif
#ifndef GLFW_INCLUDE_GLEXT
# define GLFW_INCLUDE_GLEXT
#endif

# include <iostream>
# include <GLFW/glfw3.h>
# include "Vec3.hpp"
# include "Utils.hpp"
# include "Bmp.hpp"
# include "Mat4.hpp"

class Core
{
public:
	/*
	** Window
	*/
	GLFWwindow				*window;
	int						windowWidth;
	int						windowHeight;

	/*
	** Matrices
	*/
/*	float					projMatrix[16];
	float					viewMatrix[16];*/
	Mat4<float>				projMatrix;
	Mat4<float>				viewMatrix;
	Mat4<float>				objectMatrix;

	/*
	** Camera
	*/
	Vec3<float>				cameraPos;
	Vec3<float>				cameraLookAt;

	/*
	** Shaders
	*/
	GLuint					vertexShader;
	GLuint					fragmentShader;
	GLuint					program;

	/*
	** Locations
	*/
	GLuint					projLoc;
	GLuint					viewLoc;
	GLuint					objLoc;

	GLuint					positionLoc;
	GLuint					colorLoc;

	/*
	** 3d Axes
	*/
	GLuint					axesVao;
	GLuint					axesVbo;

	Core(void);
	~Core(void);

	int						init(void);
	void					update(void);
	void					render(void);
	void					loop(void);

	/* locations */
	void					getLocations(void);

	/* textures */
	void					loadTextures(void);
	GLuint					loadTexture(char const *filename);

	/* shaders */
	int						compileShader(GLuint shader, char const *filename);
	GLuint					loadShader(GLenum type, char const *filename);
	int						loadShaders(void);
	void					attachShaders(void);
	int						linkProgram(void);
	void					deleteShaders(void);
	int						initShaders(void);

	/* 3d axes */
	void					createAxes(void);
	void					renderAxes(void);

	/* matrices */
	void					setViewMatrix(Mat4<float> &view, Vec3<float> const &dir,
										Vec3<float> const &right, Vec3<float> const &up);
	void					setCamera(Mat4<float> &view, Vec3<float> const &pos, Vec3<float> const &lookAt);
	void					buildProjectionMatrix(Mat4<float> &proj, float const &fov,
												float const &near, float const &far);

	Core					&operator=(Core const &rhs);

private:
	Core(Core const &src);
};

std::ostream				&operator<<(std::ostream &o, Core const &i);

#endif
