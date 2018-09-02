#pragma once
#include <d3dx9.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#ifndef _DEBUG
#pragma comment(lib,"d3dx9d.lib")
#endif

class Camera
{
public:
	Camera() :
		at_({0.0f, 0.0f, 0.0f}),
		eye_({0.0f, 5.0f, 20.0f}),
		up_({0.0f, 1.0f, 0.0f}),
		CameraMoveSpeed_(1.5f)
		{}
	~Camera() {}
public:
	void Update(LPDIRECT3DDEVICE9 device);

	D3DXMATRIX GetView() { return view_; }
	D3DXMATRIX GetProj() { return proj_; }
	D3DXVECTOR3 GetEye() { return eye_;  }
private:
	D3DXVECTOR3 at_;
	D3DXVECTOR3 eye_;
	D3DXVECTOR3 up_;

	D3DXMATRIX view_;
	D3DXMATRIX proj_;

	float CameraMoveSpeed_;
};