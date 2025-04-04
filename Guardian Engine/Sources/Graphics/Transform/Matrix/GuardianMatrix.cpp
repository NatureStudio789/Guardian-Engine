#include "GuardianMatrix.h"

namespace GE
{
	GMatrix4x4 GMatrix4x4::Indentity()
	{
		GMatrix4x4 result;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (i == j)
				{
					result.Data[i][j] = 1.0f;
				}
				else
				{
					result.Data[i][j] = 0.0f;
				}
			}
		}

		return result;
	}

	GMatrix4x4 GMatrix4x4::Inverse(const GMatrix4x4& m)
	{
		float det;
		GMatrix4x4 inv;

		inv.Data[0][0] = m.Data[1][1] * m.Data[2][2] * m.Data[3][3] -
			m.Data[1][1] * m.Data[2][3] * m.Data[3][2] -
			m.Data[2][1] * m.Data[1][2] * m.Data[3][3] +
			m.Data[2][1] * m.Data[1][3] * m.Data[3][2] +
			m.Data[3][1] * m.Data[1][2] * m.Data[2][3] -
			m.Data[3][1] * m.Data[1][3] * m.Data[2][2];

		inv.Data[0][1] = -m.Data[0][1] * m.Data[2][2] * m.Data[3][3] +
			m.Data[0][1] * m.Data[2][3] * m.Data[3][2] +
			m.Data[2][1] * m.Data[0][2] * m.Data[3][3] -
			m.Data[2][1] * m.Data[0][3] * m.Data[3][2] -
			m.Data[3][1] * m.Data[0][2] * m.Data[2][3] +
			m.Data[3][1] * m.Data[0][3] * m.Data[2][2];

		inv.Data[0][2] = m.Data[0][1] * m.Data[1][2] * m.Data[3][3] -
			m.Data[0][1] * m.Data[1][3] * m.Data[3][2] -
			m.Data[1][1] * m.Data[0][2] * m.Data[3][3] +
			m.Data[1][1] * m.Data[0][3] * m.Data[3][2] +
			m.Data[3][1] * m.Data[0][2] * m.Data[1][3] -
			m.Data[3][1] * m.Data[0][3] * m.Data[1][2];

		inv.Data[0][3] = -m.Data[0][1] * m.Data[1][2] * m.Data[2][3] +
			m.Data[0][1] * m.Data[1][3] * m.Data[2][2] +
			m.Data[1][1] * m.Data[0][2] * m.Data[2][3] -
			m.Data[1][1] * m.Data[0][3] * m.Data[2][2] -
			m.Data[2][1] * m.Data[0][2] * m.Data[1][3] +
			m.Data[2][1] * m.Data[0][3] * m.Data[1][2];

		inv.Data[1][0] = -m.Data[1][0] * m.Data[2][2] * m.Data[3][3] +
			m.Data[1][0] * m.Data[2][3] * m.Data[3][2] +
			m.Data[2][0] * m.Data[1][2] * m.Data[3][3] -
			m.Data[2][0] * m.Data[1][3] * m.Data[3][2] -
			m.Data[3][0] * m.Data[1][2] * m.Data[2][3] +
			m.Data[3][0] * m.Data[1][3] * m.Data[2][2];

		inv.Data[1][1] = m.Data[0][0] * m.Data[2][2] * m.Data[3][3] -
			m.Data[0][0] * m.Data[2][3] * m.Data[3][2] -
			m.Data[2][0] * m.Data[0][2] * m.Data[3][3] +
			m.Data[2][0] * m.Data[0][3] * m.Data[3][2] +
			m.Data[3][0] * m.Data[0][2] * m.Data[2][3] -
			m.Data[3][0] * m.Data[0][3] * m.Data[2][2];

		inv.Data[1][2] = -m.Data[0][0] * m.Data[1][2] * m.Data[3][3] +
			m.Data[0][0] * m.Data[1][3] * m.Data[3][2] +
			m.Data[1][0] * m.Data[0][2] * m.Data[3][3] -
			m.Data[1][0] * m.Data[0][3] * m.Data[3][2] -
			m.Data[3][0] * m.Data[0][2] * m.Data[1][3] +
			m.Data[3][0] * m.Data[0][3] * m.Data[1][2];

		inv.Data[1][3] = m.Data[0][0] * m.Data[1][2] * m.Data[2][3] -
			m.Data[0][0] * m.Data[1][3] * m.Data[2][2] -
			m.Data[1][0] * m.Data[0][2] * m.Data[2][3] +
			m.Data[1][0] * m.Data[0][3] * m.Data[2][2] +
			m.Data[2][0] * m.Data[0][2] * m.Data[1][3] -
			m.Data[2][0] * m.Data[0][3] * m.Data[1][2];

		inv.Data[2][0] = m.Data[1][0] * m.Data[2][1] * m.Data[3][3] -
			m.Data[1][0] * m.Data[2][3] * m.Data[3][1] -
			m.Data[2][0] * m.Data[1][1] * m.Data[3][3] +
			m.Data[2][0] * m.Data[1][3] * m.Data[3][1] +
			m.Data[3][0] * m.Data[1][1] * m.Data[2][3] -
			m.Data[3][0] * m.Data[1][3] * m.Data[2][1];

		inv.Data[2][1] = -m.Data[0][0] * m.Data[2][1] * m.Data[3][3] +
			m.Data[0][0] * m.Data[2][3] * m.Data[3][1] +
			m.Data[2][0] * m.Data[0][1] * m.Data[3][3] -
			m.Data[2][0] * m.Data[0][3] * m.Data[3][1] -
			m.Data[3][0] * m.Data[0][1] * m.Data[2][3] +
			m.Data[3][0] * m.Data[0][3] * m.Data[2][1];

		inv.Data[2][2] = m.Data[0][0] * m.Data[1][1] * m.Data[3][3] -
			m.Data[0][0] * m.Data[1][3] * m.Data[3][1] -
			m.Data[1][0] * m.Data[0][1] * m.Data[3][3] +
			m.Data[1][0] * m.Data[0][3] * m.Data[3][1] +
			m.Data[3][0] * m.Data[0][1] * m.Data[1][3] -
			m.Data[3][0] * m.Data[0][3] * m.Data[1][1];

		inv.Data[2][3] = -m.Data[0][0] * m.Data[1][1] * m.Data[2][3] +
			m.Data[0][0] * m.Data[1][3] * m.Data[2][1] +
			m.Data[1][0] * m.Data[0][1] * m.Data[2][3] -
			m.Data[1][0] * m.Data[0][3] * m.Data[2][1] -
			m.Data[2][0] * m.Data[0][1] * m.Data[1][3] +
			m.Data[2][0] * m.Data[0][3] * m.Data[1][1];

		inv.Data[3][0] = -m.Data[1][0] * m.Data[2][1] * m.Data[3][2] +
			m.Data[1][0] * m.Data[2][2] * m.Data[3][1] +
			m.Data[2][0] * m.Data[1][1] * m.Data[3][2] -
			m.Data[2][0] * m.Data[1][2] * m.Data[3][1] -
			m.Data[3][0] * m.Data[1][1] * m.Data[2][2] +
			m.Data[3][0] * m.Data[1][2] * m.Data[2][1];

		inv.Data[3][1] = m.Data[0][0] * m.Data[2][1] * m.Data[3][2] -
			m.Data[0][0] * m.Data[2][2] * m.Data[3][1] -
			m.Data[2][0] * m.Data[0][1] * m.Data[3][2] +
			m.Data[2][0] * m.Data[0][2] * m.Data[3][1] +
			m.Data[3][0] * m.Data[0][1] * m.Data[2][2] -
			m.Data[3][0] * m.Data[0][2] * m.Data[2][1];

		inv.Data[3][2] = -m.Data[0][0] * m.Data[1][1] * m.Data[3][2] +
			m.Data[0][0] * m.Data[1][2] * m.Data[3][1] +
			m.Data[1][0] * m.Data[0][1] * m.Data[3][2] -
			m.Data[1][0] * m.Data[0][2] * m.Data[3][1] -
			m.Data[3][0] * m.Data[0][1] * m.Data[1][2] +
			m.Data[3][0] * m.Data[0][2] * m.Data[1][1];

		inv.Data[3][3] = m.Data[0][0] * m.Data[1][1] * m.Data[2][2] -
			m.Data[0][0] * m.Data[1][2] * m.Data[2][1] -
			m.Data[1][0] * m.Data[0][1] * m.Data[2][2] +
			m.Data[1][0] * m.Data[0][2] * m.Data[2][1] +
			m.Data[2][0] * m.Data[0][1] * m.Data[1][2] -
			m.Data[2][0] * m.Data[0][2] * m.Data[1][1];

		det = m.Data[0][0] * inv.Data[0][0] + m.Data[0][1] * inv.Data[1][0] + m.Data[0][2] * inv.Data[2][0] + m.Data[0][3] * inv.Data[3][0];

		if (det == 0) 
		{
			return GMatrix4x4();
		}

		det = 1.0f / det;
		for (int i = 0; i < 4; ++i) 
		{
			for (int j = 0; j < 4; ++j) 
			{
				inv.Data[i][j] *= det;
			}
		}

		return inv;
	}

	GMatrix4x4 GMatrix4x4::Transpose(const GMatrix4x4& m)
	{
		GMatrix4x4 result;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result.Data[i][j] = m.Data[j][i];
			}
		}

		return result;
	}

	GMatrix4x4 GMatrix4x4::Translation(float tx, float ty, float tz)
	{
		GMatrix4x4 result;
		result.Data[3][0] = tx;
		result.Data[3][1] = ty;
		result.Data[3][2] = tz;

		return result;
	}

	GMatrix4x4 GMatrix4x4::Translation(const GVector3& trans)
	{
		return Translation(trans.x, trans.y, trans.z);
	}

	GMatrix4x4 GMatrix4x4::RotationEular(float pitch, float yaw, float roll)
	{
		float cosPitch = std::cos(pitch);
		float sinPitch = std::sin(pitch);
		float cosYaw = std::cos(yaw);
		float sinYaw = std::sin(yaw);
		float cosRoll = std::cos(roll);
		float sinRoll = std::sin(roll);

		GMatrix4x4 result;
		result.Data[0][0] = cosYaw * cosRoll;
		result.Data[0][1] = -cosYaw * sinRoll * cosPitch + sinYaw * sinPitch;
		result.Data[0][2] = cosYaw * sinRoll * sinPitch + sinYaw * cosPitch;
		result.Data[0][3] = 0.0f;

		result.Data[1][0] = sinRoll;
		result.Data[1][1] = cosRoll * cosPitch;
		result.Data[1][2] = -cosRoll * sinPitch;
		result.Data[1][3] = 0.0f;

		result.Data[2][0] = -sinYaw * cosRoll;
		result.Data[2][1] = sinYaw * sinRoll * cosPitch + cosYaw * sinPitch;
		result.Data[2][2] = -sinYaw * sinRoll * sinPitch + cosYaw * cosPitch;
		result.Data[2][3] = 0.0f;

		result.Data[3][0] = 0.0f;
		result.Data[3][1] = 0.0f;
		result.Data[3][2] = 0.0f;
		result.Data[3][3] = 1.0f;

		return result;
	}

	GMatrix4x4 GMatrix4x4::RotationX(float angle) {
		float cosAngle = std::cos(angle);
		float sinAngle = std::sin(angle);

		GMatrix4x4 result;
		result.Data[0][0] = 1.0f;
		result.Data[0][1] = 0.0f;
		result.Data[0][2] = 0.0f;
		result.Data[0][3] = 0.0f;

		result.Data[1][0] = 0.0f;
		result.Data[1][1] = cosAngle;
		result.Data[1][2] = -sinAngle;
		result.Data[1][3] = 0.0f;

		result.Data[2][0] = 0.0f;
		result.Data[2][1] = sinAngle;
		result.Data[2][2] = cosAngle;
		result.Data[2][3] = 0.0f;

		result.Data[3][0] = 0.0f;
		result.Data[3][1] = 0.0f;
		result.Data[3][2] = 0.0f;
		result.Data[3][3] = 1.0f;

		return result;
	}

	GMatrix4x4 GMatrix4x4::RotationY(float angle) {
		float cosAngle = std::cos(angle);
		float sinAngle = std::sin(angle);

		GMatrix4x4 result;
		result.Data[0][0] = cosAngle;
		result.Data[0][1] = 0.0f;
		result.Data[0][2] = sinAngle;
		result.Data[0][3] = 0.0f;

		result.Data[1][0] = 0.0f;
		result.Data[1][1] = 1.0f;
		result.Data[1][2] = 0.0f;
		result.Data[1][3] = 0.0f;

		result.Data[2][0] = -sinAngle;
		result.Data[2][1] = 0.0f;
		result.Data[2][2] = cosAngle;
		result.Data[2][3] = 0.0f;

		result.Data[3][0] = 0.0f;
		result.Data[3][1] = 0.0f;
		result.Data[3][2] = 0.0f;
		result.Data[3][3] = 1.0f;

		return result;
	}

	GMatrix4x4 GMatrix4x4::RotationZ(float angle) {
		float cosAngle = std::cos(angle);
		float sinAngle = std::sin(angle);

		GMatrix4x4 result;
		result.Data[0][0] = cosAngle;
		result.Data[0][1] = -sinAngle;
		result.Data[0][2] = 0.0f;
		result.Data[0][3] = 0.0f;

		result.Data[1][0] = sinAngle;
		result.Data[1][1] = cosAngle;
		result.Data[1][2] = 0.0f;
		result.Data[1][3] = 0.0f;

		result.Data[2][0] = 0.0f;
		result.Data[2][1] = 0.0f;
		result.Data[2][2] = 1.0f;
		result.Data[2][3] = 0.0f;

		result.Data[3][0] = 0.0f;
		result.Data[3][1] = 0.0f;
		result.Data[3][2] = 0.0f;
		result.Data[3][3] = 1.0f;

		return result;
	}

	GMatrix4x4 GMatrix4x4::RotationQuaternion(float x, float y, float z, float w)
	{
		float xx = x * x;
		float xy = x * y;
		float xz = x * z;
		float xw = x * w;
		float yy = y * y;
		float yz = y * z;
		float yw = y * w;
		float zz = z * z;
		float zw = z * w;

		GMatrix4x4 result;
		result.Data[0][0] = 1 - 2 * (yy + zz);
		result.Data[0][1] = 2 * (xy - zw);
		result.Data[0][2] = 2 * (xz + yw);
		result.Data[0][3] = 0;

		result.Data[1][0] = 2 * (xy + zw);
		result.Data[1][1] = 1 - 2 * (xx + zz);
		result.Data[1][2] = 2 * (yz - xw);
		result.Data[1][3] = 0;

		result.Data[2][0] = 2 * (xz - yw);
		result.Data[2][1] = 2 * (yz + xw);
		result.Data[2][2] = 1 - 2 * (xx + yy);
		result.Data[2][3] = 0;

		result.Data[3][0] = 0;
		result.Data[3][1] = 0;
		result.Data[3][2] = 0;
		result.Data[3][3] = 1;

		return result;
	}

	GMatrix4x4 GMatrix4x4::Scaling(float sx, float sy, float sz)
	{
		GMatrix4x4 result;
		result.Data[0][0] = sx;
		result.Data[1][1] = sy;
		result.Data[2][2] = sz;
		result.Data[3][3] = 1.0f;
		return result;
	}

	GMatrix4x4 GMatrix4x4::Scaling(const GVector3& scale)
	{
		return Scaling(scale.x, scale.y, scale.z);
	}

	void GMatrix4x4::Decompose(const GMatrix4x4& m, GVector3& translation, GVector3& rotation, GVector3& scale)
	{
		translation.x = m.Data[3][0];
		translation.y = m.Data[3][1];
		translation.z = m.Data[3][2];

		scale.x = std::sqrt(m.Data[0][0] * m.Data[0][0] + m.Data[1][0] * m.Data[1][0] + m.Data[2][0] * m.Data[2][0]);
		scale.y = std::sqrt(m.Data[0][1] * m.Data[0][1] + m.Data[1][1] * m.Data[1][1] + m.Data[2][1] * m.Data[2][1]);
		scale.z = std::sqrt(m.Data[0][2] * m.Data[0][2] + m.Data[1][2] * m.Data[1][2] + m.Data[2][2] * m.Data[2][2]);

		GMatrix4x4 rotationMatrix;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j) 
			{
				rotationMatrix.Data[i][j] = m.Data[i][j] / scale[i];
			}
		}

		if (rotationMatrix.Data[0][2] < 0.999f) 
		{
			if (rotationMatrix.Data[0][2] > -0.999f) 
			{
				rotation.x = std::atan2(rotationMatrix.Data[1][2], rotationMatrix.Data[2][2]);
				rotation.y = std::asin(-rotationMatrix.Data[0][2]);
				rotation.z = std::atan2(rotationMatrix.Data[0][1], rotationMatrix.Data[0][0]);
			}
			else
			{
				rotation.x = -std::atan2(-rotationMatrix.Data[1][0], rotationMatrix.Data[1][1]);
				rotation.y = XM_PI / 2.0f;
				rotation.z = 0;
			}
		}
		else
		{
			rotation.x = std::atan2(-rotationMatrix.Data[1][0], rotationMatrix.Data[1][1]);
			rotation.y = -XM_PI / 2.0f;
			rotation.z = 0;
		}
	}

	GMatrix4x4 GMatrix4x4::Perspective(float fovAngleY, float aspectRatio, float nearZ, float farZ)
	{
		GMatrix4x4 result;
		float tanHalfFovY = std::tan(fovAngleY / 2.0f);

		result.Data[0][0] = 1.0f / (aspectRatio * tanHalfFovY);
		result.Data[1][1] = 1.0f / tanHalfFovY;
		result.Data[2][2] = -(farZ + nearZ) / (farZ - nearZ);
		result.Data[2][3] = -1.0f;
		result.Data[3][2] = -(2.0f * farZ * nearZ) / (farZ - nearZ);
		result.Data[3][3] = 0.0f;

		return result;
	}

	GMatrix4x4 GMatrix4x4::LookAt(const GVector3& eyePosition,
		const GVector3& focusPosition, const GVector3& upDirection)
	{
		GVector3 f = (focusPosition - eyePosition).normalize();
		GVector3 s = f.cross(upDirection).normalize();
		GVector3 u = s.cross(f);

		GMatrix4x4 result;
		result.Data[0][0] = s.x;
		result.Data[0][1] = s.y;
		result.Data[0][2] = s.z;
		result.Data[0][3] = -s.dot(eyePosition);

		result.Data[1][0] = u.x;
		result.Data[1][1] = u.y;
		result.Data[1][2] = u.z;
		result.Data[1][3] = -u.dot(eyePosition);

		result.Data[2][0] = -f.x;
		result.Data[2][1] = -f.y;
		result.Data[2][2] = -f.z;
		result.Data[2][3] = f.dot(eyePosition);

		result.Data[3][0] = 0.0f;
		result.Data[3][1] = 0.0f;
		result.Data[3][2] = 0.0f;
		result.Data[3][3] = 1.0f;

		return result;
	}
}
