// SPDX-License-Identifier: MIT
// Copyright (c) 2026 RomanKazz

#ifndef BITIO_H
#define BITIO_H

#include <stdio.h>

/**
 * Непрозрачные структуры
 */
typedef struct BitWriter BitWriter;
typedef struct BitReader BitReader;

/**
 * Создаёт writer.
 */
BitWriter* bit_writer_create(FILE* file);

/**
 * Записывает один бит
 *
 * @return 1 при успехе, 0 при ошибке записи
 */
int bit_writer_write_bit(BitWriter* writer, int bit);

/**
 * Записывает несколько бит
 *
 * @return 1 при успехе, 0 при ошибке записи
 */
int bit_writer_write_bits(BitWriter* writer, unsigned int code, int length);

/**
 * Сбрасывает буфер
 *
 * @return 1 при успехе, 0 при ошибке записи
 */
int bit_writer_flush(BitWriter* writer);

/**
 * Освобождает writer
 */
void bit_writer_free(BitWriter* writer);

/**
 * Создаёт reader.
 */
BitReader* bit_reader_create(FILE* file);

/**
 * Читает один бит
 */
int bit_reader_read_bit(BitReader* reader);

/**
 * Освобождает reader
 */
void bit_reader_free(BitReader* reader);

#endif
