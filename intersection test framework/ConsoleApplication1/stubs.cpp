/**
  This testing framework has been developed/overhauled over time, primarily by:
  Chris Czyzewicz
  Ben Sunshine-Hill
  Cory Boatright 
  
  While the three were PhD students at UPenn (through 2013).  This particular version has some
  modifications by Cory since joining the faculty of Grove City College.
  
  Last revised 4/15/2015
*/

#include "stubs.h"

using namespace glm;

double Test_RaySphereIntersect(const vec4& P0, const vec4& V0, const mat4& T) {
	
	//NOTE: Assuming that we are using a unit sphere.

	//Invert T matrix;
	mat4 inverseT = inverse(T);
	//vec4 ray = P0 + V0 * t

	//Transform ray with inverse matrix
	vec4 rayOrigin = inverseT * P0; //Replaces P0
	vec4 rayDirection = inverseT * V0; //Replaces V0


	//Need to calculate A*t^2 + B*t + C = 0 and solve for 't'
	//A = A = V0.x^2 + V0.y^2 + V0.z^2
	//B = 2 * (V0.x * P0.x + V0.y * P0.y + V0.z * P0.z)
	//C = P0.x^2 + P0.y^2 + P0.z^2 - 1

	float dotDistance = dot(rayOrigin, rayOrigin);

	if(dotDistance <= 1)
		return -1;

	float A = rayDirection.x * rayDirection.x + rayDirection.y * rayDirection.y + rayDirection.z * rayDirection.z;
	float B = 2 * (rayDirection.x * rayOrigin.x + rayDirection.y * rayOrigin.y + rayDirection.z * rayOrigin.z);
	float C = (rayOrigin.x * rayOrigin.x + rayOrigin.y * rayOrigin.y + rayOrigin.z * rayOrigin.z) - 1;

	float discriminant = B*B - 4 * A * C;

	//Check if discriminant is less than zero. If so, return -1
	if(discriminant < 0.0)
		return -1;

	discriminant = sqrt(discriminant);

	//Use quadratic formula to solve for t
	float plus = (-0.5f)*(B + discriminant) / A;
	float minus = (-0.5f)*(B - discriminant) / A;

	//Check if both "t" values are positive
	if(plus > 0.0f && minus > 0.0f)
	{
		if(plus < minus)
			return plus;
		else
			return minus;
	}
	else if(plus > 0.0f && minus < 0.0f)
		return plus;
	else if(minus > 0.0f && plus < 0.0f)
		return minus;
	
	//Otherwise, there was no intersection
	return -1;
}

double Test_RayPolyIntersect(const vec4& P0, const vec4& V0, const vec4& p1, const vec4& p2, const vec4& p3, const mat4& T) {
	// TODO fill this in.
	// See the documentation of this function in stubs.h.

	return -1;
}

double Test_RayCubeIntersect(const vec4& P0, const vec4& V0, const mat4& T) {
	// TODO fill this in.
	// See the documentation of this function in stubs.h.

	return -1;
}