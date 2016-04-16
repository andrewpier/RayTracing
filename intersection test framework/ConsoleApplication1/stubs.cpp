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

// Tries to find the intersection of a ray and a triangle.
// This is just like the above function, but it intersects the ray with a
// triangle instead. The parameters p1, p2, and p3 specify the three
// points of the triangle, in object space.

double Test_RayPolyIntersect(const vec4& P0, const vec4& V0, const vec4& p1, const vec4& p2, const vec4& p3, const mat4& T) {
	// TODO fill this in.
	// See the documentation of this function in stubs.h.

	// Tries to find the intersection of a ray and a triangle.
	// This is just like the above function, but it intersects the ray with a
	// triangle instead. The parameters p1, p2, and p3 specify the three
	// points of the triangle, in object space.

	vec3 temp = vec3(P0.x,P0.y,P0.z);
	vec3 rayOrig = vec3((inverse(T) * vec4(temp,1))); //transform E
	//make c* matrix to multiply D with
	mat4 cStar = mat4(vec4(1.0f,1.0f,1.0f,0.0f),vec4(1.0f,1.0f,1.0f,0.0f),vec4(1.0f,1.0f,1.0f,0.0f),vec4(1.0f,1.0f,1.0f,1.0f)); 

	temp = vec3(V0.x,V0.y,V0.z);
	vec3 rayDir = vec3(inverse(matrixCompMult(T,cStar)) * vec4(temp,0)); // put the ray into world space
	vec3 v1 = vec3(p3.x - p1.x, p3.y - p1.y, p3.z - p1.z); //prep for Normal vector calc
	vec3 v2 = vec3(p3.x - p2.x, p3.y - p2.y, p3.z - p2.z);
	vec3 normal = cross(v1,v2); //calculate the normal vector
	vec3 pt = rayOrig + rayDir; //P in the notes
	if (dot(normal, pt - rayOrig) == 0.0f) //t would be undefined
		return -1.0f;
	vec3 dotVec = vec3(p1.x - rayOrig.x, p1.y - rayOrig.y, p1.z - rayOrig.z);
	float t = dot(normal, dotVec)/dot(normal, pt - rayOrig); //Find the value for t
	if (t  < 0.0f) //the ray is parallel to the plane
		return -1;
	vec3 R = rayOrig + t*(pt - rayOrig);


	//is the point in the tri??
#pragma region Triangles!!!!
	mat3 R01 = mat3(p1.y, p1.z, 1.0f, p2.y, p2.z, 1.0F, p3.y, p3.z, 1.0f);
	mat3 R02 = mat3(p1.z, p1.x, 1.0f, p2.z, p2.x, 1.0F, p3.z, p3.x, 1.0f);
	mat3 R03 = mat3(p1.x, p1.y, 1.0f, p2.x, p2.y, 1.0F, p3.x, p3.y, 1.0f);
		 
	mat3 R11 = mat3(R.y, R.z, 1.0f, p2.y, p2.z, 1.0F, p3.y, p3.z, 1.0f);
	mat3 R12 = mat3(R.z, R.x, 1.0f, p2.z, p2.x, 1.0F, p3.z, p3.x, 1.0f);
	mat3 R13 = mat3(R.x, R.y, 1.0f, p2.x, p2.y, 1.0F, p3.x, p3.y, 1.0f);
		 					   									
	mat3 R21 = mat3(R.y, R.z, 1.0f, p3.y, p3.z, 1.0F, p1.y, p1.z, 1.0f);
	mat3 R22 = mat3(R.z, R.x, 1.0f, p3.z, p3.x, 1.0F, p1.z, p1.x, 1.0f);
	mat3 R23 = mat3(R.x, R.y, 1.0f, p3.x, p3.y, 1.0F, p1.x, p1.y, 1.0f);
		 					   									
	mat3 R31 = mat3(R.y, R.z, 1.0f, p1.y, p1.z, 1.0f, p2.y, p2.z, 1.0f);
	mat3 R32 = mat3(R.z, R.x, 1.0f, p1.z, p1.x, 1.0f, p2.z, p2.x, 1.0f);
	mat3 R33 = mat3(R.x, R.y, 1.0f, p1.x, p1.y, 1.0f, p2.x, p2.y, 1.0f);
#pragma endregion 

	//Rule of Sarrus
	float tri = determinant(R01) * determinant(R01) + determinant(R02)*determinant(R02) + determinant(R03)*determinant(R03);
	float tri1 = determinant(R11) * determinant(R11) + determinant(R12)*determinant(R12) + determinant(R13)*determinant(R13);
	float tri2 = determinant(R21) * determinant(R21) + determinant(R22)*determinant(R22) + determinant(R23)*determinant(R23);
	float tri3 = determinant(R31) * determinant(R31) + determinant(R32)*determinant(R32) + determinant(R33)*determinant(R33);

	float area =  sqrt(tri);
	float area1 = sqrt(tri1)/area;
	float area2 = sqrt(tri2)/area;
	float area3 = sqrt(tri3)/area;
	float res = area1 + area2 + area3;

	//check to see if its in bounds
	//
	if (res <= 1.0f && res >= 1.0f){
		return t;
	}
	else{
		return -1.0f;
	}
	return -1.0f;
}

double Test_RayCubeIntersect(const vec4& P0, const vec4& V0, const mat4& T) {
	// TODO fill this in.
	// See the documentation of this function in stubs.h.

	return -1;
}