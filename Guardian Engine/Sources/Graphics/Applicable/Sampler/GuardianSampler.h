#ifndef _GE_GUARDIANSAMPLER_H_
#define _GE_GUARDIANSAMPLER_H_
#include "../Texture/GuardianTexture.h"

namespace GE
{
	class GUARDIAN_API GuardianSampler : public GuardianApplicable
	{
    public:
        enum Filter
        {
            GE_FILTER_MIN_MAG_MIP_POINT = 0,
            GE_FILTER_MIN_MAG_POINT_MIP_LINEAR = 0x1,
            GE_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x4,
            GE_FILTER_MIN_POINT_MAG_MIP_LINEAR = 0x5,
            GE_FILTER_MIN_LINEAR_MAG_MIP_POINT = 0x10,
            GE_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x11,
            GE_FILTER_MIN_MAG_LINEAR_MIP_POINT = 0x14,
            GE_FILTER_MIN_MAG_MIP_LINEAR = 0x15,
            GE_FILTER_ANISOTROPIC = 0x55,
            GE_FILTER_COMPARISON_MIN_MAG_MIP_POINT = 0x80,
            GE_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR = 0x81,
            GE_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x84,
            GE_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR = 0x85,
            GE_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT = 0x90,
            GE_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x91,
            GE_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT = 0x94,
            GE_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR = 0x95,
            GE_FILTER_COMPARISON_ANISOTROPIC = 0xd5,
            GE_FILTER_MINIMUM_MIN_MAG_MIP_POINT = 0x100,
            GE_FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR = 0x101,
            GE_FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x104,
            GE_FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR = 0x105,
            GE_FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT = 0x110,
            GE_FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x111,
            GE_FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT = 0x114,
            GE_FILTER_MINIMUM_MIN_MAG_MIP_LINEAR = 0x115,
            GE_FILTER_MINIMUM_ANISOTROPIC = 0x155,
            GE_FILTER_MAXIMUM_MIN_MAG_MIP_POINT = 0x180,
            GE_FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR = 0x181,
            GE_FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x184,
            GE_FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR = 0x185,
            GE_FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT = 0x190,
            GE_FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x191,
            GE_FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT = 0x194,
            GE_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR = 0x195,
            GE_FILTER_MAXIMUM_ANISOTROPIC = 0x1d5
        };

        enum AddressMode
        {
            GE_TEXTURE_ADDRESS_WRAP = 1,
            GE_TEXTURE_ADDRESS_MIRROR = 2,
            GE_TEXTURE_ADDRESS_CLAMP = 3,
            GE_TEXTURE_ADDRESS_BORDER = 4,
            GE_TEXTURE_ADDRESS_MIRROR_ONCE = 5
        };

        struct Properties
        {
            Properties()
            {
                this->SamplerFilter = GE_FILTER_MIN_MAG_MIP_LINEAR;
                this->AddressU = GE_TEXTURE_ADDRESS_WRAP;
                this->AddressV = GE_TEXTURE_ADDRESS_WRAP;
                this->AddressW = GE_TEXTURE_ADDRESS_WRAP;
            }
            Properties(Filter filter, AddressMode addressU, AddressMode addressV, AddressMode addressW)
            {
                this->SamplerFilter = filter;
                this->AddressU = addressU;
                this->AddressV = addressV;
                this->AddressW = addressW;
            }
            Properties(const Properties& other)
            {
                this->SamplerFilter = other.SamplerFilter;
                this->AddressU = other.AddressU;
                this->AddressV = other.AddressV;
                this->AddressW = other.AddressW;
            }

            Filter SamplerFilter = GE_FILTER_MIN_MAG_POINT_MIP_LINEAR;
            AddressMode AddressU = GE_TEXTURE_ADDRESS_WRAP;
            AddressMode AddressV = GE_TEXTURE_ADDRESS_WRAP;
            AddressMode AddressW = GE_TEXTURE_ADDRESS_WRAP;
        };

	public:
		GuardianSampler();
        GuardianSampler(Properties properties, int index = 0);
        GuardianSampler(const GuardianSampler& other);
        ~GuardianSampler() override = default;

        void InitializeSampler(Properties properties, int index = 0);

        void Apply() override;

        WRL::ComPtr<ID3D11SamplerState> GetSamplerState() noexcept;
        const Properties& GetSamplerProperties() const noexcept;
        const Filter& GetSamplerFilter() const noexcept;
        const AddressMode& GetSamplerAdressU() const noexcept;
        const AddressMode& GetSamplerAdressV() const noexcept;
        const AddressMode& GetSamplerAdressW() const noexcept;

        static std::shared_ptr<GuardianSampler> CreateNewSampler(Properties properties, int index = 0);

    private:
        WRL::ComPtr<ID3D11SamplerState> SamplerState;
        Properties SamplerProperties;
        UINT SamplerAppliedSlot;
	};
}

#endif