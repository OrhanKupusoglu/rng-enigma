#include <byteswap.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#include "lib_rng_enigma.h"

// Rotors
static uint64_t                 rotors[16][16];
// Ringstellungen
static char                     rs_a[16];
static char                     rs_b[16];
static uint64_t                 seed;
static bool                     debug = false;

// version is set by CMake
char* rng_get_version() {
    return RNG_ENIGMA_LIB_VERSION;
}

// initialization
void rng_set_debug(bool curr_debug) {
    debug = curr_debug;
}

bool rng_is_debug() {
    return debug;
}

bool rng_init_rotors() {
    return rng_read_random(rotors);
}

void rng_stellung_reverse(uint64_t x, char bytes[16]) {
    bytes[0]  = (x & 0x000000000000000Full) >>  0;
    bytes[1]  = (x & 0x00000000000000FFull) >>  4;
    bytes[2]  = (x & 0x0000000000000FFFull) >>  8;
    bytes[3]  = (x & 0x000000000000FFFFull) >> 12;
    bytes[4]  = (x & 0x00000000000FFFFFull) >> 16;
    bytes[5]  = (x & 0x0000000000FFFFFFull) >> 20;
    bytes[6]  = (x & 0x000000000FFFFFFFull) >> 24;
    bytes[7]  = (x & 0x00000000FFFFFFFFull) >> 28;
    bytes[8]  = (x & 0x0000000FFFFFFFFFull) >> 32;
    bytes[9]  = (x & 0x000000FFFFFFFFFFull) >> 36;
    bytes[10] = (x & 0x00000FFFFFFFFFFFull) >> 40;
    bytes[11] = (x & 0x0000FFFFFFFFFFFFull) >> 44;
    bytes[12] = (x & 0x000FFFFFFFFFFFFFull) >> 48;
    bytes[13] = (x & 0x00FFFFFFFFFFFFFFull) >> 52;
    bytes[14] = (x & 0x0FFFFFFFFFFFFFFFull) >> 56;
    bytes[15] = x >> 60;
}

void rng_stellung_forward(uint64_t x, char bytes[16]) {
    bytes[15] = (x & 0x000000000000000Full) >>  0;
    bytes[14] = (x & 0x00000000000000FFull) >>  4;
    bytes[13] = (x & 0x0000000000000FFFull) >>  8;
    bytes[12] = (x & 0x000000000000FFFFull) >> 12;
    bytes[11] = (x & 0x00000000000FFFFFull) >> 16;
    bytes[10] = (x & 0x0000000000FFFFFFull) >> 20;
    bytes[9]  = (x & 0x000000000FFFFFFFull) >> 24;
    bytes[8]  = (x & 0x00000000FFFFFFFFull) >> 28;
    bytes[7]  = (x & 0x0000000FFFFFFFFFull) >> 32;
    bytes[6]  = (x & 0x000000FFFFFFFFFFull) >> 36;
    bytes[5]  = (x & 0x00000FFFFFFFFFFFull) >> 40;
    bytes[4]  = (x & 0x0000FFFFFFFFFFFFull) >> 44;
    bytes[3]  = (x & 0x000FFFFFFFFFFFFFull) >> 48;
    bytes[2]  = (x & 0x00FFFFFFFFFFFFFFull) >> 52;
    bytes[1]  = (x & 0x0FFFFFFFFFFFFFFFull) >> 56;
    bytes[0]  = x >> 60;
}

void rng_display_ringstellungen() {
    printf("RNG ENIGMA - Ringstellung A: ");
    for (int i = 0; i < 16; i++) {
        printf("%01X ", rs_a[i]);
    }

    printf("\n             Ringstellung B: ");
    for (int i = 0; i < 16; i++) {
        printf("%01X ", rs_b[i]);
    }
    printf("\n");
}

void rng_init_ringstellungen(char* current_seed) {
    if (current_seed) {
        seed = strtoull(current_seed, NULL, 16);
    } else {
        long int ns;
        time_t sec;
        struct timespec spec;

        clock_gettime(CLOCK_REALTIME, &spec);
        sec = spec.tv_sec;
        ns = spec.tv_nsec;

        seed = (uint64_t)sec * RNG_BILLION_L + (uint64_t)ns;
    }

    rng_stellung_reverse(seed, rs_a);
    rng_stellung_forward(seed, rs_b);

    if (debug) {
        printf("RNG ENIGMA - seed: %020lu => hex: %016lX => bswap: %016lX\n",
               seed, seed, __bswap_64(seed));
        rng_display_ringstellungen();
    }
}

bool rng_enigma_init(char* curr_seed) {
    bool status = false;

    if (rng_init_rotors()) {
        rng_init_ringstellungen(curr_seed);
        status = true;

        if (debug) {
            printf("RNG ENIGMA - initialization success\n");
        }
    } else {
        fprintf(stderr, "** RNG ENIGMA - ERROR: Rotor initialization failure\n");
    }

    return status;
}

