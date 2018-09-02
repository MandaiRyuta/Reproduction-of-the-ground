#include "Light.h"

void Light::Init(LPDIRECT3DDEVICE9 device)
{
	color_ = { 0.3f, 0.3f, 0.3f, 0.3f };
	ZeroMemory(&mat_, sizeof(mat_));
	MaterialSetLight(device, mat_);

	ZeroMemory(&light_, sizeof(light_));
	LightSet(device, light_);
}

void Light::MaterialSetLight(LPDIRECT3DDEVICE9 device, D3DMATERIAL9 mat)
{
	mat.Diffuse.r = 1.0f;
	mat.Diffuse.g = 1.0f;
	mat.Diffuse.b = 1.0f;
	mat.Diffuse.a = 1.0f;

	mat.Ambient.r = 1.0f;
	mat.Ambient.g = 1.0f;
	mat.Ambient.b = 1.0f;
	mat.Ambient.a = 1.0f;
	
	device->SetMaterial(&mat);
}

void Light::LightSet(LPDIRECT3DDEVICE9 device, D3DLIGHT9 light)
{
	light.Type = D3DLIGHT_DIRECTIONAL;

	direction_ = D3DXVECTOR3(-1.0f, -3.0f, -10.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &direction_);

	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Diffuse.a = 1.0f;

	light.Ambient.r = 0.5f;
	light.Ambient.g = 0.5f;
	light.Ambient.b = 1.0f;
	light.Ambient.a = 1.0f;

	device->SetLight(0, &light);
	device->LightEnable(0, true);
	device->SetRenderState(D3DRS_AMBIENT, color_);
}
