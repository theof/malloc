HOSTTYPE ?= $(shell uname -m)_$(shell uname -s)

NAME   = libft_malloc.so
CC     ?= clang

SUBNAME = libft_malloc_$(HOSTTYPE).so

CFLAGS = -g -Wall -Werror -Wextra
CFLAGS += -std=c11 -pedantic -pedantic-errors
ifeq ($(DEBUG),yes)
	CFLAGS += -g3 -O0 -fno-inline
else
	CFLAGS += -O2 -DNDEBUG
endif

# Headers
CFLAGS     += -I./include

# Sources
SRC_PATH   += src
SOURCES    += malloc.c
SOURCES    += calloc.c
SOURCES	   += realloc.c
SOURCES    += free.c
SOURCES    += show_alloc_mem.c
SOURCES    += zone.c
SOURCES    += allocs.c


# Generation
vpath %.c $(SRC_PATH) $(addprefix $(SRC_PATH)/,$(SRC_SUBDIR))
OBJ_PATH   = .obj
OBJECTS    = $(SOURCES:%.c=$(OBJ_PATH)/%.o)
DEP_PATH   = .dep
DEPS       = $(SOURCES:%.c=$(DEP_PATH)/%.d)
BUILD_DIR  = $(OBJ_PATH) $(DEP_PATH)

# Lib tierces
LIBFT_PATH = libft
LIBFT      = $(LIBFT_PATH)/libft.a
CFLAGS     += -I$(LIBFT_PATH)/include
LDFLAGS    += -L $(LIBFT_PATH) -lft
EST_EXEC  = $(TEST_PATH)/test_$(NAME).out

# Core rules
.SECONDARY: $(OBJECTS)

all: $(DEPS) $(NAME)

-include $(DEPS)

$(NAME): $(SUBNAME)
	ln -fs $^ $@

$(SUBNAME): $(OBJECTS) | $(LIBFT)
	$(CC) -shared -o $@ $^ $(LDFLAGS)

$(LIBFT):
	$(MAKE) -C $(LIBFT_PATH)

$(OBJECTS): $(OBJ_PATH)/%.o: %.c | $(OBJ_PATH)
	$(CC) $(CFLAGS) -o $@ -c $<

$(DEP_PATH)/%.d: %.c | $(DEP_PATH)
	$(CC) $(CFLAGS) -MM $< -MT $(OBJ_PATH)/$*.o -MF $@

$(BUILD_DIR):
	@-mkdir -p $@

check: $(NAME)
	@./run_tests.sh

clean:
	$(RM) -rf $(OBJ_PATH)
	$(MAKE) -C $(LIBFT_PATH) clean

fclean: clean
	$(RM) -rf $(NAME) $(SUBNAME)
	$(RM) -rf $(DEP_PATH) $(OBJ_PATH)
	$(MAKE) -C $(LIBFT_PATH) fclean

re: fclean all

.PHONY: all $(LIBFT) clean fclean re check
