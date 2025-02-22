using Guardian;

namespace Guardian
{
    public abstract class GuardianComponent
    {
        public Entity Entity { get; set; }
    }

    public class GuardianTransformComponent : GuardianComponent
    {
        public GVector3 Translation
        {
            get
            {
                InternalMethods.TransformComponentGetTranslation(this.Entity.UUID, out GVector3 translation);
                return translation;
            }
            set
            {
                InternalMethods.TransformComponentSetTranslation(this.Entity.UUID, ref value);
            }
        }
    }
}
