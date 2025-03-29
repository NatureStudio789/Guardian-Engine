#ifndef _GE_GUARDIANVECTOR_H_
#define _GE_GUARDIANVECTOR_H_
#include "../../../Script/GuardianScriptEngine.h"

namespace GE
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

		bool operator==(const GVector2& other)
		{
			return (this->x == other.x) && (this->y == other.y);
		}

		bool operator!=(const GVector2& other)
		{
			return (this->x != other.x) || (this->y != other.y);
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
		GVector3(float a)
		{
			this->x = a;
			this->y = a;
			this->z = a;
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

		bool operator==(const GVector3& other)
		{
			return (this->x == other.x) && (this->y == other.y) && (this->z == other.z);
		}

		bool operator!=(const GVector3& other)
		{
			return (this->x != other.x) || (this->y != other.y) || (this->z != other.z);
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

		bool operator==(const GVector4& other)
		{
			return (this->x == other.x) && (this->y == other.y) && (this->z == other.z) && (this->w == other.w);
		}

		bool operator!=(const GVector4& other)
		{
			return (this->x != other.x) || (this->y != other.y) || (this->z != other.z) || (this->w != other.w);
		}

		const float length() const
		{
			return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
		}

		float x, y, z, w;
	};

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const GVector2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;

		return out;
	}

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const GVector3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;

		return out;
	}

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const GVector4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;

		return out;
	}
}

namespace YAML
{
	template<>
	struct convert<GE::GVector2>
	{
		static Node encode(const GE::GVector2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);

			return node;
		}

		static bool decode(const Node& node, GE::GVector2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();

			return true;
		}
	};

	template<>
	struct convert<GE::GVector3>
	{
		static Node encode(const GE::GVector3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);

			return node;
		}

		static bool decode(const Node& node, GE::GVector3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();

			return true;
		}
	};

	template<>
	struct convert<GE::GVector4>
	{
		static Node encode(const GE::GVector4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);

			return node;
		}

		static bool decode(const Node& node, GE::GVector4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();

			return true;
		}
	};
}

#endif // !_GE_GUARDIANVECTOR_H_