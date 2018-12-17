# rng-enigma<br>Pseudorandom Number Generator

In the spring term of 1994 [Prof. Dr. Engin Arık](https://en.wikipedia.org/wiki/Engin_Ar%C4%B1k), **RIP**,  of [Physics Department](http://www.phys.boun.edu.tr/) at [Boğaziçi University](http://www.boun.edu.tr/en_US) gave the lecture **482.03 - Computational Physics**, which I happily attented. Homework #6 was about calculating the mass and the center of mass coordinates of a torus using the [Monte-Carlo Integration](https://en.wikipedia.org/wiki/Monte_Carlo_integration) method. For this task I  had to develop a simple [Pseudorandom Number Generator](https://en.wikipedia.org/wiki/Pseudorandom_number_generator). The current code is the much improved C version of the previous *WATFOR-77* FORTRAN code.

&nbsp;

<p align="center">
<img  src= "./image/engin_arik.gif" alt="Prof. Dr. Engin Arık at her lab" >
<br>Prof. Dr. Engin Arık [1948-2007]
</p>

&nbsp;

## Design

The main inspiration for the current algorithm has been the [Enigma](https://en.wikipedia.org/wiki/Enigma_machine) **electro-mechanical rotor cipher machines**. Attempts to crack the Enigma machine code during World War II have been the subject of books and films, and therefore  **Enigma** is clearly the most well-known [cipher machine](http://www.cryptomuseum.com/crypto/enigma/index.htm) of [its era](http://users.telenet.be/d.rijmenants/en/enigmamenu.htm).

Let's start with a [Gedankenexperiment](https://www.britannica.com/science/Gedankenexperiment). What if two operators using modified Enigma machines start a *conversation*, which will be never decrypted at all? Can this conversation be **self-sustaining**?

The modified Enigma machines have 16 rotors, instead of three or four, and rotors contain hexadecimal digits instead of the 26 letters of the alphabet. Therefore each keystroke generates a hexadecimal digit. Since decryption is not a requirement, at each keystroke rotors are changed freely.

Rotors are modeled as a two dimensional array. The first dimension corresponds to the order of the rotors, and the second dimension corresponds to the typed input. With this setup of **rotor & input**, coding simplifies considerably. For historical reasons, these indices are called **Ringstellungen**.

Assuming that each operator knows his counterpart's state at each step, they can think of an algorithm:

- Their Enigma machines have a set of Identical rotors displaying only hexadecimal digits, but they start with different Ringstellungen, derived from the **seed**. Operator **A** gets his Ringstellungen using the reverse order of the seed and operator **B** uses the seed as given.
- **A** uses his Ringstellungen to **order** his rotors, and  **B**'s  Ringstellungen to **type** to generate a hexadecimal number.
- **B** uses a similar method to generate an auxiliary number.
- The output from **A** is then **XOR**'ed with the auxiliary output from **B** to generate a number. This output is accepted as the target **pseudorandom number**.
- In the next run, **A** will generate his **Ringstellungen** using this pseudorandom number in the given order, and **B** will use his auxiliary number in the reverse order.
- This procedure continues at will.

The path to the configuration file can be given, for an example see [enigma-rotors.cfg](./application/bin/enigma-rotors.cfg), or by selecting **NULL** the embedded [default configuration](./library/include/rng_enigma.h) will be used. The **seed** can explicitly be given as an hexadecimal string, like **"15594288E20BFBF1"**, or  by selecting **NULL** the current time in nanoseconds will be used as the seed. The configuration file for rotors and the seed can be set by the application's [CMakeListst.txt](.//application/CMakeLists.txt) build file.

```
add_definitions(-DRNG_CFG="enigma-rotors.cfg")
add_definitions(-DRNG_SEED="15594288E20BFBF1")
```

Without the [XOR](https://en.wikipedia.org/wiki/Exclusive_or) operation Dieharder was consistently failing at some tests, like *rgb_bitdist-4* and *dab_monobit2*. Apparently the XOR operation improves bit distributions. Using current time as the seed, this code still fails sometimes at *dab_filltree2*, but most of the time no **FAILED** results are observed.

The outputs are sensitive to the initial configuration of  the rotors. Besides each dimension uses a constant index for an ever-changing array of hexadecimal digits, which is determined at design-time. These  indices must be selected after many tests.

To better understand the algorithm, please refer to the source code [rng_enigma.c](./library/src/rng_enigma.c) .

&nbsp;

## Build

This project consists of a library to read proc data and a sample application to use this library. Both are built by [CMake](https://cmake.org/). For convenience a [build script](./build.sh) is provided:

```
$ sudo apt install cmake

$ ./build.sh help
USAGE: enter a command, no command defaults to 'build'
    build         -- call 'lib cmake; lib make; app cmake; app make'
    clean         -- call 'lib clean; app clean'
    purge         -- call 'lib purge; app purge'
    expunge       -- call 'clean; purge'
    run           -- run the test executable with ARG_OP and ARG_NUM
    lib cmake     -- call 'cmake'
    lib make      -- call 'make; make install'
    lib clean     -- call 'make clean'
    lib purge     -- purge all artifacts
    app cmake     -- call 'cmake'
    app make      -- call 'make'
    app clean     -- call 'make clean'
    app purge     -- purge all artifacts
    help          -- print this help

$ ./build.sh
++ CMaking the library project ...
-- Compile_definitions: RNG_ENIGMA_LIB_VERSION="0.3.1"
-- The C compiler identification is GNU 5.4.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /home/orhanku/ME/DEV/OK/rng-enigma/library/build
++ CMaked the library project.
++ Making the library project ...
Scanning dependencies of target rng-enigma
[ 33%] Building C object CMakeFiles/rng-enigma.dir/src/rng_enigma.c.o
[ 66%] Building C object CMakeFiles/rng-enigma.dir/src/rng_file.c.o
[100%] Linking C static library ../lib/librng-enigma.a
[100%] Built target rng-enigma
[100%] Built target rng-enigma
Install the project...
-- Install configuration: "Release"
-- Installing: /home/orhanku/ME/DEV/OK/rng-enigma/library/lib/rng_enigma.h
-- Installing: /home/orhanku/ME/DEV/OK/rng-enigma/library/lib/rng_file.h
-- Installing: /home/orhanku/ME/DEV/OK/rng-enigma/library/lib/lib_rng_enigma.h
++ Maked the library project.
++ CMaking the application project ...
-- Compile_definitions: RNG_DEBUG=false;RNG_CFG=NULL;RNG_SEED=NULL
-- The C compiler identification is GNU 5.4.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Link library      : /home/orhanku/ME/DEV/OK/rng-enigma/library/lib/librng-enigma.a
-- Include directory : /home/orhanku/ME/DEV/OK/rng-enigma/library/lib
-- Configuring done
-- Generating done
-- Build files have been written to: /home/orhanku/ME/DEV/OK/rng-enigma/application/build
++ CMaked the application project.
++ Making the application project ...
Scanning dependencies of target rng-enigma-app
[ 50%] Building C object CMakeFiles/rng-enigma-app.dir/src/main.c.o
[100%] Linking C executable ../bin/rng-enigma-app
[100%] Built target rng-enigma-app
++ Maked the application project.

$ ./build.sh run 1 10
RNG ENIGMA - seed: 1559A250E9C2D138
RNG ENIGMA - generate 10 numbers
RNG ENIGMA - generating doubles between [0.0 - 1.0]
--------------------------------------------------------------------------------
RNG ENIGMA - avg:     0.3595859641903761
--------------------------------------------------------------------------------
RNG ENIGMA - time:    0.00001174 seconds

$ ./build.sh expunge
++ Cleaning the library project ...
++ Cleaned the library project.
++ Cleaning the application project ...
++ Cleaned the application project.
++ Purging the library project ...
++ Purged the library project.
++ Purging the application project ...
++ Purged the application project.
```

&nbsp;

## Debug

The library is packaged with an application which helps with testing it.

The library can be set into the **debug** mode by the application's [CMakeListst.txt](.//application/CMakeLists.txt) build file.

```
add_definitions(-DRNG_DEBUG=true)
```

After re-building the app, debug output can be investigated.

```
$ cd application/bin/

$ ./rng-enigma-app 1 10
RNG ENIGMA - seed: 01538439301595363203 => hex: 1559A264D0177783 => bswap: 837717D064A25915
RNG ENIGMA - Ringstellung A: 3 8 7 7 7 1 0 D 4 6 2 A 9 5 5 1
             Ringstellung B: 1 5 5 9 A 2 6 4 D 0 1 7 7 7 8 3
RNG ENIGMA - initialization success

RNG ENIGMA - Rotors:
1 9 4 8 C F 2 E 6 D A 3 B 0 5 7
8 C 0 F B A E 1 2 4 3 D 5 6 7 9
9 1 F 2 0 E 3 5 7 A 4 6 8 D C B
1 A 0 7 9 8 5 F 2 3 B 4 D C 6 E
8 0 C 7 E 2 1 D A F 3 9 6 4 B 5
3 F B A 1 8 2 E 7 6 5 4 9 D 0 C
1 3 5 A 0 F 2 8 4 6 D C 7 B E 9
0 F A D C B E 1 8 7 2 5 6 4 9 3
B 0 C 9 5 7 6 1 F 2 D 3 E 8 4 A
F E D 0 1 2 C B A 9 3 4 5 6 7 8
A 1 0 F 8 9 7 3 2 C 6 B 5 D E 4
1 2 F 0 A 8 C 9 7 E 3 D B 5 4 6
C A 0 1 9 E D 4 5 B 2 F 8 6 3 7
2 F 1 0 8 7 5 6 C B 3 4 A D E 9
4 3 9 0 F 2 A 7 B 6 C 5 E 8 D 1
3 E F D 5 7 6 A 4 9 B C 2 0 1 8

RNG ENIGMA - seed: 1559A264D0177783
RNG ENIGMA - generate 10 numbers
RNG ENIGMA - generating doubles between [0.0 - 1.0]

RNG ENIGMA - pre = 14450203126273217015 : C8896F2ED1C111F7 : 1100100010001001011011110010111011010001110000010001000111110111
             aux = 01233846115883978738 : 111F808154C28FF2 : 0001000100011111100000001000000101010100110000101000111111110010
             rnd = 15678982689866882565 : D996EFAF85039E05 : 1101100110010110111011111010111110000101000000111001111000000101
RNG ENIGMA - Ringstellung A: D 9 9 6 E F A F 8 5 0 3 9 E 0 5
             Ringstellung B: 2 F F 8 2 C 4 5 1 8 0 8 F 1 1 1

RNG ENIGMA - pre = 13193870255543000815 : B71A0B17E23B9EEF : 1011011100011010000010110001011111100010001110111001111011101111
             aux = 05627997775303292884 : 4E1AAA2881A437D4 : 0100111000011010101010100010100010000001101001000011011111010100
             rnd = 17942518209070475579 : F900A13F639FA93B : 1111100100000000101000010011111101100011100111111010100100111011
RNG ENIGMA - Ringstellung A: F 9 0 0 A 1 3 F 6 3 9 F A 9 3 B
             Ringstellung B: 4 D 7 3 4 A 1 8 8 2 A A A 1 E 4

RNG ENIGMA - pre = 12447759859992406389 : ACBF53A44036A975 : 1010110010111111010100111010010001000000001101101010100101110101
             aux = 05215726273006130271 : 4861FB694433D45F : 0100100001100001111110110110100101000100001100111101010001011111
             rnd = 16491804483966565674 : E4DEA8CD04057D2A : 1110010011011110101010001100110100000100000001010111110100101010
RNG ENIGMA - Ringstellung A: E 4 D E A 8 C D 0 4 0 5 7 D 2 A
             Ringstellung B: F 5 4 D 3 3 4 4 9 6 B F 1 6 8 4

RNG ENIGMA - pre = 05657832766624249263 : 4E84A8ECBD3435AF : 0100111010000100101010001110110010111101001101000011010110101111
             aux = 10247021754069140766 : 8E34BBFE88FDC51E : 1000111000110100101110111111111010001000111111011100010100011110
             rnd = 13884618620116005041 : C0B0131235C9F0B1 : 1100000010110000000100110001001000110101110010011111000010110001
RNG ENIGMA - Ringstellung A: C 0 B 0 1 3 1 2 3 5 C 9 F 0 B 1
             Ringstellung B: E 1 5 C D F 8 8 E F B B 4 3 E 8

RNG ENIGMA - pre = 08282796407389012053 : 72F26772C94DC055 : 0111001011110010011001110111001011001001010011011100000001010101
             aux = 05836716828102773995 : 51002F07E99C4CEB : 0101000100000000001011110000011111101001100111000100110011101011
             rnd = 02590212403590433982 : 23F2487520D18CBE : 0010001111110010010010000111010100100000110100011000110010111110
RNG ENIGMA - Ringstellung A: 2 3 F 2 4 8 7 5 2 0 D 1 8 C B E
             Ringstellung B: B E C 4 C 9 9 E 7 0 F 2 0 0 1 5

RNG ENIGMA - pre = 06090375075308802990 : 54855BDCE0AC93AE : 0101010010000101010110111101110011100000101011001001001110101110
             aux = 10720730558739447290 : 94C7AFA46E56FDFA : 1001010011000111101011111010010001101110010101101111110111111010
             rnd = 13853904202377096788 : C042F4788EFA6E54 : 1100000001000010111101000111100010001110111110100110111001010100
RNG ENIGMA - Ringstellung A: C 0 4 2 F 4 7 8 8 E F A 6 E 5 4
             Ringstellung B: A F D F 6 5 E 6 4 A F A 7 C 4 9

RNG ENIGMA - pre = 04362149476473460427 : 3C8977D6525B56CB : 0011110010001001011101111101011001010010010110110101011011001011
             aux = 16156842531754177887 : E038A2AE1521755F : 1110000000111000101000101010111000010101001000010111010101011111
             rnd = 15902726471793910676 : DCB1D578477A2394 : 1101110010110001110101010111100001000111011110100010001110010100
RNG ENIGMA - Ringstellung A: D C B 1 D 5 7 8 4 7 7 A 2 3 9 4
             Ringstellung B: F 5 5 7 1 2 5 1 E A 2 A 8 3 0 E

RNG ENIGMA - pre = 05946188227348209870 : 52851AB042B320CE : 0101001010000101000110101011000001000010101100110010000011001110
             aux = 04217993724875858190 : 3A8952F1E40D610E : 0011101010001001010100101111000111100100000011010110000100001110
             rnd = 07497446926472593856 : 680C4841A6BE41C0 : 0110100000001100010010000100000110100110101111100100000111000000
RNG ENIGMA - Ringstellung A: 6 8 0 C 4 8 4 1 A 6 B E 4 1 C 0
             Ringstellung B: E 0 1 6 D 0 4 E 1 F 2 5 9 8 A 3

RNG ENIGMA - pre = 06683765222441924314 : 5CC1810EC5C2B6DA : 0101110011000001100000010000111011000101110000101011011011011010
             aux = 12544510931843109282 : AE170E3A958351A2 : 1010111000010111000011100011101010010101100000110101000110100010
             rnd = 17498330857042208632 : F2D68F345041E778 : 1111001011010110100011110011010001010000010000011110011101111000
RNG ENIGMA - Ringstellung A: F 2 D 6 8 F 3 4 5 0 4 1 E 7 7 8
             Ringstellung B: 2 A 1 5 3 8 5 9 A 3 E 0 7 1 E A

RNG ENIGMA - pre = 14047566123115450315 : C2F2FAF6371B8FCB : 1100001011110010111110101111011000110111000110111000111111001011
             aux = 10270188054021804720 : 8E87099F106E12B0 : 1000111010000111000010011001111100010000011011100001001010110000
             rnd = 05509577352116804987 : 4C75F36927759D7B : 0100110001110101111100110110100100100111011101011001110101111011
RNG ENIGMA - Ringstellung A: 4 C 7 5 F 3 6 9 2 7 7 5 9 D 7 B
             Ringstellung B: 0 B 2 1 E 6 0 1 F 9 9 0 7 8 E 8

D996EFAF85039E05    0.849959354735814410
F900A13F639FA93B    0.972665861106746465
E4DEA8CD04057D2A    0.894022512486136511
C0B0131235C9F0B1    0.752686683603122964
23F2487520D18CBE    0.140415695758582260
C042F4788EFA6E54    0.751021651681165192
DCB1D578477A2394    0.862088529458084896
680C4841A6BE41C0    0.406437412288817690
F2D68F345041E778    0.948586416503764962
4C75F36927759D7B    0.298674786732098463

--------------------------------------------------------------------------------
RNG ENIGMA - avg:     0.6876558904354333
--------------------------------------------------------------------------------
RNG ENIGMA - time:    0.00027735 seconds

```
&nbsp;

## Tests

The primary tool of testing is the famous [Dieharder](https://webhome.phy.duke.edu/~rgb/General/dieharder.php) random number test suit written by **Robert G. Brown**.

On an Ubuntu-like system **Dieharder** can easily be installed  from the reposiories. Its [man page](https://manpages.debian.org/testing/dieharder/dieharder.1.en.html) is quite informative.

```
$ sudo apt install dieharder

$ dieharder
#=============================================================================#
#            dieharder version 3.31.1 Copyright 2003 Robert G. Brown          #
#=============================================================================#

Usage:

dieharder [-a] [-d dieharder test number] [-f filename] [-B]
          [-D output flag [-D output flag] ... ] [-F] [-c separator]
          [-g generator number or -1] [-h] [-k ks_flag] [-l]
          [-L overlap] [-m multiply_p] [-n ntuple]
          [-p number of p samples] [-P Xoff]
          [-o filename] [-s seed strategy] [-S random number seed]
          [-n ntuple] [-p number of p samples] [-o filename]
          [-s seed strategy] [-S random number seed]
          [-t number of test samples] [-v verbose flag]
          [-W weak] [-X fail] [-Y Xtrategy]
          [-x xvalue] [-y yvalue] [-z zvalue]

$ man dieharder
```

All tests are run on a notebook with the given specification:

```
$ cat /etc/lsb-release
DISTRIB_ID=LinuxMint
DISTRIB_RELEASE=19
DISTRIB_CODENAME=tara
DISTRIB_DESCRIPTION="Linux Mint 19 Tara"

$ lscpu
Architecture:        x86_64
CPU op-mode(s):      32-bit, 64-bit
Byte Order:          Little Endian
CPU(s):              8
On-line CPU(s) list: 0-7
Thread(s) per core:  2
Core(s) per socket:  4
Socket(s):           1
NUMA node(s):        1
Vendor ID:           GenuineIntel
CPU family:          6
Model:               94
Model name:          Intel(R) Core(TM) i7-6700HQ CPU @ 2.60GHz
Stepping:            3
CPU MHz:             3477.529
CPU max MHz:         3500,0000
CPU min MHz:         800,0000
BogoMIPS:            5184.00
Virtualization:      VT-x
L1d cache:           32K
L1i cache:           32K
L2 cache:            256K
L3 cache:            6144K
NUMA node0 CPU(s):   0-7
Flags:               fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc art arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc cpuid aperfmperf tsc_known_freq pni pclmulqdq dtes64 monitor ds_cpl vmx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch cpuid_fault epb invpcid_single pti ssbd ibrs ibpb stibp tpr_shadow vnmi flexpriority ept vpid fsgsbase tsc_adjust bmi1 hle avx2 smep bmi2 erms invpcid rtm mpx rdseed adx smap clflushopt intel_pt xsaveopt xsavec xgetbv1 xsaves dtherm ida arat pln pts hwp hwp_notify hwp_act_window hwp_epp flush_l1d

$ gcc --version
gcc (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0
Copyright (C) 2017 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

&nbsp;

#### Option #1

This option calculates the average of N numbers between [0.0 - 1.0]. There will be no output fie, and therefore this test runs fast. The expectation is that the average of these N numbers will converge to 0.5 as N goes to infinity.

```
$ ./rng-enigma-app 1 1000000000
RNG ENIGMA - seed: 1559A28EFEEE932F
RNG ENIGMA - generate 1,000,000,000 numbers
RNG ENIGMA - generating doubles between [0.0 - 1.0]
--------------------------------------------------------------------------------
RNG ENIGMA - avg:     0.4999966913905150
--------------------------------------------------------------------------------
RNG ENIGMA - time:   70.55417242 seconds
```

&nbsp;

#### Option #2

This option generates a text file, **enigma.txt**, which consists of two columns: one 64-bit unsigned hexadecimal integer and its floating point representation as described in [Good Practice in (Pseudo) Random Number Generation for Bioinformatics Applications, page 7](http://www.cs.ucl.ac.uk/staff/d.jones/GoodPracticeRNG.pdf) by **David Jones**. The columns are seperated with a tab character, so that this file can easily be imported into a spreadsheet application. Each write operation saves ten lines.

```
$ ./rng-enigma-app 2 1000000000
RNG ENIGMA - seed: 1559A2ADE80FDA25
RNG ENIGMA - generate 1,000,000,000 numbers
RNG ENIGMA - saving text file of doubles between [0.0 - 1.0]
--------------------------------------------------------------------------------
RNG ENIGMA - avg:     0.5000001721156835
--------------------------------------------------------------------------------
RNG ENIGMA - time:  497.65150299 seconds

$ head -n 10 enigma.txt
851A0BEEDBDA5953    0.519928689780365660
B69BBE6149408BA7    0.713313959838389522
9E955141F3880FC0    0.619465902902021526
9ACFFF1B370949E6    0.604736274856837674
4198FB18C1125257    0.256240552466871607
3ABA922D30F73342    0.229409347565675104
1CD625E147B3D75A    0.112642638683339458
9808F7AC84D76BB2    0.593886832827448696
C97FACF9EC54CCD0    0.787104426399610979
CEE03BD214A8A9AE    0.808109034337265886

$ du -b enigma.txt
38000000000 enigma.txt
$ du -h enigma.txt
36G enigma.txt
```

&nbsp;

#### Option #3

This option generates a binary file, **dieharder.bin**, which is then fed into the **Dieharder** test suit. Each write operation saves ten 32-bit numbers, which are in fact five 64-bit numbers.

```
$ ./rng-enigma-app 3 1000000000
RNG ENIGMA - seed: 1559A69C26C38414
RNG ENIGMA - generate 1,000,000,000 numbers
RNG ENIGMA - saving binary file for Dieharder
--------------------------------------------------------------------------------
RNG ENIGMA - time:   33.58830852 seconds

$ hexdump -n 160 dieharder.bin
0000000 717e 3f45 b713 7214 e0aa cce5 4f0a 54de
0000010 9474 e9cf c85b 25c4 e0bc ae1e 2273 2d84
0000020 1653 e7f2 25d3 2972 3b38 2a36 7d3f 2c4b
0000030 f791 24c5 f060 a906 603e 385e 63ce 8061
0000040 d835 0f0e 8c8d 6748 3cf5 83eb 37ef 3dd4
0000050 9bc5 1351 5a1f 12f3 4467 0d8c 4c96 aee0
0000060 568a 4088 bcbe b346 94fd 385a 15b1 459d
0000070 b55d 6a2d 31ad d165 5ff6 65a7 9c65 7e83
0000080 7328 7def 207e d5f0 3e7b 68e1 6789 615f
0000090 5577 0e88 e2c1 e412 99bb b180 104c 3c0a
00000a0

$ du -b dieharder.bin
4000000000 dieharder.bin
$ du -h dieharder.bin
3,8G dieharder.bin

$ time dieharder -a -g 201 -f dieharder.bin
#=============================================================================#
#            dieharder version 3.31.1 Copyright 2003 Robert G. Brown          #
#=============================================================================#
   rng_name    |           filename             |rands/second|
 file_input_raw|                   dieharder.bin|  5.76e+07  |
#=============================================================================#
        test_name   |ntup| tsamples |psamples|  p-value |Assessment
#=============================================================================#
   diehard_birthdays|   0|       100|     100|0.26504410|  PASSED
      diehard_operm5|   0|   1000000|     100|0.18037029|  PASSED
  diehard_rank_32x32|   0|     40000|     100|0.75043826|  PASSED
    diehard_rank_6x8|   0|    100000|     100|0.95723602|  PASSED
   diehard_bitstream|   0|   2097152|     100|0.14075791|  PASSED
        diehard_opso|   0|   2097152|     100|0.94793406|  PASSED
        diehard_oqso|   0|   2097152|     100|0.54810711|  PASSED
         diehard_dna|   0|   2097152|     100|0.06338408|  PASSED
diehard_count_1s_str|   0|    256000|     100|0.56230569|  PASSED
diehard_count_1s_byt|   0|    256000|     100|0.07094638|  PASSED
 diehard_parking_lot|   0|     12000|     100|0.97977277|  PASSED
    diehard_2dsphere|   2|      8000|     100|0.25204123|  PASSED
    diehard_3dsphere|   3|      4000|     100|0.26923450|  PASSED
# The file file_input_raw was rewound 1 times
     diehard_squeeze|   0|    100000|     100|0.48564602|  PASSED
# The file file_input_raw was rewound 1 times
        diehard_sums|   0|       100|     100|0.12849806|  PASSED
# The file file_input_raw was rewound 1 times
        diehard_runs|   0|    100000|     100|0.26114289|  PASSED
        diehard_runs|   0|    100000|     100|0.57361798|  PASSED
# The file file_input_raw was rewound 1 times
       diehard_craps|   0|    200000|     100|0.03170322|  PASSED
       diehard_craps|   0|    200000|     100|0.63649854|  PASSED
# The file file_input_raw was rewound 3 times
 marsaglia_tsang_gcd|   0|  10000000|     100|0.20975543|  PASSED
 marsaglia_tsang_gcd|   0|  10000000|     100|0.34702274|  PASSED
# The file file_input_raw was rewound 3 times
         sts_monobit|   1|    100000|     100|0.96182284|  PASSED
# The file file_input_raw was rewound 3 times
            sts_runs|   2|    100000|     100|0.99011554|  PASSED
# The file file_input_raw was rewound 3 times
          sts_serial|   1|    100000|     100|0.56608842|  PASSED
          sts_serial|   2|    100000|     100|0.77885290|  PASSED
          sts_serial|   3|    100000|     100|0.95002521|  PASSED
          sts_serial|   3|    100000|     100|0.47565269|  PASSED
          sts_serial|   4|    100000|     100|0.98642165|  PASSED
          sts_serial|   4|    100000|     100|0.86066199|  PASSED
          sts_serial|   5|    100000|     100|0.92151801|  PASSED
          sts_serial|   5|    100000|     100|0.87712931|  PASSED
          sts_serial|   6|    100000|     100|0.40905060|  PASSED
          sts_serial|   6|    100000|     100|0.82292639|  PASSED
          sts_serial|   7|    100000|     100|0.06059572|  PASSED
          sts_serial|   7|    100000|     100|0.25074622|  PASSED
          sts_serial|   8|    100000|     100|0.72210464|  PASSED
          sts_serial|   8|    100000|     100|0.71112490|  PASSED
          sts_serial|   9|    100000|     100|0.28848424|  PASSED
          sts_serial|   9|    100000|     100|0.20797669|  PASSED
          sts_serial|  10|    100000|     100|0.35915587|  PASSED
          sts_serial|  10|    100000|     100|0.76153433|  PASSED
          sts_serial|  11|    100000|     100|0.40654143|  PASSED
          sts_serial|  11|    100000|     100|0.72002709|  PASSED
          sts_serial|  12|    100000|     100|0.63585153|  PASSED
          sts_serial|  12|    100000|     100|0.76006974|  PASSED
          sts_serial|  13|    100000|     100|0.87907485|  PASSED
          sts_serial|  13|    100000|     100|0.51089697|  PASSED
          sts_serial|  14|    100000|     100|0.73039452|  PASSED
          sts_serial|  14|    100000|     100|0.98746538|  PASSED
          sts_serial|  15|    100000|     100|0.76203193|  PASSED
          sts_serial|  15|    100000|     100|0.67774476|  PASSED
          sts_serial|  16|    100000|     100|0.14892945|  PASSED
          sts_serial|  16|    100000|     100|0.05768024|  PASSED
# The file file_input_raw was rewound 3 times
         rgb_bitdist|   1|    100000|     100|0.59083378|  PASSED
# The file file_input_raw was rewound 3 times
         rgb_bitdist|   2|    100000|     100|0.12317431|  PASSED
# The file file_input_raw was rewound 3 times
         rgb_bitdist|   3|    100000|     100|0.51354891|  PASSED
# The file file_input_raw was rewound 3 times
         rgb_bitdist|   4|    100000|     100|0.01563563|  PASSED
# The file file_input_raw was rewound 3 times
         rgb_bitdist|   5|    100000|     100|0.40260452|  PASSED
# The file file_input_raw was rewound 3 times
         rgb_bitdist|   6|    100000|     100|0.38866162|  PASSED
# The file file_input_raw was rewound 3 times
         rgb_bitdist|   7|    100000|     100|0.36026668|  PASSED
# The file file_input_raw was rewound 4 times
         rgb_bitdist|   8|    100000|     100|0.83529422|  PASSED
# The file file_input_raw was rewound 4 times
         rgb_bitdist|   9|    100000|     100|0.84468955|  PASSED
# The file file_input_raw was rewound 4 times
         rgb_bitdist|  10|    100000|     100|0.28755600|  PASSED
# The file file_input_raw was rewound 4 times
         rgb_bitdist|  11|    100000|     100|0.49677963|  PASSED
# The file file_input_raw was rewound 4 times
         rgb_bitdist|  12|    100000|     100|0.94148332|  PASSED
# The file file_input_raw was rewound 4 times
rgb_minimum_distance|   2|     10000|    1000|0.44761083|  PASSED
# The file file_input_raw was rewound 4 times
rgb_minimum_distance|   3|     10000|    1000|0.53554682|  PASSED
# The file file_input_raw was rewound 4 times
rgb_minimum_distance|   4|     10000|    1000|0.91437702|  PASSED
# The file file_input_raw was rewound 4 times
rgb_minimum_distance|   5|     10000|    1000|0.95759642|  PASSED
# The file file_input_raw was rewound 5 times
    rgb_permutations|   2|    100000|     100|0.29719665|  PASSED
# The file file_input_raw was rewound 5 times
    rgb_permutations|   3|    100000|     100|0.66112969|  PASSED
# The file file_input_raw was rewound 5 times
    rgb_permutations|   4|    100000|     100|0.01302705|  PASSED
# The file file_input_raw was rewound 5 times
    rgb_permutations|   5|    100000|     100|0.65432899|  PASSED
# The file file_input_raw was rewound 5 times
      rgb_lagged_sum|   0|   1000000|     100|0.16967836|  PASSED
# The file file_input_raw was rewound 5 times
      rgb_lagged_sum|   1|   1000000|     100|0.33426776|  PASSED
# The file file_input_raw was rewound 5 times
      rgb_lagged_sum|   2|   1000000|     100|0.08062410|  PASSED
# The file file_input_raw was rewound 6 times
      rgb_lagged_sum|   3|   1000000|     100|0.10113070|  PASSED
# The file file_input_raw was rewound 6 times
      rgb_lagged_sum|   4|   1000000|     100|0.97458880|  PASSED
# The file file_input_raw was rewound 7 times
      rgb_lagged_sum|   5|   1000000|     100|0.12795518|  PASSED
# The file file_input_raw was rewound 7 times
      rgb_lagged_sum|   6|   1000000|     100|0.08893704|  PASSED
# The file file_input_raw was rewound 8 times
      rgb_lagged_sum|   7|   1000000|     100|0.14324024|  PASSED
# The file file_input_raw was rewound 9 times
      rgb_lagged_sum|   8|   1000000|     100|0.61314466|  PASSED
# The file file_input_raw was rewound 10 times
      rgb_lagged_sum|   9|   1000000|     100|0.81678554|  PASSED
# The file file_input_raw was rewound 11 times
      rgb_lagged_sum|  10|   1000000|     100|0.03896130|  PASSED
# The file file_input_raw was rewound 12 times
      rgb_lagged_sum|  11|   1000000|     100|0.12191383|  PASSED
# The file file_input_raw was rewound 14 times
      rgb_lagged_sum|  12|   1000000|     100|0.75891540|  PASSED
# The file file_input_raw was rewound 15 times
      rgb_lagged_sum|  13|   1000000|     100|0.63908058|  PASSED
# The file file_input_raw was rewound 17 times
      rgb_lagged_sum|  14|   1000000|     100|0.97176034|  PASSED
# The file file_input_raw was rewound 18 times
      rgb_lagged_sum|  15|   1000000|     100|0.67523784|  PASSED
# The file file_input_raw was rewound 20 times
      rgb_lagged_sum|  16|   1000000|     100|0.17843809|  PASSED
# The file file_input_raw was rewound 22 times
      rgb_lagged_sum|  17|   1000000|     100|0.89233108|  PASSED
# The file file_input_raw was rewound 24 times
      rgb_lagged_sum|  18|   1000000|     100|0.59515376|  PASSED
# The file file_input_raw was rewound 26 times
      rgb_lagged_sum|  19|   1000000|     100|0.59455603|  PASSED
# The file file_input_raw was rewound 28 times
      rgb_lagged_sum|  20|   1000000|     100|0.31868652|  PASSED
# The file file_input_raw was rewound 30 times
      rgb_lagged_sum|  21|   1000000|     100|0.59079423|  PASSED
# The file file_input_raw was rewound 32 times
      rgb_lagged_sum|  22|   1000000|     100|0.72769109|  PASSED
# The file file_input_raw was rewound 35 times
      rgb_lagged_sum|  23|   1000000|     100|0.98800700|  PASSED
# The file file_input_raw was rewound 37 times
      rgb_lagged_sum|  24|   1000000|     100|0.00066941|   WEAK
# The file file_input_raw was rewound 40 times
      rgb_lagged_sum|  25|   1000000|     100|0.96422501|  PASSED
# The file file_input_raw was rewound 42 times
      rgb_lagged_sum|  26|   1000000|     100|0.69475391|  PASSED
# The file file_input_raw was rewound 45 times
      rgb_lagged_sum|  27|   1000000|     100|0.91943134|  PASSED
# The file file_input_raw was rewound 48 times
      rgb_lagged_sum|  28|   1000000|     100|0.99705269|   WEAK
# The file file_input_raw was rewound 51 times
      rgb_lagged_sum|  29|   1000000|     100|0.98226800|  PASSED
# The file file_input_raw was rewound 54 times
      rgb_lagged_sum|  30|   1000000|     100|0.71819819|  PASSED
# The file file_input_raw was rewound 57 times
      rgb_lagged_sum|  31|   1000000|     100|0.70788928|  PASSED
# The file file_input_raw was rewound 61 times
      rgb_lagged_sum|  32|   1000000|     100|0.77604151|  PASSED
# The file file_input_raw was rewound 61 times
     rgb_kstest_test|   0|     10000|    1000|0.36340794|  PASSED
# The file file_input_raw was rewound 61 times
     dab_bytedistrib|   0|  51200000|       1|0.48916044|  PASSED
# The file file_input_raw was rewound 61 times
             dab_dct| 256|     50000|       1|0.26678759|  PASSED
Preparing to run test 207.  ntuple = 0
# The file file_input_raw was rewound 61 times
        dab_filltree|  32|  15000000|       1|0.71313709|  PASSED
        dab_filltree|  32|  15000000|       1|0.70369707|  PASSED
Preparing to run test 208.  ntuple = 0
# The file file_input_raw was rewound 61 times
       dab_filltree2|   0|   5000000|       1|0.85862169|  PASSED
       dab_filltree2|   1|   5000000|       1|0.00068077|   WEAK
Preparing to run test 209.  ntuple = 0
# The file file_input_raw was rewound 61 times
        dab_monobit2|  12|  65000000|       1|0.98905235|  PASSED

real    26m34,913s
user    25m41,572s
sys 0m53,182s
```

&nbsp;

### Option #4

This option generates a text file, **dieharder.txt**, which is then fed into the **Dieharder** test suit. Each write operation saves ten lines of 32-bit decimal integers. Except for being human-readable, it seems that there is no advantage of the text file over the binary file.


```
$ ./rng-enigma-app 4 1000000000
RNG ENIGMA - seed: 1559A83B41B42989
RNG ENIGMA - generate 1,000,000,000 numbers
RNG ENIGMA - saving text file for Dieharder
--------------------------------------------------------------------------------
RNG ENIGMA - time:  110.16147462 seconds

$ head -n 16 dieharder.txt
#==================================================================
# generator enigma  seed = 1538445720182598025
#==================================================================
type: d
count: 1000000000
numbit: 32
3850388717
3239868083
2072527825
3737787447
3028471000
 560359257
4268793519
 543811855
3663459830
4229445939

$ du -b dieharder.txt
11000000220 dieharder.txt
$ du -h dieharder.txt
11G dieharder.txt

$ time dieharder -a -g 202 -f dieharder.txt
#=============================================================================#
#            dieharder version 3.31.1 Copyright 2003 Robert G. Brown          #
#=============================================================================#
   rng_name    |           filename             |rands/second|
     file_input|                   dieharder.txt|  7.50e+06  |
#=============================================================================#
        test_name   |ntup| tsamples |psamples|  p-value |Assessment
#=============================================================================#
   diehard_birthdays|   0|       100|     100|0.60051170|  PASSED
      diehard_operm5|   0|   1000000|     100|0.01201805|  PASSED
  diehard_rank_32x32|   0|     40000|     100|0.07418830|  PASSED
    diehard_rank_6x8|   0|    100000|     100|0.98565201|  PASSED
   diehard_bitstream|   0|   2097152|     100|0.15471803|  PASSED
        diehard_opso|   0|   2097152|     100|0.71583914|  PASSED
        diehard_oqso|   0|   2097152|     100|0.32947807|  PASSED
         diehard_dna|   0|   2097152|     100|0.62350735|  PASSED
diehard_count_1s_str|   0|    256000|     100|0.99876170|   WEAK
diehard_count_1s_byt|   0|    256000|     100|0.94096338|  PASSED
 diehard_parking_lot|   0|     12000|     100|0.33358085|  PASSED
    diehard_2dsphere|   2|      8000|     100|0.81652378|  PASSED
    diehard_3dsphere|   3|      4000|     100|0.85311234|  PASSED
# The file file_input was rewound 1 times
     diehard_squeeze|   0|    100000|     100|0.20045903|  PASSED
# The file file_input was rewound 1 times
        diehard_sums|   0|       100|     100|0.04424579|  PASSED
# The file file_input was rewound 1 times
        diehard_runs|   0|    100000|     100|0.67773878|  PASSED
        diehard_runs|   0|    100000|     100|0.58784133|  PASSED
# The file file_input was rewound 1 times
       diehard_craps|   0|    200000|     100|0.32952280|  PASSED
       diehard_craps|   0|    200000|     100|0.83013712|  PASSED
# The file file_input was rewound 3 times
 marsaglia_tsang_gcd|   0|  10000000|     100|0.67477008|  PASSED
 marsaglia_tsang_gcd|   0|  10000000|     100|0.03072541|  PASSED
# The file file_input was rewound 3 times
         sts_monobit|   1|    100000|     100|0.10073817|  PASSED
# The file file_input was rewound 3 times
            sts_runs|   2|    100000|     100|0.24203001|  PASSED
# The file file_input was rewound 3 times
          sts_serial|   1|    100000|     100|0.75181367|  PASSED
          sts_serial|   2|    100000|     100|0.55308594|  PASSED
          sts_serial|   3|    100000|     100|0.23095994|  PASSED
          sts_serial|   3|    100000|     100|0.86922270|  PASSED
          sts_serial|   4|    100000|     100|0.61684968|  PASSED
          sts_serial|   4|    100000|     100|0.32287766|  PASSED
          sts_serial|   5|    100000|     100|0.20422558|  PASSED
          sts_serial|   5|    100000|     100|0.14015559|  PASSED
          sts_serial|   6|    100000|     100|0.80658791|  PASSED
          sts_serial|   6|    100000|     100|0.52692108|  PASSED
          sts_serial|   7|    100000|     100|0.16158770|  PASSED
          sts_serial|   7|    100000|     100|0.04358734|  PASSED
          sts_serial|   8|    100000|     100|0.10137226|  PASSED
          sts_serial|   8|    100000|     100|0.56358065|  PASSED
          sts_serial|   9|    100000|     100|0.11222948|  PASSED
          sts_serial|   9|    100000|     100|0.60217685|  PASSED
          sts_serial|  10|    100000|     100|0.29213674|  PASSED
          sts_serial|  10|    100000|     100|0.20808736|  PASSED
          sts_serial|  11|    100000|     100|0.84153890|  PASSED
          sts_serial|  11|    100000|     100|0.94262762|  PASSED
          sts_serial|  12|    100000|     100|0.99859643|   WEAK
          sts_serial|  12|    100000|     100|0.99908248|   WEAK
          sts_serial|  13|    100000|     100|0.97164134|  PASSED
          sts_serial|  13|    100000|     100|0.29911617|  PASSED
          sts_serial|  14|    100000|     100|0.43884466|  PASSED
          sts_serial|  14|    100000|     100|0.43789051|  PASSED
          sts_serial|  15|    100000|     100|0.17408500|  PASSED
          sts_serial|  15|    100000|     100|0.27758397|  PASSED
          sts_serial|  16|    100000|     100|0.77820703|  PASSED
          sts_serial|  16|    100000|     100|0.43587802|  PASSED
# The file file_input was rewound 3 times
         rgb_bitdist|   1|    100000|     100|0.28916942|  PASSED
# The file file_input was rewound 3 times
         rgb_bitdist|   2|    100000|     100|0.97808733|  PASSED
# The file file_input was rewound 3 times
         rgb_bitdist|   3|    100000|     100|0.94563157|  PASSED
# The file file_input was rewound 3 times
         rgb_bitdist|   4|    100000|     100|0.03730414|  PASSED
# The file file_input was rewound 3 times
         rgb_bitdist|   5|    100000|     100|0.68918419|  PASSED
# The file file_input was rewound 3 times
         rgb_bitdist|   6|    100000|     100|0.83597121|  PASSED
# The file file_input was rewound 3 times
         rgb_bitdist|   7|    100000|     100|0.04103704|  PASSED
# The file file_input was rewound 4 times
         rgb_bitdist|   8|    100000|     100|0.73428842|  PASSED
# The file file_input was rewound 4 times
         rgb_bitdist|   9|    100000|     100|0.12302660|  PASSED
# The file file_input was rewound 4 times
         rgb_bitdist|  10|    100000|     100|0.63266851|  PASSED
# The file file_input was rewound 4 times
         rgb_bitdist|  11|    100000|     100|0.05438852|  PASSED
# The file file_input was rewound 4 times
         rgb_bitdist|  12|    100000|     100|0.54319886|  PASSED
# The file file_input was rewound 4 times
rgb_minimum_distance|   2|     10000|    1000|0.59587023|  PASSED
# The file file_input was rewound 4 times
rgb_minimum_distance|   3|     10000|    1000|0.76489504|  PASSED
# The file file_input was rewound 4 times
rgb_minimum_distance|   4|     10000|    1000|0.63349934|  PASSED
# The file file_input was rewound 4 times
rgb_minimum_distance|   5|     10000|    1000|0.03049118|  PASSED
# The file file_input was rewound 5 times
    rgb_permutations|   2|    100000|     100|0.72966053|  PASSED
# The file file_input was rewound 5 times
    rgb_permutations|   3|    100000|     100|0.73886464|  PASSED
# The file file_input was rewound 5 times
    rgb_permutations|   4|    100000|     100|0.33461494|  PASSED
# The file file_input was rewound 5 times
    rgb_permutations|   5|    100000|     100|0.68909453|  PASSED
# The file file_input was rewound 5 times
      rgb_lagged_sum|   0|   1000000|     100|0.61610501|  PASSED
# The file file_input was rewound 5 times
      rgb_lagged_sum|   1|   1000000|     100|0.30991699|  PASSED
# The file file_input was rewound 5 times
      rgb_lagged_sum|   2|   1000000|     100|0.81117074|  PASSED
# The file file_input was rewound 6 times
      rgb_lagged_sum|   3|   1000000|     100|0.24135779|  PASSED
# The file file_input was rewound 6 times
      rgb_lagged_sum|   4|   1000000|     100|0.44368943|  PASSED
# The file file_input was rewound 7 times
      rgb_lagged_sum|   5|   1000000|     100|0.51047657|  PASSED
# The file file_input was rewound 7 times
      rgb_lagged_sum|   6|   1000000|     100|0.96094210|  PASSED
# The file file_input was rewound 8 times
      rgb_lagged_sum|   7|   1000000|     100|0.57078392|  PASSED
# The file file_input was rewound 9 times
      rgb_lagged_sum|   8|   1000000|     100|0.17152447|  PASSED
# The file file_input was rewound 10 times
      rgb_lagged_sum|   9|   1000000|     100|0.26760407|  PASSED
# The file file_input was rewound 11 times
      rgb_lagged_sum|  10|   1000000|     100|0.62618627|  PASSED
# The file file_input was rewound 12 times
      rgb_lagged_sum|  11|   1000000|     100|0.67644229|  PASSED
# The file file_input was rewound 14 times
      rgb_lagged_sum|  12|   1000000|     100|0.56382569|  PASSED
# The file file_input was rewound 15 times
      rgb_lagged_sum|  13|   1000000|     100|0.46614356|  PASSED
# The file file_input was rewound 17 times
      rgb_lagged_sum|  14|   1000000|     100|0.80447862|  PASSED
# The file file_input was rewound 18 times
      rgb_lagged_sum|  15|   1000000|     100|0.79664019|  PASSED
# The file file_input was rewound 20 times
      rgb_lagged_sum|  16|   1000000|     100|0.97585933|  PASSED
# The file file_input was rewound 22 times
      rgb_lagged_sum|  17|   1000000|     100|0.91677111|  PASSED
# The file file_input was rewound 24 times
      rgb_lagged_sum|  18|   1000000|     100|0.52346346|  PASSED
# The file file_input was rewound 26 times
      rgb_lagged_sum|  19|   1000000|     100|0.93480473|  PASSED
# The file file_input was rewound 28 times
      rgb_lagged_sum|  20|   1000000|     100|0.36906746|  PASSED
# The file file_input was rewound 30 times
      rgb_lagged_sum|  21|   1000000|     100|0.59780178|  PASSED
# The file file_input was rewound 32 times
      rgb_lagged_sum|  22|   1000000|     100|0.07479217|  PASSED
# The file file_input was rewound 35 times
      rgb_lagged_sum|  23|   1000000|     100|0.97835157|  PASSED
# The file file_input was rewound 37 times
      rgb_lagged_sum|  24|   1000000|     100|0.09250538|  PASSED
# The file file_input was rewound 40 times
      rgb_lagged_sum|  25|   1000000|     100|0.27457309|  PASSED
# The file file_input was rewound 42 times
      rgb_lagged_sum|  26|   1000000|     100|0.71949724|  PASSED
# The file file_input was rewound 45 times
      rgb_lagged_sum|  27|   1000000|     100|0.27688697|  PASSED
# The file file_input was rewound 48 times
      rgb_lagged_sum|  28|   1000000|     100|0.53130139|  PASSED
# The file file_input was rewound 51 times
      rgb_lagged_sum|  29|   1000000|     100|0.27408590|  PASSED
# The file file_input was rewound 54 times
      rgb_lagged_sum|  30|   1000000|     100|0.13871812|  PASSED
# The file file_input was rewound 57 times
      rgb_lagged_sum|  31|   1000000|     100|0.88235518|  PASSED
# The file file_input was rewound 61 times
      rgb_lagged_sum|  32|   1000000|     100|0.78495059|  PASSED
# The file file_input was rewound 61 times
     rgb_kstest_test|   0|     10000|    1000|0.05881359|  PASSED
# The file file_input was rewound 61 times
     dab_bytedistrib|   0|  51200000|       1|0.53176853|  PASSED
# The file file_input was rewound 61 times
             dab_dct| 256|     50000|       1|0.57457730|  PASSED
Preparing to run test 207.  ntuple = 0
# The file file_input was rewound 61 times
        dab_filltree|  32|  15000000|       1|0.50349517|  PASSED
        dab_filltree|  32|  15000000|       1|0.73548265|  PASSED
Preparing to run test 208.  ntuple = 0
# The file file_input was rewound 61 times
       dab_filltree2|   0|   5000000|       1|0.87104232|  PASSED
       dab_filltree2|   1|   5000000|       1|0.00000943|   WEAK
Preparing to run test 209.  ntuple = 0
# The file file_input was rewound 61 times
        dab_monobit2|  12|  65000000|       1|0.92245843|  PASSED

real    147m45,100s
user    145m41,537s
sys 2m1,395s
```

&nbsp;

## SWIG and Python

Using [SWIG](https://en.wikipedia.org/wiki/SWIG) the library can be imported to Python.

> The Simplified Wrapper and Interface Generator (SWIG) is an open-source software tool used to connect computer programs or libraries written in C or C++ with scripting languages.

SWIG and the required Python 3 Dev library can easily be installed via apt:
```
$ sudo apt install swig
$ sudo apt install python3-dev

$ swig -version

SWIG Version 3.0.12

Compiled with g++ [x86_64-pc-linux-gnu]

Configured options: +pcre

Please see http://www.swig.org for reporting bugs and further information
```
The library has a new folder [swig](./library/swig), which contains its own CMake configuration file and a [SWIG interface file](http://www.swig.org/Doc1.3/SWIG.html#SWIG_nn47). For convenience a **build script** is also provided

To help the SWIG compiler the library code is changed a little bit. For example, in function **rng_read_random()**  the two-dimensional array argument is changed from **uint64_t rotors[16][16]** to **uint64_t&ast; p_rotors** to suppress **incompatible pointer type** warnings.

```
[ 40%] Building C object CMakeFiles/_rng_enigma.dir/lib_rng_enigmaPYTHON_wrap.c.o
/home/orhanku/ME/DEV/OK/SWIG/rng-enigma/library/swig/build/lib_rng_enigmaPYTHON_wrap.c: In function ‘_wrap_rng_read_random’:
/home/orhanku/ME/DEV/OK/SWIG/rng-enigma/library/swig/build/lib_rng_enigmaPYTHON_wrap.c:3751:53: warning: passing argument 2 of ‘rng_read_random’ from incompatible pointer type [-Wincompatible-pointer-types]
   result = (bool)rng_read_random((char const *)arg1,(unsigned long long (*)[16])arg2);
                                                     ^
In file included from /home/orhanku/ME/DEV/OK/SWIG/rng-enigma/library/swig/build/lib_rng_enigmaPYTHON_wrap.c:3027:0:
/home/orhanku/ME/DEV/OK/SWIG/rng-enigma/library/swig/../include/../include/rng_file.h:14:6: note: expected ‘uint64_t (*)[16] {aka long unsigned int (*)[16]}’ but argument is of type ‘long long unsigned int (*)[16]’
 bool rng_read_random(const char* file_path, uint64_t rotors[16][16]);
      ^~~~~~~~~~~~~~~
```
To build the Python bindings by SWIG the build script can be used:
```
$ cd library/swig

$ ./build.sh
++ CMaking the build project ...
-- The C compiler identification is GNU 5.4.0
-- The CXX compiler identification is GNU 5.4.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Found SWIG: /usr/bin/swig3.0 (found version "3.0.8")
-- Found PythonLibs: /usr/lib/x86_64-linux-gnu/libpython3.5m.so (found version "3.5.2")
-- Configuring done
-- Generating done
-- Build files have been written to: /home/orhanku/ME/DEV/OK/rng-enigma/library/swig/build
++ CMaked the build project.
++ Making the build project ...
Scanning dependencies of target rng_enigma_swig_compilation
[ 20%] Swig compile lib_rng_enigma.i for python
[ 20%] Built target rng_enigma_swig_compilation
Scanning dependencies of target _rng_enigma
[ 40%] Building C object CMakeFiles/_rng_enigma.dir/CMakeFiles/_rng_enigma.dir/lib_rng_enigmaPYTHON_wrap.c.o
[ 60%] Building C object CMakeFiles/_rng_enigma.dir/home/orhanku/ME/DEV/OK/rng-enigma/library/src/rng_enigma.c.o
[ 80%] Building C object CMakeFiles/_rng_enigma.dir/home/orhanku/ME/DEV/OK/rng-enigma/library/src/rng_file.c.o
[100%] Linking C shared library _rng_enigma.so
[100%] Built target _rng_enigma
++ Maked the build project.

$ cd ../lib/

$ ls
librng-enigma.a  lib_rng_enigma.h  rng_enigma.h  rng_enigma.py  _rng_enigma.so  rng_file.h

$ LD_LIBRARY_PATH=. python3
Python 3.6.7 (default, Oct 22 2018, 11:32:17)
[GCC 8.2.0] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> import rng_enigma
>>>
>>> rng_enigma.rng_get_version()
'0.3.1'
>>> rng_enigma.rng_enigma_is_initialized()
False
>>> rng_enigma.rng_enigma_init("../../application/bin/enigma-rotors.cfg", None)
True
>>> rng_enigma.rng_enigma_is_initialized()
True
>>> rotors = rng_enigma.rng_get_rotors()
>>> print(rotors)
7 5 0 B 3 A D 6 E 2 F C 8 4 9 1
9 7 6 5 D 3 4 2 1 E A B F 0 C 8
B C D 8 6 4 A 7 5 3 E 0 2 F 1 9
E 6 C D 4 B 3 2 F 5 8 9 7 0 A 1
5 B 4 6 9 3 F A D 1 2 E 7 C 0 8
C 0 D 9 4 5 6 7 E 2 8 1 A B F 3
9 E B 7 C D 6 4 8 2 F 0 A 5 3 1
3 9 4 6 5 2 7 8 1 E B C D A F 0
A 4 8 E 3 D 2 F 1 6 7 5 9 C 0 B
8 7 6 5 4 3 9 A B C 2 1 0 D E F
4 E D 5 B 6 C 2 3 7 9 8 F 0 1 A
6 4 5 B D 3 E 7 9 C 8 A 0 F 2 1
7 3 6 8 F 2 B 5 4 D E 9 1 0 A C
9 E D A 4 3 B C 6 5 7 8 0 1 F 2
1 D 8 E 5 C 6 B 7 A 2 F 0 9 3 4
8 1 0 2 C B 9 4 A 6 7 5 D F E 3
>>> seed = rng_enigma.rng_get_seed()
>>> print(seed)
1544996089255362566
>>> print("%X" % seed)
1570EDC1E76F0006
>>>
>>> rng_enigma.rng_get_uint64()
188727632785135423
>>> rng_enigma.rng_get_dbl(rng_enigma.rng_get_uint64())
0.5336570239047165
>>>
>>> a = rng_enigma.new_uint32P()
>>> b = rng_enigma.new_uint32P()
>>> rng_enigma.rng_get_uint32(a, b)
>>> a
<Swig Object of type 'uint_fast16_t *' at 0x7fe8ff9a6840>
>>> b
<Swig Object of type 'uint_fast16_t *' at 0x7fe8ff9a6810>
>>> rng_enigma.uint32P_value(a)
3127494858
>>> rng_enigma.uint32P_value(b)
1879976519
>>> rng_enigma.rng_get_flt(rng_enigma.uint32P_value(a))
0.7281765937805176
>>> rng_enigma.rng_get_flt(rng_enigma.uint32P_value(b))
0.43771612644195557
>>>
>>> CTRL+D

$ cd ../swig

$ ./build.sh expunge
++ Cleaning the build project ...
++ Cleaned the build project.
++ Purging the build project ...
++ Purged the build project.
```
