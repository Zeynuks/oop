# Объектно-ориентированное программирование

- [Лекции](https://github.com/alexey-malov/oop/blob/master/lectures/README.md)
- [Задания лабораторных работ](https://github.com/alexey-malov/oop/blob/master/tasks/README.md)

## Лабораторные работы

- [Лабораторная работа №1](lab-1/README.md)
- [Лабораторная работа №2](lab-2/README.md)
- [Лабораторная работа №3](lab-3/README.md)
- [Лабораторная работа №4](lab-4/README.md)
- [Лабораторная работа №5](lab-5/README.md)
- [Лабораторная работа №6](lab-6/README.md)
- [Лабораторная работа №7](lab-7/README.md)

## Подготовка к работе

### Установка зависимостей в extern
Все сторонние библиотеки размещаются в папке extern в корне проекта:
```bash
mkdir -p extern
cd extern
git clone https://github.com/google/googletest.git
git clone https://github.com/google/benchmark.git
```

#### Установка SFML
Установка на систему (Ubuntu / Debian)
```bash
sudo apt update
sudo apt install libsfml-dev
```
