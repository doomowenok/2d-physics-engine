#include "World.h"
#include "CollisionDetection.h"
#include "Constants.h"

World::World(const float gravity)
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

void World::AddConstraint(Constraint* constraint)
{
    constraints.push_back(constraint);
}

std::vector<Constraint*>& World::GetConstraints()
{
    return constraints;
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
        body->IntegrateForces(deltaTime);
    }

    for(Constraint* constraint: constraints)
    {
        constraint->PreSolve(deltaTime);
    }

    for(int i = 0; i < 5; i++)
    {
        for(Constraint* constraint: constraints)
        {
            constraint->Solve(deltaTime);
        }
    }

    for(Constraint* constraint: constraints)
    {
        constraint->PostSolve(deltaTime);
    }

    for(Body* body: bodies)
    {
        body->IntegrateVelocities(deltaTime);
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
            }
        }
    }
}
