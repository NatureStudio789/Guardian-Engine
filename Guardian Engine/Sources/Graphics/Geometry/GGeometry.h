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
			std::string PartName;
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

		virtual void UpdateGeometry();

		const Category& GetGeometryCategory() const noexcept;
		const std::vector<Data>& GetGeometryDataList() const noexcept;

	protected:
		Category GeometryCategory;
		std::vector<Data> GeometryDataList;

		friend class GGeometryWireframe;
	};

	class GUARDIAN_API GBoxGeometry : public GGeometry
	{
	public:
		using GGeometry::GGeometry;
		GBoxGeometry() : GGeometry()
		{
			this->GeometryCategory = GE_GEOMETRY_BOX;
		}
		GBoxGeometry(const GVector3& edgeLength);

		void UpdateGeometry() override;

		GVector3 EdgeLength;
	};

	class GUARDIAN_API GSphereGeometry : public GGeometry
	{
	public:
		using GGeometry::GGeometry;
		GSphereGeometry() : GGeometry()
		{
			this->GeometryCategory = GE_GEOMETRY_SPHERE;
		}
		GSphereGeometry(float radius);

		void UpdateGeometry() override;

		float Radius;
	};

	class GUARDIAN_API GCapsuleGeometry : public GGeometry
	{
	public:
		using GGeometry::GGeometry;
		GCapsuleGeometry() : GGeometry()
		{
			this->GeometryCategory = GE_GEOMETRY_CAPSULE;
		}
		GCapsuleGeometry(float height, float halfSphereRadius);

		void UpdateGeometry() override;

		float Height, HalfSphereRadius;
	};

	class GUARDIAN_API GPlaneGeometry : public GGeometry
	{
	public:
		using GGeometry::GGeometry;
		GPlaneGeometry() : GGeometry()
		{
			this->GeometryCategory = GE_GEOMETRY_PLANE;
		}
		GPlaneGeometry(const GVector2& edgeLength);

		void UpdateGeometry() override;

		GVector2 EdgeLength;
	};
}

#endif