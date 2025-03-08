#ifndef _GE_GUARDIANGEOMETRY_H_
#define _GE_GUARDIANGEOMETRY_H_
#include "../Mesh/GuardianMesh.h"

namespace guardian
{
	enum GUARDIAN_API GuardianGeometryType
	{
		GE_GEOMETRY_SPHERE,
		GE_GEOMETRY_BOX,
		GE_GEOMETRY_CAPSULE,
	};

	class GUARDIAN_API GuardianGeometry : public GuardianRenderableBase<GuardianGeometry>
	{
	public:
		GuardianGeometry();
		GuardianGeometry(const GuardianGeometry& other);
		~GuardianGeometry();

		void InitializeGeometry(std::shared_ptr<GuardianGraphics> graphics, const GuardianGeometryType& type);

		void UpdateGeometryTransform(XMMATRIX worldMatrix);
		void Update() override;

		const bool IsInitialized() const noexcept;

	private:
		struct GeometryVertex
		{
			GVector3 Position, Color;
		};

		void GenerateSphere(std::vector<GeometryVertex>& vertices, std::vector<UINT>& indices);
		void GenerateBox(std::vector<GeometryVertex>& vertices, std::vector<UINT>& indices);
		void GenerateCapsule(std::vector<GeometryVertex>& vertices, std::vector<UINT>& indices);
	};
}

#endif