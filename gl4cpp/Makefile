
SRC_PATH	=	src/

OBJ_PATH	=	obj/
INC_PATH	=	inc/

SRCS		=	$(shell ls $(SRC_PATH) | grep .cpp$$)
OBJS		=	$(patsubst %.cpp, $(OBJ_PATH)%.o,$(SRCS))

PLATFORM	:=	$(shell uname)
CC			=	g++
HEADER		=	-I./$(INC_PATH) -I./glfw/include
FLAGS		=	-Ofast -g -Wall -Wextra -Werror -std=gnu++11
VARS		=	-DDEBUG

GLFW_ADD	=	glfw

GLFW		=	./glfw/src/libglfw3.a

ifeq "$(PLATFORM)" "Darwin" #MAC
LIBS		=	$(GLFW) -lm -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
else ifeq "$(PLATFORM)" "Linux" #LINUX
LIBS		=	$(GLFW) -lGL -lXrandr -lXi -lXrender -ldrm -lXdamage -lXxf86vm -lXext -lX11 -lpthread -lXcursor -lm -lXinerama `libpng-config --libs`
else ifeq "$(PLATFORM)" "Win32" #WINDOWS
LIBS		=	$(GLFW) -lopengl32 -lgdi32 -luser32 -lkernel32
endif

NAME		=	gl4cpp

all: $(NAME)

$(NAME): $(GLFW) $(OBJS)
	@$(CC) $(FLAGS) $(VARS) $(HEADER) -o $(NAME) $(OBJS) $(LIBS)

$(GLFW): $(GLFW_ADD)
	git submodule init && git submodule update
	cmake glfw/CMakeLists.txt
	make -C glfw/

$(GLFW_ADD):
	git submodule add https://github.com/glfw/glfw.git

$(patsubst %, $(OBJ_PATH)%,%.o): $(SRC_PATH)$(notdir %.cpp)
	@mkdir -p $(OBJ_PATH)
	@$(CC) -c $(FLAGS) $(VARS) $(HEADER) "$<" -o "$@"

clean_glfw:
	@make -C glfw/ clean

clean:
	@rm -rf $(OBJ_PATH)

fclean: clean
	@rm -f $(NAME)

re: fclean all

l: re
	@./$(NAME)

launch: l

.PHONY: clean fclean re
