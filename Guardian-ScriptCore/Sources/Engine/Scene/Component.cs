using GE;

namespace GE
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

    public class GuardianCameraComponent : GuardianComponent
    {

    }

    public class GuardianPointLightComponent : GuardianComponent
    {

    }

    public class GuardianSphereColliderComponent : GuardianComponent
    {

    }

    public class GuardianBoxColliderComponent : GuardianComponent
    {

    }

    public class GuardianCapsuleColliderComponent : GuardianComponent
    {

    }

    public class GuardianMeshColliderComponent : GuardianComponent
    {

    }

    public class GuardianRigidBodyComponent : GuardianComponent
    {

    }
}
