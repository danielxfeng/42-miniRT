NAME	:= miniRT
LIBFT_DIR	:= libft
LIBFT_LIB	:= $(LIBFT_DIR)/libft.a
CFLAGS	:= -Wunreachable-code -Ofast -g
LIBMLX	:= ./MLX42
LIB_URL := https://github.com/codam-coding-college/MLX42
HEADERS	:= -I ./include -I $(LIBMLX)/include -I $(LIBFT_DIR)
LIBS	:= $(LIBMLX)/build/libmlx42.a
THREAD	:= src/threads/camera_thread.c src/threads/thread_utils.c src/threads/thread_hook.c \
		src/threads/thread_hook_supp.c
CAMERA	:= src/camera/camera.c src/ui/hook.c \
		src/ui/hook_supp.c
OBJ_DIR	:= obj
SRCS	:= src/main.c src/comm_utils.c src/parser/parse.c src/parser/arena.c src/parser/utils.c \
			src/parser/assign_geo_objects.c src/parser/assign_material.c src/parser/parse_utils.c \
			src/parser/create_objects.c src/parser/assign_geo_objects2.c src/parser/error.c \
			src/interval.c src/material.c \
			src/vec3/vec_methods.c src/vec3/vec_methods2.c src/vec3/vec_methods3.c src/vec3/vec_methods4.c \
			src/vec3/vec_methods5.c src/vec3/vec_methods6.c  \
			src/objects/quadrics_supp.c src/objects/objects.c src/objects/sphere.c src/objects/plane.c \
			src/objects/cylinder.c src/objects/cone.c src/objects/box.c \
			src/texture.c \
			src/camera/camera_supp.c  src/camera/camera_supp2.c\
		  	gnl/get_next_line.c gnl/get_next_line_utils.c

MANDATORY := $(SRCS) $(CAMERA)
BONUS := $(SRCS) $(THREAD)
OBJS	:= $(patsubst %.c, $(OBJ_DIR)/%.o, $(MANDATORY))
BONUS_OBJS	:= $(patsubst %.c, $(OBJ_DIR)/%.o, $(BONUS))
LIBFT	:= -L$(LIBFT_DIR) -lft

all: libmlx libft $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	@mkdir -p \
	$(OBJ_DIR)/src/vec3 \
	$(OBJ_DIR)/gnl $(OBJ_DIR)/src \
	$(OBJ_DIR)/src/objects \
	$(OBJ_DIR)/src/parser \
	$(OBJ_DIR)/src/camera \
	$(OBJ_DIR)/src/threads \
	$(OBJ_DIR)/src/ui

libft:
	@echo "building libft"
	@make -C $(LIBFT_DIR) 

libmlx: $(LIBS)

$(LIBS): $(LIBMLX)
	@cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

$(LIBMLX):
	@git clone $(LIB_URL) $(LIBMLX)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@ $(HEADERS) && printf "Compiling: $(notdir $<)\n"

$(NAME): $(OBJS) $(LIBS)
	@$(CC) $(OBJS) $(LIBS) $(LIBFT) $(HEADERS) -ldl -lglfw -pthread -lm -o $(NAME)
	@echo "\n\033[1;34m                        ######  ####### \033[0m"
	@echo "\033[1;34m  ##   ## # #    # #    #     #    #    \033[0m"
	@echo "\033[1;34m  # # # # # ##   # #    #     #    #    \033[0m"
	@echo "\033[1;34m  #  #  # # # #  # #    ######     #    \033[0m"
	@echo "\033[1;34m  #     # # #  # # #    #   #      #    \033[0m"
	@echo "\033[1;34m  #     # # #   ## #    #    #     #    \033[0m"
	@echo "\033[1;34m  #     # # #    # #    #     #    #    \033[0m"
	@echo "\n\033[1;32mWelcome to Mini RT!\033[0m"
	@echo "Usage: ./miniRT <scene.rt>"
	@echo "Example: ./miniRT scenes/example.rt"

bonus: .bonus

.bonus: $(BONUS_OBJS) $(LIBS)

	@$(CC) $(BONUS_OBJS) $(LIBS) $(LIBFT) $(HEADERS) -ldl -lglfw -pthread -lm -o $(NAME)
	@echo "\n\033[1;34m                        ######  ####### \033[0m"
	@echo "\033[1;34m  ##   ## # #    # #    #     #    #    \033[0m"
	@echo "\033[1;34m  # # # # # ##   # #    #     #    #    \033[0m"
	@echo "\033[1;34m  #  #  # # # #  # #    ######     #    \033[0m"
	@echo "\033[1;34m  #     # # #  # # #    #   #      #    \033[0m"
	@echo "\033[1;34m  #     # # #   ## #    #    #     #    \033[0m"
	@echo "\033[1;34m  #     # # #    # #    #     #    #    \033[0m"
	@echo "\n\033[1;32mWelcome to Mini RT!\033[0m"
	@echo "Usage: ./miniRT <scene.rt>"
	@echo "Example: ./miniRT scenes/example.rt"

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(LIBMLX)/build
	@make -C $(LIBFT_DIR) clean

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(LIBMLX)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re libmlx libft



