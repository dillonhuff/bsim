#include "catch.hpp"

#include "quad_value_bit_vector.h"

using namespace std;

namespace bsim {

  typedef quad_value_bit_vector dbv;

  TEST_CASE("Quad_Value vector comparison") {

    SECTION("3 bits") {
      quad_value_bit_vector b(3);
      b.set(0, 1);

      SECTION("Copy construct") {
	quad_value_bit_vector b_cpy(b);

	REQUIRE(b == b_cpy);
      }

      SECTION("Copy assign") {
	quad_value_bit_vector b_cpy = b;

	REQUIRE(b == b_cpy);
      }

      SECTION("bit by bit copy") {
	quad_value_bit_vector b_cpy(b);
	for (int i = 0; i < 3; i++) {
	  b_cpy.set(i, b.get(i));
	}

	REQUIRE(b == b_cpy);
      }
    }
  }

  TEST_CASE("Quad_Value bitvector initialization") {
    SECTION("Default initialization is zero") {
      int i = 23;

      quad_value_bit_vector a(i);

      REQUIRE(a.to_type<int>() == 0);
    }
    SECTION("1 initialization is zero") {
      int i = 1;

      quad_value_bit_vector a(16, 5);
      REQUIRE(a.to_type<uint64_t>() == 5);
    }

    SECTION("Default initialization is zero") {
      int i = 23;
      int len = 30;

      quad_value_bit_vector a(len, i);
      quad_value_bit_vector b = a;

      REQUIRE(a.to_type<int>() == i);
      REQUIRE(b.to_type<int>() == i);
    }

    SECTION("construct from uint8") {
      bv_uint8 i = 4;
      quad_value_bit_vector a(3, i);

      REQUIRE(a.to_type<bv_uint8>() == 4);
    }
    SECTION("construct from full binary string") {
      string s = "101";
      quad_value_bit_vector a(3, s);

      REQUIRE(a.to_type<bv_uint8>() == 5);
    }
    SECTION("construct from partial binary string") {
      string s = "101";
      quad_value_bit_vector a(10, s);

      REQUIRE(a.to_type<bv_uint8>() == 5);
    }

    SECTION("Copy constructor") {
      int i = 4;
      quad_value_bit_vector a(20, i);
      quad_value_bit_vector b(a);

      REQUIRE(b.bitLength() == 20);
      REQUIRE(b.to_type<int>() == 4);
    }

    SECTION("Assign from another bit vector") {
      int i = 4;
      quad_value_bit_vector a(20, i);
      quad_value_bit_vector b = a;

      cout << "b = " << b << endl;

      REQUIRE(b.bitLength() == 20);
      REQUIRE(b.to_type<int>() == 4);

      int j = i + 12;
      quad_value_bit_vector c(23, j);

      b = c;

      REQUIRE(b.bitLength() == 23);
    }

    SECTION("Initializing with explicit hex") {
      quad_value_bit_vector a("5'h3");
      quad_value_bit_vector b(5, "00011");

      REQUIRE(a == b);
    }

    SECTION("Initializing with explicit hex, ab") {
      quad_value_bit_vector a("8'hab");
      quad_value_bit_vector b(8, "10101011");

      REQUIRE(a == b);
    }

    SECTION("Initializing with explicit hex, cdef") {
      quad_value_bit_vector a("16'hcdef");
      quad_value_bit_vector b(16, "1100_1101_1110_1111");

      REQUIRE(a == b);
    }

    SECTION("Initializing with explicit hex, off width") {
      quad_value_bit_vector a("5'h18");
      quad_value_bit_vector b(5, "1_1000");

      REQUIRE(a == b);
    }
    
  }

