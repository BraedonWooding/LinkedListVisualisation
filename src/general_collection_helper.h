#ifndef LLV_GENERAL_COLLECTION_HELPER
#define LLV_GENERAL_COLLECTION_HELPER

#include "types/collection_skeleton.h"
#include "types/shared_types.h"

#define MAX_PRECISION_FLOAT_PRINT 8

size_t sizeof_uint(unsigned long long int n);

size_t sizeof_int(long long int n);

void write_str_center_incr(char **buf, size_t *offset, size_t len,
                           char *str, size_t str_len);

size_t sizeof_data(Data data, TypeTag tag);

#endif