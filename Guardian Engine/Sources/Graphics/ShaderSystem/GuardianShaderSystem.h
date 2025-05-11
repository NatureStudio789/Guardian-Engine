#ifndef _GE_GUARDIANSHADERSYSTEM_H_
#define _GE_GUARDIANSHADERSYSTEM_H_
#include "../Applicable/Sampler/GuardianSampler.h"

namespace GE
{
	class GuardianShaderSystem
	{
	public:
		static void InitializeShaderSystem();

		static void AddShaderGroup(std::shared_ptr<GuardianShaderGroup> shaderGroup);

		static std::shared_ptr<GuardianShaderGroup> GetShaderGroup(const GString& name);
		static std::shared_ptr<GuardianShaderGroup> GetShaderGroup(const GuardianUUID& id);

		const static GString PBR_MAIN_SHADER;
		const static GString WIREFRAME_SHADER;
		const static GString SOLID_SHADER;

	private:
		static std::map<GString, std::shared_ptr<GuardianShaderGroup>> ShaderGroupList;
	};
}

#endif