/*CAPSULE FUNCTIONS*/
void MakeCapsuleMesh(float* vertices, int* indices, float* color, int rings, int segments, float length)
{
    // Mesh* mesh;
    // MeshRegion* region;
    // MeshQuad* quad;

    V3 offsets[2] = {{0.0f, length / 2.0f, 0.0f}, {0.0f, -length / 2.0f, 0.0f}};

    float* v = vertices;
    int* i = indices;

    float deltaTheta = (float)(PI / rings);
    float deltaPhi = (float)( (2.0f * PI) / segments );
    
    int vertex = 0;

    float theta = 0.0f, phi = 0.0f, x, y, z;

    int ring, point;

    V3 offset = offsets[0];

    for(ring = 0; ring < rings; ++ring)
    {
        theta += deltaTheta;

        if(ring == rings / 2)
            offset = offsets[1];

        phi = 0.0f;

        for(point = 0; point < segments; ++point)
        {
            phi += deltaPhi;

            x = sinf(theta) * cosf(phi);
            y = sinf(theta) * sinf(phi);
            z = cosf(theta);
         
            *(v++) = x + offset.x;
            *(v++) = y + offset.y;
            *(v++) = z + offset.z;              
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

Capsule MakeCapsule(float d, float r)
{
    Capsule result;

    result.a = {0.0f, d, 0.0f};
    result.b = {0.0f, -d, 0.0f};
    result.r = r;

    return result;
}

void UpdateCapsule(Capsule* cap, V3 c, V3 r, float d)
{
    float x = sinf(r.y) * d;
    float y = sinf(r.x) * d;
    float z = cosf(r.x) * d;

    V3 a = {c.x + x, c.y + y, c.z + z};
    V3 b = {c.x - x, c.y - y, c.z - z};

    cap->a = a;
    cap->b = b;
}

Capsule TranslateToWorldCap(Capsule cap, V3 c, V3 r)
{
    Capsule result;

    result.r = cap.r;

    V3 ab = RVector3Scale(RVector3Sub(cap.a, cap.b), 0.5f);
    
    float d = VectorLength(ab);

    UpdateCapsule(&result, c, r, d);

    return result;
}

V3 ClosestPointOnSegment(V3 p, V3 a, V3 b)
{
    V3 seg = RVector3Sub(b, a);
    float t = Dot(RVector3Sub(p, a), seg);

    if(t <= 0.0f)
    {
        t = 0.0f;

        return a;
    }
    else
    {
        float denom = Dot(seg, seg); //Seg ^2

        if(t >= denom)
        {
            t = 1.0f;

            return b;
        }
        else
        {
            t = t / denom;
            return RVector3Add(a, RVector3Scale(seg, t));
        }
    }
}

float ClosestPtSegmentSegment(V3 s1, V3 e1, V3 s2, V3 e2, float* s, float* t, V3* c1, V3* c2)
{
    V3 d1 = RVector3Sub(e1, s1);
    V3 d2 = RVector3Sub(e2, s1);
    V3 r = RVector3Sub(s1, s2);

    float a = Dot(d1, d1);
    float e = Dot(d2, d2);
    float f = Dot(d2, r);

    float c = Dot(d1, r);
    float b = Dot(d1, d2);

    float denom = a * e - b * b;

    if(denom != 0.0f)
    {
        *s = Clamp((b * f - c * e) / denom, 0.0f, 1.0f);
    }
    else
    {
        *s = 0.0f;
    }

    *t = (b * (*s) + f) / e;

    if(*t < 0.0f)
    {
        *t = 0.0f;
        *s = Clamp(-c / a, 0.0f, 1.0f);
    }
    else if(*t > 1.0f)
    {
        *t = 1.0f;
        *s = Clamp((b - c) / a, 0.0f, 1.0f);
    }

    *c1 = RVector3Scale(RVector3Add(s1, d1), *t);
    *c2 = RVector3Scale(RVector3Add(s2, d2), *s);

    V3 dist = RVector3Sub(*c1, *c2);

    return Dot(dist, dist);
}

float SqDistPointSegment(V3 p, V3 a, V3 b)
{
    V3 ab = RVector3Sub(a, b);
    V3 ac = RVector3Sub(a, p);
    V3 bc = RVector3Sub(b, p);

    float e = Dot(ac, bc);

    if(e <= 0.0f)   return Dot(ac, ac);

    float f = Dot(ab, ab);

    if(e >= f)      return Dot(bc, bc);

    return Dot(ac, ac) - e * e / f;
}

int TestCapsuleRect(Capsule c, Rect r)
{
    V3 capsule_center = RVector3Scale(RVector3Sub(c.b, c.a), 0.5f);
    VectorAdd(&capsule_center, c.a);

	V3 closest_capsule = ClosestPointOnSegment(r.c, c.a, c.b);
    V3 closest_rect = ClosestPointOnRect(closest_capsule, r);

    V3 dist = RVector3Sub(closest_capsule, closest_rect);

    return Dot(dist, dist) <= c.r * c.r;
}

int TestCapsuleSphere(Capsule c, Sphere s)
{
    float dist = SqDistPointSegment(s.c, c.a, c.b);
    float radius = s.r + c.r;

    return dist <= radius * radius;
}

int TestCapsuleCapsule(Capsule c1, Capsule c2)
{
    float s, t;

    V3 p1, p2;

    float dist = ClosestPtSegmentSegment(c1.a, c1.b, c2.a, c2.b, &t, &s, &p1, &p2);

    float radius = c1.r + c2.r;

    return dist <= radius * radius;
}

/*CAPSULE FUNCTIONS*/