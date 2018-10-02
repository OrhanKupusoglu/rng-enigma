#ifndef _RNG_FILE_H
#define _RNG_FILE_H

#include <stdbool.h>
#include <stdio.h>

bool rng_read_random(uint64_t rotors[16][16]);

double rng_output_doubles_txt(const char* file_name, const long nums, bool rnd_sel);

void rng_output_dieharder_bin(const char* file_name, const long nums);

void rng_output_dieharder_txt(const char* file_name, const long nums);

#endif /* _RNG_FILE_H */
