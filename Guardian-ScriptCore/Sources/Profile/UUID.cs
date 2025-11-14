using System;

namespace GuardianEngine.Profile
{
    public class UUID
    {
        public UUID()
        {
            ScriptInternal.GenerateUUID(ref this.IDVarious);
        }

        public UUID(UInt64 id)
        {
            this.IDVarious = id;
        }

        public static implicit operator UInt64(UUID id)
        {
            return id.IDVarious;
        }

        public static bool operator ==(UUID id1, UUID id2)
        {
            return id1.IDVarious == id2.IDVarious;
        }

        public static bool operator !=(UUID id1, UUID id2)
        {
            return id1.IDVarious != id2.IDVarious;
        }

        public override bool Equals(object obj) => obj is UUID id && this == id;

        private UInt64 IDVarious;
    }
}
