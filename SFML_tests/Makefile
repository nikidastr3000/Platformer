CXX = g++
TARGET = main
SRC = main.cpp     	# source files(.cpp). if in folder, use relative path - src/main.cpp

SFML_INCLUDE = /home/niki/Progr/Libs/SFML-3.0.0/include
SFML_LIB = /home/niki/Progr/Libs/SFML-3.0.0/lib

# compiler flags
#-fsanitize=address: enable address sanitizer(show errors with memory issues)
#-Iinclude: allows includes(.h) from folder with name "include"
CXXFLAGS = -g -Wall -fsanitize=address -I$(SFML_INCLUDE) -Iinclude
LDFLAGS = -L$(SFML_LIB) -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network -lyaml-cpp -fsanitize=address

#Для работы с тестами нужно установить cmake and google test - читай README.md
# Google Test
GTEST_LIBS = -lgtest -lgtest_main -pthread

# Tests
TEST_BIN = test
TEST_SRC = tests/test_sample.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	LD_LIBRARY_PATH=$(SFML_LIB):$$LD_LIBRARY_PATH ./$(TARGET)

debug: $(TARGET)
	LD_LIBRARY_PATH=$(SFML_LIB):$$LD_LIBRARY_PATH gdb ./$(TARGET)

test: $(TEST_SRC)
	$(CXX) $(CXXFLAGS) -I./include $(TEST_SRC) -o $(TEST_BIN) $(GTEST_LIBS) $(LDFLAGS)

run_test: test
	LD_LIBRARY_PATH=$(SFML_LIB):$$LD_LIBRARY_PATH ./$(TEST_BIN)

clean:
	rm -f $(TARGET) $(TEST_BIN)
