This header only library provides efficient operations on arbitrary length bit
vectors. The library also includes a bit vector wrapper for unsigned integers.

# Supported Operations

1. Bitwise logical operators: and, or, nand, nor, xor, not
2. Unsigned integer arithmetic: add, subtract, and multiply

# Examples

```cpp

#include <iostream>

using namespace std;

int main() {
    bit_vector<3> a;
    a.set(1, 1);

    bit_vector<3> b;
    b.set(0, 1);

    bit_vector<3> c = a | b;

    cout << c << endl;

}
```

# Native Instruction Use for Small Bit Vectors

bsim supports operating on arbitrary length bit vectors, but for operations on small (less than 65 bits) bit vectors the library uses conditional compilation to native instructions rather than arbitrary length algorithms.

Use of native operations comes at a slight storage cost since. For example a 33 bit vector must be stored as 64 bits in order to use 64 bit operations without changing bits in memory beyond the object.

However this optimization prevents the use of costly general purpose algorithms.