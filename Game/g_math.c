#include "g_math.h"

V3 MakeV3(float x, float y, float z)
{
	V3 result = {x, y, z};

	return result;
}

V3 MakeZeroV3()
{
	V3 result = {};
	return result;
}

float Max(float a, float b)
{

	if(a > b)
		return a;
	else if(b > a)
		return b;
	else
		return a;
}

float RadToDeg(float rad)
{
	return rad *(180.0f/3.14f);
}

void SetDefaultVector(V3* result)
{
	result->x = 0.0f;
	result->y = 0.0f;
	result->z = 0.0f;
}

void SetVector(V3* result, V3 v)
{
	result->x = v.x;
	result->y = v.y;
	result->z = v.z;
}

void SetVectorFromValues(V3* result, float x, float y, float z)
{
	result->x = x;
	result->y = y;
	result->z = z;
}

void GetIdentityMatrix(M4* result)
{
  memset(result->data, 0, M4SIZE);
  result->data[0] = 1.0f;
  result->data[5] = 1.0f;
  result->data[10] = 1.0f;
  result->data[15] = 1.0f;
}

void GetOrthoMatrix(M4* result, float left, float right, float top, float bottom, float znear, float zfar)
{
	memset(result, 0, M4SIZE);
    
    result->data[0] = 2.0f / (right - left);
    result->data[5] = 2.0f / (top - bottom);
    result->data[10] = (-2.0f) / (zfar - znear);
    result->data[12] = -((right + left) / (right - left));
    result->data[13] = -((top + bottom) / (top - bottom));
    result->data[14] = -((zfar + znear) / (zfar - znear));
	result->data[15] = 1.0f;
    
}

void GetPerspectiveMatrix(M4* result, float fov, float aspect, float znear, float zfar)
{
    float rad = DegToRad(fov);
    float frustum_scale = 1.0f / (float)tan(rad / 2.0f);

    memset(result, 0, M4SIZE);
    
    result->data[0] = frustum_scale / aspect;
    result->data[5] = frustum_scale;
    result->data[10] = (zfar + znear) / (znear - zfar);
    result->data[14] = (2.0f * zfar * znear) / (znear - zfar);
    result->data[11] = -1.0f;
}

void MatrixMult(M4* result, M4 m, M4 m2)
{
	float* rData = result->data;
	float* mData = m.data;
	float* m2Data = m2.data;
	rData[0] = (mData[0] * m2Data[0]) + (mData[4] * m2Data[1]) + (mData[8] * m2Data[2]) + (mData[12] * m2Data[3]);
    rData[1] = (mData[1] * m2Data[0]) + (mData[5] * m2Data[1]) + (mData[9] * m2Data[2]) + (mData[13] * m2Data[3]);
    rData[2] = (mData[2] * m2Data[0]) + (mData[6] * m2Data[1]) + (mData[10] * m2Data[2]) + (mData[14] * m2Data[3]);
    rData[3] = (mData[3] * m2Data[0]) + (mData[7] * m2Data[1]) + (mData[11] * m2Data[2]) + (mData[15] * m2Data[3]);

    rData[4] = (mData[0] * m2Data[4]) + (mData[4] * m2Data[5]) + (mData[8] * m2Data[6]) + (mData[12] * m2Data[7]);
    rData[5] = (mData[1] * m2Data[4]) + (mData[5] * m2Data[5]) + (mData[9] * m2Data[6]) + (mData[13] * m2Data[7]);
    rData[6] = (mData[2] * m2Data[4]) + (mData[6] * m2Data[5]) + (mData[10] * m2Data[6]) + (mData[14] * m2Data[7]);
    rData[7] = (mData[3] * m2Data[4]) + (mData[7] * m2Data[5]) + (mData[11] * m2Data[6]) + (mData[15] * m2Data[7]);

    rData[8] = (mData[0] * m2Data[8]) + (mData[4] * m2Data[9]) + (mData[8] * m2Data[10]) + (mData[12] * m2Data[11]);
    rData[9] = (mData[1] * m2Data[8]) + (mData[5] * m2Data[9]) + (mData[9] * m2Data[10]) + (mData[13] * m2Data[11]);
    rData[10] = (mData[2] * m2Data[8]) + (mData[6] * m2Data[9]) + (mData[10] * m2Data[10]) + (mData[14] * m2Data[11]);
    rData[11] = (mData[3] * m2Data[8]) + (mData[7] * m2Data[9]) + (mData[11] * m2Data[10]) + (mData[15] * m2Data[11]);

    rData[12] = (mData[0] * m2Data[12]) + (mData[4] * m2Data[13]) + (mData[8] * m2Data[14]) + (mData[12] * m2Data[15]);
    rData[13] = (mData[1] * m2Data[12]) + (mData[5] * m2Data[13]) + (mData[9] * m2Data[14]) + (mData[13] * m2Data[15]);
    rData[14] = (mData[2] * m2Data[12]) + (mData[6] * m2Data[13]) + (mData[10] * m2Data[14]) + (mData[14] * m2Data[15]);
    rData[15] = (mData[3] * m2Data[12]) + (mData[7] * m2Data[13]) + (mData[11] * m2Data[14]) + (mData[15] * m2Data[15]);
}



