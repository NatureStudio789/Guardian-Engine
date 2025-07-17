#include "GFramebufferRegistry.h"
#include "../GGraphicsContextRegistry.h"

namespace GE
{
	std::shared_ptr<GEventProcesser> GFramebufferRegistry::RegistryEventProcesser;
	std::map<std::string, std::shared_ptr<GFramebuffer>> GFramebufferRegistry::FramebufferList;


	void GFramebufferRegistry::InitializeFramebufferRegistry()
	{
		RegistryEventProcesser = std::make_shared<GEventProcesser>();
		RegistryEventProcesser->OnEvent<GWindowResizeEvent>([](const GWindowResizeEvent& event)
		{
			GUARDIAN_SETUP_AUTO_THROW();

			if (GGraphicsContextRegistry::GetCurrentGraphicsContext() && 
				event.WindowHandle == GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetContextWindowHandle() &&
				!FramebufferList.empty())
			{
				auto& context = GGraphicsContextRegistry::GetCurrentGraphicsContext();
				context->GetGraphicsFence()->FlushFence(context->GetGraphicsCommandQueue());

				GUARDIAN_AUTO_THROW(context->GetGraphicsCommandList()->GetCommandListObject()->Reset(
					context->GetGraphicsCommandList()->GetCommandListAllocator().Get(), null));

				context->GetGraphicsSwapChain()->ResizeBuffer(event.ResizeWidth, event.ResizeHeight);

				for (auto& framebuffer : FramebufferList)
				{
					framebuffer.second->ResizeFramebuffer(context, event.ResizeWidth, event.ResizeHeight);
				}

				context->ExecuteCommandList();

				context->GetGraphicsFence()->FlushFence(context->GetGraphicsCommandQueue());
			}
		});
	}

	void GFramebufferRegistry::RegisterFramebuffer(const std::string& name)
	{
		if (FramebufferList.count(name) > 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("The framebuffer named '{}' has been in registry.", name));
		}

		FramebufferList[name] = std::make_shared<GFramebuffer>(GGraphicsContextRegistry::GetCurrentGraphicsContext());
	}

	std::shared_ptr<GFramebuffer> GFramebufferRegistry::GetFramebuffer(const std::string& name)
	{
		if (FramebufferList.count(name) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No framebuffer named '{}' found.", name));
		}

		return FramebufferList[name];
	}

	std::shared_ptr<GFramebuffer> GFramebufferRegistry::GetFramebuffer(const GUUID& id)
	{
		for (auto& framebuffer : FramebufferList)
		{
			if (framebuffer.second->GetFramebufferId() == id)
			{
				return framebuffer.second;
				break;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("No framebuffer with id '{}' found.", (unsigned long long)id));
	}
}
