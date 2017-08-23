#pragma once

#include <bitset>

namespace bsim {

  template<int N>
  class bit_vector {
    unsigned char bits[(N / 8) + 1];

  public:
    bit_vector() {
      for (int i = 0; i < N; i++) {
	set(i, 0);
      }
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

    // ~bit_vector() {
    //   delete bits;
    // }
  };

  template<int N>
  class unsigned_int {
  protected:
    bit_vector<N> bits;
  public:
    void set(const int ind, const unsigned char val) {
      bits.set(ind, val);
    }

    unsigned char get(const int ind) const { return bits.get(ind); }
    
  };

  template<int N>
  static inline unsigned_int<N> operator+(const unsigned_int<N>& a,
					  const unsigned_int<N>& b) {
    unsigned_int<N> res;
    unsigned char carry = 0;
    for (int i = 0; i < N; i++) {
      res.set(i, 0x01 & (carry + a.get(i) + b.get(i)));
      carry = a.get(i) & b.get(i);
    }
    return res;
  }

}
