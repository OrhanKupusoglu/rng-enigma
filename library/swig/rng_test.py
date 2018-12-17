import rng_enigma

rng_version = rng_enigma.rng_get_version()
print("RNG ENIGMA - version: {}".format(rng_version))

rng_initialized = rng_enigma.rng_enigma_is_initialized()
print("\nRNG ENIGMA - initialized? {}".format(rng_initialized))

cfg_path = "../../application/bin/enigma-rotors.cfg"
rng_cfg = rng_enigma.rng_enigma_init(cfg_path, None)
print("RNG ENIGMA - read config: '{}'? {}".format(cfg_path, rng_cfg))

rng_initialized = rng_enigma.rng_enigma_is_initialized()
print("RNG ENIGMA - initialized? {}".format(rng_initialized))

rotors = rng_enigma.rng_get_rotors()
print("\nRNG ENIGMA - Rotors:")
print(rotors)

seed = rng_enigma.rng_get_seed()
print("\nRNG ENIGMA - seed: {}".format(seed))
print("RNG ENIGMA - seed: {:X}".format(seed))

test_size = 10

print("\nRNG ENIGMA - uint64 & double:")
for i in range(0, test_size):
    rng_uint64 = rng_enigma.rng_get_uint64()
    rng_dbl = rng_enigma.rng_get_dbl(rng_uint64)
    print("{0:>22} : {1:22.21f}".format(rng_uint64, rng_dbl))

test_size = test_size // 2

print("\nRNG ENIGMA - uint32 & float:")
for i in range(0, test_size):
    a = rng_enigma.new_uint32P()
    b = rng_enigma.new_uint32P()
    rng_enigma.rng_get_uint32(a, b)
    rng_uint32_a = rng_enigma.uint32P_value(a)
    rng_uint32_b = rng_enigma.uint32P_value(b)
    rng_flt_a = rng_enigma.rng_get_flt(rng_enigma.uint32P_value(a))
    rng_flt_b = rng_enigma.rng_get_flt(rng_enigma.uint32P_value(b))
    print("{0:>12} : {1:12.11f}".format(rng_uint32_a, rng_flt_a))
    print("{0:>12} : {1:12.11f}".format(rng_uint32_b, rng_flt_b))
