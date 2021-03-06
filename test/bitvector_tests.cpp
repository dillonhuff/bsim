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

    SECTION("Unsigned division") {

      SECTION("9 bit numbers") {
    	unsigned_int<9> a("000000001");
    	unsigned_int<9> b("100101001");

    	REQUIRE(b / a == b);
      }

      SECTION("22 bit numbers") {
    	unsigned_int<22> a((bv_uint32) 2345);
    	unsigned_int<22> b((bv_uint32) 5);

    	unsigned_int<22> expected((bv_uint32) 2345 / 5);

    	cout << "Expected = " << expected << endl;
    	cout << "Actual   = " << (b / a) << endl;

    	REQUIRE(a / b == expected);
      }
    }

    // SECTION("Signed division") {
    //   int val_a = -23;
    //   signed_int<32> a(val_a);

    //   int val_b = 7;
    //   signed_int<32> b(val_b);

    //   signed_int<32> res = a / b;

    //   REQUIRE(res.to_type<int>() == (-23 / 7));
    // }

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

      SECTION("Greater than") {

	SECTION("7 bit numbers") {
	  unsigned_int<7> a("0101000");
	  unsigned_int<7> b("1101000");

	  REQUIRE(b > a);
	}

	SECTION("9 bit numbers") {
	  unsigned_int<9> a("100101010");
	  unsigned_int<9> b("100101000");

	  REQUIRE(!(b > a));
	}

      }

      SECTION("Less than") {

	SECTION("18 bit numbers") {
	  unsigned_int<17> a("01001011110101010");
	  unsigned_int<17> b("10001001101010011");


	  REQUIRE(a < b);
	}
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
	bv_sint32 s = -230;

	signed_int<32> a(s);

	REQUIRE(a.as_native_int32() == s);
      }

      SECTION("Construct with sign extension") {
	bv_sint32 s = -230;

	signed_int<12> a(s);

	REQUIRE(a.as_native_int32() == s);
      }

      SECTION("Sign extending negative 6 bit number") {
	signed_int<6> a;
	a.set(0, 1);
	a.set(2, 1);
	a.set(5, 1);

	signed_int<8> b = sign_extend<6, 8>(a);

	REQUIRE(b.get(7) == 1);
	
      }

      SECTION("Sign extending positive 6 bit number") {
	signed_int<6> a;
	a.set(0, 1);
	a.set(2, 1);

	signed_int<8> b = sign_extend<6, 8>(a);

	REQUIRE(b.get(7) == 0);
      }
      
      SECTION("Addition") {

	SECTION("29 bit numbers") {
	  int ai = 235;
	  int bi = 12890;

	  signed_int<29> a(ai);
	  signed_int<29> b(bi);

	  signed_int<29> res =
	    signed_int_operations<29>::add_general_width(a, b);

	  bv_sint32 resi = res.as_native_int32();
	  bv_sint32 int_sum = ai + bi;

	  cout << "result    =    " << res << endl;
	  cout << "resi      = " << bitset<32>(resi) << endl;
	  cout << "int sum   = " << bitset<32>(int_sum) << endl;

	  REQUIRE(res.as_native_int32() == (ai + bi));
	}
      }

      SECTION("Multiplication") {

	SECTION("29 bit numbers") {
	  int ai = 235;
	  int bi = -54;

	  signed_int<29> a(ai);
	  signed_int<29> b(bi);

	  signed_int<29> res =
	    signed_int_operations<29>::mul_general_width(a, b);

	  bv_sint32 resi = res.as_native_int32();
	  bv_sint32 int_sum = ai + bi;

	  REQUIRE(res.as_native_int32() == (ai * bi));
	}
      }

      SECTION("Subtraction") {
	int ai = -112;
	int bi = -984;

	signed_int<13> a(ai);
	signed_int<13> b(bi);

	signed_int<13> res =
	  signed_int_operations<13>::sub_general_width(a, b);

	cout << "a   = " << a << endl;
	cout << "b   = " << b << endl;
	cout << "res = " << res << endl;

	bv_sint32 diff = ai - bi;

	REQUIRE(res.as_native_int32() == diff);
      }

      SECTION("Greater than") {

	SECTION("Both negative, a > b") {
	  signed_int<6> a("100100");
	  signed_int<6> b("100000");

	  REQUIRE(a > b);
	}

	SECTION("Both negative, !(a > b)") {
	  signed_int<6> a("100100");
	  signed_int<6> b("110001");

	  REQUIRE(!(a > b));
	}

	SECTION("One negative and one positive") {
	  signed_int<11> a("10001010101");
	  signed_int<11> b("00000000000");

	  REQUIRE(b > a);

	  REQUIRE(!(a > b));
	}

	SECTION("Both positive") {
	  signed_int<12> a("011010110101");
	  signed_int<12> b("010001001110");

	  REQUIRE(a > b);
	}

      }

      SECTION("Greater than or equal to") {

	SECTION("Equal to") {
	  signed_int<12> a("011010110101");

	  REQUIRE(a >= a);

	}
      }

    }

  }

  TEST_CASE("Adding bit vectors with general add") {

    bit_vector<29> a("00000000000000000000011101011");
    bit_vector<29> b("00000000000000011001001011010");

    bit_vector<29> c =
      add_general_width_bv(a, b);

    REQUIRE(c.get(28) != 1);
  }

}
