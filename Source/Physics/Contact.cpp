#include "Contact.h"

void Contact::ResolvePenetration() const
{
    float da = depth / (a->inverseMass + b->inverseMass) * a->inverseMass;
    float db = depth / (a->inverseMass + b->inverseMass) * b->inverseMass;

    a->position -= normal * da;
    b->position += normal * db;
}
