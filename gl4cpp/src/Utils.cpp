
#ifndef GLFW_INCLUDE_GLCOREARB
# define GLFW_INCLUDE_GLCOREARB
#endif
#ifndef GLFW_INCLUDE_GLEXT
# define GLFW_INCLUDE_GLEXT
#endif

#include <iostream>
#include <GLFW/glfw3.h>

void
checkGlError(std::string file, int line)
{
	GLenum		err;

	err = glGetError();
	if (err != GL_NO_ERROR)
	{
		if (err == GL_INVALID_ENUM)
			std::cerr << "GL: Invalid enum in " << file << " line " << line << std::endl;
		else if (err == GL_INVALID_VALUE)
			std::cerr << "GL: Invalid value in " << file << " line " << line << std::endl;
		else if (err == GL_INVALID_OPERATION)
			std::cerr << "GL: Invalid operation in " << file << " line " << line << std::endl;
		else if (err == GL_INVALID_FRAMEBUFFER_OPERATION)
			std::cerr << "GL: Invalid framebuffer operation in " << file << " line " << line << std::endl;
		else if (err == GL_OUT_OF_MEMORY)
			std::cerr << "GL: Out of memory in " << file << " line " << line << std::endl;
	}
}
