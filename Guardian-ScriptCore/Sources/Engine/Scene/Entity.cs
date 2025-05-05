using System;
using GE;


namespace GE
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
                InternalMethods.TransformComponentSetTranslation(this.UUID, ref value);
            }
        }

        public T CreateComponent<T>() where T : GuardianComponent, new()
        {
            if (HasComponent<T>())
            {
                return GetComponent<T>();
            }

            Type ComponentType = typeof(T);
            InternalMethods.EntityCreateComponent(this.UUID, ComponentType);
            T component = new T { Entity = this };
            return component;
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
