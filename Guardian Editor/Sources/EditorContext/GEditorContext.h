#ifndef _GE_GEDITORCONTEXT_H_
#define _GE_GEDITORCONTEXT_H_
#include "../Event/GEditorEvent.h"

namespace GE
{
	class GEditorContext
	{
	public:
		GEditorContext();
		GEditorContext(std::shared_ptr<GFramebuffer> contextFramebuffer);
		GEditorContext(const GEditorContext& other);
		~GEditorContext();

		void InitializeEditorContext(std::shared_ptr<GFramebuffer> contextFramebuffer);

		void BeginRendering();
		void EndUpRendering();

		ImGuiIO& GetEditorIO() const noexcept;

		static std::shared_ptr<GEditorContext> CreateNewEditorContext(std::shared_ptr<GFramebuffer> contextFramebuffer)
		{			
			return std::make_shared<GEditorContext>(contextFramebuffer);
		}

	private:
		std::shared_ptr<GFramebuffer> ContextFramebuffer;
		std::shared_ptr<GDescriptorHeap> EditorDescriptorHeap;

		friend class GEditorRenderGraph;
	};
}

#endif