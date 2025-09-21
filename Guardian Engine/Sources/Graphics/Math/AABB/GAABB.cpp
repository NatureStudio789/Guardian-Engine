#include "GAABB.h"

namespace GE
{
    GAABB::GAABB()
    {
        this->Max = GVector3();
        this->Min = GVector3();
    }

    GAABB::GAABB(const GVector3& max, const GVector3& min)
    {
        this->Max = max;
        this->Min = min;
    }

    GAABB::GAABB(const GAABB& other)
    {
        this->Min = other.Min;
        this->Max = other.Max;
    }

    void GAABB::SetTransform(GMatrix transformMatrix)
    {
        this->Max = GMatrix::Multiply(transformMatrix, this->Max);
        this->Min = GMatrix::Multiply(transformMatrix, this->Min);
    }

    bool GAABB::Intersect(const GVector3& rayOrigin, const GVector3& rayDirection, float* distance) const
    {
        GVector3 dirfrac;
        dirfrac.x = 1.0f / rayDirection.x;
        dirfrac.y = 1.0f / rayDirection.y;
        dirfrac.z = 1.0f / rayDirection.z;

        float t1 = (this->Min.x - rayOrigin.x) * dirfrac.x;
        float t2 = (this->Max.x - rayOrigin.x) * dirfrac.x;
        float t3 = (this->Min.y - rayOrigin.y) * dirfrac.y;
        float t4 = (this->Max.y - rayOrigin.y) * dirfrac.y;
        float t5 = (this->Min.z - rayOrigin.z) * dirfrac.z;
        float t6 = (this->Max.z - rayOrigin.z) * dirfrac.z;

        float tmin = max(max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
        float tmax = std::min(std::min(max(t1, t2), max(t3, t4)), max(t5, t6));

        if (tmax < 0)
        {
            if (distance)
            {
                *distance = tmax;
            }
            return false;
        }

        if (tmin > tmax)
        {
            if (distance)
            {
                *distance = tmax;
            }
            return false;
        }

        if (distance)
        {
            *distance = tmin;
        }
        return true;
    }
}
