# Подробное описание Makefile для SFMLProjectTemplate

Этот файл объясняет каждую строчку и каждый флаг из Makefile вашего проекта.

---

## Переменные

```
CXX = g++
```
**CXX** — основной компилятор C++ (здесь используется g++).

```
TARGET = main
```
**TARGET** — имя итогового исполняемого файла (бинарника) для вашей игры/программы.

```
SRC = src/main.cpp src/player.cpp src/map.cpp src/camera.cpp src/monster.cpp src/eventSystem.cpp src/weapon.cpp
```
**SRC** — список исходных файлов, которые будут компилироваться для основного приложения.

```
SFML_INCLUDE = /home/niki/Progr/Libs/SFML-3.0.0/include
SFML_LIB = /home/niki/Progr/Libs/SFML-3.0.0/lib
```
**SFML_INCLUDE** — путь к папке, где лежат заголовочные файлы SFML.
**SFML_LIB** — путь к папке, где лежат скомпилированные библиотеки SFML.

```
CXXFLAGS = -g -Wall -fsanitize=address -I$(SFML_INCLUDE) -Iinclude
```
**CXXFLAGS** — флаги компилятора для всех исходников:
- `-g` — добавляет отладочную информацию (для работы с gdb и AddressSanitizer).
- `-Wall` — включает все основные предупреждения компилятора.
- `-fsanitize=address` — включает AddressSanitizer (выявляет ошибки работы с памятью).
- `-I$(SFML_INCLUDE)` — добавляет путь к SFML-инклудникам.
- `-Iinclude` — добавляет путь к вашей папке "include"(это папка с моими заголовочными файлами). Без этого флага папки должны лежать в корне проекта.

```
LDFLAGS = -L$(SFML_LIB) -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network -fsanitize=address
```
**LDFLAGS** — флаги для линковки:
- `-L$(SFML_LIB)` — путь к SFML-библиотекам.
- `-lsfml-graphics` — подключает графическую библиотеку SFML.
- `-lsfml-window` — подключает библиотеку окон SFML.
- `-lsfml-system` — подключает системную библиотеку SFML.
- `-lsfml-audio` — подключает аудио-библиотеку SFML.
- `-lsfml-network` — подключает сетевую библиотеку SFML.
- `-fsanitize=address` — включает AddressSanitizer на этапе линковки.

```
GTEST_LIBS = -lgtest -lgtest_main -pthread
```
**GTEST_LIBS** — флаги для линковки Google Test:
- `-lgtest` — основная библиотека Google Test.
- `-lgtest_main` — содержит функцию main для тестов.
- `-pthread` — поддержка многопоточности (требуется для gtest).

```
SRC_NO_MAIN = src/player.cpp src/map.cpp src/camera.cpp src/monster.cpp src/eventSystem.cpp src/weapon.cpp
```
**SRC_NO_MAIN** — исходники без main.cpp (используются для тестов, чтобы не было конфликта с main в gtest).

```
TEST_BIN = test
```
**TEST_BIN** — имя итогового исполняемого файла для тестов.

```
TEST_SRC = tests/test_sample.cpp tests/test_player.cpp
```
**TEST_SRC** — список файлов с тестами.

---

## Цели (rules)

```
all: $(TARGET)
```
**all** — цель по умолчанию, собирает основной бинарник.

```
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)
```
Сборка основного приложения:
- Компилирует все исходники из SRC.
- Применяет флаги компилятора и линковки.
- Выводит бинарник с именем main.

```
run: $(TARGET)
	LD_LIBRARY_PATH=$(SFML_LIB):$$LD_LIBRARY_PATH ./$(TARGET)
```
**run** — запускает собранное приложение, предварительно добавляя путь к SFML-библиотекам в переменную окружения LD_LIBRARY_PATH.

```
debug: $(TARGET)
	LD_LIBRARY_PATH=$(SFML_LIB):$$LD_LIBRARY_PATH gdb ./$(TARGET)
```
**debug** — запускает gdb для отладки приложения, с нужным LD_LIBRARY_PATH.

```
test: $(TEST_SRC) $(SRC_NO_MAIN)
	$(CXX) $(CXXFLAGS) -I./include $(TEST_SRC) $(SRC_NO_MAIN) -o $(TEST_BIN) $(GTEST_LIBS) $(LDFLAGS)
```
**test** — собирает тестовый бинарник:
- Компилирует все тесты и исходники без main.cpp.
- Линкует с Google Test и SFML.
- Выводит бинарник с именем test.

```
run_test: test
	LD_LIBRARY_PATH=$(SFML_LIB):$$LD_LIBRARY_PATH ./$(TEST_BIN)
```
**run_test** — запускает тесты, добавляя путь к SFML-библиотекам.

```
clean:
	rm -f $(TARGET) $(TEST_BIN)
```
**clean** — удаляет скомпилированные бинарники.

---

## Примечания
- Если добавляете новые исходные файлы или тесты, не забудьте обновить переменные SRC, SRC_NO_MAIN и TEST_SRC.
- Если путь к SFML или gtest отличается — обновите переменные SFML_INCLUDE, SFML_LIB, GTEST_LIBS.
- AddressSanitizer помогает находить ошибки с памятью, но может замедлять выполнение.




## Весь Makefile(с тестами и моими файлами(.cpp .h)):

CXX = g++
TARGET = main
SRC = src/main.cpp src/player.cpp src/map.cpp src/camera.cpp src/monster.cpp src/eventSystem.cpp src/weapon.cpp

SFML_INCLUDE = /home/niki/Progr/Libs/SFML-3.0.0/include
SFML_LIB = /home/niki/Progr/Libs/SFML-3.0.0/lib

CXXFLAGS = -g -Wall -fsanitize=address -I$(SFML_INCLUDE) -Iinclude
LDFLAGS = -L$(SFML_LIB) -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network -fsanitize=address

GTEST_LIBS = -lgtest -lgtest_main -pthread

SRC_NO_MAIN = src/player.cpp src/map.cpp src/camera.cpp src/monster.cpp src/eventSystem.cpp src/weapon.cpp

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