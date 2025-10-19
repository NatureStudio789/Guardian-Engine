#include "GGeometry.h"
#include "../Renderer/Renderable/Wireframe/GWireframe.h"

namespace GE
{
	GGeometry::GGeometry()
	{
		this->GeometryDataList = {};
		this->GeometryCategory = GE_GEOMETRY_UNDEFINED;
	}

	GGeometry::GGeometry(const Category& category, const std::vector<Data>& data)
	{
		this->InitializeGeometry(category, data);
	}

	GGeometry::GGeometry(const GGeometry& other)
	{
		this->GeometryCategory = other.GeometryCategory;
		this->GeometryDataList = other.GeometryDataList;
	}

	GGeometry::~GGeometry()
	{
		this->GeometryCategory = GE_GEOMETRY_UNDEFINED;
		this->GeometryDataList = {};
	}

	void GGeometry::InitializeGeometry(const Category& category, const std::vector<Data>& data)
	{
		this->GeometryCategory = category;
		this->GeometryDataList = data;
	}

	void GGeometry::UpdateGeometry()
	{

	}

	const GGeometry::Category& GGeometry::GetGeometryCategory() const noexcept
	{
		return this->GeometryCategory;
	}

	const std::vector<GGeometry::Data>& GGeometry::GetGeometryDataList() const noexcept
	{
		return this->GeometryDataList;
	}

	GBoxGeometry::GBoxGeometry(const GVector3& edgeLength)
	{
		this->EdgeLength = edgeLength;

		GGeometry::Data BoxData;
		BoxData.Vertices = {
			{ -0.5f, -0.5f,  0.5f },
			{  0.5f, -0.5f,  0.5f },
			{  0.5f,  0.5f,  0.5f },
			{ -0.5f,  0.5f,  0.5f },

			{ -0.5f, -0.5f, -0.5f },
			{  0.5f, -0.5f, -0.5f },
			{  0.5f,  0.5f, -0.5f },
			{ -0.5f,  0.5f, -0.5f } 
		};

		BoxData.Indices.push_back(0); BoxData.Indices.push_back(1); BoxData.Indices.push_back(2);
		BoxData.Indices.push_back(0); BoxData.Indices.push_back(2); BoxData.Indices.push_back(3);

		BoxData.Indices.push_back(4); BoxData.Indices.push_back(6); BoxData.Indices.push_back(5);
		BoxData.Indices.push_back(4); BoxData.Indices.push_back(7); BoxData.Indices.push_back(6);

		BoxData.Indices.push_back(4); BoxData.Indices.push_back(0); BoxData.Indices.push_back(3);
		BoxData.Indices.push_back(4); BoxData.Indices.push_back(3); BoxData.Indices.push_back(7);

		BoxData.Indices.push_back(1); BoxData.Indices.push_back(5); BoxData.Indices.push_back(6);
		BoxData.Indices.push_back(1); BoxData.Indices.push_back(6); BoxData.Indices.push_back(2);

		BoxData.Indices.push_back(3); BoxData.Indices.push_back(2); BoxData.Indices.push_back(6);
		BoxData.Indices.push_back(3); BoxData.Indices.push_back(6); BoxData.Indices.push_back(7);

		BoxData.Indices.push_back(4); BoxData.Indices.push_back(5); BoxData.Indices.push_back(1);
		BoxData.Indices.push_back(4); BoxData.Indices.push_back(1); BoxData.Indices.push_back(0);

		BoxData.PartName = "Box";

		BoxData.LocalTransform = GTransform(GVector3(), GVector3(), GVector3(1.0f));

		this->GeometryDataList.push_back(BoxData);
	}

	void GBoxGeometry::UpdateGeometry()
	{
		this->GeometryDataList[0].LocalTransform.Scale = this->EdgeLength;
	}

