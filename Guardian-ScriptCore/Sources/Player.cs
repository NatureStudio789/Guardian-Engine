using System;
using Guardian;

namespace Guardian
{
    class Player : Entity
    {
        public void Initialize()
        {
            Console.WriteLine($"On Player Initialize! - {this.UUID} ");
        }

        public void Update(float deltaTime)
        {
            Console.WriteLine($"On Player Update! {deltaTime} ");
            Console.WriteLine($"C# said 'SBDS Hello!~'");
        }
    }
}
