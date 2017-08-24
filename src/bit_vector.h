#pragma once

#include <bitset>
#include <cassert>
#include <iostream>
#include <stdint.h>
#include <type_traits>

#define GEN_NUM_BYTES(N) (((N) / 8) + 1 - (((N) % 8 == 0)))
#define NUM_BYTES_GT_8(N) GEN_NUM_BYTES(N)
#define NUM_BYTES_GT_4(N) (N <= 64 ? 8 : NUM_BYTES_GT_8(N))
#define NUM_BYTES_GT_2(N) (N <= 32 ? 4 : NUM_BYTES_GT_4(N))
#define NUM_BYTES_GT_1(N) (N <= 16 ? 2 : NUM_BYTES_GT_2(N))
#define NUM_BYTES(N) (N <= 8 ? (1) : NUM_BYTES_GT_1(N))

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
      for (int i = 0; i < NUM_BYTES(N); i++) {
	bits[i] = other.bits[i];
	//set(i, other.get(i));
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
      // The & 0x01 only seems to be needed for logical not
      old ^= (-(val & 0x01) ^ old) & (1 << bit_num);

      bits[byte_num] = old;
    }

    unsigned char get(const int ind) const {
      int byte_num = ind / 8;
      int bit_num = ind % 8;

      unsigned char target_byte = bits[byte_num];
      return 0x01 & (target_byte >> bit_num);
    }

    inline bool equals(const bit_vector<N>& other) const {

      for (int i = 0; i < N; i++) {
	if (get(i) != other.get(i)) {
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


    unsigned_int(const bv_uint8 val) : bits(val) {}
    unsigned_int(const bv_uint16 val) : bits(val) {}
    unsigned_int(const bv_uint32 val) : bits(val) {}
    unsigned_int(const bv_uint64 val) : bits(val) {}

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

    static inline
    unsigned_int<Width>
    sub_general_width(const unsigned_int<Width>& a,
		      const unsigned_int<Width>& b) {
      unsigned_int<Width> diff;
      unsigned_int<Width> b_cpy = b;

      std::cout << "a = " << a << std::endl;
      std::cout << "b = " << b << std::endl;
      std::cout << "b_cpy = " << b_cpy << std::endl;

      for (int i = 0; i < Width; i++) {
	if ((a.get(i) == 0) &&
	    (b_cpy.get(i) == 1)) {
	  assert(false);
	} else if (a.get(i) == b.get(i)) {
	  diff.set(i, 0);
	} else {
	  assert(false);
	}
      }

      return diff;
    }    

    static inline
    unsigned_int<Width>
    add_general_width(const unsigned_int<Width>& a,
		      const unsigned_int<Width>& b) {

      unsigned_int<Width> res;
      unsigned char carry = 0;
      for (int i = 0; i < Width; i++) {
	unsigned char sum = a.get(i) + b.get(i) + carry;

	unsigned char z_i = sum & 0x01; //sum % 2;
	res.set(i, z_i);
	if (sum >= 2) {
	  carry = 1;
	}

      }

      return res;
    }

    template<int Q = Width>
    static inline
    typename std::enable_if<Q >= 65, unsigned_int<Q> >::type
    add(const unsigned_int<Width>& a,
	const unsigned_int<Width>& b) {
      return add_general_width(a, b);
    }

    template<int Q = Width>
    static inline
    typename std::enable_if<(33 <= Q) && (Q <= 64), unsigned_int<Q> >::type
    add(const unsigned_int<Width>& a,
	const unsigned_int<Width>& b) {

      //std::cout << "a = " << a.as_native_uint64() << std::endl;
      //std::cout << "b = " << b.as_native_uint64() << std::endl;
      bv_uint64 res = a.as_native_uint64() + b.as_native_uint64();

      return unsigned_int<Width>(res);
    }

    template<int Q = Width>
    static inline
    typename std::enable_if<(17 <= Q) && (Q <= 32), unsigned_int<Q> >::type
    add(const unsigned_int<Width>& a,
	const unsigned_int<Width>& b) {

      //std::cout << "a 32 bit = " << a.as_native_uint32() << std::endl;
      //std::cout << "b 32 bit = " << b.as_native_uint32() << std::endl;
      bv_uint32 res = a.as_native_uint32() + b.as_native_uint32();

      return unsigned_int<Width>(res);
    }
      
    template<int Q = Width>
    static inline
    typename std::enable_if<(9 <= Q) && (Q <= 16), unsigned_int<Q> >::type
    add(const unsigned_int<Width>& a,
	const unsigned_int<Width>& b) {

      //std::cout << "a 16 bit = " << a.as_native_uint16() << std::endl;
      //std::cout << "b 16 bit = " << b.as_native_uint16() << std::endl;
      bv_uint16 res = a.as_native_uint16() + b.as_native_uint16();

      return unsigned_int<Width>(res);
    }
      
    template<int Q = Width>
    static inline
    typename std::enable_if<(1 <= Q) && (Q <= 8), unsigned_int<Q> >::type
    add(const unsigned_int<Width>& a,
	const unsigned_int<Width>& b) {

      bv_uint8 res = +(a.as_native_uint8()) + +(b.as_native_uint8());

      return unsigned_int<Width>(res);
    }
      
  };

  template<int N>
  static inline unsigned_int<N> operator+(const unsigned_int<N>& a,
					  const unsigned_int<N>& b) {
    return unsigned_int_operations<N>::add(a, b);
  }

  template<int Width>
  class bit_vector_operations {
  public:

    template<int Q = Width>
    static inline
    typename std::enable_if<Q >= 65, bit_vector<Q> >::type
    land(const bit_vector<Width>& a,
	 const bit_vector<Width>& b) {
      bit_vector<Width> a_and_b;
      for (int i = 0; i < Width; i++) {
	a_and_b.set(i, a.get(i) & b.get(i));
      }
      return a_and_b;

    }

    template<int Q = Width>
    static inline
    typename std::enable_if<33 <= Q && Q <= 64, bit_vector<Q> >::type
    land(const bit_vector<Width>& a,
	 const bit_vector<Width>& b) {
      bv_uint64 a_and_b = a.as_native_uint64() & b.as_native_uint64();
      return bit_vector<Width>(a_and_b);
    }
    
    template<int Q = Width>
    static inline
    typename std::enable_if<17 <= Q && Q <= 32, bit_vector<Q> >::type
    land(const bit_vector<Width>& a,
	 const bit_vector<Width>& b) {
      bv_uint32 a_and_b = a.as_native_uint32() & b.as_native_uint32();
      return bit_vector<Width>(a_and_b);
    }
    
    template<int Q = Width>
    static inline
    typename std::enable_if<9 <= Q && Q <= 16, bit_vector<Q> >::type
    land(const bit_vector<Width>& a,
	 const bit_vector<Width>& b) {
      bv_uint16 a_and_b = a.as_native_uint16() & b.as_native_uint16();
      return bit_vector<Width>(a_and_b);
    }
    
    template<int Q = Width>
    static inline
    typename std::enable_if<1 <= Q && Q <= 8, bit_vector<Q> >::type
    land(const bit_vector<Width>& a,
	 const bit_vector<Width>& b) {
      bv_uint8 a_and_b = a.as_native_uint8() & b.as_native_uint8();
      return bit_vector<Width>(a_and_b);
    }



    static inline bit_vector<Width> lnot(const bit_vector<Width>& a) {
      bit_vector<Width> not_a;
      for (int i = 0; i < Width; i++) {
	not_a.set(i, ~a.get(i));
      }
      return not_a;

    }
      
    static inline bit_vector<Width> lor(const bit_vector<Width>& a,
					const bit_vector<Width>& b) {
      bit_vector<Width> a_or_b;
      for (int i = 0; i < Width; i++) {
	a_or_b.set(i, a.get(i) | b.get(i));
      }
      return a_or_b;

    }

    static inline
    bit_vector<Width>
    lxor(const bit_vector<Width>& a,
	 const bit_vector<Width>& b) {
      bit_vector<Width> a_or_b;
      for (int i = 0; i < Width; i++) {
	a_or_b.set(i, a.get(i) ^ b.get(i));
      }
      return a_or_b;

    }
    
  };

  template<int N>
  static inline bit_vector<N> operator~(const bit_vector<N>& a) {
    return bit_vector_operations<N>::lnot(a);
  }
  
  template<int N>
  static inline bit_vector<N> operator&(const bit_vector<N>& a,
					const bit_vector<N>& b) {
    return bit_vector_operations<N>::land(a, b);
  }

  template<int N>
  static inline bit_vector<N> operator|(const bit_vector<N>& a,
					const bit_vector<N>& b) {
    return bit_vector_operations<N>::lor(a, b);
  }

  template<int N>
  static inline bit_vector<N> operator^(const bit_vector<N>& a,
					const bit_vector<N>& b) {
    return bit_vector_operations<N>::lxor(a, b);
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
