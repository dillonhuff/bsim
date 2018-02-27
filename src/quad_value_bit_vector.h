#pragma once

#include <bitset>
#include <cassert>
#include <iostream>
#include <stdint.h>
#include <type_traits>

// This is a comment

#define GEN_NUM_BYTES(N) (((N) / 8) + 1 - (((N) % 8 == 0)))
#define NUM_BYTES(N) GEN_NUM_BYTES(N)

typedef int8_t  bv_sint8;
typedef int32_t  bv_sint32;

typedef uint8_t  bv_uint8;
typedef uint16_t bv_uint16;
typedef uint32_t bv_uint32;
typedef uint64_t bv_uint64;

namespace bsim {

  static std::string hex_digit_to_binary(const char hex_digit) {
    switch (hex_digit) {
    case '0':
      return "0000";
    case '1':
      return "0001";
    case '2':
      return "0010";
    case '3':
      return "0011";
    case '4':
      return "0100";
    case '5':
      return "0101";
    case '6':
      return "0110";
    case '7':
      return "0111";
    case '8':
      return "1000";
    case '9':
      return "1001";
    case 'a':
      return "1010";
    case 'b':
      return "1011";
    case 'c':
      return "1100";
    case 'd':
      return "1101";
    case 'e':
      return "1110";
    case 'f':
      return "1111";
      
    default:
      assert(false);
    }

    assert(false);
  }

  class quad_value {
  protected:
    unsigned char value;

  public:
    quad_value() : value(0) {}

    quad_value(unsigned char v) : value(v) {
      assert(v < 4);
    }

    unsigned char get_char() const {
      return value;
    }

    quad_value plus(const quad_value& other) const {
      assert(false);
    }
    
    bool equals(const quad_value& other) const {
      return false;
    }
  };

  static inline quad_value operator+(const quad_value& a,
                                     const quad_value& b) {
    return a.plus(b);
  }

  static inline quad_value operator&(const quad_value& a,
                                     const quad_value& b) {
    assert(false);
  }

  static inline quad_value operator|(const quad_value& a,
                                     const quad_value& b) {
    assert(false);
  }

  static inline quad_value operator^(const quad_value& a,
                                     const quad_value& b) {
    assert(false);
  }

  static inline bool operator>(const quad_value& a,
                               const quad_value& b) {
    assert(false);
  }

  static inline bool operator<(const quad_value& a,
                               const quad_value& b) {
    assert(false);
  }
  
  static inline quad_value operator~(const quad_value& a) {
    assert(false);
  }
  
  static inline bool operator==(const quad_value& a,
                                const quad_value& b) {
    return a.equals(b);
  }

  static inline bool operator!=(const quad_value& a,
                                const quad_value& b) {
    return !(a == b);
  }
  
  class quad_value_bit_vector {
    std::vector<quad_value> bits;
    int N;

  public:

    quad_value_bit_vector() : N(0) {}

    quad_value_bit_vector(const int N_) : N(N_) {
      //bits.resize(NUM_BYTES(N));
      bits.resize(N);
      for (uint i = 0; i < bits.size(); i++) {
	bits[i] = quad_value(0);
      }
      
      for (int i = 0; i < N; i++) {
	set(i, 0);
      }
    }

