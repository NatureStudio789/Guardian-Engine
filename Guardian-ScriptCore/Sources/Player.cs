using System;
using Guardian;

namespace Guardian
{
    class Player : Entity
    {
        public GuardianTransformComponent Transform;

        public void Initialize()
        {
            Console.WriteLine($"On Player Initialize! - {this.UUID} ");
            
            this.Transform = this.GetComponent<GuardianTransformComponent>();
        }

        public void Update(float deltaTime)
        {
            GVector3 translation = this.Transform.Translation;

            if (Input.IsKeyPressed('W'))
            {
                translation.z -= 0.1f;
            }
            if (Input.IsKeyPressed('S'))
            {
                translation.z += 0.1f;
            }
            if (Input.IsKeyPressed('A'))
            {
                translation.x += 0.1f;
            }
            if (Input.IsKeyPressed('D'))
            {
                translation.x -= 0.1f;
            }

            this.Transform.Translation = translation;
        }
    }
}