void MatrixVectorMult(V4* out, M4 result, V4 v)
{
	float* data = result.data;

	out->x = 0.0f; out->y = 0.0f;

	out->x += data[0] * v.x;
    out->x += data[4] * v.y;
    out->x += data[8] * v.z;
    out->x += data[12] * v.w;

    out->y += data[1] * v.x;
    out->y += data[5] * v.y;
    out->y += data[9] * v.z;
    out->y += data[13] * v.w;

    out->y += data[2] * v.x;
    out->y += data[6] * v.y;
    out->y += data[10] * v.z;
    out->y += data[14] * v.w;

    out->w += data[3] * v.x;
    out->w += data[7] * v.y;
    out->w += data[11] * v.z;
    out->w += data[15] * v.w;
}

V4 RMatrixVectorMult(M4 result, V4 v)
{
	V4 out = {};

	float* data = result.data;

	out.x = 0.0f; out.y = 0.0f;

	out.x += data[0] * v.x;
    out.x += data[4] * v.y;
    out.x += data[8] * v.z;
    out.x += data[12] * v.w;

    out.y += data[1] * v.x;
    out.y += data[5] * v.y;
    out.y += data[9] * v.z;
    out.y += data[13] * v.w;

    out.y += data[2] * v.x;
    out.y += data[6] * v.y;
    out.y += data[10] * v.z;
    out.y += data[14] * v.w;

    out.w += data[3] * v.x;
    out.w += data[7] * v.y;
    out.w += data[11] * v.z;
    out.w += data[15] * v.w;

    return out;
}

