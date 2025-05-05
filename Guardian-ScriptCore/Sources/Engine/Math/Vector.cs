namespace GE
{
    public struct GVector2
    {
        public GVector2(float x, float y)
        {
            this.x = x;
            this.y = y;
        }

        public float x, y;
    }

    public struct GVector3
    {
        public GVector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public float x, y, z;
    }

    public struct GVector4
    {
        public GVector4(float x, float y, float z, float w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }

        public float x, y, z, w;
    }
}