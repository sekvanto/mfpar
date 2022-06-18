#ifndef ADAPTIVE_HUFFMAN_H
#define ADAPTIVE_HUFFMAN_H

#include <stdio.h>

#include "../../common.h"
#include "../../data.h"

/* For c++ compatibility */
#ifdef __cplusplus
extern "C" {
#endif

int adaptive_huffman_archive(Data* data);
int adaptive_huffman_unarchive(Data* data);

#ifdef __cplusplus
}
#endif

#endif
