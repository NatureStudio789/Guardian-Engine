#include "GMatrix.h"
#include "../Camera/Projection/GProjection.h"

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

	GMatrix::GMatrix(const float* data)
	{
		XMFLOAT4X4 float4x4(data);
		this->Matrix = XMLoadFloat4x4(&float4x4);
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

	void GMatrix::Decompose(GVector3& translation, GVector3& rotation, GVector3& scale) const
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
		GVector4 quaternion = { FQuaternion.x, FQuaternion.y, FQuaternion.z, FQuaternion.w };
		rotation = GVector4::QuaternionToEuler(quaternion);
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

	const GVector3 GMatrix::Multiply(const GMatrix& matrix, const GVector3& vector)
	{
		XMVECTOR Output = XMVector3Transform(XMVectorSet(vector.x, vector.y, vector.z, 0.0f), matrix);
		
		XMFLOAT3 FOutput;
		XMStoreFloat3(&FOutput, Output);

		return { FOutput.x, FOutput.y, FOutput.z };
	}

	const GVector3 GMatrix::MultiplyCoord(const GMatrix& matrix, const GVector3& vector)
	{
		XMVECTOR Output = XMVector3TransformCoord(XMVectorSet(vector.x, vector.y, vector.z, 0.0f), matrix);

		XMFLOAT3 FOutput;
		XMStoreFloat3(&FOutput, Output);

		return { FOutput.x, FOutput.y, FOutput.z };
	}

	const GVector3 GMatrix::MultiplyNormal(const GMatrix& matrix, const GVector3& vector)
	{
		XMVECTOR Output = XMVector3TransformNormal(XMVectorSet(vector.x, vector.y, vector.z, 0.0f), matrix);

		XMFLOAT3 FOutput;
		XMStoreFloat3(&FOutput, Output);

		return { FOutput.x, FOutput.y, FOutput.z };
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

	GMatrix GMatrix::RotationMatrix(const GVector3& rotation)
	{
		return RotationXMatrix(rotation.x) * RotationYMatrix(rotation.y) * RotationZMatrix(rotation.z);
	}

	GMatrix GMatrix::RotationMatrix(float x, float y, float z)
	{
		return RotationXMatrix(x) * RotationYMatrix(y) * RotationZMatrix(z);
	}

	GMatrix GMatrix::RotationXMatrix(float x)
	{
		return XMMatrixRotationX(GUtil::DegreeToAngle(x));
	}

	GMatrix GMatrix::RotationYMatrix(float y)
	{
		return XMMatrixRotationY(GUtil::DegreeToAngle(y));
	}

	GMatrix GMatrix::RotationZMatrix(float z)
	{
		return XMMatrixRotationZ(GUtil::DegreeToAngle(z));
	}

	GMatrix GMatrix::RotationPitchYawRollMatrix(const GVector3& rotation)
	{
		return XMMatrixRotationRollPitchYaw(
			GUtil::DegreeToAngle(rotation.x), GUtil::DegreeToAngle(rotation.y), GUtil::DegreeToAngle(rotation.z));
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

	GMatrix GMatrix::PerspectiveMatrix(const GPerspectiveProjection& projection)
	{
		return XMMatrixPerspectiveFovLH((projection.FOV / 360.0f) * XM_2PI, projection.Aspect, projection.NearZ, projection.FarZ);
	}

	GMatrix GMatrix::PerspectiveMatrix(float fovAngle, float aspect, float nearZ, float farZ)
	{
		return XMMatrixPerspectiveFovLH(fovAngle, aspect, nearZ, farZ);
	}

	GMatrix GMatrix::OrthographicsMatrix(const GOrthographicsProjection& projection)
	{
		return XMMatrixOrthographicLH(projection.ViewWidth, projection.ViewHeight, projection.NearZ, projection.FarZ);
	}

	GMatrix GMatrix::OrthographicsMatrix(float viewWidth, float viewHeight, float nearZ, float farZ)
	{
		return XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ);
	}

	GMatrix GMatrix::LookAtMatrix(const GVector3& eyePosition, const GVector3& target, const GVector3& upDirection)
	{
		XMFLOAT3 EyePosition = { eyePosition.x, eyePosition.y, eyePosition.z };
		XMFLOAT3 Target = { target.x, target.y, target.z };
		XMFLOAT3 UpDirection = { upDirection.x, upDirection.y, upDirection.z };

		return XMMatrixLookAtLH(XMLoadFloat3(&EyePosition), XMLoadFloat3(&Target), XMLoadFloat3(&UpDirection));
	}
}
