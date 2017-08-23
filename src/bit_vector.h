#pragma once

#include <bitset>
#include <cassert>
#include <iostream>

#define NUM_BYTES(N) (((N) / 8) + 1 - (((N) % 8 == 0)))

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
      for (int i = 0; i < NUM_BYTES(N); i++) {
	if (this->bits[i] != other.bits[i]) {
	  return false;
	}
      }

      return true;
    }

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

  template<int N>
  static inline bit_vector<N> operator&(const bit_vector<N>& a,
					const bit_vector<N>& b) {
    bit_vector<N> a_and_b;
    for (int i = 0; i < N; i++) {
      a_and_b.set(i, a.get(i) & b.get(i));
    }
    return a_and_b;
  }

  template<int N>
  static inline bool operator!=(const bit_vector<N>& a,
				const bit_vector<N>& b) {
    return !a.equals(b);
  }
  
  template<int N>
  static inline bool operator==(const bit_vector<N>& a,
				const bit_vector<N>& b) {
    return a.equals(b);
  }

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
}
