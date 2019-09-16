CXX = g++
SRC_DIR = ./src/
OBJETOS_DIR = ./obj/
BUILD_DIR = ./build/
SRCS = main.cpp
BIN_NOME = cc
LIBFLAGS = -lSDL2 -std=c++0x -pthread -lSDL2_ttf -w



OBJS = $(SRCS:.cpp=.o)
OBJS_FINAL = $(OBJS:%.o=$(OBJETOS_DIR)%.o)
BIN = $(BUILD_DIR)$(BIN_NOME)

all : $(BIN)

$(BIN) : $(OBJS_FINAL)
	$(CXX) $(OBJS_FINAL) -o $(BIN) $(LIBFLAGS)

$(OBJS_FINAL) : $(OBJETOS_DIR)%.o : $(SRC_DIR)%.cpp
	$(CXX) -c $< -o $@ $(LIBFLAGS)

clean :
	rm  $(BIN) $(OBJS_FINAL)
