
#ifndef SHARED_UTILS_H_INCLUDED
#define SHARED_UTILS_H_INCLUDED

#include <d3dx9.h>
#include <iostream>

namespace shared {
	namespace utils {
		static const float FP_ZERO = 0.000001f;
		static const D3DXVECTOR3 VEC3_ZERO(0.0f, 0.0f, 0.0f);

		inline float absMin(float a, float b) {
			return abs(a) < abs(b) ? a : b;
		}

		inline D3DXVECTOR3 Vec3ComponentMin(D3DXVECTOR3 a, D3DXVECTOR3 b) {
			return D3DXVECTOR3(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
		}

		inline D3DXVECTOR3 Vec3ComponentAbsMin(D3DXVECTOR3 a, D3DXVECTOR3 b) {
			return D3DXVECTOR3(absMin(a.x, b.x), absMin(a.y, b.y), absMin(a.z, b.z));
		}

		inline D3DXVECTOR3 Vec3MagnitudeMin(D3DXVECTOR3 a, D3DXVECTOR3 b) {
			return (D3DXVec3Length(&a) > D3DXVec3Length(&b)) ? a : b;
		}

		inline void printVec(D3DXVECTOR3 vec) {
			std::cout << "< " << vec.x << " , " << vec.y << " , " << vec.z << " >";
		}
	}
}

#endif