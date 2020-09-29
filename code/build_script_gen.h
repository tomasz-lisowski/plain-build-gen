#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "parse.h"

#ifndef BUILD_SCRIPT_GEN_H
#define BUILD_SCRIPT_GEN_H

string_t gen_build_script(config_t *cfg_parsed);

#endif