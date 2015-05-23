
#include "Core.hpp"

Core::Core(void)
{
	return ;
}

Core::~Core(void)
{
	glfwDestroyWindow(this->window);
	glfwTerminate();
	return ;
}

static void
key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	(void)scancode;
	(void)mods;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int
Core::printError(std::ostream &msg, int const &code)
{
	std::cerr << dynamic_cast<std::ostringstream &>(msg).str() << std::endl;
	return (code);
}

int
Core::printError(std::string const &msg, int const &code)
{
	std::cerr << msg << std::endl;
	return (code);
}

void *
Core::printError(std::string const &msg)
{
	std::cerr << msg << std::endl;
	return (0);
}

std::string
Core::getFileContents(std::string const &filename)
{
	std::ifstream		in(filename, std::ios::in | std::ios::binary);
	std::string			contents;

	in.seekg(0, std::ios::end);
	contents.resize(in.tellg());
	in.seekg(0, std::ios::beg);
	in.read(&contents[0], contents.size());
	in.close();
	return (contents);
}

GLuint
Core::loadTexture(char const *filename)
{
	GLuint				texture;
	Bmp					bmp;

	if (!bmp.load(filename))
		return (printError("Failed to load bmp !", 0));
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.width, bmp.height,
				0, GL_RGB, GL_UNSIGNED_BYTE, bmp.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);
	checkGlError(__FILE__, __LINE__);
	return (texture);
}

void
Core::loadTextures(void)
{

}

void
Core::getLocations(void)
{

}

void
Core::setIdentityMatrix(float *mat, int const &size)
{
	int				i;
	int const		s2 = size * size;

	i = -1;
	while (i++, i < s2)
		mat[i] = 0.0f;
	i = -1;
	while (i++, i < size)
		mat[i + i * size] = 1.0f;
}

void
Core::buildProjectionMatrix(float *proj, float const &fov,
							float const &near, float const &far)
{
	float const			f = 1.0f / tan(fov * (M_PI / 360.0));
	float const			ratio = (1.0f * this->windowWidth) / this->windowHeight;

	/*
	1 0 0 0
	0 1 0 0
	0 0 1 0
	0 0 0 1
	*/
	this->setIdentityMatrix(proj, 4);
	/*
	a 0 0 0
	0 b e 0
	0 0 c f
	0 0 d 1
	*/
	proj[0] = f / ratio; // a
	proj[1 * 4 + 1] = f; // b
	proj[2 * 4 + 2] = (far + near) / (near - far); // c
	proj[3 * 4 + 2] = (2.0f * far * near) / (near - far); // d
	proj[2 * 4 + 3] = -1.0f; // e
	proj[3 * 4 + 3] = 0.0f; // f
}

void
Core::setViewMatrix(float *view, Vec3<float> const &dir,
					Vec3<float> const &right, Vec3<float> const &up)
{
	/*
	rx		ux		-dx		0
	ry		uy		-dy		0
	rz		uz		-dz		0
	0		0		0		1
	*/
	// first column
	view[0] = right.x;
	view[4] = right.y;
	view[8] = right.z;
	view[12] = 0.0f;
	// second column
	view[1] = up.x;
	view[5] = up.y;
	view[9] = up.z;
	view[13] = 0.0f;
	// third column
	view[2] = -dir.x;
	view[6] = -dir.y;
	view[10] = -dir.z;
	view[14] = 0.0f;
	// fourth column
	view[3] = 0.0f;
	view[7] = 0.0f;
	view[11] = 0.0f;
	view[15] = 1.0f;
}

void
Core::setTranslationMatrix(float *translation, float const &x,
							float const &y, float const &z)
{
	/*
	1		0		0		0
	0		1		0		0
	0		0		1		0
	x		y		z		1
	*/
	this->setIdentityMatrix(translation, 4);
	translation[12] = x;
	translation[13] = y;
	translation[14] = z;
}

void
Core::multiplyMatrix(float *a, float const *b)
{
	float		res[16];
	int			i;
	int			j;
	int			k;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			res[j * 4 + i] = 0.0f;
			k = 0;
			while (k < 4)
			{
				res[j * 4 + i] += a[k * 4 + i] * b[j * 4 + k];
				k++;
			}
			j++;
		}
		i++;
	}
	memcpy(a, res, sizeof(float) * 16);
}

