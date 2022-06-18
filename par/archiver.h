#ifndef ARCHIVER_H
#define ARCHIVER_H

#include "common.h"
#include "data.h"

/* For c++ compatibility */
#ifdef __cplusplus
extern "C" {
#endif

int archive(Data* data);
int unarchive(Data* data);
int archiveError(const char* message, ...);

#ifdef __cplusplus
}
#endif

#endif
