#ifndef _GE_GCAMERA_H_
#define _GE_GCAMERA_H_
#include "Projection/GProjection.h"

namespace GE
{
	class GUARDIAN_API GCamera
	{
	public:
		GCamera();
		GCamera(const GVector3& position, const GVector3& direction, 
			const GPerspectiveProjection& projection, bool isFreeLook = true);
		GCamera(const GCamera& other);
		~GCamera();
		
		void Translate(const GVector3& translation);
		void Rotate(const GVector3& rotation);

		const GMatrix GetViewMatrix() const noexcept;
		
		const GVector3 GetForwardVector() const noexcept;
		const GVector3 GetBackwardVector() const noexcept;
		const GVector3 GetLeftVector() const noexcept;
		const GVector3 GetRightVector() const noexcept;
		const GVector3 GetUpVector() const noexcept;
		const GVector3 GetDownVector() const noexcept;

		GVector3 Position;
		GVector3 Rotation;
		GPerspectiveProjection Projection;
		bool IsFreeLook;

	private:
		const GMatrix GetRotationMatrix() const noexcept;
	};
}

#endif