using GE;

namespace GE
{
    public class Input
    {
        public static bool IsKeyPressed(uint keyCode)
        {
            return InternalMethods.InputIsKeyPressed(keyCode);
        }
    }
}