    quad_value_bit_vector(const std::string& str_raw) : N(0) {
      std::string bv_size = "";
      int ind = 0;
      while (str_raw[ind] != '\'') {
        assert(isdigit(str_raw[ind]));
        bv_size += str_raw[ind];
        ind++;
      }

      assert (str_raw[ind] == '\'');

      ind++;

      char format = str_raw[ind];

      assert((format == 'b') ||
             (format == 'h') ||
             (format == 'd'));

      ind++;

      std::string digits = "";
      while (ind < ((int) str_raw.size())) {
        digits += str_raw[ind];
        ind++;
      }

      int num_bits = stoi(bv_size);
      N = num_bits;
      //bits.resize(NUM_BYTES(num_bits));
      bits.resize(num_bits);
      for (int i = 0; i < ((int) bits.size()); i++) {
        bits[i] = 0;
      }

      // TODO: Check that digits are not too long

      assert(format == 'h');

      int bit_ind = 0;
      for (int i = digits.size() - 1; i >= 0; i--) {
        char hex_digit = digits[i];
        std::string hex_to_binary = hex_digit_to_binary(hex_digit);

        assert(hex_to_binary.size() == 4);

        int k = 0;
        for (int j = hex_to_binary.size() - 1; j >= 0; j--) {
          // Dont add past the end
          if ((bit_ind + k) < bitLength()) {
            set(bit_ind + k, hex_to_binary[j]);
            k++;
          } else {
            assert(hex_to_binary[j] == '0');
          }
        }
        bit_ind += 4;
      }

    }

    quad_value_bit_vector(const int N_, const std::string& str_raw) : N(N_) {
      int num_digits = 0;
      std::string str;
      for (int i = 0; i < ((int) str_raw.size()); i++) {
	if (isdigit(str_raw[i])) {
	  num_digits++;
	  str += str_raw[i];
	} else {
	  assert(str_raw[i] == '_');
	}
      }
      assert(num_digits <= N);

      int len = str.size();      
      //bits.resize(NUM_BYTES(N));
      bits.resize(N);
      for (int i = len - 1; i >= 0; i--) {
        unsigned char val = (str[i] == '0') ? 0 : 1;
        int ind = len - i - 1;
        set(ind, val);
      }
      for (int i = N-1; i>=len; i--) {
        set(i,0);
      }
    }

    quad_value_bit_vector(const int N_, const int val) : N(N_) {
      //bits.resize(NUM_BYTES(N));
      bits.resize(N);
      *((int*) (&(bits[0]))) = val;
    }

    std::string hex_string() {
      // std::string hex = std::to_string(N) + "'h";

      // for (int i = bits.size() - 1; i >= 0; i--) {
      //   char bit_h = bits[i] & 0x0f;
      //   char bit_l = (bits[i] >> 4) & 0x0f;

      //   hex += bit_l > 9 ? bit_l + 87 : bit_l + 48;
      //   hex += bit_h > 9 ? bit_h + 87 : bit_h + 48;
      // }
      // return hex;
      assert(false);
    }
    
    quad_value_bit_vector(const quad_value_bit_vector& other) {
      bits.resize(other.bits.size());
      N = other.bitLength();
      for (int i = 0; i < NUM_BYTES(N); i++) {
	bits[i] = other.bits[i];
      }
    }

    quad_value_bit_vector& operator=(const quad_value_bit_vector& other) {
      if (&other == this) {
    	return *this;
      }

      bits.resize(other.bits.size());

      N = other.bitLength();
      for (int i = 0; i < NUM_BYTES(N); i++) {
        bits[i] = other.bits[i];
      }


      return *this;
    }

    inline void set(const int ind, const int v) {
      assert((v == 0) || (v == 1));

      bits[ind] = quad_value(v);
    }

    inline void set(const int ind, const quad_value val) {
      //const unsigned char val) {
      // int byte_num = ind / 8;
      // int bit_num = ind % 8;

      // unsigned char old = bits[byte_num];
      // // The & 0x01 only seems to be needed for logical not
      // old ^= (-(val & 0x01) ^ old) & (1 << bit_num);

      //bits[byte_num] = old;
      bits[ind] = val;
    }

    quad_value get(const int ind) const {
      return bits[ind];
      // int byte_num = ind / 8;
      // int bit_num = ind % 8;

      // unsigned char target_byte = bits[byte_num];
      // return 0x01 & (target_byte >> bit_num);
    }

    inline bool equals(const quad_value_bit_vector& other) const {

      if (other.bitLength() != this->bitLength()) {
        return false;
      }

      for (int i = 0; i < N; i++) {
	if (get(i) != other.get(i)) {
	  return false;
	}
      }

      return true;
    }

