This header only library provides efficient operations on arbitrary length bit
vectors. The library also includes a bit vector wrapper for unsigned integers.

## Supported Operations

    Bitwise logical operators: and, or, nand, nor, xor, not
    Left and right shifts
    Unsigned integer arithmetic: add, subtract, and multiply
    Signed integer arithmetic: add, subtract, and multiply
    TODO: More convenient ways of initializing bit vectors. Use numbers, manual bit setting, strings representing numbers?

## Examples


## Native Instruction Use for Small Bit Vectors

bsim supports operating on arbitrary length bit vectors, but for operations on small (less than 65 bits) bit vectors the library uses conditional compilation to native instructions rather than arbitrary length algorithms.

Use of native operations comes at a slight storage cost since. For example a 33 bit vector must be stored as 64 bits in order to use 64 bit operations without changing bits in memory beyond the object.

However this optimization prevents the use of costly general purpose algorithms.