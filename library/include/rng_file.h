#ifndef _RNG_FILE_H
#define _RNG_FILE_H

#include <stdbool.h>
#include <stdio.h>


bool rng_read_random(const char* file_path, uint64_t* p_rotors);

double rng_output_doubles_txt(const char* file_path, const long nums, bool rnd_sel);

void rng_output_dieharder_bin(const char* file_path, const long nums);

void rng_output_dieharder_txt(const char* file_path, const long nums);

#endif /* _RNG_FILE_H */
