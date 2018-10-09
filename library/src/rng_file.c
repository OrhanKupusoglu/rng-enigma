#include <stdlib.h>
#include <string.h>

#include "lib_rng_enigma.h"

#define RNG_FILE_RINGS          "enigma-rotors.cfg"

bool rng_read_file(const char* file_name, char** buffer, long* file_size) {
    bool    status = false;
    FILE*   fl;

    fl = fopen(file_name, "rb");

    if (fl && file_size) {
        // get file size
        if (*file_size == 0) {
            fseek(fl, 0, SEEK_END);
            *file_size = ftell(fl);
            fseek(fl, 0, SEEK_SET);
            (*file_size)++; // +1 for '\0'
        }

        if (*buffer == NULL) {
            *buffer = (char*)malloc(*file_size);
        }

        if (*buffer) {
            memset(*buffer, 0, *file_size);
            fread(*buffer, 1, (*file_size - 1), fl);
            status = true;
        } else {
            fprintf(stderr, "** RNG ENIGMA - ERROR - malloc failure\n");
        }

        fclose(fl);
    } else {
        fprintf(stderr, "** RNG ENIGMA - ERROR - unable to open the file '%s'\n", file_name);
    }

    return status;
}

// read config
bool rng_read_random(uint64_t rotors[16][16]) {
    bool    status = false;
    long    size   = 16 * 32;
    char    buffer[size];
    char*   p_buff = buffer;
    bool    debug = rng_is_debug();

    if (rng_read_file(RNG_FILE_RINGS, &p_buff, &size)) {
        for (int i = 0; i < 16; i++) {
            sscanf(p_buff + (i * 32), "%lX %lX %lX %lX %lX %lX %lX %lX %lX %lX %lX %lX %lX %lX %lX %lX\n",
                                      &rotors[i][0],  &rotors[i][1],  &rotors[i][2],  &rotors[i][3],
                                      &rotors[i][4],  &rotors[i][5],  &rotors[i][6],  &rotors[i][7],
                                      &rotors[i][8],  &rotors[i][9],  &rotors[i][10], &rotors[i][11],
                                      &rotors[i][12], &rotors[i][13], &rotors[i][14], &rotors[i][15]);
        }

        if (debug) {
            printf("\nRNG ENIGMA - Rotors:\n");
            for (int i = 0; i < 16; i++) {
                for (int j = 0; j < 16; j++) {
                    printf("%01lX ", rotors[i][j]);
                }
                printf("\n");
            }
            printf("\n");
        }

        status = true;
    } else {
        fprintf(stderr, "** RNG ENIGMA - ERROR - unable to read file '%s'\n", RNG_FILE_RINGS);
    }

    return status;
}

FILE* rng_save_bin_file(const char* file_name) {
    bool        status = false;
    FILE*       fl;

    fl = fopen(file_name, "wb");

    return fl;
}

void rng_write_file_bin(FILE* fl, uint64_t rnd[], size_t size) {
    fwrite(rnd, sizeof(uint64_t) * size, 1, fl);
}

void rng_write_file_line(FILE* fl, char* buffer) {
    fwrite(buffer, strlen(buffer), 1, fl);
}

void rng_write_file_txt(FILE* fl, char* buffer, size_t buffer_size) {
    fwrite(buffer, buffer_size, 1, fl);
}

void rng_close_file(FILE* fl) {
    fclose(fl);
}

