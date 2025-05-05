using System;
using System.Runtime.CompilerServices;

namespace GE
{
    internal class InternalMethods
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void EntityCreateComponent(ulong uuid, Type componentType);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool EntityHasComponent(ulong uuid, Type componentType);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void EntityRemoveComponent(ulong uuid, Type componentType);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponentGetTranslation(ulong uuid, out GVector3 translation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponentSetTranslation(ulong uuid, ref GVector3 translation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool InputIsKeyPressed(uint keyCode);
    }
}
