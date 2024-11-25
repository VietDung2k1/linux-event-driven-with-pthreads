CXXFLAGS	+= -I./main/pthread-os

VPATH += main/pthread-os

OBJ += $(OBJ_DIR)/pos.o
OBJ += $(OBJ_DIR)/timer.o