#ifndef _GE_GMISSION_H_
#define _GE_GMISSION_H_
#include "../RenderGraph/GLitRenderGraph.h"

namespace GE
{
	class GUARDIAN_API GMission
	{
	public:
		GMission();
		GMission(const std::string& targetGraphName, 
			std::shared_ptr<GFramebuffer> framebuffer, std::shared_ptr<GCamera> camera);
		GMission(const GMission& other);
		~GMission();

		void SetTargetGraphName(const std::string& targetGraphName);
		void SetRenderFramebuffer(std::shared_ptr<GFramebuffer> framebuffer);
		void SetRenderCamera(std::shared_ptr<GCamera> camera);

		void Request();

		const std::string& GetTargetRenderGraphName() const noexcept;
		std::shared_ptr<GFramebuffer> GetRenderFramebuffer() const noexcept;
		std::shared_ptr<GCamera> GetRenderCamera() const noexcept;

		static std::shared_ptr<GMission> CreateNewMission(const std::string& targetGraphName,
			std::shared_ptr<GFramebuffer> framebuffer, std::shared_ptr<GCamera> camera)
		{
			return std::make_shared<GMission>(targetGraphName, framebuffer, camera);
		}

	private:
		std::string TargetGraphName;
		std::shared_ptr<GFramebuffer> RenderFramebuffer;
		std::shared_ptr<GCamera> RenderCamera;
	};
}

#endif