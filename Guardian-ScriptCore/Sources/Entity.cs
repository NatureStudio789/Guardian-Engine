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
                InternalMethods.EntityGetTranslation(this.UUID, out GVector3 translation);
                return translation;
            }

            set
            {
                InternalMethods.EntitySetTranslation(this.UUID, ref value);
            }
        }
    }
}
