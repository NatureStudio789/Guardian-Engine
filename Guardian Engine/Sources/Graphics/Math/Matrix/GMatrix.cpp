#include "GMatrix.h"

namespace GE
{
	GMatrix::GMatrix()
	{
		this->Matrix = XMMatrixIdentity();
	}

	GMatrix::GMatrix(GVector4 r0, GVector4 r1, GVector4 r2, GVector4 r3)
	{
		this->Matrix = XMMatrixSet(
			r0.x, r0.y, r0.z, r0.w,
			r1.x, r1.y, r1.z, r1.w,
			r2.x, r2.y, r2.z, r2.w,
			r3.x, r3.y, r3.z, r3.w);
	}

	GMatrix::GMatrix(
		float m00, float m01, float m02, float m03, 
		float m10, float m11, float m12, float m13, 
		float m20, float m21, float m22, float m23, 
		float m30, float m31, float m32, float m33)
	{
		this->Matrix = XMMatrixSet(
			 m00,  m01,  m02,  m03,
			 m10,  m11,  m12,  m13,
			 m20,  m21,  m22,  m23,
			 m30,  m31,  m32,  m33);
	}

	GMatrix::GMatrix(XMMATRIX xmmatrix)
	{
		this->Matrix = xmmatrix;
	}

	GMatrix::GMatrix(const GMatrix& other)
	{
		this->Matrix = other.Matrix;
	}

	void GMatrix::Identity()
	{
		this->Matrix = XMMatrixIdentity();
	}

	void GMatrix::Inverse()
	{
		XMVECTOR Determinant;
		this->Matrix = XMMatrixInverse(&Determinant, this->Matrix);

		if (!XMVectorGetX(Determinant) != 0.0f)
		{
			throw GUARDIAN_ERROR_EXCEPTION("This matrix CANNOT be inversed!");
		}
	}

	void GMatrix::Transpose()
	{
		this->Matrix = XMMatrixTranspose(this->Matrix);
	}

	void GMatrix::Decompose(GVector3& translation, GVector4& quaternion, GVector3& scale) const
	{
		XMVECTOR Scale, Translation, Quaternion;
		if (!XMMatrixDecompose(&Scale, &Quaternion, &Translation, this->Matrix))
		{
			throw GUARDIAN_ERROR_EXCEPTION("This matrix is NOT a world transform matrix!");
		}

		XMFLOAT3 FTranslation, FScale;
		XMFLOAT4 FQuaternion;
		XMStoreFloat3(&FTranslation, Translation);
		XMStoreFloat3(&FScale, Scale);
		XMStoreFloat4(&FQuaternion, Quaternion);

		translation = { FTranslation.x, FTranslation.y, FTranslation.z };
		quaternion = { FQuaternion.x, FQuaternion.y, FQuaternion.z, FQuaternion.w };
		scale = { FScale.x, FScale.y, FScale.z };
	}

	GMatrix::operator const XMMATRIX& () const noexcept
	{
		return this->Matrix;
	}

	GMatrix::operator XMMATRIX& () noexcept
	{
		return this->Matrix;
	}

	GMatrix GMatrix::operator+(const GMatrix& matrix) const noexcept
	{
		return GMatrix(this->Matrix + matrix.Matrix);
	}

	GMatrix GMatrix::operator-(const GMatrix& matrix) const noexcept
	{
		return GMatrix(this->Matrix - matrix.Matrix);
	}

	GMatrix GMatrix::operator*(const GMatrix& matrix) const noexcept
	{
		return GMatrix(this->Matrix * matrix.Matrix);
	}

	GMatrix GMatrix::IdentityMatrix()
	{
		GMatrix m;
		m.Identity();

		return m;
	}

	GMatrix GMatrix::TranslationMatrix(const GVector3& translation)
	{
		return XMMatrixTranslation(translation.x, translation.y, translation.z);
	}

	GMatrix GMatrix::TranslationMatrix(float x, float y, float z)
	{
		return TranslationMatrix({ x, y, z });
	}

	GMatrix GMatrix::RotationXMatrix(float x)
	{
		return XMMatrixRotationX(GConverter::DegreeToAngle(x));
	}

	GMatrix GMatrix::RotationYMatrix(float y)
	{
		return XMMatrixRotationY(GConverter::DegreeToAngle(y));
	}

	GMatrix GMatrix::RotationZMatrix(float z)
	{
		return XMMatrixRotationZ(GConverter::DegreeToAngle(z));
	}

	GMatrix GMatrix::RotationPitchYawRollMatrix(const GVector3& rotation)
	{
		return XMMatrixRotationRollPitchYaw(
			GConverter::DegreeToAngle(rotation.x), GConverter::DegreeToAngle(rotation.y), GConverter::DegreeToAngle(rotation.z));
	}

	GMatrix GMatrix::RotationPitchYawRollMatrix(float x, float y, float z)
	{
		return RotationPitchYawRollMatrix({ x, y, z });
	}

	GMatrix GMatrix::ScalingMatrix(const GVector3& scale)
	{
		return XMMatrixScaling(scale.x, scale.y, scale.z);
	}

	GMatrix GMatrix::ScalingMatrix(float x, float y, float z)
	{
		return ScalingMatrix({ x, y, z });
	}
}
