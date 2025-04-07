#include "World.h"
#include "CollisionDetection.h"
#include "Constants.h"

World::World(float gravity)
{
    this->gravity = -gravity;
}

World::~World()
{
    for(Body* body: bodies)
    {
        delete body;
    }
}

void World::AddBody(Body* body)
{
    bodies.push_back(body);
}

std::vector<Body*> &World::GetBodies()
{
    return bodies;
}

void World::AddForce(const Vec2& force)
{
    forces.push_back(force);
}

void World::AddTorque(float torque)
{
    torques.push_back(torque);
}

void World::Update(float deltaTime)
{
    for(Body* body: bodies)
    {
        Vec2 weight = Vec2(0.0f, body->mass * gravity * PIXELS_PER_METER);
        body->AddForce(weight);

        for(Vec2 force: forces)
        {
            body->AddForce(force);
        }

        for(float torque: torques)
        {
            body->AddTorque(torque);
        }
    }

    for(Body* body: bodies)
    {
        body->Update(deltaTime);
    }

    CheckCollisions();
}

void World::CheckCollisions()
{
    for(Body* body: bodies)
    {
        body->isColliding = false;
    }

    for(int i = 0; i < bodies.size() - 1; i++)
    {
        for(int j = i + 1; j < bodies.size(); j++)
        {
            Body* a = bodies[i];
            Body* b = bodies[j];

            Contact contact;

            if(CollisionDetection::IsColliding(a, b, contact))
            {
                contact.ResolveCollision();

                a->isColliding = true;
                b->isColliding = true;

                // if (debug)
                // {
                //     Graphics::DrawFillCircle(contact.start.x, contact.start.y, 3, 0xFFFF00FF);
                //     Graphics::DrawFillCircle(contact.end.x, contact.end.y, 3, 0xFFFF00FF);
                //     Graphics::DrawLine(contact.start.x, contact.start.y, contact.start.x + contact.normal.x * 15,
                //                        contact.start.y + contact.normal.y * 15, 0xFFFF00FF);
                // }
            }
        }
    }
}
