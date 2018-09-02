#pragma once
#include "../Renderer/Renderer.h"

#define FVF_MESHFILED3D ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

typedef struct {
	D3DXVECTOR3 pos;
	D3DXVECTOR3 Normal;
	D3DCOLOR color;
	D3DXVECTOR2 texcoord;
}MESHFILED_VERTEX3D;

struct MESHFILED_SIZE {
	int filed_x;
	int filed_y;
	float size_x;
	float size_z;
};

class MeshFiled
{
public:
	MeshFiled() {}
	~MeshFiled() {}
public:
	void Init(LPDIRECT3DDEVICE9 device);
	void Update(LPDIRECT3DDEVICE9 device);
	void Draw(LPDIRECT3DDEVICE9 device, D3DXMATRIX view, D3DXMATRIX proj, D3DXVECTOR3 eye, D3DXVECTOR3 light);
	void Uninit();
private:
	MeshInfomation meshinfo_;
	LPDIRECT3DTEXTURE9 texture;
	LPDIRECT3DTEXTURE9 bumptexture;
	LPDIRECT3DVERTEXDECLARATION9 pDecl_;
	MESHFILED_SIZE meshsize_;
	MESHFILED_VERTEX3D* mpv;

	LPD3DXEFFECT effect_;
	D3DXMATRIX world_;

	int number_of_vertices_;
	int number_of_indices_;
	int number_of_primities_;

	int SetnX;
	int SetnZ;
};