  TEST_CASE("Quad value serialization") {

    SECTION("Print out as hex string") {
      quad_value_bit_vector a("5'h08");

      REQUIRE(a.hex_string() == "5'h08");
    }

    SECTION("Print out as binary string, with z values") {
      quad_value_bit_vector a(6, "11zxx0");

      REQUIRE(a.binary_string() == "11zxx0");
    }
    
    SECTION("Print out as hex string longer") {
      quad_value_bit_vector a("32'ha936d4c0");

      REQUIRE(a.hex_string() == "32'ha936d4c0");
    }

    SECTION("Print out as hex string longer") {
      quad_value_bit_vector a("32'ha9x6dzc0");

      REQUIRE(a.hex_string() == "32'ha9x6dzc0");
    }

    SECTION("Build z values as binary, then print as hex") {
      quad_value_bit_vector a(5, "1zzzz");

      REQUIRE(a.hex_string() == "5'h1z");
    }
    
    SECTION("Print out as hex string longer and not power of 4 width") {
      quad_value_bit_vector a("37'h14a936d4c0");

      SECTION("Bit length is 37") {
        REQUIRE(a.bitLength() == 37);
      }

      REQUIRE(a.hex_string() == "37'h14a936d4c0");
    }

    SECTION("Serialize and de-serialize") {
      quad_value_bit_vector a("39'h1ca236f4c0");

      quad_value_bit_vector b(a.hex_string());

      REQUIRE(a == b);
    }
    
  }

