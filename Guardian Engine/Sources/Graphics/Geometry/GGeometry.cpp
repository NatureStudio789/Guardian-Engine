#include "GGeometry.h"

namespace GE
{
	GGeometry::GGeometry()
	{
		this->GeometryData = {};
		this->GeometryCategory = GE_GEOMETRY_UNDEFINED;
	}

	GGeometry::GGeometry(const Category& category, const std::vector<Data>& data)
	{
		this->InitializeGeometry(category, data);
	}

	GGeometry::GGeometry(const GGeometry& other)
	{
		this->GeometryCategory = other.GeometryCategory;
		this->GeometryData = other.GeometryData;
	}

	GGeometry::~GGeometry()
	{
		this->GeometryCategory = GE_GEOMETRY_UNDEFINED;
		this->GeometryData = {};
	}

	void GGeometry::InitializeGeometry(const Category& category, const std::vector<Data>& data)
	{
		this->GeometryCategory = category;
		this->GeometryData = data;
	}

	const GGeometry::Category& GGeometry::GetGeometryCategory() const noexcept
	{
		return this->GeometryCategory;
	}

	const std::vector<GGeometry::Data>& GGeometry::GetGeometryData() const noexcept
	{
		return this->GeometryData;
	}

	GBoxGeometry::GBoxGeometry(const GVector3& edgeLength)
	{
		this->EdgeLength = edgeLength;
	}

	GSphereGeometry::GSphereGeometry(float radius)
	{
		this->Radius = radius;
	}

	GCapsuleGeometry::GCapsuleGeometry(float height, float halfSphereRadius)
	{
		this->Height = height;
		this->HalfSphereRadius = halfSphereRadius;
	}

	GPlaneGeometry::GPlaneGeometry(const GVector2& edgeLength)
	{
		this->EdgeLength = edgeLength;

		std::vector<Data> Data;
	}
}
