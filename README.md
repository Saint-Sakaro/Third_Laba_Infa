# Лабораторная работа №3

Этот проект реализует абстрактные типы данных на C++ в рамках лабораторной работы №3.

## Требования

- Компилятор C++ (C++17 или выше): `g++`, `clang++`, или MSVC.
- CMake (версия 3.10 или выше).
- Google Test для запуска тестов.
- ОС: Windows, Linux, или macOS.

## Установка зависимостей

### Ubuntu/Debian
```bash
sudo apt update
sudo apt install -y build-essential cmake libgtest-dev
cd /usr/src/gtest
sudo cmake .
sudo make
sudo cp lib/*.a /usr/lib
```

macOS (с Homebrew)
```bash
brew install cmake googletest
```

Windows

- Установите CMake.
- Установите Google Test:

Сборка проекта

- Склонируйте репозиторий:
```bash
git clone https://github.com/Saint-Sakaro/Third_Laba_Infa.git
cd Third_Laba_Infa
```

- Создайте директорию для сборки:
```bash
mkdir build
cd build
```

- Сконфигурируйте проект с помощью CMake:
```bash
cmake ..
```

- Соберите проект:
```bash
cmake --build .
```

- Запустите тесты:
```bash
./tests
```

## Если у вас возникли вопросы, свяжитесь со мной: [fedor1belov@gmail.com].
