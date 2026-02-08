# Инструкция по запуску Google Test для SFMLProjectTemplate

## Шаг 1. Установить зависимости
(sudo apt-get update)
- **CMake** (для сборки gtest):
  - Ubuntu: `sudo apt install cmake`
- **Google Test**:
  - Ubuntu: `sudo apt install libgtest-dev`
  - Затем собрать:
    ```bash
    cd /usr/src/gtest
    sudo cmake .
    sudo make
    sudo cp /usr/src/googletest/googletest/lib/*.a /usr/lib
    ```
    (не забудь потом вернуться в папку проекта)
- **SFML** (уже должен быть установлен, путь прописан в Makefile)

## Шаг 2. Структура проекта
- Папка `tests/` содержит тесты (файлы `test_*.cpp`).
- Тесты используют Google Test и исходники проекта.

## Шаг 3. Сборка и запуск тестов
- Собрать тесты: `make test`
- Запустить тесты: `make run_test`

## Шаг 4. Добавление новых тестов
- Добавляйте новые файлы в папку `tests/`.
- Пропишите их в переменной `TEST_SRC` в Makefile.

---

# Описание тестов
В каждом тесте есть подробные комментарии, объясняющие их работу. Используйте их как примеры для написания своих тестов.

