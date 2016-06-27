V3 EulerIntegrate(V3 deltaP, V3 velcoity, float deltaT)
{
    V3 offset = RVector3Scale(deltaP, 0.5f);

    VectorScalarMult(&offset, deltaT * deltaT);
    // 1/2at^2 * vt
    V3 v = RVector3Scale(velcoity, deltaT);

    return RVector3Add(offset, v);
}

void AllocatePhysicsManager(PhysicsManager* manager, u32 size)
{
    manager->bodies = (RigidBody*)GetMemory(size * sizeof(RigidBody));
    manager->count = 0;
}

void AllocateCollisionManager(CollisionManager* manager, u32 size)
{
    manager->shapes = (char*)GetMemory(size);
    manager->used = 0;
    manager->capacity = size;
    manager->count = 0;
}

void PushRect(CollisionManager* manager, V3 c, V3 d)
{
    Rect r = {c, d};
    
    u32* data = (u32*)(manager->shapes + manager->used);
    u32 size = sizeof(Rect);

    *data = size;

    data++;

    memcpy(data, &r, size);
}

void PushDefaultRect(CollisionManager* manager)
{
    Rect r = {{0.0f,0.0f,0.0f},{0.5f, 0.5f, 0.5f}};
    
    u32* data = (u32*)(manager->shapes + manager->used);
    u32 size = sizeof(Rect);

    *data = size;

    data++;

    memcpy(data, &r, size);
}

void PushShape(CollisionManager* manager, void* shape, u32 size)
{
    u32* data = (u32*)(manager->shapes + manager->used);

    *data = size;

    data++;

    memcpy(data, shape, size);

}

void MoveRect(CollisionManager* manager, RigidBody* body, Rect rect, V3 deltaP, float deltaT)
{
    V3 velocity;
    V3 offset = EulerIntegrate(deltaP, body->v, deltaT);

    float epsilon = 0.001f;

    VectorAdd(&velocity, RVector3Scale(deltaP, deltaT));

    //Check For Collisions
    for(int it = 0; it < 4; ++it)
    {
        Contact contacts[5];
        int contact_count = 0;

        float t = 1.0f;

        if( VectorLength(offset) )
        {
            V3 point;

            int result = 0;
            int bytes = 0;
            u32 size = 0;

            int count = 0;

            char* shapes = manager->shapes;

            while(bytes < manager->used)
            {
                u32* t_shapes = (u32*)(shapes++);
                size = *t_shapes;

                if(size == RECT_SIZE)
                {
                    Rect* r = (Rect*)(shapes + bytes);
                    
                    result = TestRectRect(rect, *r, &point);

                    if(result)
                    {
                        Contact c = {};
                        c.p1 = ClosestPointOnRect(body->c, *r);
                        c.p2 = ClosestPointOnRect(c.p1, rect);
                        V3 dist = RVector3Sub(c.p1, c.p2);

                        float tHit = Max(0.0f, VectorLength(dist) / VectorLength(offset) - epsilon);

                        if(tHit < t) t = tHit;

                        c.t = tHit;
                        //c.d = CalculateClosingV(body->c, body->v, , );
                        
                        Rect minkowski_rect = *r;
                        VectorAdd(&minkowski_rect.d, rect.d); 

                        c.normal = GetRectNormal(minkowski_rect, body->c);

                        contacts[contact_count++] = c;
                    }
                }                
                else if(size == SPHERE_SIZE)
                {
                    Sphere* s = (Sphere*)(shapes + bytes);

                    result = TestSphereRect(*s, rect, &point);

                    if(result)
                    {
                        //TO DO: Create Contact
                    }
                }
                else if(size == CAPSULE_SIZE)
                {
                    Capsule* cap = (Capsule*)(shapes + bytes);

                    result = TestCapsuleRect(*cap, rect);

                    if(result)
                    {
                        
                    }
                }
                else
                {
                    //GJK
                }

                bytes += size;
                ++count;
                
            }//End of Entities

            //Calculate New Positions and Velocities
            VectorAdd(&body->c, RVector3Scale(offset, t));

            if(t == 1.0f)   break;

            {
                int contact = 0;
                while(contact < contact_count)
                {
                    Contact* c = &contacts[contact++];
                    V3 normal = c->normal;
                    V3 v = body->v;

                    float contact_speed = Dot(normal, v);

                    float dot = Dot(v, normal);
                    VectorScalarMult(&normal, dot);
                    VectorSubtract(&body->v, v, normal);

                    normal = c->normal;

                    dot = Dot(offset, normal);
                    VectorScalarMult(&normal, dot);
                    VectorSubtract(&offset, offset, normal);
                    
                }
            }
             
        }
        else
        {
            break;
        }
    }//End of Iteration
}