double rng_output_doubles_txt(const char* file_name, const long nums, bool save) {
    bool    debug = rng_is_debug();
    double  total = 0.0;

    if (rng_enigma_is_initialized()) {
        FILE* fl;
        bool go = false;

        if (save) {
            fl = rng_save_bin_file(file_name);
            if (fl) {
                go = true;
            } else {
                fprintf(stderr, "** RNG ENIGMA - ERROR - unable to save the file '%s'\n", file_name);
            }
        } else {
            go = true;
        }

        if (go) {
            char        buffer[380];
            char*       p_buff = buffer;
            int         j;
            int         c;
            uint64_t    rnd;
            double      dbl;
            bool        fill = debug || save;

            memset(buffer, '\0', 380);

            for (long i = 0; i < nums; i++) {
                c = 0;
                for (j = 0; j < 10; j++) {
                    rnd = rng_get_uint64();
                    dbl = rng_get_dbl(rnd);
                    total += dbl;

                    if (fill) {
                        sprintf(p_buff + c, "%016lX\t%20.18F\n", rnd, dbl);
                        c += 38;
                    }
                }

                if (debug) {
                    printf("\n%s\n", buffer);
                }

                if (save) {
                    rng_write_file_txt(fl, buffer, 380);
                }
            }

            if (save) {
                rng_close_file(fl);
            }
        }
    } else {
        fprintf(stderr, "** RNG ENIGMA - ERROR - not initialized\n");
    }

    return (total / ((double)nums * 10));
}

void rng_output_dieharder_bin(const char* file_name, const long nums) {
    bool debug = rng_is_debug();

    if (rng_enigma_is_initialized()) {
        FILE* fl = rng_save_bin_file(file_name);
        if (fl) {
            int j = 0;
            uint64_t rnd[5];

            for (long i = 0; i < nums; i++) {
                for (j = 0; j < 5; j++) {
                    rnd[j] = rng_get_uint64();
                }

                if (debug) {
                    printf("%016lX%016lX%016lX%016lX%016lX",
                           rnd[0], rnd[1], rnd[2], rnd[3], rnd[4]);
                }

                rng_write_file_bin(fl, rnd, 5);
            }

            rng_close_file(fl);
        } else {
            fprintf(stderr, "** RNG ENIGMA - ERROR - unable to save the file '%s'\n", file_name);
        }
    } else {
        fprintf(stderr, "** RNG ENIGMA - ERROR - not initialized\n");
    }
}

void rng_output_dieharder_txt(const char* file_name, const long nums) {
    bool debug = rng_is_debug();

    if (rng_enigma_is_initialized()) {
        FILE* fl = rng_save_bin_file(file_name);
        if (fl) {
            char buffer[128];
            memset(buffer, '\0', 128);

            rng_write_file_line(fl, "#==================================================================\n");
            sprintf(buffer, "# generator enigma  seed = %lu\n", rng_get_seed());
            rng_write_file_line(fl, buffer);
            rng_write_file_line(fl, "#==================================================================\n");
            rng_write_file_line(fl, "type: d\n");
            sprintf(buffer, "count: %lu\n", (10 * nums));
            rng_write_file_line(fl, buffer);
            rng_write_file_line(fl, "numbit: 32\n");
            memset(buffer, '\0', 128);

            int j = 0;
            uint32_t rnd[10];

            for (long i = 0; i < nums; i++) {
                for (j = 0; j < 10; j += 2) {
                    rng_get_uint32(&rnd[j], &rnd[j + 1]);
                }

                sprintf(buffer, "%10u\n%10u\n%10u\n%10u\n%10u\n%10u\n%10u\n%10u\n%10u\n%10u\n",
                                rnd[0], rnd[1], rnd[2], rnd[3], rnd[4],
                                rnd[5], rnd[6], rnd[7], rnd[8], rnd[9]);

                if (debug) {
                    printf("\n%08X%08X%08X%08X%08X%08X%08X%08X%08X%08X\n",
                           rnd[0], rnd[1], rnd[2], rnd[3], rnd[4],
                           rnd[5], rnd[6], rnd[7], rnd[8], rnd[9]);
                }

                rng_write_file_txt(fl, buffer, 110);
            }

            rng_close_file(fl);
        } else {
            fprintf(stderr, "** RNG ERROR - unable to save the file '%s'\n", file_name);
        }
    } else {
        fprintf(stderr, "** RNG ENIGMA - ERROR - not initialized\n");
    }
}
