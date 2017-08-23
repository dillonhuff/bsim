#pragma once

#include <bitset>
#include <cassert>
#include <iostream>
#include <stdint.h>
#include <type_traits>

#define NUM_BYTES(N) (((N) / 8) + 1 - (((N) % 8 == 0)))

typedef uint8_t  bv_uint8;
typedef uint16_t bv_uint16;
typedef uint32_t bv_uint32;
typedef uint64_t bv_uint64;

namespace bsim {

  template<int N>
  class bit_vector {
    unsigned char bits[NUM_BYTES(N)];

  public:
    bit_vector() {
      for (int i = 0; i < N; i++) {
	set(i, 0);
      }
    }

    bit_vector(const bv_uint64 val) {
      *((bv_uint64*)(&bits)) = val;
    }
    
    bit_vector(const bv_uint32 val) {
      *((bv_uint32*)(&bits)) = val;
    }

    bit_vector(const bv_uint16 val) {
      *((bv_uint16*)(&bits)) = val;
    }

    bit_vector(const bv_uint8 val) {
      *((bv_uint8*)(&bits)) = val;
    }
    
    bit_vector(const bit_vector<N>& other) {
      for (int i = 0; i < N; i++) {
	set(i, other.get(i));
      }
    }

    bit_vector<N>& operator=(const bit_vector<N>& other) {
      if (&other == this) {
    	return *this;
      }

      for (int i = 0; i < N; i++) {
    	set(i, other.get(i));
      }

      return *this;
    }

    inline void set(const int ind, const unsigned char val) {
      int byte_num = ind / 8;
      int bit_num = ind % 8;

      unsigned char old = bits[byte_num];
      old ^= (-val ^ old) & (1 << bit_num);

      bits[byte_num] = old;
    }

    unsigned char get(const int ind) const {
      int byte_num = ind / 8;
      int bit_num = ind % 8;

      unsigned char target_byte = bits[byte_num];
      return 0x01 & (target_byte >> bit_num);
    }

    inline bool equals(const bit_vector<N>& other) const {
      for (int i = 0; i < NUM_BYTES(N) - 1; i++) {
	if (this->bits[i] != other.bits[i]) {
	  return false;
	}
      }

      return true;

    }

    inline bv_uint64 as_native_uint64() const {
      return *((bv_uint64*) (&bits));
    }

    inline bv_uint32 as_native_uint32() const {
      return *((bv_uint32*) (&bits));
    }

    inline bv_uint16 as_native_uint16() const {
      return *((bv_uint16*) (&bits));
    }

    inline bv_uint8 as_native_uint8() const {
      return *((bv_uint8*) (&bits));
    }
    
  };

  template<int N>
  static inline std::ostream& operator<<(std::ostream& out, const bit_vector<N>& a) {
    for (int i = N - 1; i >= 0; i--) {
      if (a.get(i) == 0) {
	out << "0";
      } else if (a.get(i) == 1) {
	out << "1";
      } else {
	assert(false);
      }
    }

    return out;
  }

  template<int N>
  static inline bool operator==(const bit_vector<N>& a,
				const bit_vector<N>& b) {
    return a.equals(b);
  }

  template<int N>
  class unsigned_int {
  protected:
    bit_vector<N> bits;
  public:
    unsigned_int() {}


    unsigned_int(const bv_uint32 val) : bits(val) {}

    void set(const int ind, const unsigned char val) {
      bits.set(ind, val);
    }

    unsigned char get(const int ind) const { return bits.get(ind); }

    inline bool equals(const unsigned_int<N>& other) const {
      return (this->bits).equals((other.bits));
    }

    inline bv_uint64 as_native_uint64() const {
      return bits.as_native_uint64();
    }
    
    inline bv_uint32 as_native_uint32() const {
      return bits.as_native_uint32();
    }

    inline bv_uint16 as_native_uint16() const {
      return bits.as_native_uint16();
    }

    inline bv_uint8 as_native_uint8() const {
      return bits.as_native_uint8();
    }
    
    inline std::ostream& print(std::ostream& out) const {
      out << bits << "U";
      return out;
    }
    
  };

  template<int Width>
  class unsigned_int_operations {
  public:

    template<int Q = Width>
    static inline
    //static inline unsigned_int<Width>
    typename std::enable_if<Q >= 0, unsigned_int<Q> >::type
    add(const unsigned_int<Width>& a,
	const unsigned_int<Width>& b) {

      unsigned_int<Width> res;
      unsigned char carry = 0;
      for (int i = 0; i < Width; i++) {
	res.set(i, 0x01 & (carry + a.get(i) + b.get(i)));
	carry = a.get(i) & b.get(i);
      }
      return res;
    }

    // static inline unsigned_int<32> add(const unsigned_int<32>& a,
    // 				       const unsigned_int<32>& b) {

