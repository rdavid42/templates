#ifndef UTILS_HPP
# define UTILS_HPP

#ifndef GLFW_INCLUDE_GLCOREARB
# define GLFW_INCLUDE_GLCOREARB
#endif
#ifndef GLFW_INCLUDE_GLEXT
# define GLFW_INCLUDE_GLEXT
#endif

# include <string>
# include <ostream>
# include <sys/stat.h>
# include <iostream>
# include <fstream>
# include <ostream>
# include <fcntl.h>
# include <unistd.h>
# include <sstream>
# include <GLFW/glfw3.h>


# define BUFSIZE			8192

void				checkGlError(std::string file, int line);
char *				readFile(char const *filename);
int					printError(std::ostream &msg, int const &code);
int					printError(std::string const &msg, int const &code);
void *				printError(std::string const &msg);
std::string			getFileContents(std::string const &filename);

#endif