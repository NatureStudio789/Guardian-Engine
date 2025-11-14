using System;
using GuardianEngine.Gameplay;
using GuardianEngine.Math;
using GuardianEngine.Profile;

namespace GuardianEngine.Gameplay
{
    public abstract class Component
    {
        public Entity Entity { get; set; }
    }

    public class TransformComponent : Component
    {
        public Vector3 Translation
        {
            set
            {
                ScriptInternal.SetEntityTransformTranslation(this.Entity.EntityId, ref value);
            }

            get
            {
                ScriptInternal.GetEntityTransformTranslation(this.Entity.EntityId, out Vector3 translation);
                return translation;
            }
        }
    }

    public class CameraComponent : Component
    {

    }
}
