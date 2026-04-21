#ifndef HUFFMAN_CODEC_H
#define HUFFMAN_CODEC_H

#include <stdint.h>
#include <stdio.h>

/**
 * Записывает заголовок архива.
 *
 * Формат:
 * - "HUFF" (4 байта) - сигнатура файла
 * - original_size (8 байт) - размер исходного файла
 * - freq[256] (2048 байт) - таблица частот в little-endian uint64_t
 *
 * @param out файловый поток для записи
 * @param freq массив частот
 * @param original_size размер исходного файла в байтах
 */
int write_header(FILE* out, const uint64_t freq[256], uint64_t original_size);

/**
 * Читает заголовок архива.
 *
 * Проверяет сигнатуру файла и извлекает:
 * - размер исходного файла
 * - таблицу частот
 *
 * @param in файловый поток для чтения
 * @param freq массив для записи частот
 * @param original_size указатель для сохранения размера
 * @return 1 при успехе, 0 при ошибке формата
 */
int read_header(FILE* in, uint64_t freq[256], uint64_t* original_size);

/**
 * Сжимает файл с использованием кодирования Хаффмана.
 *
 * @param input путь к входному файлу
 * @param output путь к архиву
 * @return 0 при успехе, 1 при ошибке
 */
int compress_file(const char* input, const char* output);

/**
 * Распаковывает архив Хаффмана.
 *
 * @param input путь к архиву
 * @param output путь к выходному файлу
 * @return 0 при успехе, 1 при ошибке
 */
int decompress_file(const char* input, const char* output);

#endif  // HUFFMAN_CODEC_H
