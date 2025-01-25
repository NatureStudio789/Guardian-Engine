#ifndef _GE_GUARDIANVECTOR_H_
#define _GE_GUARDIANVECTOR_H_
#include "../../../Application/Input/GuardianInput.h"

namespace guardian
{
	class GUARDIAN_API GVector2
	{
	public:
		GVector2()
		{
			this->x = 0.0f;
			this->y = 0.0f;
		}
		GVector2(float x, float y)
		{
			this->x = x;
			this->y = y;
		}
		GVector2(const GVector2& other)
		{
			this->x = other.x;
			this->y = other.y;
		}

		GVector2 operator+(const GVector2& other) const
		{
			return GVector2(this->x + other.x, this->y + other.y);
		}

		GVector2 operator-(const GVector2& other) const
		{
			return GVector2(this->x - other.x, this->y - other.y);
		}

		GVector2 operator*(const GVector2& other) const
		{
			return GVector2(this->x * other.x, this->y * other.y);
		}

		GVector2 operator*(int a) const
		{
			return GVector2(this->x * a, this->y * a);
		}

		GVector2 operator+=(const GVector2& other)
		{
			this->x += other.x;
			this->y += other.y;

			return *this;
		}

		GVector2 operator-=(const GVector2& other)
		{
			this->x -= other.x;
			this->y -= other.y;

			return *this;
		}

		GVector2 operator*=(const GVector2& other)
		{
			this->x *= other.x;
			this->y *= other.y;

			return *this;
		}

		GVector2 operator*=(float a)
		{
			this->x *= a;
			this->y *= a;

			return *this;
		}

		const float length() const
		{
			return std::sqrt(this->x * this->x + this->y * this->y);
		}

		float x, y;
	};

	class GUARDIAN_API GVector3
	{
	public:
		GVector3()
		{
			this->x = 0.0f;
			this->y = 0.0f;
			this->z = 0.0f;
		}
		GVector3(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
		GVector3(const GVector3& other)
		{
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
		}

		GVector3 operator+(const GVector3& other) const
		{
			return GVector3(this->x + other.x, this->y + other.y, this->z + other.z);
		}

		GVector3 operator-(const GVector3& other) const
		{
			return GVector3(this->x - other.x, this->y - other.y, this->z - other.z);
		}

		GVector3 operator*(const GVector3& other) const
		{
			return GVector3(this->x * other.x, this->y * other.y, this->z * other.z);
		}

		GVector3 operator*(float a) const
		{
			return GVector3(this->x * a, this->y * a, this->z * a);
		}

		GVector3 operator+=(const GVector3& other)
		{
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;

			return *this;
		}

		GVector3 operator-=(const GVector3& other)
		{
			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;

			return *this;
		}

		GVector3 operator*=(const GVector3& other)
		{
			this->x *= other.x;
			this->y *= other.y;
			this->z *= other.z;

			return *this;
		}

		GVector3 operator*=(float a)
		{
			this->x *= a;
			this->y *= a;
			this->z *= a;

			return *this;
		}

		const float length() const
		{
			return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
		}

		float x, y, z;
	};

	class GUARDIAN_API GVector4
	{
	public:
		GVector4()
		{
			this->x = 0.0f;
			this->y = 0.0f;
			this->z = 0.0f;
			this->w = 0.0f;
		}
		GVector4(float x, float y, float z, float w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}
		GVector4(const GVector4& other)
		{
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			this->w = other.w;
		}

		GVector4 operator+(const GVector4& other) const
		{
			return GVector4(this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w);
		}

		GVector4 operator-(const GVector4& other) const
		{
			return GVector4(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w);
		}

		GVector4 operator*(const GVector4& other) const
		{
			return GVector4(this->x * other.x, this->y * other.y, this->z * other.z, this->w * other.w);
		}

		GVector4 operator*(float a) const
		{
			return GVector4(this->x * a, this->y * a, this->z * a, this->w * a);
		}

		GVector4 operator+=(const GVector4& other)
		{
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			this->w += other.w;

			return *this;
		}

		GVector4 operator-=(const GVector4& other)
		{
			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			this->w -= other.w;

			return *this;
		}

		GVector4 operator*=(const GVector4& other)
		{
			this->x *= other.x;
			this->y *= other.y;
			this->z *= other.z;
			this->w *= other.w;

			return *this;
		}

		GVector4 operator*=(float a)
		{
			this->x *= a;
			this->y *= a;
			this->z *= a;
			this->w *= a;

			return *this;
		}

		const float length() const
		{
			return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
		}

		float x, y, z, w;
	};
}

#endif // !_GE_GUARDIANVECTOR_H_