#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "bit_vector.h"

using namespace std;

namespace bsim {

  TEST_CASE("Num bytes test") {
    
    SECTION("1 byte to store 1 bit") {
      REQUIRE(NUM_BYTES(1) == 1);
    }

    SECTION("1 byte to store 8 bits") {
      REQUIRE(NUM_BYTES(8) == 1);
    }

    SECTION("2 bytes to store 13 bits") {
      REQUIRE(NUM_BYTES(13) == 2);
    }
    
    SECTION("2 bytes to store 16 bits") {
      REQUIRE(NUM_BYTES(16) == 2);
    }

    SECTION("4 bytes to store 17 bits") {
      REQUIRE(NUM_BYTES(16) == 2);
    }
    
    SECTION("4 bytes to store 32 bits") {
      REQUIRE(NUM_BYTES(32) == 4);
    }

    SECTION("8 bytes to store 33 bits") {
      REQUIRE(NUM_BYTES(33) == 8);
    }
    
    SECTION("8 bytes to store 64 bits") {
      REQUIRE(NUM_BYTES(64) == 8);
    }

    SECTION("9 bytes to store 65 bits") {
      REQUIRE(NUM_BYTES(65) == 9);
    }
  }

  TEST_CASE("Bit vector comparison") {

    SECTION("3 bits") {
      unsigned_int<3> b;
      b.set(0, 1);

      SECTION("Copy construct") {
	unsigned_int<3> b_cpy(b);

	REQUIRE(b == b_cpy);
      }

      SECTION("Copy assign") {
	unsigned_int<3> b_cpy = b;

	REQUIRE(b == b_cpy);
      }

      SECTION("bit by bit copy") {
	unsigned_int<3> b_cpy(b);
	for (int i = 0; i < 3; i++) {
	  b_cpy.set(i, b.get(i));
	}

	REQUIRE(b == b_cpy);
      }
    }
  }

