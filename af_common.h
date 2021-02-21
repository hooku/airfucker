#ifndef __AF_COMMON_H_
#define __AF_COMMON_H_

/* Stdlib includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "ie80211/ie80211.h"
#include "psp/psp.h"

#define AF_NAME         "AF"

#define SUCCESS         0
#define FAILURE         -1

#define MAX_LOG_CHAR    256

#define SWAP_16(x)  (((x) >> 8) | ((x & 0x00FF) << 8))
#define SWAP_32(x)  (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))

extern void af_log(char* format, ...);

#endif // __AF_COMMON_H_
