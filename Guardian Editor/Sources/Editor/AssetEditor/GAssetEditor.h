#ifndef _GE_GASSETEDITOR_H_
#define _GE_GASSETEDITOR_H_
#include <Editor/GEditor.h>
#include "../../EditorContext/GEditorContext.h"

namespace GE
{
	class GAssetEditor : public EUI::GEditor
	{
	public:
		void Initialize() override;

		void Update() override;

	private:
		std::shared_ptr<EUI::GPanel> AssetBrowserPanel;
		std::string AssetDirectory;
		std::string CurrentDirectory;

		std::shared_ptr<GTexture> DirectoryTexture;
		std::shared_ptr<GTexture> FileTexture;
		std::shared_ptr<GTexture> BackTexture;
	};
}

#endif