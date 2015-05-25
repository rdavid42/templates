
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
Core::buildProjectionMatrix(Mat4<float> &proj, float const &fov,
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
	proj.setIdentity();
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
	std::cerr << proj << std::endl;
}

void
Core::setViewMatrix(Mat4<float> &view, Vec3<float> const &dir,
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
Core::setCamera(Mat4<float> &view, Vec3<float> const &pos, Vec3<float> const &lookAt)
{
	Vec3<float>		dir;
	Vec3<float>		right;
	Vec3<float>		up;
	Mat4<float>		translation;

	up.set(0.0f, 1.0f, 0.0f);
	dir.set(lookAt - pos);
	dir.normalize();
	right.crossProduct(dir, up);
	right.normalize();
	up.crossProduct(right, dir);
	up.normalize();
	this->setViewMatrix(view, dir, right, up);
	translation.setTranslation(-pos);
	view.multiply(translation);
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
Core::loadShaders(void)
{
	if (!(this->vertexShader = this->loadShader(GL_VERTEX_SHADER, "./shaders/vertex_shader.gls")))
		return (printError("Failed to load vertex shader !", 0));
	if (!(this->fragmentShader = this->loadShader(GL_FRAGMENT_SHADER, "./shaders/fragment_shader.gls")))
		return (printError("Failed to load fragment shader !", 0));
	return (1);
}

void
Core::attachShaders(void)
{
	glAttachShader(this->program, this->vertexShader);
	glAttachShader(this->program, this->fragmentShader);
}

int
Core::linkProgram(void)
{
	GLint			logSize;
	GLint			state;
	char			*linkLog;

	glLinkProgram(this->program);
	glGetProgramiv(this->program, GL_LINK_STATUS, &state);
	if (state != GL_TRUE)
	{
		glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &logSize);
		linkLog = new char[logSize + 1];
		memset(linkLog, '\0', logSize + 1);
		glGetProgramInfoLog(this->program, logSize, &logSize, linkLog);
		std::cerr	<< "Failed to link program !" << std::endl
					<< linkLog;
		delete [] linkLog;
		return (0);
	}
	return (1);
}

void
Core::deleteShaders(void)
{
	glDeleteShader(this->vertexShader);
	glDeleteShader(this->fragmentShader);
}

int
Core::initShaders(void)
{
	if (!loadShaders())
		return (0);
	if (!(this->program = glCreateProgram()))
		return (printError("Failed to create program !", 0));
	this->attachShaders();
	glBindFragDataLocation(this->program, 0, "out_fragment");
	if (!this->linkProgram())
		return (0);
	this->deleteShaders();
	return (1);
}

void
Core::loadTextures(void)
{

}

void
Core::getLocations(void)
{
	this->positionLoc = glGetAttribLocation(this->program, "position");
	this->colorLoc = glGetAttribLocation(this->program, "vert_color");
	this->projLoc = glGetUniformLocation(this->program, "proj_matrix");
	this->viewLoc = glGetUniformLocation(this->program, "view_matrix");
}

void
Core::createAxes(void)
{
	static GLfloat		axesVertices[36] =
	{
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &axesVao);
	glBindVertexArray(axesVao);
	glGenBuffers(1, &axesVbo);
	// bind vertices/colors vbo
	glBindBuffer(GL_ARRAY_BUFFER, axesVbo);
	// create vertices/colors
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 36, axesVertices, GL_STATIC_DRAW);

	// create pointer to vertices
	glEnableVertexAttribArray(positionLoc);
	glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void *)0);

	// create pointer to colors
	glEnableVertexAttribArray(colorLoc);
	glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void *)(sizeof(GLfloat) * 3));

	// check for errors
	checkGlError(__FILE__, __LINE__);
}

int
Core::init(void)
{
	this->windowWidth = 1920;
	this->windowHeight = 1080;
	if (!glfwInit())
		return (0);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	this->window = glfwCreateWindow(this->windowWidth, this->windowHeight,
									"gl4", NULL, NULL);
	if (!this->window)
	{
		glfwTerminate();
		return (0);
	}
	glfwMakeContextCurrent(this->window); // make the opengl context of the window current on the main thread
	glfwSwapInterval(1); // VSYNC 60 fps max
	glfwSetKeyCallback(this->window, key_callback);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	if (!this->initShaders())
		return (0);
	this->loadTextures();
	this->getLocations();
	this->buildProjectionMatrix(this->projMatrix, 53.13f, 1.0f, 30.0f);
	this->cameraPos.set(5.0f, 5.0f, 5.0f);
	this->cameraLookAt.set(0.0f, 0.0f, 0.0f);
	this->setCamera(this->viewMatrix, this->cameraPos, this->cameraLookAt);
	this->createAxes();
	// this->projMatrix.rotate(Vec3<float>(0, 0, 0), 45);
	return (1);
}

void
Core::renderAxes(void)
{
	glBindVertexArray(axesVao);
	glBindBuffer(GL_ARRAY_BUFFER, axesVbo);
	glDrawArrays(GL_LINES, 0, 6);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void
Core::update(void)
{
	this->setCamera(this->viewMatrix, this->cameraPos, this->cameraLookAt);
}

void
Core::render(void)
{
	glUseProgram(this->program);
	glUniformMatrix4fv(this->projLoc, 1, GL_FALSE, this->projMatrix.val);
	glUniformMatrix4fv(this->viewLoc, 1, GL_FALSE, this->viewMatrix.val);
	this->renderAxes();
	checkGlError(__FILE__, __LINE__);
}

void
Core::loop(void)
{
	while (!glfwWindowShouldClose(this->window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
