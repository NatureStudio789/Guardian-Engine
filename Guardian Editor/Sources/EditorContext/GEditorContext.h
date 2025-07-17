#ifndef _GE_GEDITORCONTEXT_H_
#define _GE_GEDITORCONTEXT_H_
#include <GEngine.h>

namespace GE
{
	class GEditorContext
	{
	public:
		GEditorContext();
		GEditorContext(const GEditorContext& other);
		~GEditorContext();

		void InitializeEditorContext();

		void BeginRendering();
		void EndUpRendering();

		ImGuiIO& GetEditorIO() const noexcept;

		static std::shared_ptr<GEditorContext> CreateNewEditorContext()
		{
			auto& context = std::make_shared<GEditorContext>();
			context->InitializeEditorContext();
			return context;
		}

	private:
		std::shared_ptr<GDescriptorHeap> EditorDescriptorHeap;
	};
}

#endif