	GSphereGeometry::GSphereGeometry(float radius)
	{
		this->Radius = radius;

		GGeometry::Data SphereData;

		UINT segments = 20;
		UINT rings = 20;
		for (UINT i = 0; i <= rings; ++i) 
		{
			float theta = DirectX::XM_PI * static_cast<float>(i) / rings;
			float sinTheta = std::sin(theta);
			float cosTheta = std::cos(theta);

			for (UINT j = 0; j <= segments; ++j) 
			{
				float phi = 2.0f * DirectX::XM_PI * static_cast<float>(j) / segments;
				float sinPhi = std::sin(phi);
				float cosPhi = std::cos(phi);

				GVector3 pos;
				pos.x = radius * sinTheta * cosPhi;
				pos.y = radius * sinTheta * sinPhi;
				pos.z = radius * cosTheta;
				SphereData.Vertices.push_back(pos);
			}
		}

		for (UINT i = 0; i < rings; ++i) 
		{
			for (UINT j = 0; j < segments; ++j) 
			{
				UINT v0 = i * (segments + 1) + j;
				UINT v1 = i * (segments + 1) + (j + 1);
				UINT v2 = (i + 1) * (segments + 1) + (j + 1);
				UINT v3 = (i + 1) * (segments + 1) + j;

				SphereData.Indices.push_back(v0);
				SphereData.Indices.push_back(v1);
				SphereData.Indices.push_back(v2);

				SphereData.Indices.push_back(v0);
				SphereData.Indices.push_back(v2);
				SphereData.Indices.push_back(v3);
			}
		}

		SphereData.PartName = "Sphere";

		SphereData.LocalTransform = GTransform(GVector3(), GVector3(), GVector3(1.0f));

		this->GeometryDataList.push_back(SphereData);
	}

	void GSphereGeometry::UpdateGeometry()
	{
		this->GeometryDataList[0].LocalTransform.Scale = GVector3(this->Radius * 2.0f);
	}

