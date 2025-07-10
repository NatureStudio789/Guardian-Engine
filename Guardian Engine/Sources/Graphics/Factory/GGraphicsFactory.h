#ifndef _GE_GGRAPHICSFACTORY_H_
#define _GE_GGRAPHICSFACTORY_H_
#include "../Math/Vector/GVector.h"

namespace GE
{
	class GUARDIAN_API GGraphicsFactory
	{
	public:
		GGraphicsFactory();
		GGraphicsFactory(HWND windowHandle);
		GGraphicsFactory(const GGraphicsFactory& other);
		~GGraphicsFactory();

		void InitializeGraphicsFactory(HWND windowHandle);

		WRL::ComPtr<IDXGIFactory5> GetFactoryObject();
		WRL::ComPtr<IDXGIAdapter1> GetFactoryAdapter();

		static std::shared_ptr<GGraphicsFactory> CreateNewGraphicsFactory(HWND windowHandle)
		{
			return std::make_shared<GGraphicsFactory>(windowHandle);
		}

	private:
		WRL::ComPtr<IDXGIFactory5> FactoryObject;
		WRL::ComPtr<IDXGIAdapter1> FactoryAdapter;
	};
}

#endif