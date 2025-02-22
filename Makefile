CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++20 -Iinclude
LDFLAGS = -lcurl

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
EXEC = ziom.out

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJ) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
  
