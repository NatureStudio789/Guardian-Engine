#ifndef _GE_GUARDIANMATRIX_H_
#define _GE_GUARDIANMATRIX_H_
#include "../Vector/GuardianVector.h"

namespace GE
{
	class GUARDIAN_API GMatrix4x4
	{
	public:
		GMatrix4x4()
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					if (i == j)
					{
						this->Data[i][j] = 1.0f;
					}
					else
					{
						this->Data[i][j] = 0.0f;
					}
				}
			}
		}

		GMatrix4x4(const float data[4][4])
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					this->Data[i][j] = data[i][j];
				}
			}
		}

		GMatrix4x4 operator=(const GMatrix4x4& other)
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					this->Data[i][j] = other.Data[i][j];
				}
			}

			return (*this);
		}

		GMatrix4x4 operator+(const GMatrix4x4& other)
		{
			GMatrix4x4 result;

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					result.Data[i][j] = this->Data[i][j] + other.Data[i][j];
				}
			}

			return result;
		}

		GMatrix4x4 operator*(const GMatrix4x4& other)
		{
			GMatrix4x4 result;

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					result.Data[i][j] = 0.0f;

					for (int k = 0; k < 4; k++)
					{
						result.Data[i][j] += this->Data[i][k] * other.Data[k][j];
					}
				}
			}

			return result;
		}

		GVector3 operator*(const GVector3& other)
		{
			float newX = other.x * Data[0][0] + other.y * Data[1][0] + other.z * Data[2][0] + Data[3][0];
			float newY = other.x * Data[0][1] + other.y * Data[1][1] + other.z * Data[2][1] + Data[3][1];
			float newZ = other.x * Data[0][2] + other.y * Data[1][2] + other.z * Data[2][2] + Data[3][2];

			return GVector3(newX, newY, newZ);
		}

		GMatrix4x4 operator+=(const GMatrix4x4& other)
		{
			return ((*this) = (*this) + other);
		}

		GMatrix4x4 operator*=(const GMatrix4x4& other)
		{
			return ((*this) = (*this) * other);
		}

		static GMatrix4x4 Indentity();
		static GMatrix4x4 Inverse(const GMatrix4x4& m);
		static GMatrix4x4 Transpose(const GMatrix4x4& m);

		static GMatrix4x4 Translation(float tx, float ty, float tz);
		static GMatrix4x4 Translation(const GVector3& trans);

		static GMatrix4x4 RotationEular(float pitch, float yaw, float roll);
		static GMatrix4x4 RotationX(float angle);
		static GMatrix4x4 RotationY(float angle);
		static GMatrix4x4 RotationZ(float angle);
		static GMatrix4x4 RotationQuaternion(float x, float y, float z, float w);

		static GMatrix4x4 Scaling(float sx, float sy, float sz);
		static GMatrix4x4 Scaling(const GVector3& scale);

		static void Decompose(const GMatrix4x4& m, GVector3& translation, GVector3& rotation, GVector3& scale);

		static GMatrix4x4 Perspective(float fovAngleY, float aspectRatio, float nearZ, float farZ);
		static GMatrix4x4 LookAt(const GVector3& eyePosition,
			const GVector3& focusPosition, const GVector3& upDirection);

		float Data[4][4];
	};
}

#endif