#include "GGraphicsFactory.h"

namespace GE
{
	GGraphicsFactory::GGraphicsFactory()
	{
	}

	GGraphicsFactory::GGraphicsFactory(HWND windowHandle)
	{
		this->InitializeGraphicsFactory(windowHandle);
	}

	GGraphicsFactory::GGraphicsFactory(const GGraphicsFactory& other)
	{
		this->FactoryObject = other.FactoryObject;
		this->FactoryAdapter = other.FactoryAdapter;
	}

	GGraphicsFactory::~GGraphicsFactory()
	{

	}

	void GGraphicsFactory::InitializeGraphicsFactory(HWND windowHandle)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		GUARDIAN_AUTO_THROW(
			CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)this->FactoryObject.GetAddressOf()));

		for (UINT AdapterIndex = 0; 
			this->FactoryObject->EnumAdapters1(AdapterIndex, &this->FactoryAdapter) != DXGI_ERROR_NOT_FOUND; 
			AdapterIndex++)
		{
			DXGI_ADAPTER_DESC1 AdapterDesc;
			GUARDIAN_CLEAR_MEMORY(AdapterDesc);
			this->FactoryAdapter->GetDesc1(&AdapterDesc);
			if (AdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				continue;
			}

			if (GUARDIAN_SUCCEEDED(D3D12CreateDevice(
				this->FactoryAdapter.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), null)))
			{
				break;
			}
		}
	}

	WRL::ComPtr<IDXGIFactory4> GGraphicsFactory::GetFactoryObject()
	{
		return this->FactoryObject;
	}

	WRL::ComPtr<IDXGIAdapter1> GGraphicsFactory::GetFactoryAdapter()
	{
		return this->FactoryAdapter;
	}
}
