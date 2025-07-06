#include "GDevice.h"

namespace GE
{
	GDevice::GDevice()
	{

	}

	GDevice::GDevice(std::shared_ptr<GGraphicsFactory> factory)
	{
		this->InitializeDevice(factory);
	}

	GDevice::GDevice(const GDevice& other)
	{
		this->DeviceObject = other.DeviceObject;
	}

	GDevice::~GDevice()
	{

	}

	void GDevice::InitializeDevice(std::shared_ptr<GGraphicsFactory> factory)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		GUARDIAN_AUTO_THROW(D3D12CreateDevice(factory->GetFactoryAdapter().Get(), D3D_FEATURE_LEVEL_12_1,
			__uuidof(ID3D12Device), (void**)this->DeviceObject.GetAddressOf()));
	}

	WRL::ComPtr<ID3D12Device> GDevice::GetDeviceObject()
	{
		return this->DeviceObject;
	}
}
