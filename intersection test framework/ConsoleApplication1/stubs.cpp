/**
  This testing framework has been developed/overhauled over time, primarily by:
  Chris Czyzewicz
  Ben Sunshine-Hill
  Cory Boatright 
  
  While the three were PhD students at UPenn (through 2013).  This particular version has some
  modifications by Cory since joining the faculty of Grove City College.
  
  Last revised 4/15/2015


  Code added by: Andrew Pier, Sam Gill, Garth Brooks
*/

#include "stubs.h"
#include <limits>

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

	// Tries to find the intersection of a ray and a triangle.
	// This is just like the above function, but it intersects the ray with a
	// triangle instead. The parameters p1, p2, and p3 specify the three
	// points of the triangle, in object space.
		//Invert T matrix;
	mat4 inverseT = inverse(T);
	
	//Transform ray with inverse matrix
	vec4 rayOrigin    = inverseT * P0;    //Replaces P0
	vec4 rayDirection = inverseT * V0; //Replaces V0

	vec3 normal1 = vec3(p3.x - p1.x, p3.y - p1.y, p3.z - p1.z); 
	vec3 normal2 = vec3(p3.x - p2.x, p3.y - p2.y, p3.z - p2.z);

	vec3 normal = cross(normal1,normal2); //calculate the normal vector
	vec4 pt = rayOrigin + rayDirection; //P in the notes

	if (dot(normal, vec3(pt) - vec3(rayOrigin)) == 0.0f) 
		return -1.0f;

	vec3 vec = vec3(p1.x - rayOrigin.x, p1.y - rayOrigin.y, p1.z - rayOrigin.z);// dot product vector
	float t = dot(normal, vec)/dot(normal, vec3(pt) - vec3(rayOrigin)); //Find the value for t
	if (t  < 0.0f) // not gonna hit
		return -1;
	vec4 R = rayOrigin + t*(pt - rayOrigin);


#pragma region Triangles!!!!
	mat3 R01 = mat3(p1.y, p1.z, 1.0f, p2.y, p2.z, 1.0f, p3.y, p3.z, 1.0f);
	mat3 R02 = mat3(p1.z, p1.x, 1.0f, p2.z, p2.x, 1.0f, p3.z, p3.x, 1.0f);
	mat3 R03 = mat3(p1.x, p1.y, 1.0f, p2.x, p2.y, 1.0f, p3.x, p3.y, 1.0f);
		 
	mat3 R11 = mat3(R.y, R.z, 1.0f, p2.y, p2.z, 1.0f, p3.y, p3.z, 1.0f);
	mat3 R12 = mat3(R.z, R.x, 1.0f, p2.z, p2.x, 1.0f, p3.z, p3.x, 1.0f);
	mat3 R13 = mat3(R.x, R.y, 1.0f, p2.x, p2.y, 1.0f, p3.x, p3.y, 1.0f);
		 					   									
	mat3 R21 = mat3(R.y, R.z, 1.0f, p3.y, p3.z, 1.0f, p1.y, p1.z, 1.0f);
	mat3 R22 = mat3(R.z, R.x, 1.0f, p3.z, p3.x, 1.0f, p1.z, p1.x, 1.0f);
	mat3 R23 = mat3(R.x, R.y, 1.0f, p3.x, p3.y, 1.0f, p1.x, p1.y, 1.0f);
		 					   									
	mat3 R31 = mat3(R.y, R.z, 1.0f, p1.y, p1.z, 1.0f, p2.y, p2.z, 1.0f);
	mat3 R32 = mat3(R.z, R.x, 1.0f, p1.z, p1.x, 1.0f, p2.z, p2.x, 1.0f);
	mat3 R33 = mat3(R.x, R.y, 1.0f, p1.x, p1.y, 1.0f, p2.x, p2.y, 1.0f);
#pragma endregion 

	//Rule of Sarrus
	float tri =  determinant(R01) * determinant(R01) + determinant(R02) * determinant(R02) + determinant(R03) * determinant(R03);
	float tri1 = determinant(R11) * determinant(R11) + determinant(R12) * determinant(R12) + determinant(R13) * determinant(R13);
	float tri2 = determinant(R21) * determinant(R21) + determinant(R22) * determinant(R22) + determinant(R23) * determinant(R23);
	float tri3 = determinant(R31) * determinant(R31) + determinant(R32) * determinant(R32) + determinant(R33) * determinant(R33);

	float area =  sqrt(tri);
	float area1 = sqrt(tri1)/area;
	float area2 = sqrt(tri2)/area;
	float area3 = sqrt(tri3)/area;
	float res = area1 + area2 + area3;

	//check to see if its in bounds
	if (res <= 1.0f && res >= 1.0f){
		return t;
	}
	return -1.0f;
}


float maximum( float a, float b, float c )
{
   float max = ( a < b ) ? b : a;
   return ( ( max < c ) ? c : max );
}
float minimum( float a, float b, float c )
{	
   float max = ( a > b ) ? b : a;
   return ( ( max > c ) ? c : max );
}
double Test_RayCubeIntersect(const vec4& P0, const vec4& V0, const mat4& T) {
	// TODO fill this in.
	// See the documentation of this function in stubs.h.

	//Invert T matrix;
	mat4 inverseT = inverse(T);
	
	//Transform ray with inverse matrix
	vec4 rayOrigin    = inverseT * P0;    //Replaces P0
	vec4 rayDirection = inverseT * V0; //Replaces V0
		
	//rayOrigin    = T * P0;    //Replaces P0
	//rayDirection = T * V0; //Replaces V0
	float signOfx;
	float signOfy;
	float signOfz;

	if (rayOrigin.x <= 0) signOfx = 1; else signOfx = -1;
	if (rayOrigin.y <= 0) signOfy = 1; else signOfy = -1;
	if (rayOrigin.z <= 0) signOfz = 1; else signOfz = -1;


	float nearx = (signOfx * -0.5 - rayOrigin.x )/(rayDirection.x);
	float neary = (signOfy * -0.5 - rayOrigin.y )/(rayDirection.y);
	float nearz = (signOfz * -0.5 - rayOrigin.z )/(rayDirection.z);

	float farx  = (signOfx * +0.5 - rayOrigin.x )/(rayDirection.x);
	float fary  = (signOfy * +0.5 - rayOrigin.y )/(rayDirection.y);
	float farz  = (signOfz * +0.5 - rayOrigin.z )/(rayDirection.z);


	
	if (rayDirection.x == 0 && abs(rayOrigin.x) <= 0.5) {
		nearx = INT_MIN;
		farx  = INT_MAX;
	}

	if (rayDirection.y == 0 && abs(rayOrigin.y) <= 0.5) {
		neary = INT_MIN;
		fary  = INT_MAX;
	}

	if (rayDirection.z == 0 && abs(rayOrigin.y) <= 0.5) {
		nearz = INT_MIN;
		farz  = INT_MAX;
	}

	float near = maximum(nearx, neary, nearz);
	float far  = minimum(farx , fary , farz );

	if (near <= far)
		return near;

	return -1;
}