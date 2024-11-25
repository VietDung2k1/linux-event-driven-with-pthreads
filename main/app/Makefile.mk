CXXFLAGS	+= -I./main/app

VPATH += main/app

OBJ += $(OBJ_DIR)/app.o
OBJ += $(OBJ_DIR)/task_list.o
OBJ += $(OBJ_DIR)/task_shell.o
OBJ += $(OBJ_DIR)/task_demo.o
OBJ += $(OBJ_DIR)/shell.o