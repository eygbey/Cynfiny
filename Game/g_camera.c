#include "g_camera.h"

void InitCamera(Camera* camera, V3 pos)
{	
	SetDefaultVector(&camera->position);

	SetVector(&camera->position, pos);

	camera->rotation.y = 0.0f;
	camera->rotation.x = 0.0f;
	camera->rotation.z = 0.0f;//DegToRad(15.0f);
}

void UpdateViewMatrix(M4* result, V3 pos, V3 rot)
{
	M4 rotation;
	M4 translation;
	V3 position;

	SetVector(&position, pos);
	GetVectorInverse(&position);

	GetRotationMatrix(&rotation, rot);
	GetTranslationMatrix(&translation, position);
	
	MatrixMult(result, rotation, translation);
}