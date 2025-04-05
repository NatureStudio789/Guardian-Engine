#ifndef _GE_GUARDIANSAMPLER_H_
#define _GE_GUARDIANSAMPLER_H_
#include "../Texture/GuardianTexture.h"

namespace GE
{
	enum GUARDIAN_API GuardianSamplerFilter
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

	class GUARDIAN_API GuardianSampler : public GuardianApplicable
	{
	public:
		GuardianSampler();
        GuardianSampler(std::shared_ptr<GuardianGraphics> graphics, GuardianSamplerFilter filter, int index = 0);
        GuardianSampler(const GuardianSampler& other);
        ~GuardianSampler() override = default;

        void InitializeSampler(std::shared_ptr<GuardianGraphics> graphics, GuardianSamplerFilter filter, int index = 0);

        void Apply(std::shared_ptr<GuardianGraphics> graphics) override;

        WRL::ComPtr<ID3D11SamplerState> GetSamplerState() noexcept;
        const GuardianSamplerFilter& GetSamplerFilter() const noexcept;

        static std::shared_ptr<GuardianSampler> CreateNewSampler(
            std::shared_ptr<GuardianGraphics> graphics, GuardianSamplerFilter filter, int index = 0);

    private:
        WRL::ComPtr<ID3D11SamplerState> SamplerState;
        GuardianSamplerFilter SamplerFilter;
        UINT SamplerAppliedSlot;
	};
}

#endif