bool rng_enigma_is_initialized() {
    return seed > 0;
}

uint64_t rng_get_seed() {
    return seed;
}

// https://stackoverflow.com/questions/1024389/print-an-int-in-binary-representation-using-c
char* int2bin(uint64_t u) {
    size_t bits = sizeof(uint64_t) * CHAR_BIT;
    char* str = malloc(bits + 1);

    if (!str) {
        return NULL;
    }

    str[bits] = 0;

    for(; bits--; u >>= 1) {
        str[bits] = u & 1 ? '1' : '0';
    }

    return str;
}

// generate numbers
uint64_t rng_get_uint64() {
    uint64_t pre;
    uint64_t aux;
    uint64_t num;

    pre = (rotors[rs_a[15]][rs_b[0 ]] << 60)
        | (rotors[rs_a[14]][rs_b[1 ]] << 56)
        | (rotors[rs_a[13]][rs_b[2 ]] << 52)
        | (rotors[rs_a[12]][rs_b[3 ]] << 48)
        | (rotors[rs_a[11]][rs_b[4 ]] << 44)
        | (rotors[rs_a[10]][rs_b[5 ]] << 40)
        | (rotors[rs_a[9 ]][rs_b[6 ]] << 36)
        | (rotors[rs_a[8 ]][rs_b[7 ]] << 32)
        | (rotors[rs_a[7 ]][rs_b[8 ]] << 28)
        | (rotors[rs_a[6 ]][rs_b[9 ]] << 24)
        | (rotors[rs_a[5 ]][rs_b[10]] << 20)
        | (rotors[rs_a[4 ]][rs_b[11]] << 16)
        | (rotors[rs_a[3 ]][rs_b[12]] << 12)
        | (rotors[rs_a[2 ]][rs_b[13]] <<  8)
        | (rotors[rs_a[1 ]][rs_b[14]] <<  4)
        | (rotors[rs_a[0 ]][rs_b[15]]);

    aux = (rotors[rs_b[14]][rs_a[2 ]] << 60)
        | (rotors[rs_b[13]][rs_a[3 ]] << 56)
        | (rotors[rs_b[12]][rs_a[4 ]] << 52)
        | (rotors[rs_b[11]][rs_a[5 ]] << 48)
        | (rotors[rs_b[10]][rs_a[6 ]] << 44)
        | (rotors[rs_b[9 ]][rs_a[7 ]] << 40)
        | (rotors[rs_b[8 ]][rs_a[8 ]] << 36)
        | (rotors[rs_b[7 ]][rs_a[9 ]] << 32)
        | (rotors[rs_b[6 ]][rs_a[10]] << 28)
        | (rotors[rs_b[5 ]][rs_a[11]] << 24)
        | (rotors[rs_b[4 ]][rs_a[12]] << 20)
        | (rotors[rs_b[3 ]][rs_a[13]] << 16)
        | (rotors[rs_b[2 ]][rs_a[14]] << 12)
        | (rotors[rs_b[1 ]][rs_a[15]] <<  8)
        | (rotors[rs_b[0 ]][rs_a[0 ]] <<  4)
        | (rotors[rs_b[15]][rs_a[1 ]]);

    num = pre ^ aux;

    rng_stellung_forward(num, rs_a);
    rng_stellung_reverse(aux, rs_b);

    if (debug) {
        printf("\nRNG ENIGMA - pre = %020lu : %016lX : %s\n             aux = %020lu : %016lX : %s\n             rnd = %020lu : %016lX : %s\n",
               pre, pre, int2bin(pre),
               aux, aux, int2bin(aux),
               num, num, int2bin(num));
        rng_display_ringstellungen();
    }

    return num;
}

void rng_get_uint32(uint32_t* rnd1, uint32_t* rnd2) {
    uint64_t rnd64 = rng_get_uint64();
    *rnd1 = (uint32_t)(rnd64 >> 32);
    *rnd2 = (uint32_t)rnd64;
}

// "Dieharder" by Robert G. Brown
// Copyright Robert G. Brown, 2018
// http://webhome.phy.duke.edu/~rgb/General/dieharder.php
// http://www.cs.ucl.ac.uk/staff/d.jones/GoodPracticeRNG.pdf
// PDF, p.7

float rng_get_flt(uint32_t u) {
    float x;

    u = (u >> 9) | 0x3F800000;
    *((uint32_t *)&x) = u;
    x -= 1.0F;

    return x;
}

double rng_get_dbl(uint64_t u) {
    double x;

    u = (u >> 12) | 0x3FF0000000000000ULL;
    *((uint64_t*)&x) = u;
    x -= 1.0;

    return x;
}
