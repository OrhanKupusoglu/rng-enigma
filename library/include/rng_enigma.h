#ifndef _RNG_ENIGMA_H
#define _RNG_ENIGMA_H

#include <stdbool.h>
#include <stdint.h>


#define RNG_BILLION_L           1000000000L
#define RNG_BILLION_D           1000000000.0
#define RNG_ROTORS_DEFAULT      "1 9 4 8 C F 2 E 6 D A 3 B 0 5 7 " \
                                "8 C 0 F B A E 1 2 4 3 D 5 6 7 9 " \
                                "9 1 F 2 0 E 3 5 7 A 4 6 8 D C B " \
                                "1 A 0 7 9 8 5 F 2 3 B 4 D C 6 E " \
                                "8 0 C 7 E 2 1 D A F 3 9 6 4 B 5 " \
                                "3 F B A 1 8 2 E 7 6 5 4 9 D 0 C " \
                                "1 3 5 A 0 F 2 8 4 6 D C 7 B E 9 " \
                                "0 F A D C B E 1 8 7 2 5 6 4 9 3 " \
                                "B 0 C 9 5 7 6 1 F 2 D 3 E 8 4 A " \
                                "F E D 0 1 2 C B A 9 3 4 5 6 7 8 " \
                                "A 1 0 F 8 9 7 3 2 C 6 B 5 D E 4 " \
                                "1 2 F 0 A 8 C 9 7 E 3 D B 5 4 6 " \
                                "C A 0 1 9 E D 4 5 B 2 F 8 6 3 7 " \
                                "2 F 1 0 8 7 5 6 C B 3 4 A D E 9 " \
                                "4 3 9 0 F 2 A 7 B 6 C 5 E 8 D 1 " \
                                "3 E F D 5 7 6 A 4 9 B C 2 0 1 8"


char* rng_get_version();

void rng_set_debug(bool curr_debug);

bool rng_is_debug();

bool rng_enigma_init(const char* file_path, const char* seed);

char* rng_get_rotors();

bool rng_enigma_is_initialized();

uint64_t rng_get_seed();

uint64_t rng_get_uint64();

void rng_get_uint32(uint32_t* rnd1, uint32_t* rnd2);

float rng_get_flt(uint32_t u);

double rng_get_dbl(uint64_t u);

#endif /* _RNG_ENIGMA_H */
