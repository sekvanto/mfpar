#ifndef DATA_H
#define DATA_H

/* For c++ compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "common.h"

typedef enum {
    ALG_HUFFMAN,
    ALG_LZ4,
    ALG_PAQ9,
    ALG_ADAPTIVE_HUFFMAN,
    ALG_UNRECOGNIZED
} AlgorithmType;

typedef struct {
    AlgorithmType val;
    const char    *str;
} str_conversion;

void dataError(const char* message);
AlgorithmType str_to_algorithm_type (const char *str);

typedef struct {
    char* fileIn;
    char* fileOut;
    bool isArchiving;
    AlgorithmType algorithmType;

    double efficiency; /* File compression/decompression ratio (in percents, less is better) */
    double time;       /* How much time operation took (in seconds) */
    long fileInSize;   /* Size of input file (in bytes) */
    long fileOutSize;  /* Size of output file (in bytes) */
} Data;

void initData(Data* data);

#ifdef __cplusplus
};
#endif

#endif
