V3 ClosestPointOnRect(V3 p, Rect r)
{
    V3 out;

    float v = p.x;
    float min = r.c.x - r.d.x;
    float max = r.c.x + r.d.x;
    if (v < min) v = min; // v = max(v, r.min[i])
    if (v > max) v = max; // v = min(v, r.max[i])
    out.x = v;

    v = p.y;
    min = r.c.y - r.d.y;
    max = r.c.y + r.d.y;
    if (v < min) v = min; // v = max(v, r.min[i])
    if (v > max) v = max; // v = min(v, r.max[i])
    out.y = v;

    v = p.z;
    min = r.c.z - r.d.z;
    max = r.c.z + r.d.z;
    if (v < min) v = min; // v = max(v, r.min[i])
    if (v > max) v = max; // v = min(v, r.max[i])
    out.z = v;
    
    return out;
}

int TestRectRect(Rect r1, Rect r2, V3* p)
{
    float sumX = r1.d.x + r2.d.x;
    float sumY = r1.d.y + r2.d.y;
    float sumZ = r1.d.z + r2.d.z;

    V3 dist = RVector3Sub(r1.c, r2.c);

    if(fabs(dist.x) >= sumX) return 0;
    if(fabs(dist.y) >= sumY) return 0;
    if(fabs(dist.z) >= sumZ) return 0;

    return 1;
}

//Check other two axises for edge intersection
V3 CheckEdges(float p[], int point, int axis, float dims[], V3 normals[])
{
    for(int i = 0, j = 0; i < 3; ++i, j += 2)
    {
        if(i == point) continue;

        if(p[i] == dims[j])
            return RCross(normals[axis], normals[j]);
        if(p[i] == dims[j+1])
            return RCross(normals[axis], normals[j+1]);
    }
    
     return normals[axis];
}

V3 GetRectNormal(Rect r, V3 center)
{
    V3 closest = ClosestPointOnRect(center, r);

    float* p = (float*)&closest;

    float axises[6] = {
        {r.c.x + r.d.x},//x
        {r.c.x - r.d.x},
        {r.c.y + r.d.y},//y
        {r.c.y - r.d.y},
        {r.c.z - r.d.z},//z
        {r.c.z + r.d.z}
    };

    V3 normals[6] = {
        {1.0f, 0.0f, 0.0f},//x
        {-1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},//y
        {0.0f, -1.0f, 0.0f},
        {0.0f, 0.0f, -1.0f},//z
        {0.0f, 0.0f, 1.0f}
    };

    for(int point = 0, axis = 0; point < 3; ++point, axis+=2)
    {
        if(p[point] == axises[axis])
            return CheckEdges(p, point, axis, axises, normals);
        else if(p[point] == axises[axis+1])
            return CheckEdges(p, point, axis+1, axises, normals);
        else{

        }
    }

    return {0.0f, 0.0f, 0.0f};
}

#if 0
u32 CreateDefautRectMesh()
{
    float v[] = {
        -0.5f,-0.5f,0.5f,
        -0.5f,0.5f,0.5f,
        -0.5f,-0.5f,-0.5f,
        -0.5f,0.5f,-0.5f,
        0.5f,-0.5f,0.5f,
        0.5f,0.5f,0.5f,
        0.5f,-0.5f,-0.5f,
        0.5f,0.5f,-0.5f
    };

    int i[] = {
        3,2,0,
        7,6,2,
        5,4,6,
        1,0,4,
        2,6,4,
        7,3,1,
        1,3,2,
        3,7,2,
        7,5,6,
        5,1,4,
        0,1,4,
        5,8,1
    };

    float uv[] = {
        0.3f,0.0f,
        0.6f,0.0f,
        0.6f,0.3,
        0.0f,0.3f,
        0.3f,0.3f,
        0.3f,0.6f,
        0.0f,0.0f,
        0.6f,0.6f,
        1.0f,0.3f,
        0.3f,1.0f,
        0.0f,1.0f,
        0.0f,0.6f,
        1.0f,0.0f,
    };

    float n[] = {
        -1.0f 0.0f 0.0f,
        0.0f, 0.0f, -1.0f,
        1.0f,0.0f,0.0f,
        0.0f,0.0f,1.0f,
        0.0f,-1.0f,0.0f,
        0.0f,1.0f,0.0f
    }
}
#endif