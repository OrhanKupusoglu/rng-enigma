#include <locale.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lib_rng_enigma.h"

#define RNG_LOCALE              "en_US.UTF-8"
#define RNG_FILE_DBL            "enigma.txt"
#define RNG_FILE_BIN            "dieharder.bin"
#define RNG_FILE_TXT            "dieharder.txt"
#define RNG_SEP                 "--------------------------------------------------------------------------------"
#define RNG_INVALID             "invalid op selection"
#define RNG_HELP                "RNG ENIGMA - v%s\n" \
                                "Usage:\n" \
                                "arg#1 - enter the op selection\n" \
                                "\t1 - generate doubles between [0.0 - 1.0] and calculate the avg\n" \
                                "\t2 - generate doubles between [0.0 - 1.0] and save the data: \"" RNG_FILE_DBL "\"\n" \
                                "\t3 - generate binary file for Dieharder: \"" RNG_FILE_BIN "\"\n" \
                                "\t4 - generate text file for Dieharder: \"" RNG_FILE_TXT "\"\n" \
                                "arg#2 - enter the number of pseudo random numbers\n"

int main(int argc, char** argv) {
    int       rnd_sel = 0;
    uint64_t  rnd_count = 0;

    if (argc >= 3) {
        rnd_sel = atoi(argv[1]);
        if (rnd_sel > 0) {
            rnd_count = atoll(argv[2]);
            if (rnd_count > 0) {
                struct timespec t_start;
                struct timespec t_end;
                double avg;
                double t_duration;

                setlocale(LC_NUMERIC, RNG_LOCALE);

                rng_set_debug(RNG_DEBUG);

                // if seed is NULL
                // then current time in nanoseconds will be the seed
                // else the string (f.e. "15594288E20BFBF1") is presumed to be a base 16 number
                rng_enigma_init(RNG_SEED);

                if (rnd_count < 10) {
                    rnd_count = 10;
                }
                rnd_count /= 10;

                printf("RNG ENIGMA - seed: %016lX\n", rng_get_seed());
                printf("RNG ENIGMA - generate %'ld numbers\n", (rnd_count * 10));

                clock_gettime(CLOCK_REALTIME, &t_start);

                switch (rnd_sel) {
                    case 1:
                    case 2:
                        (rnd_sel == 2) ? printf("RNG ENIGMA - saving text file of doubles between [0.0 - 1.0]\n")
                                       : printf("RNG ENIGMA - generating doubles between [0.0 - 1.0]\n");
                        avg = rng_output_doubles_txt(RNG_FILE_DBL, rnd_count, (rnd_sel == 2));
                        printf("%s\nRNG ENIGMA - avg:\t%20.16f\n", RNG_SEP, avg);
                        break;

                    case 3:
                        printf("RNG ENIGMA - saving binary file for Dieharder\n");
                        rng_output_dieharder_bin(RNG_FILE_BIN, rnd_count);
                        break;

                    case 4:
                        printf("RNG ENIGMA - saving text file for Dieharder\n");
                        rng_output_dieharder_txt(RNG_FILE_TXT, rnd_count);
                        break;

                    default:
                        printf("** RNG ENIGMA - %s\n", RNG_INVALID);
                }

                clock_gettime(CLOCK_REALTIME, &t_end);

                t_duration = (t_end.tv_sec - t_start.tv_sec) +
                             (t_end.tv_nsec - t_start.tv_nsec) / RNG_BILLION_D;

                printf("%s\nRNG ENIGMA - time:\t%12.8f seconds\n",
                       RNG_SEP, t_duration);
            } else {
                printf("** RNG ENIGMA - missing number count\n");
            }
        } else {
            printf("** RNG ENIGMA - %s\n", RNG_INVALID);
        }
    } else {
        printf(RNG_HELP, get_enigma_version());
    }

    return EXIT_SUCCESS;
}