    //   std::cout << "a = " << a.as_native_uint32() << std::endl;
    //   std::cout << "b = " << b.as_native_uint32() << std::endl;
    //   bv_uint32 res = a.as_native_uint32() + b.as_native_uint32();

    //   return unsigned_int<32>(res);
    // }
    
  };

  // template<>
  // class unsigned_int_operations<32> {
  // public:
  //   static inline unsigned_int<32> add(const unsigned_int<32>& a,
  // 				       const unsigned_int<32>& b) {

  //     std::cout << "a = " << a.as_native_uint32() << std::endl;
  //     std::cout << "b = " << b.as_native_uint32() << std::endl;
  //     bv_uint32 res = a.as_native_uint32() + b.as_native_uint32();

  //     return unsigned_int<32>(res);
  //   }
  // };

  template<int N>
  static inline unsigned_int<N> operator+(const unsigned_int<N>& a,
					  const unsigned_int<N>& b) {
    return unsigned_int_operations<N>::add(a, b);
  }

  template<int Width>
  class bit_vector_operations {
  public:
    static inline bit_vector<Width> land(const bit_vector<Width>& a,
					 const bit_vector<Width>& b) {
      bit_vector<Width> a_and_b;
      for (int i = 0; i < Width; i++) {
	a_and_b.set(i, a.get(i) & b.get(i));
      }
      return a_and_b;

    }

  };

  template<>
  class bit_vector_operations<64> {
  public:
    static inline bit_vector<64> land(const bit_vector<64>& a,
				      const bit_vector<64>& b) {
      return bit_vector<64>(a.as_native_uint64() & b.as_native_uint64());
    }

    static inline bit_vector<64> lor(const bit_vector<64>& a,
				     const bit_vector<64>& b) {
      return bit_vector<64>(a.as_native_uint64() | b.as_native_uint64());
    }

    static inline bit_vector<64> lxor(const bit_vector<64>& a,
				      const bit_vector<64>& b) {
      return bit_vector<64>(a.as_native_uint64() ^ b.as_native_uint64());
    }
    
  };


  template<>
  class bit_vector_operations<32> {
  public:
    static inline bit_vector<32> land(const bit_vector<32>& a,
				      const bit_vector<32>& b) {
      return bit_vector<32>(a.as_native_uint32() & b.as_native_uint32());
    }

    static inline bit_vector<32> lor(const bit_vector<32>& a,
				     const bit_vector<32>& b) {
      return bit_vector<32>(a.as_native_uint32() | b.as_native_uint32());
    }

    static inline bit_vector<32> lxor(const bit_vector<32>& a,
				      const bit_vector<32>& b) {
      return bit_vector<32>(a.as_native_uint32() ^ b.as_native_uint32());
    }
    
  };


  template<>
  class bit_vector_operations<16> {
  public:
    static inline bit_vector<16> land(const bit_vector<16>& a,
  				      const bit_vector<16>& b) {
      std::cout << "Using 16 bit and" << std::endl;
      return bit_vector<16>((bv_uint16)(a.as_native_uint16() & b.as_native_uint16()));
    }

    static inline bit_vector<16> lor(const bit_vector<16>& a,
    				     const bit_vector<16>& b) {
      return bit_vector<16>((bv_uint16)(a.as_native_uint16() | b.as_native_uint16()));
    }

    static inline bit_vector<16> lxor(const bit_vector<16>& a,
    				      const bit_vector<16>& b) {
      return bit_vector<16>((bv_uint16)(a.as_native_uint16() ^ b.as_native_uint16()));
    }
    
  };

  template<>
  class bit_vector_operations<8> {
  public:
    static inline bit_vector<8> land(const bit_vector<8>& a,
				     const bit_vector<8>& b) {
      return bit_vector<8>((bv_uint8)(a.as_native_uint8() & b.as_native_uint8()));
    }

    static inline bit_vector<8> lor(const bit_vector<8>& a,
				    const bit_vector<8>& b) {
      return bit_vector<8>((bv_uint8)(a.as_native_uint8() | b.as_native_uint8()));
    }

    static inline bit_vector<8> lxor(const bit_vector<8>& a,
				     const bit_vector<8>& b) {
      return bit_vector<8>((bv_uint8)(a.as_native_uint8() ^ b.as_native_uint8()));
    }
    
  };

  template<int N>
  static inline bit_vector<N> operator&(const bit_vector<N>& a,
					const bit_vector<N>& b) {
    return bit_vector_operations<N>::land(a, b);
  }

  template<int N>
  static inline bool operator!=(const bit_vector<N>& a,
				const bit_vector<N>& b) {
    return !a.equals(b);
  }

  template<int N>
  static inline bool operator==(const unsigned_int<N>& a,
				const unsigned_int<N>& b) {
    return a.equals(b);
  }
  
  template<int N>
  static inline std::ostream&
  operator<<(std::ostream& out, const unsigned_int<N>& a) {
    a.print(out);
    return out;
  }
}