  TEST_CASE("Quad_Value bitvector arithmetic") {

    SECTION("Setting bit vector values") {

      SECTION("Length 1 vector") {
	quad_value_bit_vector a(1);
	a.set(0, 1);

	REQUIRE(a.get(0) == 1);
      }

      SECTION("Length 37 vector") {
	quad_value_bit_vector a(37);
	a.set(14, 0);
	a.set(17, 1);
	a.set(36, 1);
	a.set(2, 0);

	REQUIRE(a.get(14) == 0);
	REQUIRE(a.get(17) == 1);
	REQUIRE(a.get(36) == 1);
	REQUIRE(a.get(2) == 0);
      }

      SECTION("Bit vector with unknowns") {
        dbv a(5, "10x11");
        dbv b(5, "10111");

        REQUIRE( a != b);
      }
    }

    SECTION("Subtracting unsigned numbers") {

      SECTION("3 bit numbers") {
	quad_value_bit_vector a(3);
	a.set(2, 1);
	
	quad_value_bit_vector b(3);
	b.set(0, 1);

	quad_value_bit_vector correct(3);
	correct.set(0, 1);
	correct.set(1, 1);

	auto c =
	  sub_general_width_bv(a, b);

	REQUIRE(c == correct);
      }

      SECTION("Subtracting with unknowns") {
        dbv a(4, "1xx1");
        dbv b(4, "1100");

        auto c = sub_general_width_bv(a, b);

        REQUIRE(same_representation(c, dbv(4, "xxxx")));
      }

      SECTION("8 bit case with overflow") {
	quad_value_bit_vector a(8);
	a.set(7, 1);

	quad_value_bit_vector b(8);
	b.set(7, 1);
	b.set(0, 1);

	quad_value_bit_vector c =
	  sub_general_width_bv(a, b);

	bv_uint8 ac = 128;
	bv_uint8 bc = 129;

	bv_uint8 res = ac - bc;

	REQUIRE(res == c.as_native_uint8());
	
      }
    }

//       SECTION("Testing plus operator") {
// 	quad_value_bit_vector<230> a;
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

//     }

    SECTION("Unsigned multiplication") {

      SECTION("32 bit numbers") {
	quad_value_bit_vector a(32);
	a.set(1, 1);
	a.set(7, 1);
	a.set(23, 1);

	quad_value_bit_vector b(32);
	b.set(1, 1);
	b.set(8, 1);
	b.set(24, 1);

	quad_value_bit_vector c =
	  mul_general_width_bv(a, b);

	bv_uint32 an = a.as_native_uint32();
	bv_uint32 bn = b.as_native_uint32();
	bv_uint32 cn = an * bn;

	REQUIRE(c == quad_value_bit_vector(32, cn));
      }

      SECTION("Unknown values") {
        dbv a("32'hxx0fx1b3");
        dbv b("32'h11934891");

        REQUIRE(same_representation(mul_general_width_bv(a, b), dbv("32'hxxxxxxxx")));
      }
    }

    SECTION("Logical and bit vectors") {

      SECTION("x values") {
        dbv a(4, "x10x");
        dbv b(4, "10x0");

        REQUIRE(same_representation(a & b, dbv(4, "x000")));
      }

      SECTION("3 bit numbers") {
	quad_value_bit_vector a(3);
	a.set(0, 0);
	a.set(1, 1);
	a.set(2, 0);

	quad_value_bit_vector b(3);
	b.set(0, 1);
	b.set(1, 1);
	b.set(2, 0);

	quad_value_bit_vector c = a & b;

	quad_value_bit_vector expected(3);
	expected.set(0, 0);
	expected.set(1, 1);
	expected.set(2, 0);

	REQUIRE(c == expected);
      }

      SECTION("13 bit numbers") {
	quad_value_bit_vector a(13);
	a.set(0, 0);
	a.set(1, 1);
	a.set(2, 0);
	a.set(12, 1);

	quad_value_bit_vector b(13);
	b.set(0, 1);
	b.set(1, 1);
	b.set(2, 0);

	quad_value_bit_vector c = a & b;

	quad_value_bit_vector expected(13);
	expected.set(0, 0);
	expected.set(1, 1);
	expected.set(2, 0);

	REQUIRE(c == expected);
      }

      SECTION("32 bit numbers") {
	quad_value_bit_vector a(32);
	a.set(23, 1);
	a.set(4, 1);

	quad_value_bit_vector b(32);
	b.set(4, 1);
	b.set(9, 1);

	quad_value_bit_vector c = a & b;

	quad_value_bit_vector correct(32);
	correct.set(4, 1);

	REQUIRE(c == correct);
      }

      SECTION("41 bit numbers") {
	quad_value_bit_vector a(41);
	a.set(23, 1);
	a.set(9, 1);

	quad_value_bit_vector b(41);
	b.set(4, 1);
	b.set(9, 1);

	quad_value_bit_vector c = a & b;

	quad_value_bit_vector correct(41);
	correct.set(9, 1);

	REQUIRE(c == correct);
      }

    }

    SECTION("Addition") {
      quad_value_bit_vector a(33, 10);
      quad_value_bit_vector b(33, 23);

      REQUIRE(add_general_width_bv(a, b) == quad_value_bit_vector(33, 33));
    }

    SECTION("Bitwise or") {

      SECTION("Unknown values") {
        dbv a(6, "0x0xx1");
        dbv b(6, "x1xx0x");

        REQUIRE(same_representation(a | b, dbv(6, "x1xxx1")));
      }

      SECTION("3 bits") {
    	quad_value_bit_vector a(3);
    	a.set(0, 1);
    	a.set(2, 1);

    	quad_value_bit_vector b(3);
    	b.set(0, 1);

    	quad_value_bit_vector c = a | b;

    	quad_value_bit_vector correct(3);
    	correct.set(0, 1);
    	correct.set(2, 1);

    	REQUIRE(c == correct);
      }

      SECTION("1129 bits") {
    	quad_value_bit_vector a(1129);
    	a.set(1000, 1);
    	a.set(932, 1);
    	a.set(60, 1);

    	quad_value_bit_vector b(1129);
    	b.set(1001, 1);
    	b.set(932, 1);
    	b.set(287, 1);

    	quad_value_bit_vector c = a | b;

    	quad_value_bit_vector correct(1129);
    	correct.set(1001, 1);
    	correct.set(1000, 1);
    	correct.set(932, 1);
    	correct.set(287, 1);
    	correct.set(60, 1);

    	REQUIRE(c == correct);
      }
      
    }

    SECTION("Bitwise XOR") {

      SECTION("83 bit vectors") {
    	quad_value_bit_vector a(83);
    	a.set(82, 1);
    	a.set(60, 1);
    	a.set(13, 1);

    	quad_value_bit_vector b(83);
    	b.set(60, 1);

    	quad_value_bit_vector c = a ^ b;

    	quad_value_bit_vector correct(83);
    	correct.set(82, 1);
    	correct.set(13, 1);

    	REQUIRE(c == correct);
	
      }

      SECTION("Unknowns") {
        dbv a(3, "1x0");
        dbv b(3, "xxx");

        REQUIRE(same_representation(a ^ b, dbv(3, "xxx")));
      }
    }

    SECTION("Bitwise not") {

      SECTION("Unknowns") {
        dbv a(1, "x");
        REQUIRE(same_representation(~a, a));
      }

      SECTION("77 bits") {
    	quad_value_bit_vector a(77);
    	a.set(49, 1);
    	a.set(12, 1);

    	quad_value_bit_vector c = ~a;

    	quad_value_bit_vector correct(77);
    	for (int i = 0; i < 77; i++) {
    	  if ((i != 49) && (i != 12)) {
    	    correct.set(i, 1);
    	  }
    	}

    	REQUIRE(c == correct);
	
      }
    }
  }

