
#ifndef SHARED_UTILS_H_INCLUDED
#define SHARED_UTILS_H_INCLUDED

#include <d3dx9.h>
#include <iostream>

#define DEBUG false

static const float PI = 3.1415926f;

inline std::ostream& operator <<(std::ostream& os, const D3DXVECTOR3 v) {
	return os << "<" << v.x << " , " << v.y << " , " << v.z << ">";
}

inline std::ostream& operator <<(std::ostream& os, const D3DXQUATERNION q) {
	return os << "<" << q.x << " , " << q.y << " , " << q.z << " , " << q.w << ">";
}

inline std::ostream& operator <<(std::ostream& os, const D3DXMATRIX mat) {
	return os << "MATRIX: " << std::endl << 
		"|| " << mat(0, 0) << " | " << mat(0, 1) << " | "  << mat(0, 2) << " | "  << mat(0, 3) << " || "  << std::endl <<
		"|| " << mat(1, 0) << " | " << mat(1, 1) << " | "  << mat(1, 2) << " | "  << mat(1, 3) << " || "  << std::endl <<
		"|| " << mat(2, 0) << " | " << mat(2, 1) << " | "  << mat(2, 2) << " | "  << mat(2, 3) << " || "  << std::endl <<
		"|| " << mat(3, 0) << " | " << mat(3, 1) << " | "  << mat(3, 2) << " | "  << mat(3, 3) << " || "  << std::endl;
}

namespace shared {
	namespace utils {

		static const float FP_ZERO = 0.000001f;
		static const D3DXVECTOR3 VEC3_ZERO(0.0f, 0.0f, 0.0f);

		inline float absMin(float a, float b) {
			return abs(a) < abs(b) ? a : b;
		};

		inline D3DXVECTOR3 Vec3ComponentMin(D3DXVECTOR3 a, D3DXVECTOR3 b) {
			return D3DXVECTOR3(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
		};

		inline D3DXVECTOR3 Vec3ComponentAbsMin(D3DXVECTOR3 a, D3DXVECTOR3 b) {
			return D3DXVECTOR3(absMin(a.x, b.x), absMin(a.y, b.y), absMin(a.z, b.z));
		};

		inline D3DXVECTOR3 Vec3MagnitudeMin(D3DXVECTOR3 a, D3DXVECTOR3 b) {
			return (D3DXVec3Length(&a) > D3DXVec3Length(&b)) ? a : b;
		};

		// *** RANDOM VARIABLE HELPERS *** //
		inline float rand_float() {
			return (float)(rand() % RAND_MAX) / (float)RAND_MAX;
		}
		
		inline float rand_float(float range) {
			return rand_float() * range;
		}

		inline float rand_float(float start, float range) {
			return rand_float() * range + start;
		}

		inline D3DXVECTOR3 Vec3RandNormalized() {
			D3DXVECTOR3 ret(rand_float() - 0.5f, rand_float() - 0.5f, rand_float() - 0.5f);
			return *D3DXVec3Normalize(&ret, &ret);
		}

		inline D3DXVECTOR3 Vec3RandRange(float range) {
			return Vec3RandNormalized() * (rand_float(range));
		}

		inline D3DXVECTOR3 Vec3RandRange(float start, float range) {
			return Vec3RandNormalized() * (rand_float(start, range));
		}
	}
}

#endif