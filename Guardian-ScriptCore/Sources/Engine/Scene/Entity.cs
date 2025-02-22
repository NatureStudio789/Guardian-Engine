using System;
using Guardian;


namespace Guardian
{
    public class Entity
    {
        protected Entity()
        {
            this.UUID = 0;
        }

        internal Entity(ulong uuid)
        {
            this.UUID = uuid;
        }

        public readonly ulong UUID;

        public GVector3 Translation
        {
            get
            {
                return GetComponent<GuardianTransformComponent>().Translation;
            }

            set
            {
                
            }
        }

        public bool HasComponent<T>() where T : GuardianComponent, new()
        {
            Type ComponentType = typeof(T);
            return InternalMethods.EntityHasComponent(this.UUID, ComponentType);
        }

        public T GetComponent<T>() where T : GuardianComponent, new()
        {
            if (!this.HasComponent<T>())
            {
                return null;
            }

            return new T() { Entity = this };
        }
    }
}
