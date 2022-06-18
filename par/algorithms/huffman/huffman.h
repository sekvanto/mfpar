#ifndef HUFFMAN_H
#define HUFFMAN_H

/* For c++ compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "../../common.h"
#include "../../data.h"

int huffman_archive(Data* data);
int huffman_unarchive(Data* data);

#ifdef __cplusplus
}
#endif

#endif
