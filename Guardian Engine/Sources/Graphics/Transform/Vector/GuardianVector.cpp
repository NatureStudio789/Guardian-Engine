#include "GuardianVector.h"
#include "../Matrix/GuardianMatrix.h"

namespace GE
{
	GVector3 GVector3::operator*(const GMatrix4x4& m) const
	{
		float newX = this->x * m.Data[0][0] + this->y * m.Data[1][0] + this->z * m.Data[2][0] + m.Data[3][0];
		float newY = this->x * m.Data[0][1] + this->y * m.Data[1][1] + this->z * m.Data[2][1] + m.Data[3][1];
		float newZ = this->x * m.Data[0][2] + this->y * m.Data[1][2] + this->z * m.Data[2][2] + m.Data[3][2];
		return GVector3(newX, newY, newZ);
	}

	GVector3 GVector3::operator*=(const GMatrix4x4& m)
	{
		return ((*this) = (*this) * m);
	}
}