  TEST_CASE("Quad value comparison") {

    SECTION("Equal and not equal") {
      SECTION("23 bit numbers") {
	quad_value_bit_vector a(23, "01010010101010101111001");
	quad_value_bit_vector b(23, "01010010101010101111001");
	quad_value_bit_vector c(23, "11010010101110101111001");

	REQUIRE(a == b);
	REQUIRE(a != c);
      }
    }

    SECTION("Greater than") {

      SECTION("7 bit numbers") {
	quad_value_bit_vector a(7, "0101000");
	quad_value_bit_vector b(7, "1101000");

	REQUIRE(b > a);
      }

      SECTION("9 bit numbers") {
	quad_value_bit_vector a(9, "100101010");
	quad_value_bit_vector b(9, "100101000");

	REQUIRE(!(b > a));
      }

    }

    SECTION("Less than") {

      SECTION("18 bit numbers") {
	quad_value_bit_vector a(17, "01001011110101010");
	quad_value_bit_vector b(17, "10001001101010011");


	REQUIRE(a < b);
      }
    }
		 
  }

  TEST_CASE("Quad value reduction operations") {

    SECTION("andr") {

      SECTION("All ones gives one") {
	dbv a(5, "11111");

	REQUIRE(andr(a) == dbv(1, "1"));
      }

      SECTION("Not all ones gives zero") {
	dbv a(7, "1101101");

	REQUIRE(andr(a) == dbv(1, "0"));
      }

    }

    SECTION("orr") {

      SECTION("All zeroes gives zero") {
	dbv a(5, "00000");

	REQUIRE(orr(a) == dbv(1, "0"));
      }

      SECTION("Not all zero gives one") {
	dbv a(7, "0001100");

	REQUIRE(orr(a) == dbv(1, "1"));
      }

    }

    SECTION("xorr") {

      SECTION("Even number of zeros gives zero") {
	dbv a(6, "111010");
	REQUIRE(xorr(a) == dbv(1, "0"));
      }

      SECTION("Odd number of zeros gives zero") {
	dbv a(9, "111010010");
	REQUIRE(xorr(a) == dbv(1, "1"));
      }

    }

  }

  TEST_CASE("Quad value signed comparison") {

    SECTION("Greater than") {

      SECTION("Both negative, a > b") {
	dbv a(6, "100100");
	dbv b(6, "100000");

	REQUIRE(signed_gt(a, b));
      }

      SECTION("Both negative, !(a > b)") {
	dbv a(6, "100100");
	dbv b(6, "110001");

	//REQUIRE(!(a > b));
	REQUIRE(!signed_gt(a, b));
      }

      SECTION("One negative and one positive") {
	dbv a(11, "10001010101");
	dbv b(11, "00000000000");

	//REQUIRE(b > a);
	REQUIRE(signed_gt(b, a));

	//REQUIRE(!(a > b));
	REQUIRE(!signed_gt(a, b));
      }

      SECTION("Both positive") {
	dbv a(12, "011010110101");
	dbv b(12, "010001001110");

	//REQUIRE(a > b);
	REQUIRE(signed_gt(a, b));
      }

    }

    SECTION("Greater than or equal to") {

      SECTION("Equal to") {
	dbv a(12, "011010110101");

	REQUIRE(signed_gte(a, a));

      }
    }
    
  }

