#include <stdlib.h>
#include <string.h>

#include "lib_rng_enigma.h"


bool rng_read_file(const char* file_path, char** buffer, int* file_size) {
    bool    status = false;
    FILE*   fl;

    fl = fopen(file_path, "rb");

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
            size_t read_size = fread(*buffer, 1, (*file_size - 1), fl);
            status = (read_size == *file_size);
        } else {
            fprintf(stderr, "** RNG ENIGMA - ERROR - malloc failure\n");
        }

        fclose(fl);
    } else {
        fprintf(stderr, "** RNG ENIGMA - ERROR - unable to open the file '%s'\n", file_path);
    }

    return status;
}

bool rng_read_random(const char* file_path, uint64_t* p_rotors) {
    bool        status = false;
    bool        read   = true;
    int         size   = 16 * 32;
    char        buffer[size];
    char*       p_buff = buffer;
    bool        debug = rng_is_debug();

    if (p_rotors) {
        if (file_path == NULL) {
            p_buff = RNG_ROTORS_DEFAULT;
        } else {
            read = rng_read_file(file_path, &p_buff, &size);
        }

        if (read) {
            for (int i = 0; i < 16; i++) {
                sscanf(p_buff + (i * 32), "%lX %lX %lX %lX %lX %lX %lX %lX %lX %lX %lX %lX %lX %lX %lX %lX\n",
                                          p_rotors + (i * 16) + 0,   p_rotors + (i * 16) + 1,   p_rotors + (i * 16) + 2,   p_rotors + (i * 16) + 3,
                                          p_rotors + (i * 16) + 4,   p_rotors + (i * 16) + 5,   p_rotors + (i * 16) + 6,   p_rotors + (i * 16) + 7,
                                          p_rotors + (i * 16) + 8,   p_rotors + (i * 16) + 9,   p_rotors + (i * 16) + 10,  p_rotors + (i * 16) + 11,
                                          p_rotors + (i * 16) + 12,  p_rotors + (i * 16) + 13,  p_rotors + (i * 16) + 14,  p_rotors + (i * 16) + 15);
            }

            status = true;
        } else {
            fprintf(stderr, "** RNG ENIGMA - ERROR - unable to read file '%s'\n", file_path);
        }
    }

    return status;
}

FILE* rng_save_bin_file(const char* file_path) {
    bool        status = false;
    FILE*       fl;

    fl = fopen(file_path, "wb");

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

double rng_output_doubles_txt(const char* file_path, const long nums, bool save) {
    bool    debug = rng_is_debug();
    double  total = 0.0;

    if (rng_enigma_is_initialized()) {
        FILE* fl;
        bool go = false;

        if (save) {
            fl = rng_save_bin_file(file_path);
            if (fl) {
                go = true;
            } else {
                fprintf(stderr, "** RNG ENIGMA - ERROR - unable to save the file '%s'\n", file_path);
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

void rng_output_dieharder_bin(const char* file_path, const long nums) {
    bool debug = rng_is_debug();

    if (rng_enigma_is_initialized()) {
        FILE* fl = rng_save_bin_file(file_path);
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
            fprintf(stderr, "** RNG ENIGMA - ERROR - unable to save the file '%s'\n", file_path);
        }
    } else {
        fprintf(stderr, "** RNG ENIGMA - ERROR - not initialized\n");
    }
}

void rng_output_dieharder_txt(const char* file_path, const long nums) {
    bool debug = rng_is_debug();

    if (rng_enigma_is_initialized()) {
        FILE* fl = rng_save_bin_file(file_path);
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
            fprintf(stderr, "** RNG ERROR - unable to save the file '%s'\n", file_path);
        }
    } else {
        fprintf(stderr, "** RNG ENIGMA - ERROR - not initialized\n");
    }
}
