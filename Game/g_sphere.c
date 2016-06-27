V3 GetSphereNormal(Sphere s, V3 point)
{
    return RVector3Sub(s.c, point);
}

void MakeSphereMesh(float* vertices, int* indices, int rings, int segments)
{
    float* v = vertices;
    int* i = indices;

    float deltaTheta = (float)(PI / rings);
    float deltaPhi = (float)( (2.0f * PI) / segments );
    
    int vertex = 0;

    float theta = 0.0f, phi = 0.0f, x, y, z;

    int ring, point;

    for(ring = 0; ring < rings; ++ring)
    {
        theta += deltaTheta;

        phi = 0.0f;

        for(point = 0; point < segments; ++point)
        {
            phi += deltaPhi;

            x = sinf(theta) * cosf(phi);
            y = sinf(theta) * sinf(phi);
            z = cosf(theta);
         
            *(v++) = x;
            *(v++) = y;
            *(v++) = z;              
        }
    }

    for(ring = 0; ring < rings; ++ring)
    {
        for(point = 0; point < segments; ++point)
        {
        
            if(point + 1 == segments)
            {
                *(i++) = ring * segments + point; //9
                *(i++) = ring * segments; //0
                *(i++) = (ring + 1) * segments + point; //19

                *(i++) = ring * segments; //0
                *(i++) = (ring + 1) * segments; //10
                *(i++) = (ring + 1) * segments + point; //19
            }
            else
            {
                *(i++) = ring * segments + point;
                *(i++) = ring * segments + (point + 1);
                *(i++) = (ring + 1) * segments + point;

                *(i++) = ring * segments + (point + 1);
                *(i++) = (ring + 1) * segments + point;
                *(i++) = (ring + 1) * segments + (point + 1);
            }
            
                                          
        }
    }
}

Sphere MakeSphere(V3 pos, float radius)
{
    Sphere result;
    result.c = pos;
    result.r = radius;
    return result;
}

int TestSphereRect(Sphere s, Rect r, V3* p)
{
    V3 v = {};

    *p = ClosestPointOnRect(s.c, r);
    // Sphere and AArr intersect if the (squared) distance from sphere
    // center to point p is less than the (squared) sphere radius
    VectorSubtract(&v, *p, s.c);

    return Dot(v, v) <= s.r * s.r;
}

int TestSphereSphere(Sphere s1, Sphere s2)
{
    float distance = VectorLength(RVector3Sub(s1.c, s2.c));

    return distance >= s1.r + s2.r;
}