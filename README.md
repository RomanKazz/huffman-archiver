# Huffman Archiver

[![CI](https://github.com/RomanKazz/huffman-archiver/actions/workflows/ci.yml/badge.svg?branch=main&event=push)](https://github.com/RomanKazz/huffman-archiver/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Консольный архиватор файлов на основе алгоритма Хаффмана.

## Что умеет

- Сжимает и распаковывает текстовые и бинарные файлы.
- Работает с пустыми файлами и файлами больше 4 ГБ.
- Хранит в архиве таблицу частот и восстанавливает дерево при распаковке.
- Содержит модульные, интеграционные и стресс-тесты.

## Зависимости

Для сборки и запуска нужны:

- CMake >= 3.16
- C-компилятор с поддержкой C11 (`gcc` или `clang`)
- `make` или `ninja`
- `bash`

Для экспериментов дополнительно нужны:

- `python3`
- `bc`

Проект разрабатывался и тестировался на `macOS` и `Linux`.

## Сборка

```bash
cmake -S . -B build
cmake --build build
```

Санитайзеры включены по умолчанию. При необходимости их можно отключить:

```bash
cmake -S . -B build -DENABLE_SANITIZERS=OFF
cmake --build build
```

## Использование

Сжатие:

```bash
./build/huff c input.txt archive.huff
```

Распаковка:

```bash
./build/huff d archive.huff output.txt
```

Пример:

```bash
cmake -S . -B build
cmake --build build
./build/huff c tests/data/text.txt archive.huff
./build/huff d archive.huff result.txt
cmp -s tests/data/text.txt result.txt
```

## Формат архива

Архив имеет следующий формат:

```text
magic      (4 bytes)     "HUFF"
size       (8 bytes)     размер исходного файла, little-endian uint64_t
freq[256]  (2048 bytes)  таблица частот, little-endian uint64_t
data       (...)         сжатый битовый поток
```

Таблица частот используется для восстановления дерева Хаффмана при распаковке.

## Тесты

Модульные тесты:

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Интеграционные тесты:

```bash
cmake -S . -B build
cmake --build build
bash tests/test.sh
```

Все проверки:

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
bash tests/test.sh
```

## Эксперименты

```bash
./experiments/run_all.sh
```

Результаты:

- `experiments/benchmark/result.csv` — сырые замеры.
- `experiments/plots/*.png` — графики.

## Структура проекта

```text
src/         исходный код архиватора
include/     публичные заголовочные файлы
tests/       модульные и интеграционные тесты
experiments/ скрипты для замеров и анализа
```

## Как помочь проекту

- Сообщить о найденной ошибке или предложить улучшение.
- Добавить тест на новый крайний случай.
- Прислать pull request с описанием изменения и результатами проверок.

## Лицензия

Проект распространяется под лицензией MIT.
