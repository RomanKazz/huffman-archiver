#ifndef BITIO_H
#define BITIO_H

#include <stdio.h>

/**
 * Непрозрачные структуры
 */
typedef struct BitWriter BitWriter;
typedef struct BitReader BitReader;

/**
 * Создаёт writer
 */
BitWriter* bw_create(FILE* file);

/**
 * Записывает один бит
 */
void bw_write_bit(BitWriter* bw, int bit);

/**
 * Записывает несколько бит
 */
void bw_write_bits(BitWriter* bw, unsigned int code, int length);

/**
 * Сбрасывает буфер
 */
void bw_flush(BitWriter* bw);

/**
 * Освобождает writer
 */
void bw_free(BitWriter* bw);

/**
 * Создаёт reader
 */
BitReader* br_create(FILE* file);

/**
 * Читает один бит
 */
int br_read_bit(BitReader* br);

/**
 * Освобождает reader
 */
void br_free(BitReader* br);

#endif
