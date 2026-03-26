# Huffman Archiver

Консольный архиватор файлов на основе алгоритма кодирования Хаффмана.

## Зависимости

Для сборки требуется:

- CMake >= 3.16
- C компилятор (gcc / clang)
- make (или ninja)

## Поддерживаемые платформы

Проект разрабатывался и тестировался на:

- macOS (Apple Clang)
- Linux (gcc / clang)

Должен собираться на любой POSIX-совместимой системе при наличии CMake и компилятора C.

Windows напрямую не тестировался, но может работать через WSL или MinGW.

## Сборка

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Использование

### Сжатие файла
```bash
./huff c input.txt archive.huff
```

### Распаковка файла
```bash
./huff d archive.huff output.txt
```

## Пример

```bash
cmake --build build
./build/huff c tests/data/text.txt archive.huff
./build/huff d archive.huff result.txt
```

## Алгоритм

Программа использует кодирование Хаффмана — алгоритм сжатия без потерь.

Основные этапы:
1. Подсчёт частоты каждого байта во входном файле
2. Построение дерева Хаффмана
3. Генерация префиксных кодов
4. Запись сжатого битового потока

При распаковке дерево восстанавливается из сохранённой таблицы частот.

## Формат архива

Файл архива имеет следующую структуру:
```
magic      (4 bytes)  "HUFF"
size       (8 bytes)  размер исходного файла
freq[256] (1024 B)   таблица частот байтов
data       (...)      сжатый битовый поток
```

Таблица частот используется для восстановления дерева Хаффмана при распаковке.

## Тесты

### Интеграционные тесты
```bash
cmake --build build
bash tests/test.sh
```

### Модульные тесты
```bash
cmake --build build
ctest
```

### Все тесты
```bash
cmake --build build
ctest
bash tests/test.sh
```

### Эксперименты
```bash
./experiments/run_all.sh
```

### Результаты экспериментов

- `experiments/benchmark/result.csv` — сырые данные.
- `experiments/plots/*.png` — графики.

## Структура проекта

```
src/
  main.c            CLI интерфейс
  huffman_tree.c    дерево Хаффмана
  huffman_codec.c   логика сжатия
  bitio.c           битовый ввод/вывод

include/
  *.h               публичный API

tests/
  data/             тестовые файлы
  unit/             модульные тесты
  test.sh           интеграционные тесты
  
experiments/
  benchmark/        бенчмарки
  plots/            графики         

build/              артефакты сборки
```

## Лицензия

MIT License
