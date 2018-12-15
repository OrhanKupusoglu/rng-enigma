%module rng_enigma

%{
#include <stdint.h>
#include "../include/rng_enigma.h"
#include "../include/rng_file.h"
%}

%include <stdint.i>

%include "../include/rng_enigma.h"
%include "../include/rng_file.h"

//%include "cpointer.i"
//%pointer_functions(uint64_t, p)
