#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "archiver.h"
#include "algorithms/huffman/huffman.h"
#include "algorithms/adaptive_huffman/adaptive_huffman.h"

typedef int (*ArchiveFn)(Data* data);

typedef struct {
    ArchiveFn archiveFunction;
    ArchiveFn unarchiveFunction;
} Operations;

int archiveError(const char* message, ...) {
    va_list args;
    va_start (args, message);

    printf("Archive error: ");
    vprintf (message, args);
    printf(".\n");

    va_end (args);
    return 1;
}

static long file_size(char filename[]) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        archiveError("can't open file: %s", filename);
        return FAILURE;
    }
    fseek(fp, 0L, SEEK_END);
    // calculating the size of the file
    long int res = ftell(fp);
    fclose(fp);
    return res;
}

static int init(Data* data) {
    data->fileInSize = file_size(data->fileIn);
    if (data->fileInSize == FAILURE) {
        return FAILURE;
    }

    fileIn = fopen(data->fileIn, "rb");
    if (fileIn == NULL) {
        archiveError("can't open file: %s", data->fileIn);
        return FAILURE;
    }
    fileOut = fopen(data->fileOut, "wb");
    if (fileOut == NULL) {
        archiveError("output error: %s", data->fileOut);
        return FAILURE;
    }
    return 0;
}

static void post(Data* data) {
    fclose(fileIn);
    fclose(fileOut);
    data->fileOutSize = file_size(data->fileOut);
    data->efficiency = ((double) data->fileOutSize / data->fileInSize) * 100;
}

Operations operations[] = {
    [ALG_HUFFMAN]          = {huffman_archive,          huffman_unarchive},
    [ALG_ADAPTIVE_HUFFMAN] = {adaptive_huffman_archive, adaptive_huffman_unarchive},
};

int archive(Data* data) {
    if (init(data) != 0) {
        return FAILURE;
    }

    printf("Compressing the file: %s\n\n", data->fileIn);
    printf("Saving to file: %s\n\n", data->fileOut);

    int success = operations[data->algorithmType].archiveFunction(data);
    
    post(data);
    return success;
}

int unarchive(Data* data) {
    if (init(data) != 0) {
        return FAILURE;
    }

    printf("Decompressing the file: %s\n\n", data->fileIn);
    printf("Saving to file: %s\n\n", data->fileOut);

    int success = operations[data->algorithmType].unarchiveFunction(data);

    post(data);
    return success;
}