    template<typename ConvType>
    ConvType to_type() const {
      ConvType tmp = *(const_cast<ConvType*>((const ConvType*) (&(bits[0]))));
      //TODO FIXME I am a sketchy hack.
      ConvType mask = sizeof(ConvType) > bits.size() ? (1<<N)-1 : -1; 
      return tmp & mask;
    }

    inline bv_uint64 as_native_int32() const {
      return to_type<bv_sint32>();
    }
    
    inline bv_uint64 as_native_uint64() const {
      return to_type<bv_uint64>();
    }

    inline bv_uint32 as_native_uint32() const {
      return to_type<bv_uint32>();
    }

    inline bv_uint16 as_native_uint16() const {
      return to_type<bv_uint16>();
    }

    inline bv_uint8 as_native_uint8() const {
      return to_type<bv_uint8>();
    }

    inline int bitLength() const {
      return N;
    }
    
  };

  static inline std::ostream& operator<<(std::ostream& out,
					 const quad_value_bit_vector& a) {
    const int N = a.bitLength();
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

  static inline bool operator==(const quad_value_bit_vector& a,
				const quad_value_bit_vector& b) {
    return a.equals(b);
  }

  static inline quad_value highBit(const quad_value_bit_vector& a) {
    return a.get(a.bitLength() - 1);
  }

  static inline
  quad_value_bit_vector
  add_general_width_bv(const quad_value_bit_vector& a,
  		       const quad_value_bit_vector& b) {

    quad_value_bit_vector res(a.bitLength());
    unsigned char carry = 0;
    for (int i = 0; i < ((int) a.bitLength()); i++) {
      quad_value sum = a.get(i) + b.get(i) + carry;

      carry = 0;

      unsigned char z_i = sum.get_char() & 0x01; //sum % 2;
      res.set(i, quad_value(z_i));
      if (sum.get_char() >= 2) {
  	carry = 1;
      }

    }

    return res;
  }

  static inline
  quad_value_bit_vector
  sub_general_width_bv(const quad_value_bit_vector& a,
  		       const quad_value_bit_vector& b) {
    int Width = a.bitLength();
    quad_value_bit_vector diff(a.bitLength());
    quad_value_bit_vector a_cpy = a;

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

  static inline
  quad_value_bit_vector
  mul_general_width_bv(const quad_value_bit_vector& a,
  		       const quad_value_bit_vector& b) {
    int Width = a.bitLength();
    quad_value_bit_vector full_len(2*Width);

    for (int i = 0; i < Width; i++) {
      if (b.get(i) == 1) {

  	quad_value_bit_vector shifted_a(2*Width);

  	for (int j = 0; j < Width; j++) {
  	  shifted_a.set(j + i, a.get(j));
  	}

  	full_len =
  	  add_general_width_bv(full_len, shifted_a);
      }
    }

    quad_value_bit_vector res(Width);
    for (int i = 0; i < Width; i++) {
      res.set(i, full_len.get(i));
    }
    return res;
  }    
  
  class quad_value_bit_vector_operations {
  public:

    static inline
    quad_value_bit_vector
    land(const quad_value_bit_vector& a,
  	 const quad_value_bit_vector& b) {
      quad_value_bit_vector a_and_b(a.bitLength());
      for (int i = 0; i < a.bitLength(); i++) {
  	a_and_b.set(i, a.get(i) & b.get(i));
      }
      return a_and_b;

    }

    static inline quad_value_bit_vector lnot(const quad_value_bit_vector& a) {
      quad_value_bit_vector not_a(a.bitLength());
      for (int i = 0; i < a.bitLength(); i++) {
  	not_a.set(i, ~a.get(i));
      }
      return not_a;

    }
      
    static inline quad_value_bit_vector lor(const quad_value_bit_vector& a,
					 const quad_value_bit_vector& b) {
      quad_value_bit_vector a_or_b(a.bitLength());
      for (int i = 0; i < a.bitLength(); i++) {
  	a_or_b.set(i, a.get(i) | b.get(i));
      }
      return a_or_b;
    }

    static inline
    quad_value_bit_vector
    lxor(const quad_value_bit_vector& a,
  	 const quad_value_bit_vector& b) {
      quad_value_bit_vector a_or_b(a.bitLength());
      for (int i = 0; i < a.bitLength(); i++) {
  	a_or_b.set(i, a.get(i) ^ b.get(i));
      }
      return a_or_b;

    }
    
  };

  static inline quad_value_bit_vector operator~(const quad_value_bit_vector& a) {
    return quad_value_bit_vector_operations::lnot(a);
  }
  
  static inline quad_value_bit_vector operator&(const quad_value_bit_vector& a,
					     const quad_value_bit_vector& b) {
    return quad_value_bit_vector_operations::land(a, b);
  }

  static inline quad_value_bit_vector operator|(const quad_value_bit_vector& a,
					     const quad_value_bit_vector& b) {
    return quad_value_bit_vector_operations::lor(a, b);
  }

  static inline quad_value_bit_vector operator^(const quad_value_bit_vector& a,
					     const quad_value_bit_vector& b) {
    return quad_value_bit_vector_operations::lxor(a, b);
  }

  static inline bool operator!=(const quad_value_bit_vector& a,
  				const quad_value_bit_vector& b) {
    return !a.equals(b);
  }

  static inline bool operator>(const quad_value_bit_vector& a,
  			       const quad_value_bit_vector& b) {
    int N = a.bitLength();
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

  static inline bool operator>=(const quad_value_bit_vector& a,
				const quad_value_bit_vector& b) {
    return (a > b) || (a == b);
  }
  
  static inline bool operator<(const quad_value_bit_vector& a,
  			       const quad_value_bit_vector& b) {
    if (a == b) { return false; }

    return !(a > b);
  }

  static inline quad_value_bit_vector
  andr(const quad_value_bit_vector& a) {
    for (int i = 0; i < a.bitLength(); i++) {
      if (a.get(i) != 1) {
	return quad_value_bit_vector(1, "0");
      }
    }

    return quad_value_bit_vector(1, "1");
  }

  static inline quad_value_bit_vector
  orr(const quad_value_bit_vector& a) {
    for (int i = 0; i < a.bitLength(); i++) {
      if (a.get(i) == 1) {
	return quad_value_bit_vector(1, "1");
      }
    }

    return quad_value_bit_vector(1, "0");
  }

  static inline quad_value_bit_vector
  xorr(const quad_value_bit_vector& a) {
    int numSet = 0;
    for (int i = 0; i < a.bitLength(); i++) {
      if (a.get(i) == 1) {
	numSet++;
      }
    }

    if ((numSet % 2) == 0) {
      return quad_value_bit_vector(1, "0");
    }

    return quad_value_bit_vector(1, "1");
  }
  
  static inline bool
  signed_gt(const quad_value_bit_vector& a,
	    const quad_value_bit_vector& b) {

    assert(a.bitLength() == b.bitLength());

    int N = a.bitLength();

    // a negative b non-negative
    if ((a.get(N - 1) == 1) && (b.get(N - 1) == 0)) {
      return false;
    }

    // b negative a non-negative
    if ((b.get(N - 1) == 1) && (a.get(N - 1) == 0)) {
      return true;
    }

    // Both negative or both non-negative
    //if ((a.get(N - 1) == 1) && (b.get(N - 1) == 1)) {

    for (int i = N - 2; i >= 0; i--) {
      if (a.get(i) > b.get(i)) {
  	return true;
      }

      if (a.get(i) < b.get(i)) {
  	return false;
      }
    }

    return false;

  }

  static inline bool signed_gte(const quad_value_bit_vector& a,
  				const quad_value_bit_vector& b) {
    return (signed_gt(a, b)) || (a == b);
  }

  static inline
  bv_uint64 get_shift_int(const quad_value_bit_vector& shift_amount) {
    bv_uint64 shift_int = 0;
    if (shift_amount.bitLength() > 64) {
      assert(false);
    }

    else if (shift_amount.bitLength() > 32) {
      shift_int = shift_amount.to_type<bv_uint64>();
    }

    else if (shift_amount.bitLength() > 16) {
      shift_int = (bv_uint64) (shift_amount.to_type<bv_uint32>());
    }

    else if (shift_amount.bitLength() > 8) {
      shift_int = (bv_uint64) (shift_amount.to_type<bv_uint16>());
    } else {
      shift_int = (bv_uint64) (shift_amount.to_type<bv_uint8>());
    }

    //std::cout << "shift_int = " << shift_int << std::endl;
    assert(shift_int < 65);

    return shift_int;
  }

  static inline quad_value_bit_vector
  lshr(const quad_value_bit_vector& a,
       const quad_value_bit_vector& shift_amount) {
    quad_value_bit_vector res(a.bitLength());

    bv_uint64 shift_int = get_shift_int(shift_amount);

    if (shift_int == 0) {
      return a;
    }

    //unsigned char sign_bit = a.get(a.bitLength() - 1);
    for (uint i = a.bitLength() - 1; i >= shift_int; i--) {
      res.set(i - shift_int, a.get(i));
    }

    for (uint i = a.bitLength() - 1; i >= (a.bitLength() - shift_int); i--) {
      res.set(i, 0);
    }

    return res;
  }

  // Arithmetic shift right
  static inline
  quad_value_bit_vector
  ashr(const quad_value_bit_vector& a,
       const quad_value_bit_vector& shift_amount) {

    if (shift_amount == quad_value_bit_vector(shift_amount.bitLength(), 0)) {
      return a;
    }

    quad_value_bit_vector res(a.bitLength());

    bv_uint64 shift_int = get_shift_int(shift_amount);

    quad_value sign_bit = a.get(a.bitLength() - 1);
    for (uint i = a.bitLength() - 1; i >= shift_int; i--) {
      res.set(i - shift_int, a.get(i));
    }

    for (uint i = a.bitLength() - 1; i >= (a.bitLength() - shift_int); i--) {
      res.set(i, sign_bit);
    }

    return res;
  }
  
  static inline
  quad_value_bit_vector
  shl(const quad_value_bit_vector& a,
      const quad_value_bit_vector& shift_amount) {
    quad_value_bit_vector res(a.bitLength());

    bv_uint64 shift_int = get_shift_int(shift_amount);    
    for (int i = shift_int; i < a.bitLength(); i++) {
      res.set(i, a.get(i - shift_int));
    }

    return res;
  }

  static inline
  quad_value_bit_vector
  concat(const quad_value_bit_vector& a,
	 const quad_value_bit_vector& b) {
    quad_value_bit_vector res(a.bitLength() + b.bitLength());
    for (int i = 0; i < a.bitLength(); i++) {
      res.set(i, a.get(i));
    }
    for (int i = 0; i < ((int) b.bitLength()); i++) {
      res.set(i + a.bitLength(), b.get(i));
    }

    return res;
  }
  
  static inline
  quad_value_bit_vector
  slice(const quad_value_bit_vector& a,
	const int start,
	const int end) {
    quad_value_bit_vector res(end - start);

    for (int i = 0; i < res.bitLength(); i++) {
      res.set(i, a.get(i + start));
    }
    return res;
  }
  

  static inline
  quad_value_bit_vector
  extend(const quad_value_bit_vector& a, const int extra_bits) {
    quad_value_bit_vector res(a.bitLength() + extra_bits);
    for (uint i = 0; i < a.bitLength(); i++) {
      res.set(i, a.get(i));
    }

    return res;
  }

}