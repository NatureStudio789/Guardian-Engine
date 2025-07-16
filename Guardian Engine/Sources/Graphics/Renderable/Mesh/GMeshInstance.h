#ifndef _GE_GMESHINSTANCE_H_
#define _GE_GMESHINSTANCE_H_
#include "../GRenderableBase.h"

namespace GE
{
	class GUARDIAN_API GMeshInstance : public GRenderableBase<GMeshInstance>
	{
	public:
		struct Vertex
		{
			GVector3 Position;
		};

		struct Data
		{
			std::string MeshInstanceName;
			std::vector<Vertex> VerticesData;
			std::vector<UINT> IndicesData;
		};

	public:
		GMeshInstance();
		GMeshInstance(const Data& data);
		GMeshInstance(const GMeshInstance& other);
		~GMeshInstance() override;

		void InitializeMeshInstance(const Data& data);

		void Update() override;

		const Data& GetMeshInstanceData() const noexcept;
		
		static std::shared_ptr<GMeshInstance> CreateNewMeshInstance(const Data& data)
		{
			return std::make_shared<GMeshInstance>(data);
		}

	private:
		Data InstanceData;
	};
}

#endif