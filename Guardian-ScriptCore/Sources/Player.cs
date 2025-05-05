using System;
using GE;

namespace GE
{
    class Player : Entity
    {
        public float Speed = 0.0f;
        public float Mass = 11.45f;
        public int State = 114514;
        public float TimeStep = 0.0f;

        public void Initialize()
        {
            Console.WriteLine($"On Player Initialize! - {this.UUID} ");
            this.TimeStep = 0.0f;
        }

        public void Update(float deltaTime)
        {
            this.TimeStep += deltaTime / 1000.0f;
            GVector3 translation = this.Translation;

            if (Input.IsKeyPressed('W'))
            {
                translation.z += Speed;
            }
            if (Input.IsKeyPressed('S'))
            {
                translation.z -= Speed;
            }
            if (Input.IsKeyPressed('A'))
            {
                translation.x -= Speed;
            }
            if (Input.IsKeyPressed('D'))
            {
                translation.x += Speed;
            }
            if (Input.IsKeyPressed(0x20))
            {
                translation.y += Speed;
            }
            if (Input.IsKeyPressed(0x10))
            {
                translation.y -= Speed;
            }

            this.Translation = translation;
        }
    }
}