  TEST_CASE("Quad value casting") {
    int j = 3;
    dbv a(65, j);

    SECTION("uint 8") {
      auto i = a.as_native_uint8();

      REQUIRE(i == 3);
    }
    
    SECTION("uint 16") {
      auto i = a.as_native_uint16();

      REQUIRE(i == 3);
    }
    
    SECTION("uint 32") {
      auto i = a.as_native_uint32();

      REQUIRE(i == 3);
    }

    SECTION("uint 64") {
      auto i = a.as_native_uint64();

      REQUIRE(i == 3);
    }

  }

  TEST_CASE("Quad value shifting") {
    dbv a(9, "001010110");

    // shift amount is 5
    dbv shift_amount(3, "101");

    SECTION("Shift left") {
      dbv res = shl(a, shift_amount);

      REQUIRE(res == dbv(9, "011000000"));
    }

    SECTION("lshr by bit vector width") {
      dbv a(16, "0010111011010100");
      dbv b(16, "0000000000010000");

      REQUIRE(lshr(a, b) == dbv(16, "0000000000000000"));
    }

    SECTION("Shift left zeros at end") {
      dbv b_man(23, "11001111111111000010001");

      cout << "b_man = " << b_man << endl;

      auto b_ext = extend(b_man, 4);

      cout << "b_ext = " << b_ext << endl;

      b_ext = shl(b_ext, quad_value_bit_vector(10, 3));

      cout << "b_ext = " << b_ext << endl;

      REQUIRE(b_ext.get(0) == 0);
      
      //	b_ext      = 010011001111111111000010001
    }

    SECTION("Arithmetic shift right with one") {
      dbv a(12, "100000100000");
      dbv shift(5, "00011");
      dbv res = ashr(a, shift);

      REQUIRE(res == dbv(12, "111100000100"));
    }

    SECTION("Arithmetic shift right by zero") {
      dbv a(33, "000000000000000000000000000000000");
      dbv b(33, "000000000000000000000000000000000");

      dbv res = ashr(a, b);

      REQUIRE(res == a);
    }



    SECTION("Arithmetic shift right with one") {
      dbv a(12, "010000100000");
      dbv shift(5, "00100");
      dbv res = ashr(a, shift);

      REQUIRE(res == dbv(12, "000001000010"));
    }

    SECTION("Arithmetic shift right by more than the base bit vector width") {
      dbv a(10, "1100000000");
      dbv shift(6, "100000");
      dbv res = ashr(a, shift);

      REQUIRE(res == dbv(10, "1111111111"));
    }
    
    SECTION("Logical shift right with one") {
      dbv a(12, "100000100000");
      dbv shift(5, "00011");
      dbv res = lshr(a, shift);

      REQUIRE(res == dbv(12, "000100000100"));
    }

    SECTION("Logical shift right by one") {
      dbv a(25, "0100000000000000000000000");
      dbv val(8, "00000001");

      REQUIRE(lshr(a, val) == dbv(25, "0010000000000000000000000"));
    }

    SECTION("Logical shift right with one") {
      dbv a(12, "010000100000");
      dbv shift(5, "00100");
      dbv res = lshr(a, shift);

      REQUIRE(res == dbv(12, "000001000010"));
    }
    
  }

  TEST_CASE("Testing subtraction") {
    dbv a(32, 347);
    dbv b(32, -347);

    REQUIRE(negate_general_width_bv(a) == b);
  }

}