V3 RVector3Add(V3 v1, V3 v2)
{
	V3 result; 
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

V3 RVector3Sub(V3 v1, V3 v2)
{
	V3 result; 
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

V3 RVector3Scale(V3 v, float scalar)
{
	V3 result;
	result.x = v.x * scalar;
	result.y = v.y * scalar;
	result.z = v.z * scalar; 
	return result;
}

V3 RVector3Mult(V3 v1, V3 v2)
{
	V3 result; 
	result.x = v1.x * v2.x;
	result.y = v1.y * v2.y;
	result.z = v1.z * v2.z;
	return result;
}

V3 RVector3Div(V3 v1, float scalar)
{
	V3 result; 
	result.x = v1.x / scalar;
	result.y = v1.y / scalar;
	result.z = v1.z / scalar;
	return result;
}

void Vector3Add(V3* result, V3 v1, V3 v2)
{
	result->x = v1.x + v2.x;
	result->y = v1.y + v2.y;
	result->z = v1.z + v2.z;
}

void VectorAdd(V3* result, V3 v)
{
	result->x += v.x;
	result->y += v.y;
	result->z += v.z;
}

void VectorSubtract(V3* result, V3 a, V3 b)
{
	result->x = a.x - b.x;
	result->y = a.y - b.y;
	result->z = a.z - b.z;
}

void VectorMult(V3* result, V3 v)
{
	result->x *= v.x;
	result->y *= v.y;
	result->z *= v.z;
}

void VectorDivideScalar(V3* result, float s)
{
	if(s == 0.0f)
		return;
	result->x /= s;
	result->y /= s;
	result->z /= s;
}

void VectorDivide(V3* result, V3 v2)
{
	if(v2.x) result->x /= v2.x;
	if(v2.y) result->y /= v2.y;
	if(v2.z) result->z /= v2.z;
}

void VectorScalarMult(V3* result, float s)
{
	result->x *= s;
	result->y *= s;
	result->z *= s;
}

void GetForwardVector(V3* result, M4* model)
{
	result->x = model->data[8];	//Z_X
	result->y = model->data[9];	//Z_Y
	result->z = model->data[10];	//Z_Z
}

void GetRightVector(V3* result, M4* model)
{
	result->x = model->data[0];	//X_X
	result->y = model->data[1];	//X_Y
	result->z = model->data[2];	//X_Z
}

float Dot(V3 a, V3 b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

float Clamp(float t, float min, float max)
{
	if(t < min)	return min;
	if(t > max)	return max;

	return t;
}

float Clamp01(float t)
{
	if(t < 0.0f)	return 0.0f;
	if(t > 1.0f)	return 1.0f;

	return t;
}

V3 RCross(V3 a, V3 b)
{
	V3 result;
    result.x = (a.y * b.z) - (a.z * b.y);
    result.y = (a.z * b.x) - (a.x * b.z);
    result.z = (a.x * b.y) - (a.y * b.x);
    return result;
}

void Cross(V3* result, V3 a, V3 b)
{
    result->x = (a.y * b.z) - (a.z * b.y);
    result->y = (a.z * b.x) - (a.x * b.z);
    result->z = (a.x * b.y) - (a.y * b.x);
}

float VectorLength(V3 v)
{
    return fabsf( sqrtf( (v.x * v.x) + (v.y * v.y) + (v.z * v.z) ) );
}

void VectorNormalize(V3* v)
{
	float length = VectorLength(*v);
	v->x /= length;
	v->y /= length;
	v->z /= length;
}


void Translate(V3* result, V3 v)
{
	result->x += v.x;
	result->y += v.y;
	result->z += v.z;
}

void RotateX(M4* result, float theta)
{
	float cosT = (float)cosf(theta);
	float sinT = (float)sinf(theta);

	result->data[0] = 1.0f;
	result->data[5] = cosT;
	result->data[6] = sinT;
	result->data[9] = -sinT;
	result->data[10] = cosT;
	result->data[15] = 1.0f;
}

void RotateY(M4* result, float theta)
{
	float cosT = (float)cosf(theta);
	float sinT = (float)sinf(theta);

    
	result->data[0] = cosT;
	result->data[5] = 1.0f;
	result->data[8] = sinT;
	result->data[2] = -sinT;
	result->data[10] = cosT;
	result->data[15] = 1.0f;
		
}

void RotateZ(M4* result, float theta)
{
	float cosT = (float)cosf(theta);
	float sinT = (float)sinf(theta);

	result->data[0] = cosT;
	result->data[1] = sinT;
	result->data[4] = -sinT;
	result->data[5] = cosT;
	result->data[10] = 1.0f;
	result->data[15] = 1.0f;	
}

void GetTranslationMatrix(M4* result, V3 p)
{
	GetIdentityMatrix(result);
	result->data[12] = p.x;
	result->data[13] = p.y;
	result->data[14] = p.z;
}

void GetRotationMatrix(M4* result, V3 r)
{
	M4 x;
	M4 y;
	M4 zy;
	M4 z;

	GetIdentityMatrix(result);
	GetIdentityMatrix(&x);
	GetIdentityMatrix(&y);
	GetIdentityMatrix(&z);

	RotateX(&x, r.x);
	RotateY(&y, r.y);

	MatrixMult(&zy, z, y);
	MatrixMult(result, x, zy);
}

void GetVectorInverse(V3* result)
{
	result->x *= -1;
	result->y *= -1;
	result->z *= -1;
}

void GetInverseOfVector(V3* result, V3 v)
{
	result->x = v.x * -1;
	result->y = v.y * -1;
	result->z = v.z * -1;
}

float GetAngle(V3 v, V3 v2)
{
	float dot;

	//VectorNormalize(&v);
	//VectorNormalize(&v2);

    dot = Dot(v, v2);
    dot = dot / (VectorLength(v) * VectorLength(v2));

    return RadToDeg( acosf(dot) );
}

void GetMM(M4* result, V3 pos, V3 rot)
{
    M4 rotation;
    M4 translation;

    GetRotationMatrix(&rotation, rot);
    GetTranslationMatrix(&translation, pos);

    MatrixMult(result, translation, rotation);
}
/*
typedef struct
{
	V3 points[3];
} Face;

void CalculateNormals(float* normals, float* vertices, int size, int* indices, int num_indices)
{
   	V3 a = {0.0f};
   	V3 b = {0.0f};
   	V3 c = {0.0f};

   	int i = 0;

	while( i < size )
	{
	   V3 n = {0.0f};

	   int num_faces = 0;

	   Face faces[10] = {};

	   int pos = 1;

	   for(int index = 0; index < num_indices; ++index)
	   {
	      Face face = {};

	      if(pos > 3)	pos = 1;

	      if(indices[index] == i)
	      {
	      	if(pos == 1)
	      	{
	      		face.points[0].x = vertices[indices[index]];
	      		face.points[0].y = vertices[indices[index]];
	      		face.points[0].z = vertices[indices[index]];


	      		face.points[1].x = vertices[indices[index+1]];
	      		face.points[1].y = vertices[indices[index+1]];
	      		face.points[1].z = vertices[indices[index+1]];


	      		face.points[2].x = vertices[indices[index+2]];
	      		face.points[2].y = vertices[indices[index+2]];
	      		face.points[2].z = vertices[indices[index+2]];
	      	}
	      	else if(pos == 2)
	      	{
	      		face.points[0].x = vertices[indices[index]];
	      		face.points[0].y = vertices[indices[index]];
	      		face.points[0].z = vertices[indices[index]];


	      		face.points[1].x = vertices[indices[index-1]].x;
	      		face.points[1].y = vertices[indices[index-1]].y;
	      		face.points[1].z = vertices[indices[index-1]].z;


	      		face.points[2].x = vertices[indices[index+1]].x;
	      		face.points[2].y = vertices[indices[index+1]].y;
	      		face.points[2].z = vertices[indices[index+1]].z;
	      	}
	      	else
	      	{
	      		face.points[0].x = vertices[indices[index]].x;
	      		face.points[0].y = vertices[indices[index]].y;
	      		face.points[0].z = vertices[indices[index]].z;


	      		face.points[1].x = vertices[indices[index-1]].x;
	      		face.points[1].y = vertices[indices[index-1]].y;
	      		face.points[1].z = vertices[indices[index-1]].z;


	      		face.points[2].x = vertices[indices[index-2]].x;
	      		face.points[2].y = vertices[indices[index-2]].y;
	      		face.points[2].z = vertices[indices[index-2]].z;
	      	}
	      }

	      faces[num_faces++] = face;

	      ++pos;
	   }

	   //Get Adjacent Faces To V
	   for(int face = 0; face < num_faces, ++face)
	   {

	      n.x += faces[face].x;
	      n.y += faces[face].y;
	      n.z += faces[face].z;
	   }

	   	VectorNormalize(&n);

	  	normals[i++] = n.x;
	  	normals[i++] = n.y;
	  	normals[i++] = n.z;
	}
}
*/