  TEST_CASE("Bitvector arithmetic") {

    SECTION("Setting bit vector values") {

      SECTION("Length 1 vector") {
	bit_vector<1> a;
	a.set(0, 1);

	REQUIRE(a.get(0) == 1);
      }

      SECTION("Length 37 vector") {
	bit_vector<37> a;
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

    SECTION("Subtracting unsigned numbers") {

      SECTION("3 bit numbers") {
	unsigned_int<3> a;
	a.set(2, 1);
	
	unsigned_int<3> b;
	b.set(0, 1);

	unsigned_int<3> correct;
	correct.set(0, 1);
	correct.set(1, 1);

	auto c =
	  unsigned_int_operations<3>::sub_general_width(a, b);

	REQUIRE(c == correct);
      }

      SECTION("8 bit case with overflow") {
	unsigned_int<8> a;
	a.set(7, 1);

	unsigned_int<8> b;
	b.set(7, 1);
	b.set(0, 1);

	unsigned_int<8> c =
	  unsigned_int_operations<8>::sub_general_width(a, b);

	bv_uint8 ac = 128;
	bv_uint8 bc = 129;

	bv_uint8 res = ac - bc;

	REQUIRE(res == c.as_native_uint8());
	
      }

      SECTION("Testing plus operator") {
	unsigned_int<230> a;
	a.set(220, 1);
	a.set(23, 1);
	a.set(3, 1);

	auto c = a - a;
	unsigned_int<230> zr;

	REQUIRE(c == zr);
      }
    }

    SECTION("Adding unsigned numbers") {

      SECTION("4 bit numbers") {
	unsigned_int<4> a;
	a.set(3, 1);
	a.set(0, 1);

	unsigned_int<4> b;
	b.set(0, 1);

	unsigned_int<4> c = a + b;

	REQUIRE(c.get(3) == 1);
	REQUIRE(c.get(2) == 0);
	REQUIRE(c.get(1) == 1);
	REQUIRE(c.get(0) == 0);
      }

      SECTION("7 bit add double overflow") {
	unsigned_int<7> a; // 1000000
	a.set(6, 1);

	unsigned_int<7> b;
	b.set(6, 1);       // 0100000

	auto a2 = a + a; // Should be zero
	auto a3 = a2 + a2;
	auto ares = (a3 + b) + b;

	auto a2_gen = 
	  unsigned_int_operations<7>::add_general_width(a, a);
	auto a3_gen = 
	  unsigned_int_operations<7>::add_general_width(a2_gen, a2_gen);
	auto a3_gen_res_1 =
	  unsigned_int_operations<7>::add_general_width(a3_gen, b);
	auto a3_gen_res =
	  unsigned_int_operations<7>::add_general_width(a3_gen_res_1, b);

	REQUIRE(ares == a3_gen_res);
      }

      SECTION("8 bit add overflow") {
	unsigned_int<8> a;
	a.set(0, 1);
	a.set(1, 1);
	a.set(2, 1);
	a.set(3, 1);
	a.set(4, 1);
	a.set(5, 1);
	a.set(6, 1);
	a.set(7, 1);

	unsigned_int<8> b;
	b.set(1, 1);

	unsigned_int<8> c = a + b;

	auto c_gen =
	  unsigned_int_operations<8>::add_general_width(a, b);
	
	REQUIRE(c_gen == c);
      }

      SECTION("15 bit numbers") {
	unsigned_int<15> a;
	a.set(2, 1);

	unsigned_int<15> b;
	b.set(5, 1);

	unsigned_int<15> res = a + b;

	unsigned_int<15> correct;
	correct.set(2, 1);
	correct.set(5, 1);

	REQUIRE(res == correct);
      }

      SECTION("32 bit numbers") {
	unsigned_int<32> a;
	a.set(0, 1);

	unsigned_int<32> b;
	b.set(0, 1);

	unsigned_int<32> c = a + b;

	unsigned_int<32> correct;
	correct.set(1, 1);

	REQUIRE(c == correct);
      }
      
      SECTION("137 bit numbers") {
	unsigned_int<137> a;
	a.set(136, 1);

	unsigned_int<137> b;
	b.set(136, 1);

	unsigned_int<137> c = a + b;

	REQUIRE(c.get(136) == 0);

      }
    }

    SECTION("Unsigned multiplication") {

      SECTION("32 bit numbers") {
	unsigned_int<32> a;
	a.set(1, 1);
	a.set(7, 1);
	a.set(23, 1);

	unsigned_int<32> b;
	b.set(1, 1);
	b.set(8, 1);
	b.set(24, 1);

	unsigned_int<32> c =
	  unsigned_int_operations<32>::mul_general_width(a, b);

	bv_uint32 an = a.as_native_uint32();
	bv_uint32 bn = b.as_native_uint32();
	bv_uint32 cn = an * bn;

	REQUIRE(c == unsigned_int<32>(cn));
      }
		 
    }

    SECTION("Logical and bit vectors") {

      SECTION("3 bit numbers") {
	bit_vector<3> a;
	a.set(0, 0);
	a.set(1, 1);
	a.set(2, 0);

	bit_vector<3> b;
	b.set(0, 1);
	b.set(1, 1);
	b.set(2, 0);

	bit_vector<3> c = a & b;

	bit_vector<3> expected;
	expected.set(0, 0);
	expected.set(1, 1);
	expected.set(2, 0);

	REQUIRE(c == expected);
      }

      SECTION("13 bit numbers") {
	bit_vector<13> a;
	a.set(0, 0);
	a.set(1, 1);
	a.set(2, 0);
	a.set(12, 1);

	bit_vector<13> b;
	b.set(0, 1);
	b.set(1, 1);
	b.set(2, 0);

	bit_vector<13> c = a & b;

	bit_vector<13> expected;
	expected.set(0, 0);
	expected.set(1, 1);
	expected.set(2, 0);

	REQUIRE(c == expected);
      }

      SECTION("32 bit numbers") {
	bit_vector<32> a;
	a.set(23, 1);
	a.set(4, 1);

	bit_vector<32> b;
	b.set(4, 1);
	b.set(9, 1);

	bit_vector<32> c = a & b;

	bit_vector<32> correct;
	correct.set(4, 1);

	REQUIRE(c == correct);
      }

      SECTION("41 bit numbers") {
	bit_vector<41> a;
	a.set(23, 1);
	a.set(9, 1);

	bit_vector<41> b;
	b.set(4, 1);
	b.set(9, 1);

	bit_vector<41> c = a & b;

	bit_vector<41> correct;
	correct.set(9, 1);

	REQUIRE(c == correct);
      }

    }

    SECTION("Logical or") {

      SECTION("3 bits") {
	bit_vector<3> a;
	a.set(0, 1);
	a.set(2, 1);

	bit_vector<3> b;
	b.set(0, 1);

	bit_vector<3> c = a | b;

	bit_vector<3> correct;
	correct.set(0, 1);
	correct.set(2, 1);

	REQUIRE(c == correct);
      }

      SECTION("1129 bits") {
	bit_vector<1129> a;
	a.set(1000, 1);
	a.set(932, 1);
	a.set(60, 1);

	bit_vector<1129> b;
	b.set(1001, 1);
	b.set(932, 1);
	b.set(287, 1);

	bit_vector<1129> c = a | b;

	bit_vector<1129> correct;
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
	bit_vector<83> a;
	a.set(82, 1);
	a.set(60, 1);
	a.set(13, 1);

	bit_vector<83> b;
	b.set(60, 1);

	bit_vector<83> c = a ^ b;

	bit_vector<83> correct;
	correct.set(82, 1);
	correct.set(13, 1);

	REQUIRE(c == correct);
	
      }
    }

    SECTION("Logical not") {

      SECTION("77 bits") {
	bit_vector<77> a;
	a.set(49, 1);
	a.set(12, 1);

	bit_vector<77> c = ~a;

	bit_vector<77> correct;
	for (int i = 0; i < 77; i++) {
	  if ((i != 49) && (i != 12)) {
	    correct.set(i, 1);
	  }
	}

	REQUIRE(c == correct);
	
      }
    }

    SECTION("Signed numbers") {

      SECTION("Construct from signed integer") {
	int s = -230;

	signed_int<32> a(s);

	REQUIRE(a.to_type<int>() == s);
      }

      SECTION("Construct with sign extension") {
      }

      SECTION("Sign extending 6 bit number") {
	signed_int<6> a;
	a.set(0, 1);
	a.set(2, 1);
	a.set(5, 1);

	signed_int<8> b = sign_extend<6, 8>(a);

	REQUIRE(b.get(7) == 1);
	
      }

      SECTION("Addition") {

	SECTION("29 bit numbers") {
	}
      }
    }

  }

}
