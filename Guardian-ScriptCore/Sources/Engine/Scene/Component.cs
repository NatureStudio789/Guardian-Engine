using Guardian;

namespace Guardian
{
    public abstract class Component
    {
        public Entity Entity { get; set; }
    }

    public class TransformComponent : Component
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
