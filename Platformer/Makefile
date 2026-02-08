CXX = g++
TARGET = main
SRC = src/main.cpp src/player.cpp src/map.cpp src/camera.cpp src/monster.cpp src/eventSystem.cpp src/weapon.cpp src/animation.cpp

SFML_INCLUDE = /home/niki/Progr/Libs/SFML-3.0.0/include
SFML_LIB = /home/niki/Progr/Libs/SFML-3.0.0/lib

CXXFLAGS = -g -Wall -fsanitize=address -I$(SFML_INCLUDE) -Iinclude
LDFLAGS = -L$(SFML_LIB) -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network -lyaml-cpp -fsanitize=address

GTEST_LIBS = -lgtest -lgtest_main -pthread

SRC_NO_MAIN = src/player.cpp src/map.cpp src/camera.cpp src/monster.cpp src/eventSystem.cpp src/weapon.cpp src/animation.cpp

TEST_BIN = test
TEST_SRC = tests/test_sample.cpp tests/test_player.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	LD_LIBRARY_PATH=$(SFML_LIB):$$LD_LIBRARY_PATH ./$(TARGET)

debug: $(TARGET)
	LD_LIBRARY_PATH=$(SFML_LIB):$$LD_LIBRARY_PATH gdb ./$(TARGET)

test: $(TEST_SRC) $(SRC_NO_MAIN)
	$(CXX) $(CXXFLAGS) -I./include $(TEST_SRC) $(SRC_NO_MAIN) -o $(TEST_BIN) $(GTEST_LIBS) $(LDFLAGS)

run_test: test
	LD_LIBRARY_PATH=$(SFML_LIB):$$LD_LIBRARY_PATH ./$(TEST_BIN)

clean:
	rm -f $(TARGET) $(TEST_BIN)