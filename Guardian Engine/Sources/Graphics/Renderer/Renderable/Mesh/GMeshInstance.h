#ifndef _GE_GMESHINSTANCE_H_
#define _GE_GMESHINSTANCE_H_
#include "../GRenderable.h"

namespace GE
{
	class GUARDIAN_API GMeshInstance : public GRenderable
	{
	public:
		struct Vertex
		{
			GVector3 Position;
			GVector2 TextureCoord;
		};

		struct Data
		{
			std::vector<Vertex> Vertices;
			std::vector<UINT> Indices;
		};

	public:
		GMeshInstance();
		GMeshInstance(const std::string& name, const Data& data);
		GMeshInstance(const GMeshInstance& other);
		~GMeshInstance() override;

		void InitializeMeshInstance(const std::string& name, const Data& data);

		static std::shared_ptr<GMeshInstance> CreateNewMeshInstance(const std::string& name, const Data& data)
		{
			return std::make_shared<GMeshInstance>(name, data);
		}

	private:
		Data InstanceData;
	};
}

#endif