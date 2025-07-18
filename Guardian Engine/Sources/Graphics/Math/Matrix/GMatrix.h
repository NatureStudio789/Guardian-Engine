#ifndef _GE_GMATRIX_H_
#define _GE_GMATRIX_H_
#include "../Vector/GVector.h"

namespace GE
{
	class GUARDIAN_API GMatrix
	{
	public:
		GMatrix();
		GMatrix(GVector4 r0, GVector4 r1, GVector4 r2, GVector4 r3);
		GMatrix(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);
		GMatrix(XMMATRIX xmmatrix);
		GMatrix(const GMatrix& other);

		void Identity();
		void Inverse();
		void Transpose();
		void Decompose(GVector3& translation, GVector4& quaternion, GVector3& scale) const;

		operator const XMMATRIX& () const noexcept;
		operator XMMATRIX& () noexcept;
		GMatrix operator+(const GMatrix& matrix) const noexcept;
		GMatrix operator-(const GMatrix& matrix) const noexcept;
		GMatrix operator*(const GMatrix& matrix) const noexcept;

		static GMatrix IdentityMatrix();
		static GMatrix TranslationMatrix(const GVector3& translation);
		static GMatrix TranslationMatrix(float x, float y, float z);
		static GMatrix RotationXMatrix(float x);
		static GMatrix RotationYMatrix(float y);
		static GMatrix RotationZMatrix(float z);
		static GMatrix RotationPitchYawRollMatrix(const GVector3& rotation);
		static GMatrix RotationPitchYawRollMatrix(float x, float y, float z);
		static GMatrix ScalingMatrix(const GVector3& scale);
		static GMatrix ScalingMatrix(float x, float y, float z);

	private:
		XMMATRIX Matrix;
	};
}

#endif