###############################################################################
# author: VietDung
# date: 25/11/2024
###############################################################################
NAME_MODULE	= ptheard_os
CXX		= g++
CC		= gcc
OBJ_DIR		= build_$(NAME_MODULE)

OPTIMIZE_OPTION	+=	-g -O3
WARNNING_OPTION	+=	-Werror -W -Wno-missing-field-initializers

include main/pthread-os/Makefile.mk
include main/sys/Makefile.mk
include main/app/Makefile.mk
include main/common/Makefile.mk

CXXFLAGS	+= -I/usr/local/include
CXXFLAGS	+= -I/usr/include

# CXX compiler option
CXXFLAGS	+=	$(OPTIMIZE_OPTION)	\
			$(WARNNING_OPTION)	\
			-std=c++11		\
			-Wall			\
			-Winline		\
			-pipe			\
			-g			\
#			-DRELEASE	\

# Library paths
LDFLAGS += -L/usr/local/lib
LDFLAGS += -L/usr/include
LDFLAGS += -Wl,-Map=$(OBJ_DIR)/$(NAME_MODULE).map

#Library libs
LDLIBS +=	-lpthread		\
		-lrt			\
		-lm			\

all: create $(OBJ_DIR)/$(NAME_MODULE)

create:
	@echo mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.cpp
	@echo CXX $<
	@$(CXX) -c -o $@ $< $(CXXFLAGS) $(LDFLAGS) $(LDLIBS)

$(OBJ_DIR)/%.o: %.c
	@echo CXX $<
	@$(CC) -c -o $@ $< $(CXXFLAGS) $(LDFLAGS) $(LDLIBS)

$(OBJ_DIR)/$(NAME_MODULE): $(OBJ)
	@echo ---------- START LINK PROJECT ----------
	@echo $(CXX) -o $@ $^ $(CXXFLAGS)
	@$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS) $(LDLIBS)

.PHONY: run
run:
	@sudo LD_LIBRARY_PATH=/usr/local/lib/ $(OBJ_DIR)/$(NAME_MODULE)

.PHONY: clean
clean:
	@echo rm -rf $(OBJ_DIR)
	@rm -rf $(OBJ_DIR)