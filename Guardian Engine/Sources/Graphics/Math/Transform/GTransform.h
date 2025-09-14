#ifndef _GE_GTRANSFORM_H_
#define _GE_GTRANSFORM_H_
#include "../AABB/GAABB.h"

namespace GE
{
	class GUARDIAN_API GTransform
	{
	public:
		GTransform();
		GTransform(const GVector3& position,
			const GVector3& rotation,
			const GVector3& scale);
		GTransform(const GTransform& other);

		void SetPosition(const GVector3& position);
		void SetPosition(float x, float y, float z);
		void SetRotation(const GVector3& rotation);
		void SetRotation(float x, float y, float z);
		void SetScale(const GVector3& scale);
		void SetScale(float x, float y, float z);

		void Translate(const GVector3& translation);
		void Translate(float tx, float ty, float tz);
		void Rotate(const GVector3& rotation);
		void Rotate(float rx, float ry, float rz);
		void Scaling(const GVector3& scale);
		void Scaling(float sx, float sy, float sz);

		const GMatrix GetTransformMatrix() const noexcept;
		const GVector3 GetForwardVector() const noexcept;
		const GVector3 GetBackwardVector() const noexcept;
		const GVector3 GetRightVector() const noexcept;
		const GVector3 GetLeftVector() const noexcept;
		const GVector3 GetUpVector() const noexcept;
		const GVector3 GetDownVector() const noexcept;

		GVector3 Position;
		GVector3 Rotation;
		GVector3 Scale;

	private:
		const GMatrix GetRotationMatrix() const noexcept;
	};
}

#endif