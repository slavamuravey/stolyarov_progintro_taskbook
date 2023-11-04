#ifndef UTILS_H_SENTRY
#define UTILS_H_SENTRY

#include <string.h>
#include <stdbool.h>
#include "dynamic_array.h"

bool match_cmd(const char *str, const char *cmd);
int extract_cmd(struct dynamic_array *buf, char *cmd);

#endif