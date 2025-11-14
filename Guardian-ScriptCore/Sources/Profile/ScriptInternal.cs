using GuardianEngine.Math;
using System;
using System.Runtime.CompilerServices;

namespace GuardianEngine.Profile
{
    internal class ScriptInternal
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void GenerateUUID(ref UInt64 id);


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void CreateEntityComponent(UInt64 entityId, Type componentType);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool HasEntityComponent(UInt64 entityId, Type componentType);


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void GetEntityTransformTranslation(UInt64 id, out Vector3 translation);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void SetEntityTransformTranslation(UInt64 id, ref Vector3 translation);
    }
}