void
Core::setCamera(float *view, Vec3<float> const &pos, Vec3<float> const &lookAt)
{
	Vec3<float>		dir;
	Vec3<float>		right;
	Vec3<float>		up;
	float			translation[16];

	up.set(0.0f, 1.0f, 0.0f);
	dir.set(lookAt - pos);
	dir.normalize();
	right.crossProduct(dir, up);
	right.normalize();
	up.crossProduct(right, dir);
	up.normalize();
	this->setViewMatrix(view, dir, right, up);
	this->setTranslationMatrix(translation, -pos.x, -pos.y, -pos.z);
	this->multiplyMatrix(view, translation);
}

char *
Core::readFile(char const *filename)
{
	struct stat		file_stat;
	int				fd;
	int				i;
	int				j;
	int				ret;
	char			buf[BUFSIZE];
	char			*file;

	if ((fd = open(filename, O_RDONLY)) == -1)
		return ((char *)printError("Failed to open file!"));
	if (fstat(fd, &file_stat) == -1)
		return ((char *)printError("Failed to retrieve file stat!"));
	file = new char[file_stat.st_size + 1];
	i = 0;
	while ((ret = read(fd, buf, BUFSIZE)) != 0)
	{
		if (ret == -1)
			return (close(fd), (char *)printError("Failed to read file!"));
		j = 0;
		while (j < ret)
			file[i + j] = buf[j];
		i += ret;
	}
	file[i] = '\0';
	close(fd);
	return (file);
}

int
Core::compileShader(GLuint shader, char const *filename)
{
	GLint			logsize;
	GLint			state;
	char			*compileLog;

	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &state);
	if (state != GL_TRUE)
	{
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logsize);
		compileLog = new char[logsize + 1];
		memset(compileLog, '\0', logsize + 1);
		glGetShaderInfoLog(shader, logsize, &logsize, compileLog);
		std::cerr	<< "Failed to compile shader `"
					<< filename
					<< "`: " << std::endl
					<< compileLog;
		delete compileLog;
		return (0);
	}
	return (1);
}

GLuint
Core::loadShader(GLenum type, char const *filename)
{
	GLuint			shader;
	char			*source;

	shader = glCreateShader(type);
	if (shader == 0)
		return (printError("Failed to create shader !", 0));
	if (!(source = readFile(filename)))
		return (printError("Failed to read file !", 0));
	glShaderSource(shader, 1, (char const **)&source, 0);
	if (!compileShader(shader, filename))
		return (0);
	delete source;
	return (shader);
}

int
Core::init(void)
{
	this->windowWidth = 1920;
	this->windowHeight = 1080;
	if (!glfwInit())
		return (0);
	this->window = glfwCreateWindow(this->windowWidth, this->windowHeight,
									"42run", NULL, NULL);
	if (!this->window)
	{
		glfwTerminate();
		return (0);
	}
	this->loadTextures();
	this->getLocations();
	this->buildProjectionMatrix(this->projMatrix, 53.13f, 1.0f, 30.0f);
	this->cameraPos.set(5.0f, 0.0f, 0.0f);
	this->cameraLookAt.set(0.0f, 0.0f, 0.0f);
	this->setCamera(this->viewMatrix, this->cameraPos, this->cameraLookAt);
	glfwMakeContextCurrent(this->window); // make the opengl context of the window current on the main thread
	glfwSwapInterval(1); // VSYNC 60 fps max
	glfwSetKeyCallback(this->window, key_callback);
	return (1);
}

void
Core::update(void)
{

}

void
Core::render(void)
{

}

void
Core::loop(void)
{
	while (!glfwWindowShouldClose(this->window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		this->update();
		this->render();
		glfwSwapBuffers(this->window);
		glfwPollEvents();
	}
}

Core &
Core::operator=(Core const &rhs)
{
	if (this != &rhs)
	{
		// copy members here
	}
	return (*this);
}

std::ostream &
operator<<(std::ostream &o, Core const &i)
{
	o	<< "Core: " << &i;
	return (o);
}
