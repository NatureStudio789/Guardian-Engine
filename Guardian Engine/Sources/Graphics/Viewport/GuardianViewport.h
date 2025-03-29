#ifndef _GE_GUARDIANVIEWPORT_H_
#define _GE_GUARDIANVIEWPORT_H_
#include "../../Physics/RigidBody/GuardianDynamicRigidBody.h"
	
namespace GE
{
	class GUARDIAN_API GuardianGraphics;

	class GUARDIAN_API GuardianViewport
	{
	public:
		GuardianViewport() = default;
		GuardianViewport(const GuardianViewport& other);
		GuardianViewport(const D3D11_VIEWPORT& viewport) noexcept;
		GuardianViewport(const GVector2& position, const GVector2& size) noexcept;
		~GuardianViewport() = default;

		void InitializeViewport(const GVector2& position, const GVector2& size) noexcept;

		void ApplyViewport(std::shared_ptr<GuardianGraphics> graphics);

		const D3D11_VIEWPORT& GetViewportObject() const noexcept;

	private:
		D3D11_VIEWPORT ViewportObject;
	};
}

#endif