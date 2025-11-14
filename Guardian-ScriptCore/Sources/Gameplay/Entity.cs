using GuardianEngine.Profile;
using System;
using GuardianEngine.Math;

namespace GuardianEngine.Gameplay
{
    public class Entity
    {
        protected Entity()
        {
            this.EntityId = new UUID(0);
        }

        internal Entity(UUID id)
        {
            this.EntityId = id;
        }

        public Vector3 Translation
        {
            get
            {
                return GetComponent<TransformComponent>().Translation;
            }

            set
            {
                this.GetComponent<TransformComponent>().Translation = value;
            }
        }

        public T CreateComponent<T>() where T : Component, new()
        {
            if (HasComponent<T>())
            {
                return GetComponent<T>();
            }

            Type ComponentType = typeof(T);
            ScriptInternal.CreateEntityComponent(this.EntityId, ComponentType);
            T component = new T { Entity = this };
            return component;
        }

        public bool HasComponent<T>() where T : Component, new()
        {
            Type ComponentType = typeof(T);
            return ScriptInternal.HasEntityComponent(this.EntityId, ComponentType);
        }

        public T GetComponent<T>() where T : Component, new()
        {
            if (!this.HasComponent<T>())
            {
                return null;
            }

            return new T() { Entity = this };
        }

        public readonly UUID EntityId;
    }
}
