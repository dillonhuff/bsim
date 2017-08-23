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

    SECTION("5 bytes to store 33 bits") {
      REQUIRE(NUM_BYTES(33) == 5);
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

    }
  }

  

}
