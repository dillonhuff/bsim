#pragma once

#include <bitset>

namespace bsim {

  template<int N>
  class bit_vector {
    std::bitset<N> bits;

  public:
    bit_vector() {
      for (int i = 0; i < N; i++) {
	set(i, 0);
      }
    }

    void set(const int ind, const unsigned char val) {
      bits[ind] = val;
    }

    unsigned char get(const int ind) const { return bits[ind]; }
  };

  template<int N>
  static inline bit_vector<N> operator+(const bit_vector<N>& a,
					const bit_vector<N>& b) {
    return a;
  }

}
