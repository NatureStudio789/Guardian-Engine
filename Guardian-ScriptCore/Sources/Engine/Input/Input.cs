using Guardian;

namespace Guardian
{
    public class Input
    {
        public static bool IsKeyPressed(uint keyCode)
        {
            return InternalMethods.InputIsKeyPressed(keyCode);
        }
    }
}