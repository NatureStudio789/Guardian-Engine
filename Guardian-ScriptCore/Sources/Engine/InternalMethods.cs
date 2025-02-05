using System;
using System.Runtime.CompilerServices;

namespace Guardian
{
    public static class InternalMethods
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void EntityGetTranslation(ulong uuid, out GVector3 translation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void EntitySetTranslation(ulong uuid, ref GVector3 translation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool InputIsKeyPressed(uint keyCode);
    }
}
