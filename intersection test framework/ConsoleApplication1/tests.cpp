/**
  This testing framework has been developed/overhauled over time, primarily by:
  Chris Czyzewicz
  Ben Sunshine-Hill
  Cory Boatright 
  
  While the three were PhD students at UPenn (through 2013).  This particular version has some
  modifications by Cory since joining the faculty of Grove City College.
  
  Last revised 4/15/2015
*/

#include "tests.h"
#include "stubs.h"
#include "glm/glm.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>

using namespace glm;

void RunRaySphereTests();
void RunRayPolyTests();
void RunRayCubeTests();
void RunYourTests();
void RunGradingTests();

typedef bool (*TestFunc)();

int g_numTests = 0;
int g_numSuccessful = 0;

void ReportTest(std::string name, bool result);

template<typename T>
void RunTest(std::string name, T const& testValue, T const& expectedValue) {
	ReportTest(name, testValue == expectedValue);
}

template<>
void RunTest<double>(std::string name, double const& testValue, double const& expectedValue) {
	ReportTest(name, (std::abs(testValue-expectedValue) / std::abs(expectedValue)) < 1e-3);
}

void RunTests() {
	std::cout.sync_with_stdio(true);

	RunRaySphereTests();
	RunRayPolyTests();
	RunRayCubeTests();
	RunYourTests();
	RunGradingTests();

	std::cout << g_numSuccessful << " of " << g_numTests << " tests successful. ";
	if(g_numTests == g_numSuccessful) {
		std::cout << "A winner is you!";
	}
	std::cout << std::endl;
}

const double SQRT_HALF = 0.70710678; // square root of one half
const double SQRT_TWO = 1.41421356; // figure it out

const mat4 IDENTITY_MATRIX = mat4();
const mat4 DOUBLE_MATRIX(vec4(2.0f, 0.0f, 0.0f, 0.0f),
                         vec4(0.0f, 2.0f, 0.0f, 0.0f),
                         vec4(0.0f, 0.0f, 2.0f, 0.0f),
                         vec4(0.0f, 0.0f, 0.0f, 1.0f));
const mat4 TALLANDSKINNY_MATRIX(vec4(0.5f, 0.0f, 0.0f, 0.0f),
								vec4(0.0f, 2.0f, 0.0f, 0.0f),
								vec4(0.0f, 0.0f, 0.5f, 0.0f),
								vec4(0.0f, 0.0f, 0.0f, 1.0f));
const mat4 BACK5_MATRIX(vec4(1.0f, 0.0f, 0.0f, 0.0f),
                        vec4(0.0f, 1.0f, 0.0f, 0.0f),
                        vec4(0.0f, 0.0f, 1.0f, 0.0f),
                        vec4(0.0f, 0.0f, -5.0f, 1.0f));
const mat4 BACK5ANDTURN_MATRIX(vec4(SQRT_HALF, 0.0f, -SQRT_HALF, 0.0f),
                                  vec4(0.0f, 1.0f, 0.0f, 0.0f),
                                  vec4(SQRT_HALF, 0.0f, SQRT_HALF, 0.0f),
                                  vec4(0.0f, 0.0f, -5.0f, 1.0f));


const vec3 ZERO_VECTOR(0.0f, 0.0f, 0.0f);
const vec3 HALFX_VECTOR(0.5f, 0.0f, 0.0f);
const vec3 THIRDX_VECTOR(0.33333333333333333f, 0.0f, 0.0f);
const vec3 NEGX_VECTOR(-1.0f, 0.0f, 0.0f);
const vec3 NEGZ_VECTOR(0.0f, 0.0f, -1.0f);
const vec3 NEGY_VECTOR(0.0f, -1.0f, 0.0f);
const vec3 POSZ_VECTOR(0.0f, 0.0f, 1.0f);
const vec3 POSXPOSZ_VECTOR(1.0f, 0.0f, 1.0f);
const vec3 ZNEGTEN_VECTOR(0.0f, 0.0f, -10.0f);
const vec3 ZPOSTEN_VECTOR(0.0f, 0.0f, 10.0f);
const vec3 YPOSTEN_VECTOR(0.0f, 10.0f, 0.0f);
const vec3 XPOSTEN_VECTOR(10.0f, 0.0f, 0.0f);
const vec3 POSXNEGZ_NORM_VECTOR(SQRT_HALF, 0.0f, -SQRT_HALF);
const vec3 NEGFIVEOFIVE_VECTOR(-5.0f, 0.0f, 5.0f);

const vec4 POINT_N1N10(-1.0f, -1.0f, 0.0f, 1.0f);
const vec4 POINT_1N10(1.0f, -1.0f, 0.0f, 1.0f);
const vec4 POINT_010(0.0f, 1.0f, 0.0f, 1.0f);
const vec4 POINT_N2N10(-2.0f, -1.0f, 0.0f, 1.0f);
const vec4 POINT_2N10(2.0f, -1.0f, 0.0f, 1.0f);

const double TEN_KAZILLION = 1e26;

