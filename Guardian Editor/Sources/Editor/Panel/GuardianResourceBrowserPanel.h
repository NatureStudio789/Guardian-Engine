#ifndef _GE_GUARDIANRESOURCEBROWSERPANEL_H_
#define _GE_GUARDIANRESOURCEBROWSERPANEL_H_
#include "GuardianPanel.h"
#include <Graphics/Applicable/Texture/GuardianTexture.h>
#include <Application/ResourceSystem/GuardianResourceSystem.h>

namespace guardian
{
	class GuardianResourceBrowserPanel : public GuardianPanel
	{
	public:
		GuardianResourceBrowserPanel();
		GuardianResourceBrowserPanel(const GString& resourcesDir);
		GuardianResourceBrowserPanel(const GuardianResourceBrowserPanel& ) = default;
		~GuardianResourceBrowserPanel() = default;

		void SetResourceDirectory(const GString& resourcesDir);

		void Render() override;

	private:
		void RenderMenuBar();
		void RenderDirectory(const GString& directory);
		GuardianTexture SetFileUnitIcon(std::filesystem::directory_entry directoryEntry);

		GString ResourceDirectory;
		GString CurrentDirectory;

		GuardianTexture FolderIcon;
		GuardianTexture ModelFileIcon;
		GuardianTexture TextureFileIcon;
		GuardianTexture DefaultFileIcon;
	};
}

#endif