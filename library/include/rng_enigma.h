#ifndef _RNG_ENIGMA_H
#define _RNG_ENIGMA_H

#include <stdbool.h>
#include <stdint.h>

#define RNG_BILLION_L           1000000000L
#define RNG_BILLION_D           1000000000.0

char* rng_get_version();

void rng_set_debug(bool curr_debug);

bool rng_is_debug();

bool rng_enigma_init(char* seed);

bool rng_enigma_is_initialized();

uint64_t rng_get_seed();

uint64_t rng_get_uint64();

void rng_get_uint32(uint32_t* rnd1, uint32_t* rnd2);

float rng_get_flt(uint32_t u);

double rng_get_dbl(uint64_t u);

#endif /* _RNG_ENIGMA_H */