void RunRaySphereTests() {
	RunTest(
		"Easy sphere",
		Test_RaySphereIntersect(vec4(ZERO_VECTOR, 1.0f), vec4(NEGZ_VECTOR, 0.0f), BACK5_MATRIX),
		4.0);
		
	RunTest(
		"Offset a bit",
		Test_RaySphereIntersect(vec4(HALFX_VECTOR, 1.0f), vec4(NEGZ_VECTOR, 0.0f), BACK5_MATRIX),
		4.13397);

	RunTest(
		"What sphere",
		Test_RaySphereIntersect(vec4(ZNEGTEN_VECTOR, 1.0f), vec4(NEGZ_VECTOR, 0.0f), BACK5_MATRIX),
		-1.0);

	RunTest(
		"Looking back",
		Test_RaySphereIntersect(vec4(ZNEGTEN_VECTOR, 1.0f), vec4(POSZ_VECTOR, 0.0f), BACK5_MATRIX),
		4.0); // TODO change this to the right number 
		//I think the number should be "4.0"

	RunTest(
		"West pole",
		Test_RaySphereIntersect(vec4(ZERO_VECTOR, 1.0f), vec4(NEGZ_VECTOR, 0.0f), BACK5ANDTURN_MATRIX),
		4.0);

	RunTest(
		"Another angle",
		Test_RaySphereIntersect(vec4(NEGFIVEOFIVE_VECTOR, 1.0f), vec4(POSXNEGZ_NORM_VECTOR, 0.0f), IDENTITY_MATRIX),
		(5.0 * SQRT_TWO) - 1);
}

void RunRayPolyTests() {
	RunTest(
		"Hi, Tri",
		Test_RayPolyIntersect(vec4(POSZ_VECTOR, 1.0f), vec4(NEGZ_VECTOR, 0.0f), POINT_N1N10, POINT_1N10, POINT_010, IDENTITY_MATRIX),
		1.0);

	RunTest(
		"Bye, Tri",
		Test_RayPolyIntersect(vec4(POSXPOSZ_VECTOR, 1.0f), vec4(NEGZ_VECTOR, 0.0f), POINT_N1N10, POINT_1N10, POINT_010, IDENTITY_MATRIX),
		-1.0);

	RunTest(
		"Looking back",
		Test_RayPolyIntersect(vec4(POSZ_VECTOR, 1.0f), vec4(POSZ_VECTOR, 0.0f), POINT_N1N10, POINT_1N10, POINT_010, IDENTITY_MATRIX),
		-1.0);

	RunTest(
		"Flip it good",
		Test_RayPolyIntersect(vec4(POSZ_VECTOR, 1.0f), vec4(NEGZ_VECTOR, 0.0f), POINT_010, POINT_1N10, POINT_N1N10, IDENTITY_MATRIX),
		1.0);

	RunTest(
		"It moves",
		Test_RayPolyIntersect(vec4(ZERO_VECTOR, 1.0f), vec4(NEGZ_VECTOR, 0.0f), POINT_N1N10, POINT_1N10, POINT_010, BACK5ANDTURN_MATRIX),
		5.0);

	RunTest(
		"And turns",
		Test_RayPolyIntersect(vec4(HALFX_VECTOR, 1.0f), vec4(NEGZ_VECTOR, 0.0f), POINT_N2N10, POINT_2N10, POINT_010, BACK5ANDTURN_MATRIX),
		TEN_KAZILLION); // TODO change this to the right number
}

void RunRayCubeTests() {
	RunTest(
		"Behold the cube",
		Test_RayCubeIntersect(vec4(ZERO_VECTOR, 1.0f), vec4(NEGZ_VECTOR, 0.0f), BACK5_MATRIX),
		4.5);

	RunTest(
		"The cube abides",
		Test_RayCubeIntersect(vec4(THIRDX_VECTOR, 1.0f), vec4(NEGZ_VECTOR, 0.0f), BACK5_MATRIX),
		4.5);

	RunTest(
		"Cuuuube!",
		Test_RayCubeIntersect(vec4(NEGX_VECTOR, 1.0f), vec4(NEGZ_VECTOR, 0.0f), BACK5_MATRIX),
		-1.0);

	RunTest(
		"Looking sharp, edge",
		Test_RayCubeIntersect(vec4(ZERO_VECTOR, 1.0f), vec4(NEGZ_VECTOR, 0.0f), BACK5ANDTURN_MATRIX),
		5.0 - SQRT_HALF);

	RunTest(
		"Big cube",
		Test_RayCubeIntersect(vec4(ZPOSTEN_VECTOR, 1.0f), vec4(NEGZ_VECTOR, 0.0f), DOUBLE_MATRIX),
		9.0);

	RunTest(
		"Strafing the cube",
		Test_RayCubeIntersect(vec4(NEGFIVEOFIVE_VECTOR, 1.0f), vec4(POSXNEGZ_NORM_VECTOR, 0.0f), IDENTITY_MATRIX),
		TEN_KAZILLION); // TODO change this to the right number
}

void RunYourTests() {
	// It can be very useful to put tests of your own here. The unit tests above do NOT test everything!
}

void RunGradingTests() {
	// Leave this function alone; we'll put stuff in it for grading.
}

void ReportTest(std::string name, bool result) {
	std::cout << std::setfill('.') << std::setw(50) << std::left << name;
	std::cout << (result ? "SUCCESS" : "**FAILURE**") << std::endl;
	g_numTests++;
	if(result) {
		g_numSuccessful++;
	}
	else {
		// It can be very useful to put a breakpoint here
	}

}