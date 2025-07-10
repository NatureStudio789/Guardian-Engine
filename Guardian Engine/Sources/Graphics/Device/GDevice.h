#ifndef _GE_GDEVICE_H_
#define _GE_GDEVICE_H_
#include "../Factory/GGraphicsFactory.h"

namespace GE
{
	class GUARDIAN_API GDevice
	{
	public:
		GDevice();
		GDevice(std::shared_ptr<GGraphicsFactory> factory);
		GDevice(const GDevice& other);
		~GDevice();

		void InitializeDevice(std::shared_ptr<GGraphicsFactory> factory);

		const UINT GetDescriptorSize(D3D12_DESCRIPTOR_HEAP_TYPE type) const;
		WRL::ComPtr<ID3D12Device> GetDeviceObject();

		static std::shared_ptr<GDevice> CreateNewDevice(std::shared_ptr<GGraphicsFactory> factory)
		{
			return std::make_shared<GDevice>(factory);
		}

	private:
		WRL::ComPtr<ID3D12Device> DeviceObject;
	};
}

#endif