#pragma once
#include "../Renderer/Renderer.h"

class Light
{
public:
	Light() {}
	~Light() {}

public:
	void Init(LPDIRECT3DDEVICE9 device);
	void Uninit() {}
	void Update() {}
	void Draw() {}
	D3DXVECTOR3 GetDirection() { return direction_; }
private:
	void MaterialSetLight(LPDIRECT3DDEVICE9 device, D3DMATERIAL9 mat);
	void LightSet(LPDIRECT3DDEVICE9 device, D3DLIGHT9 light);
private:
	D3DMATERIAL9 mat_;
	D3DLIGHT9 light_;
	D3DXVECTOR3 direction_;
	D3DXCOLOR color_;
};