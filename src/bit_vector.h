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

typedef int8_t  bv_sint8;
typedef int32_t  bv_sint32;

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

    bit_vector(const std::string& str) {
      assert(str.size() == N);

      for (int i = N - 1; i >= 0; i--) {
	unsigned char val = (str[i] == '0') ? 0 : 1;
	int ind = N - i - 1;
	set(ind, val);
      }
    }

    bit_vector(const int val) {
      *((int*) (&bits)) = val;
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

    template<typename ConvType>
    ConvType to_type() const {
      return *((ConvType*) (&bits));
    }

    inline bv_uint64 as_native_int32() const {
      return *((bv_sint32*) (&bits));
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

    unsigned_int(const std::string& bitstr) : bits(bitstr){}

    unsigned_int(const bit_vector<N>& bits_) : bits(bits_) {}

    unsigned_int(const bv_uint8 val) : bits(val) {}
    unsigned_int(const bv_uint16 val) : bits(val) {}
    unsigned_int(const bv_uint32 val) : bits(val) {}
    unsigned_int(const bv_uint64 val) : bits(val) {}

    void set(const int ind, const unsigned char val) {
      bits.set(ind, val);
    }

    bit_vector<N> get_bits() const { return bits; }    

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

  template<int N>
  class signed_int {
  protected:
    bit_vector<N> bits;

  public:
    signed_int() {}


    signed_int(const bit_vector<N>& bits_) : bits(bits_) {}

    signed_int(const int val) : bits(val) {}

    signed_int(const std::string& bitstr) : bits(bitstr) {}

    signed_int(const bv_uint8 val) : bits(val) {}
    signed_int(const bv_uint16 val) : bits(val) {}
    signed_int(const bv_uint32 val) : bits(val) {}
    signed_int(const bv_uint64 val) : bits(val) {}

    void set(const int ind, const unsigned char val) {
      bits.set(ind, val);
    }

    bit_vector<N> get_bits() const { return bits; }

    unsigned char get(const int ind) const { return bits.get(ind); }

    inline bool equals(const signed_int<N>& other) const {
      return (this->bits).equals((other.bits));
    }

    template<int HighWidth>
    signed_int<HighWidth> sign_extend() const {
      signed_int<HighWidth> hw;

      for (int i = 0; i < N; i++) {
	hw.set(i, get(i));
      }
    
      if (get(N - 1) == 0) {
      
	return hw;
      }

      for (int i = N; i < HighWidth; i++) {
	hw.set(i, 1);
      }

      return hw;
    }
    
    bv_sint32 as_native_int32() const {
      if (N < 32) {
	signed_int<32> extended = sign_extend<32>();

	bit_vector<32> bv = extended.get_bits();

	return bv.as_native_int32();
      }

      if (N == 32) {
	return get_bits().as_native_int32();
      }

      assert(false);
    }

    template<typename ConvType>
    ConvType to_type() const {
      return bits.template to_type<ConvType>();
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
      out << bits << "S";
      return out;
    }
    
  };

  template<int Width>
  static inline
  bit_vector<Width>
  add_general_width_bv(const bit_vector<Width>& a,
		       const bit_vector<Width>& b) {

    bit_vector<Width> res;
    unsigned char carry = 0;
    for (int i = 0; i < Width; i++) {
      unsigned char sum = a.get(i) + b.get(i) + carry;

      carry = 0;

      unsigned char z_i = sum & 0x01; //sum % 2;
      res.set(i, z_i);
      if (sum >= 2) {
	carry = 1;
      }

    }

    return res;
  }

  template<int Width>  
  static inline
  bit_vector<Width>
  mul_general_width_bv(const bit_vector<Width>& a,
		       const bit_vector<Width>& b) {
    bit_vector<2*Width> full_len;

    for (int i = 0; i < Width; i++) {
      if (b.get(i) == 1) {

	bit_vector<2*Width> shifted_a;

	for (int j = 0; j < Width; j++) {
	  shifted_a.set(j + i, a.get(j));
	}

	full_len =
	  add_general_width_bv(full_len, shifted_a);
      }
    }

    bit_vector<Width> res;
    for (int i = 0; i < Width; i++) {
      res.set(i, full_len.get(i));
    }
    return res;
  }    

  template<int Width>
  static inline
  bit_vector<Width>
  sub_general_width_bv(const bit_vector<Width>& a,
		       const bit_vector<Width>& b) {
    bit_vector<Width> diff;
    bit_vector<Width> a_cpy = a;

    bool underflow = false;
    for (int i = 0; i < Width; i++) {

      if ((a_cpy.get(i) == 0) &&
	  (b.get(i) == 1)) {

	int j = i + 1;

	diff.set(i, 1);	  

	// Modify to carry
	while ((j < Width) && (a_cpy.get(j) != 1)) {
	  a_cpy.set(j, 1);
	  j++;
	}

	if (j >= Width) {
	  underflow = true;
	} else {
	  a_cpy.set(j, 0);
	}

      } else if (a_cpy.get(i) == b.get(i)) {
	diff.set(i, 0);
      } else if ((a_cpy.get(i) == 1) &&
		 (b.get(i) == 0)) {
	diff.set(i, 1);
      } else {
	assert(false);
      }
    }

    return diff;
  }    
  
  template<int Width>
  class signed_int_operations {
  public:

    static inline
    signed_int<Width>
    add_general_width(const signed_int<Width>& a,
		      const signed_int<Width>& b) {

      bit_vector<Width> bits =
	add_general_width_bv(a.get_bits(), b.get_bits());

      signed_int<Width> c(bits);
      return c;
    }

    static inline
    signed_int<Width>
    mul_general_width(const signed_int<Width>& a,
		      const signed_int<Width>& b) {

      bit_vector<Width> bits =
	mul_general_width_bv(a.get_bits(), b.get_bits());

      signed_int<Width> c(bits);
      return c;
    }

    static inline
    signed_int<Width>
    sub_general_width(const signed_int<Width>& a,
		      const signed_int<Width>& b) {

      bit_vector<Width> bits =
	sub_general_width_bv(a.get_bits(), b.get_bits());

      signed_int<Width> c(bits);
      return c;
    }
    
  };  

  template<int Width>
  class unsigned_int_operations {
  public:


    template<int Q = Width>
    static inline
    typename std::enable_if<Q >= 65, unsigned_int<Q> >::type
    sub(const unsigned_int<Width>& a,
	const unsigned_int<Width>& b) {
      return sub_general_width(a, b);
    }

    static inline
    unsigned_int<Width>
    mul_general_width(const unsigned_int<Width>& a,
		      const unsigned_int<Width>& b) {
      bit_vector<Width> bits =
	mul_general_width_bv(a.get_bits(), b.get_bits());

      unsigned_int<Width> c(bits);
      return c;

    }    

    static inline
    unsigned_int<Width>
    sub_general_width(const unsigned_int<Width>& a,
		      const unsigned_int<Width>& b) {
      bit_vector<Width> bits =
	sub_general_width_bv(a.get_bits(), b.get_bits());

      unsigned_int<Width> c(bits);
      return c;

    }    
    
    static inline
    unsigned_int<Width>
    add_general_width(const unsigned_int<Width>& a,
		      const unsigned_int<Width>& b) {

      bit_vector<Width> bits =
	add_general_width_bv(a.get_bits(), b.get_bits());

      unsigned_int<Width> c(bits);
      return c;
      
      // unsigned_int<Width> res;
      // unsigned char carry = 0;
      // for (int i = 0; i < Width; i++) {
      // 	unsigned char sum = a.get(i) + b.get(i) + carry;

      // 	unsigned char z_i = sum & 0x01; //sum % 2;
      // 	res.set(i, z_i);
      // 	if (sum >= 2) {
      // 	  carry = 1;
      // 	}

      // }

      // return res;
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

  template<int N>
  static inline unsigned_int<N> operator-(const unsigned_int<N>& a,
					  const unsigned_int<N>& b) {
    return unsigned_int_operations<N>::sub(a, b);
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
  static inline bool operator==(const signed_int<N>& a,
				const signed_int<N>& b) {
    return a.bits() == b.bits();
  }

  template<int N>
  static inline bool operator!=(const unsigned_int<N>& a,
				const unsigned_int<N>& b) {
    return !(a == b);
  }
  
  template<int N>
  static inline bool operator>(const unsigned_int<N>& a,
			       const unsigned_int<N>& b) {
    for (int i = N - 1; i >= 0; i--) {
      if (a.get(i) > b.get(i)) {
	return true;
      }

      if (a.get(i) < b.get(i)) {
	return false;
      }
    }

    return false;
  }

  template<int N>
  static inline bool operator<(const unsigned_int<N>& a,
			       const unsigned_int<N>& b) {
    if (a == b) { return false; }

    return !(a > b);
  }

  template<int N>
  static inline bool operator>(const signed_int<N>& a,
			       const signed_int<N>& b) {
    if ((a.get(N - 1) == 1) && (b.get(N - 1) == 1)) {
      assert(false);
    }

    assert(false);
  }

  template<int N>
  static inline bool operator!=(const signed_int<N>& a,
				const signed_int<N>& b) {
    return !(a == b);
  }
  
  template<int N>
  static inline std::ostream&
  operator<<(std::ostream& out, const unsigned_int<N>& a) {
    a.print(out);
    return out;
  }

  template<int N>
  static inline std::ostream&
  operator<<(std::ostream& out, const signed_int<N>& a) {
    a.print(out);
    return out;
  }
  
  template<int LowWidth, int HighWidth>
  signed_int<HighWidth> sign_extend(const signed_int<LowWidth>& a) {
    signed_int<HighWidth> hw;

    for (int i = 0; i < LowWidth; i++) {
      hw.set(i, a.get(i));
    }
    
    if (a.get(LowWidth - 1) == 0) {
      
      return hw;
    }

    for (int i = LowWidth; i < HighWidth; i++) {
      hw.set(i, 1);
    }

    return hw;
  }
}
