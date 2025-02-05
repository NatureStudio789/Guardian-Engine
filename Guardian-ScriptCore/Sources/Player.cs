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

            GVector3 translation = this.Translation;

            if (Input.IsKeyPressed('W'))
            { 
                translation.z += 0.1f; 
            }
            if (Input.IsKeyPressed('S'))
            {
                translation.z -= 0.1f;
            }
            if (Input.IsKeyPressed('A'))
            {
                translation.x -= 0.1f;
            }
            if (Input.IsKeyPressed('D'))
            {
                translation.x += 0.1f;
            }

            this.Translation = translation;
        }
    }
}
