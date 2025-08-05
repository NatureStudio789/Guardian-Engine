#ifndef _GE_GSHADER_H_
#define _GE_GSHADER_H_
#include "../../Material/GMaterial.h"

namespace GE
{
	class GUARDIAN_API GShader
	{
	public:
		enum Category
		{
			GE_NONE_SHADER,
			GE_VERTEX_SHADER,
			GE_PIXEL_SHADER,
			GE_DOMAIN_SHADER,
			GE_HULL_SHADER,
			GE_GEOMETRY_SHADER,
		};

	public:
		GShader();
		GShader(Category category, std::string shaderFilePath, bool isBinaryFile = false);
		GShader(const GShader& other);
		~GShader();

		void InitializeShader(Category category, std::string shaderFilePath, bool isBinaryFile = false);

		const Category& GetShaderCategory() const noexcept;
		WRL::ComPtr<ID3DBlob> GetShaderByteCode();

		static std::shared_ptr<GShader> CreateNewShader(
			Category category, std::string shaderFilePath, bool isBinaryFile = false)
		{
			return std::make_shared<GShader>(category, shaderFilePath, isBinaryFile);
		}

	private:
		Category ShaderCategory;
		WRL::ComPtr<ID3DBlob> ShaderByteCode;

		friend class GPipelineState;
	};
}

#endif