#include "data.h"

str_conversion conversion [] = {
    {ALG_HUFFMAN,          "huffman"},
    {ALG_ADAPTIVE_HUFFMAN, "adaptive-huffman"},
};

void dataError(const char* message) {
    printf("Error: %s.\n", message);
    exit(FAILURE);
}

AlgorithmType str_to_algorithm_type (const char *str) {
    for (size_t j = 0; j < sizeof (conversion) / sizeof (conversion[0]);  ++j)
        if (!strcmp (str, conversion[j].str))
            return conversion[j].val;
    dataError("incorrect algorithm type");
    AlgorithmType null = {0};
    return null;
}

void initData(Data* data) {
    data->fileIn = "";
    data->fileOut = "";
    data->isArchiving = false;
    data->algorithmType = ALG_HUFFMAN;

    data->efficiency = 0;
    data->time = 0;
    data->fileInSize = 0;
    data->fileOutSize = 0;
}
