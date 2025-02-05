#include "GuardianConverter.h"

namespace guardian
{
    const GWideString GuardianConverter::StringToWideString(const GString& string)
    {
        return GWideString(string.begin(), string.end());
    }

    const GString GuardianConverter::HRESULTToString(HRESULT hr)
    {
		char* MsgBuffer = null;
		DWORD MsgLen = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, null, hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&MsgBuffer), 0, null);
		if (MsgLen == 0)
		{
			return "Undefined error code";
		}

		GString ErrorString = MsgBuffer;
		LocalFree(MsgBuffer);
		return ErrorString;
    }

	const XMFLOAT3 GuardianConverter::QuaternionToEulerAngles(const XMFLOAT4& quaternion)
	{
		float pitch, yaw, roll;

		float sinPitch = 2.0f * (quaternion.w * quaternion.x + quaternion.y * quaternion.z);
		float cosPitch = 1.0f - 2.0f * (quaternion.x * quaternion.x + quaternion.y * quaternion.y);
		pitch = atan2(sinPitch, cosPitch);

		float sinYaw = 2.0f * (quaternion.w * quaternion.y - quaternion.z * quaternion.x);
		if (fabs(sinYaw) >= 1.0f)
		{
			yaw = copysign(XM_PIDIV2, sinYaw);
		}
		else
		{
			yaw = asin(sinYaw);
		}

		float sinRoll = 2.0f * (quaternion.w * quaternion.z + quaternion.x * quaternion.y);
		float cosRoll = 1.0f - 2.0f * (quaternion.y * quaternion.y + quaternion.z * quaternion.z);
		roll = atan2(sinRoll, cosRoll);

		return XMFLOAT3(pitch * (180.0f / XM_PI), yaw * (180.0f / XM_PI), roll * (180.0f / XM_PI));
	}

	const XMFLOAT4 GuardianConverter::EulerAnglesToQuaternion(const XMFLOAT3& eulerAngles)
	{
		XMFLOAT4 Quaternion = XMFLOAT4();
		float yaw = (eulerAngles.z / 360.0f) * XM_2PI;
		float pitch = (eulerAngles.y / 360.0f) * XM_2PI;
		float roll = (eulerAngles.x / 360.0f) * XM_2PI;
		float cy = cos(yaw * 0.5f);
		float sy = sin(yaw * 0.5f);
		float cr = cos(roll * 0.5f);
		float sr = sin(roll * 0.5f);
		float cp = cos(pitch * 0.5f);
		float sp = sin(pitch * 0.5f);
		Quaternion.w = cy * cr * cp + sy * sr * sp;
		Quaternion.x = cy * sr * cp - sy * cr * sp;
		Quaternion.y = cy * cr * sp + sy * sr * cp;
		Quaternion.z = sy * cr * cp - cy * sr * sp;

		return Quaternion;
	}
}
