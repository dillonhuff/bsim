#include "catch.hpp"

#include "dynamic_bit_vector.h"

using namespace std;

namespace bsim {

  TEST_CASE("Dynamic vector comparison") {

    SECTION("3 bits") {
      dynamic_bit_vector b(3);
      b.set(0, 1);

      SECTION("Copy construct") {
	dynamic_bit_vector b_cpy(b);

	REQUIRE(b == b_cpy);
      }

      SECTION("Copy assign") {
	dynamic_bit_vector b_cpy = b;

	REQUIRE(b == b_cpy);
      }

      SECTION("bit by bit copy") {
	dynamic_bit_vector b_cpy(b);
	for (int i = 0; i < 3; i++) {
	  b_cpy.set(i, b.get(i));
	}

	REQUIRE(b == b_cpy);
      }
    }
  }

  TEST_CASE("Dynamic bitvector arithmetic") {

    SECTION("Setting bit vector values") {

      SECTION("Length 1 vector") {
	dynamic_bit_vector a(1);
	a.set(0, 1);

	REQUIRE(a.get(0) == 1);
      }

      SECTION("Length 37 vector") {
	dynamic_bit_vector a(37);
	a.set(14, 0);
	a.set(17, 1);
	a.set(36, 1);
	a.set(2, 0);

	REQUIRE(a.get(14) == 0);
	REQUIRE(a.get(17) == 1);
	REQUIRE(a.get(36) == 1);
	REQUIRE(a.get(2) == 0);
      }
      
    }

//     SECTION("Subtracting unsigned numbers") {

//       SECTION("3 bit numbers") {
// 	unsigned_int<3> a;
// 	a.set(2, 1);
	
// 	unsigned_int<3> b;
// 	b.set(0, 1);

// 	unsigned_int<3> correct;
// 	correct.set(0, 1);
// 	correct.set(1, 1);

// 	auto c =
// 	  unsigned_int_operations<3>::sub_general_width(a, b);

// 	REQUIRE(c == correct);
//       }

//       SECTION("8 bit case with overflow") {
// 	unsigned_int<8> a;
// 	a.set(7, 1);

// 	unsigned_int<8> b;
// 	b.set(7, 1);
// 	b.set(0, 1);

// 	unsigned_int<8> c =
// 	  unsigned_int_operations<8>::sub_general_width(a, b);

// 	bv_uint8 ac = 128;
// 	bv_uint8 bc = 129;

// 	bv_uint8 res = ac - bc;

// 	REQUIRE(res == c.as_native_uint8());
	
//       }

//       SECTION("Testing plus operator") {
// 	unsigned_int<230> a;
// 	a.set(220, 1);
// 	a.set(23, 1);
// 	a.set(3, 1);

// 	auto c = a - a;
// 	unsigned_int<230> zr;

// 	REQUIRE(c == zr);
//       }
//     }

//     SECTION("Adding unsigned numbers") {

//       SECTION("4 bit numbers") {
// 	unsigned_int<4> a;
// 	a.set(3, 1);
// 	a.set(0, 1);

// 	unsigned_int<4> b;
// 	b.set(0, 1);

// 	unsigned_int<4> c = a + b;

// 	REQUIRE(c.get(3) == 1);
// 	REQUIRE(c.get(2) == 0);
// 	REQUIRE(c.get(1) == 1);
// 	REQUIRE(c.get(0) == 0);
//       }

//       SECTION("7 bit add double overflow") {
// 	unsigned_int<7> a; // 1000000
// 	a.set(6, 1);

// 	unsigned_int<7> b;
// 	b.set(6, 1);       // 0100000

// 	auto a2 = a + a; // Should be zero
// 	auto a3 = a2 + a2;
// 	auto ares = (a3 + b) + b;

// 	auto a2_gen = 
// 	  unsigned_int_operations<7>::add_general_width(a, a);
// 	auto a3_gen = 
// 	  unsigned_int_operations<7>::add_general_width(a2_gen, a2_gen);
// 	auto a3_gen_res_1 =
// 	  unsigned_int_operations<7>::add_general_width(a3_gen, b);
// 	auto a3_gen_res =
// 	  unsigned_int_operations<7>::add_general_width(a3_gen_res_1, b);

// 	REQUIRE(ares == a3_gen_res);
//       }

//       SECTION("8 bit add overflow") {
// 	unsigned_int<8> a;
// 	a.set(0, 1);
// 	a.set(1, 1);
// 	a.set(2, 1);
// 	a.set(3, 1);
// 	a.set(4, 1);
// 	a.set(5, 1);
// 	a.set(6, 1);
// 	a.set(7, 1);

// 	unsigned_int<8> b;
// 	b.set(1, 1);

// 	unsigned_int<8> c = a + b;

// 	auto c_gen =
// 	  unsigned_int_operations<8>::add_general_width(a, b);
	
// 	REQUIRE(c_gen == c);
//       }

//       SECTION("15 bit numbers") {
// 	unsigned_int<15> a;
// 	a.set(2, 1);

// 	unsigned_int<15> b;
// 	b.set(5, 1);

// 	unsigned_int<15> res = a + b;

// 	unsigned_int<15> correct;
// 	correct.set(2, 1);
// 	correct.set(5, 1);

// 	REQUIRE(res == correct);
//       }

//       SECTION("32 bit numbers") {
// 	unsigned_int<32> a;
// 	a.set(0, 1);

// 	unsigned_int<32> b;
// 	b.set(0, 1);

// 	unsigned_int<32> c = a + b;

// 	unsigned_int<32> correct;
// 	correct.set(1, 1);

// 	REQUIRE(c == correct);
//       }
      
//       SECTION("137 bit numbers") {
// 	unsigned_int<137> a;
// 	a.set(136, 1);

// 	unsigned_int<137> b;
// 	b.set(136, 1);

// 	unsigned_int<137> c = a + b;

// 	REQUIRE(c.get(136) == 0);

//       }
//     }

//     SECTION("Unsigned division") {

//       SECTION("9 bit numbers") {
// 	unsigned_int<9> a("000000001");
// 	unsigned_int<9> b("100101001");

// 	REQUIRE(b / a == b);
//       }

//       SECTION("22 bit numbers") {
// 	unsigned_int<22> a((bv_uint32) 2345);
// 	unsigned_int<22> b((bv_uint32) 5);

// 	unsigned_int<22> expected((bv_uint32) 2345 / 5);

// 	cout << "Expected = " << expected << endl;
// 	cout << "Actual   = " << (b / a) << endl;

// 	REQUIRE(b / a == expected);
//       }
//     }

//     SECTION("Unsigned multiplication") {

//       SECTION("32 bit numbers") {
// 	unsigned_int<32> a;
// 	a.set(1, 1);
// 	a.set(7, 1);
// 	a.set(23, 1);

// 	unsigned_int<32> b;
// 	b.set(1, 1);
// 	b.set(8, 1);
// 	b.set(24, 1);

// 	unsigned_int<32> c =
// 	  unsigned_int_operations<32>::mul_general_width(a, b);

// 	bv_uint32 an = a.as_native_uint32();
// 	bv_uint32 bn = b.as_native_uint32();
// 	bv_uint32 cn = an * bn;

// 	REQUIRE(c == unsigned_int<32>(cn));
//       }

    SECTION("Logical and bit vectors") {

      SECTION("3 bit numbers") {
	dynamic_bit_vector a(3);
	a.set(0, 0);
	a.set(1, 1);
	a.set(2, 0);

	dynamic_bit_vector b(3);
	b.set(0, 1);
	b.set(1, 1);
	b.set(2, 0);

	dynamic_bit_vector c = a & b;

	dynamic_bit_vector expected(3);
	expected.set(0, 0);
	expected.set(1, 1);
	expected.set(2, 0);

	REQUIRE(c == expected);
      }

      SECTION("13 bit numbers") {
	dynamic_bit_vector a(13);
	a.set(0, 0);
	a.set(1, 1);
	a.set(2, 0);
	a.set(12, 1);

	dynamic_bit_vector b(13);
	b.set(0, 1);
	b.set(1, 1);
	b.set(2, 0);

	dynamic_bit_vector c = a & b;

	dynamic_bit_vector expected(13);
	expected.set(0, 0);
	expected.set(1, 1);
	expected.set(2, 0);

	REQUIRE(c == expected);
      }

      SECTION("32 bit numbers") {
	dynamic_bit_vector a(32);
	a.set(23, 1);
	a.set(4, 1);

	dynamic_bit_vector b(32);
	b.set(4, 1);
	b.set(9, 1);

	dynamic_bit_vector c = a & b;

	dynamic_bit_vector correct(32);
	correct.set(4, 1);

	REQUIRE(c == correct);
      }

      SECTION("41 bit numbers") {
	dynamic_bit_vector a(41);
	a.set(23, 1);
	a.set(9, 1);

	dynamic_bit_vector b(41);
	b.set(4, 1);
	b.set(9, 1);

	dynamic_bit_vector c = a & b;

	dynamic_bit_vector correct(41);
	correct.set(9, 1);

	REQUIRE(c == correct);
      }

    }

    SECTION("Logical or") {

      SECTION("3 bits") {
    	dynamic_bit_vector a(3);
    	a.set(0, 1);
    	a.set(2, 1);

    	dynamic_bit_vector b(3);
    	b.set(0, 1);

    	dynamic_bit_vector c = a | b;

    	dynamic_bit_vector correct(3);
    	correct.set(0, 1);
    	correct.set(2, 1);

    	REQUIRE(c == correct);
      }

      SECTION("1129 bits") {
    	dynamic_bit_vector a(1129);
    	a.set(1000, 1);
    	a.set(932, 1);
    	a.set(60, 1);

    	dynamic_bit_vector b(1129);
    	b.set(1001, 1);
    	b.set(932, 1);
    	b.set(287, 1);

    	dynamic_bit_vector c = a | b;

    	dynamic_bit_vector correct(1129);
    	correct.set(1001, 1);
    	correct.set(1000, 1);
    	correct.set(932, 1);
    	correct.set(287, 1);
    	correct.set(60, 1);

    	REQUIRE(c == correct);
      }
      
    }

    SECTION("Logical XOR") {

      SECTION("83 bit vectors") {
    	dynamic_bit_vector a(83);
    	a.set(82, 1);
    	a.set(60, 1);
    	a.set(13, 1);

    	dynamic_bit_vector b(83);
    	b.set(60, 1);

    	dynamic_bit_vector c = a ^ b;

    	dynamic_bit_vector correct(83);
    	correct.set(82, 1);
    	correct.set(13, 1);

    	REQUIRE(c == correct);
	
      }
    }

    SECTION("Logical not") {

      SECTION("77 bits") {
    	dynamic_bit_vector a(77);
    	a.set(49, 1);
    	a.set(12, 1);

    	dynamic_bit_vector c = ~a;

    	dynamic_bit_vector correct(77);
    	for (int i = 0; i < 77; i++) {
    	  if ((i != 49) && (i != 12)) {
    	    correct.set(i, 1);
    	  }
    	}

    	REQUIRE(c == correct);
	
      }
    }
  }

  TEST_CASE("Comparison") {

    SECTION("Greater than") {

      SECTION("7 bit numbers") {
	dynamic_bit_vector a(7, "0101000");
	dynamic_bit_vector b(7, "1101000");

	REQUIRE(b > a);
      }

      SECTION("9 bit numbers") {
	dynamic_bit_vector a(9, "100101010");
	dynamic_bit_vector b(9, "100101000");

	REQUIRE(!(b > a));
      }

    }

    SECTION("Less than") {

      SECTION("18 bit numbers") {
	dynamic_bit_vector a(17, "01001011110101010");
	dynamic_bit_vector b(17, "10001001101010011");


	REQUIRE(a < b);
      }
    }
		 
  }

}
    

//   TEST_CASE("Adding bit vectors with general add") {

//     dynamic_bit_vector<29> a("00000000000000000000011101011");
//     dynamic_bit_vector<29> b("00000000000000011001001011010");

//     dynamic_bit_vector<29> c =
//       add_general_width_bv(a, b);

//     REQUIRE(c.get(28) != 1);
//   }

