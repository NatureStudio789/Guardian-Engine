#ifndef _GE_GGEOMETRY_H_
#define _GE_GGEOMETRY_H_
#include "../Lighting/GLightRegistry.h"

namespace GE
{
	class GUARDIAN_API GGeometry
	{
	public:
		enum Category
		{
			GE_GEOMETRY_UNDEFINED,
			GE_GEOMETRY_BOX,
			GE_GEOMETRY_SPHERE,
			GE_GEOMETRY_CAPSULE,
			GE_GEOMETRY_PLANE
		};

		struct Data
		{
			std::vector<GVector3> Vertices;
			std::vector<UINT> Indices;

			GTransform LocalTransform;
		};

	public:
		GGeometry();
		GGeometry(const Category& category, const std::vector<Data>& data);
		GGeometry(const GGeometry& other);
		virtual ~GGeometry();

		void InitializeGeometry(const Category& category, const std::vector<Data>& data);

		const Category& GetGeometryCategory() const noexcept;
		const std::vector<Data>& GetGeometryData() const noexcept;

	private:
		Category GeometryCategory;
		std::vector<Data> GeometryData;
	};

	class GUARDIAN_API GBoxGeometry : public GGeometry
	{
	public:
		using GGeometry::GGeometry;
		GBoxGeometry(const GVector3& edgeLength);

		GVector3 EdgeLength;
	};

	class GUARDIAN_API GSphereGeometry : public GGeometry
	{
	public:
		using GGeometry::GGeometry;
		GSphereGeometry(float radius);

		float Radius;
	};

	class GUARDIAN_API GCapsuleGeometry : public GGeometry
	{
	public:
		using GGeometry::GGeometry;
		GCapsuleGeometry(float height, float halfSphereRadius);

		float Height, HalfSphereRadius;
	};

	class GUARDIAN_API GPlaneGeometry : public GGeometry
	{
	public:
		using GGeometry::GGeometry;
		GPlaneGeometry(const GVector2& edgeLength);

		GVector2 EdgeLength;
	};
}

#endif