	GCapsuleGeometry::GCapsuleGeometry(float height, float halfSphereRadius)
	{
		this->Height = height;
		this->HalfSphereRadius = halfSphereRadius;

		GGeometry::Data TopHalfSphereData;

		UINT segments = 16;
		UINT hemisphereRings = 16;

		const float halfCylinderHeight = 0.5f;
		const float SphereRadius = 0.5f;

		for (UINT ring = 0; ring <= hemisphereRings; ++ring) 
		{
			float theta = (DirectX::XM_PI * 0.5f) * static_cast<float>(ring) / hemisphereRings;
			float sinTheta = std::sin(theta);
			float cosTheta = std::cos(theta);

			for (UINT seg = 0; seg <= segments; ++seg) 
			{
				float phi = 2.0f * DirectX::XM_PI * static_cast<float>(seg) / segments;
				float sinPhi = std::sin(phi);
				float cosPhi = std::cos(phi);

				GVector3 v;
				v.x = SphereRadius * sinTheta * cosPhi;
				v.y = SphereRadius * cosTheta;
				v.z = SphereRadius * sinTheta * sinPhi;
				TopHalfSphereData.Vertices.push_back(v);
			}
		}

		for (UINT ring = 0; ring < hemisphereRings; ++ring) 
		{
			for (UINT seg = 0; seg < segments; ++seg) 
			{
				UINT v0 = ring * (segments + 1) + seg;
				UINT v1 = ring * (segments + 1) + (seg + 1);
				UINT v2 = (ring + 1) * (segments + 1) + (seg + 1);
				UINT v3 = (ring + 1) * (segments + 1) + seg;

				TopHalfSphereData.Indices.push_back(v0);
				TopHalfSphereData.Indices.push_back(v1);
				TopHalfSphereData.Indices.push_back(v2);

				TopHalfSphereData.Indices.push_back(v0);
				TopHalfSphereData.Indices.push_back(v2);
				TopHalfSphereData.Indices.push_back(v3);
			}
		}

		TopHalfSphereData.PartName = "Top Half Sphere";

		TopHalfSphereData.LocalTransform = GTransform(GVector3(), GVector3(), GVector3(1.0f));

		this->GeometryDataList.push_back(TopHalfSphereData);

		GGeometry::Data CylinderData;

		for (UINT seg = 0; seg <= segments; ++seg) 
		{
			float phi = 2.0f * DirectX::XM_PI * static_cast<float>(seg) / segments;
			float cosPhi = std::cos(phi);
			float sinPhi = std::sin(phi);

			GVector3 topV;
			topV.x = SphereRadius * cosPhi;
			topV.y = halfCylinderHeight;
			topV.z = SphereRadius * sinPhi;
			CylinderData.Vertices.push_back(topV);

			GVector3 bottomV;
			bottomV.x = SphereRadius * cosPhi;
			bottomV.y = -halfCylinderHeight;
			bottomV.z = SphereRadius * sinPhi;
			CylinderData.Vertices.push_back(bottomV);
		}

		for (UINT seg = 0; seg < segments; ++seg) 
		{
			UINT topCurr = seg * 2;
			UINT topNext = (seg + 1) * 2;
			UINT bottomNext = topNext + 1;
			UINT bottomCurr = topCurr + 1;

			CylinderData.Indices.push_back(topCurr);
			CylinderData.Indices.push_back(topNext);
			CylinderData.Indices.push_back(bottomNext);

			CylinderData.Indices.push_back(topCurr);
			CylinderData.Indices.push_back(bottomNext);
			CylinderData.Indices.push_back(bottomCurr);
		}

		CylinderData.PartName = "Cylinder";

		CylinderData.LocalTransform = GTransform(GVector3(), GVector3(), GVector3(1.0f));

		this->GeometryDataList.push_back(CylinderData);

		GGeometry::Data BottomHalfSphereData;

		for (UINT ring = 0; ring <= hemisphereRings; ++ring) 
		{
			float theta = DirectX::XM_PI * 0.5f + (DirectX::XM_PI * 0.5f) * static_cast<float>(ring) / hemisphereRings;
			float sinTheta = std::sin(theta);
			float cosTheta = std::cos(theta);

			for (UINT seg = 0; seg <= segments; ++seg) 
			{
				float phi = 2.0f * DirectX::XM_PI * static_cast<float>(seg) / segments;
				float sinPhi = std::sin(phi);
				float cosPhi = std::cos(phi);

				GVector3 v;
				v.x = SphereRadius * sinTheta * cosPhi;
				v.y = SphereRadius * cosTheta;
				v.z = SphereRadius * sinTheta * sinPhi;
				BottomHalfSphereData.Vertices.push_back(v);
			}
		}

		for (UINT ring = 0; ring < hemisphereRings; ++ring) 
		{
			for (UINT seg = 0; seg < segments; ++seg) 
			{
				UINT v0 = ring * (segments + 1) + seg;
				UINT v1 = ring * (segments + 1) + (seg + 1);
				UINT v2 = (ring + 1) * (segments + 1) + (seg + 1);
				UINT v3 = (ring + 1) * (segments + 1) + seg;

				BottomHalfSphereData.Indices.push_back(v0);
				BottomHalfSphereData.Indices.push_back(v1);
				BottomHalfSphereData.Indices.push_back(v2);

				BottomHalfSphereData.Indices.push_back(v0);
				BottomHalfSphereData.Indices.push_back(v2);
				BottomHalfSphereData.Indices.push_back(v3);
			}
		}

		BottomHalfSphereData.PartName = "Bottom Half Sphere";

		BottomHalfSphereData.LocalTransform = GTransform(GVector3(), GVector3(), GVector3(1.0f));

		this->GeometryDataList.push_back(BottomHalfSphereData);
	}

	void GCapsuleGeometry::UpdateGeometry()
	{
		this->GeometryDataList[0].LocalTransform.Position.y = this->Height / 2.0f;
		this->GeometryDataList[0].LocalTransform.Scale = GVector3(this->HalfSphereRadius);
		
		this->GeometryDataList[1].LocalTransform.Scale.x = this->HalfSphereRadius;
		this->GeometryDataList[1].LocalTransform.Scale.y = this->Height;
		this->GeometryDataList[1].LocalTransform.Scale.z = this->HalfSphereRadius;

		this->GeometryDataList[2].LocalTransform.Position.y = -this->Height / 2.0f;
		this->GeometryDataList[2].LocalTransform.Scale = GVector3(this->HalfSphereRadius);
	}

	GPlaneGeometry::GPlaneGeometry(const GVector2& edgeLength)
	{
		this->EdgeLength = edgeLength;

		std::vector<Data> Data;
	}

	void GPlaneGeometry::UpdateGeometry()
	{
		this->GeometryDataList[0].LocalTransform.Scale = GVector3(this->EdgeLength.x, 1.0f, this->EdgeLength.y);
	}
}
