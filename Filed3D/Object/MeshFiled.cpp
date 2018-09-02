#include "MeshFiled.h"
#include "../main.h"

void MeshFiled::Init(LPDIRECT3DDEVICE9 device)
{
	meshsize_.filed_x = 50;
	meshsize_.filed_y = 50;
	meshsize_.size_x = 20.0f;
	meshsize_.size_z = 20.0f;

	int nCount = 0;
	float fStartX = -this->meshsize_.size_x * (this->meshsize_.filed_x / 2), fStartY = 0.0f, fStartZ = this->meshsize_.size_z * (this->meshsize_.filed_y / 2);

	int nCx = this->meshsize_.filed_x + 1, nCy = this->meshsize_.filed_y + 1;

	this->number_of_vertices_ = nCx * nCy;														//頂点数
	this->number_of_indices_ = (nCx * 2 + 1) * (nCy - 1) + ((nCy - 2) * 1);				//インデックス数
	this->number_of_primities_ = this->number_of_indices_ - 2;								//Primitive数


	if (FAILED(device->CreateVertexBuffer(
		sizeof(MESHFILED_VERTEX3D) * this->number_of_vertices_,
		D3DUSAGE_WRITEONLY,
		FVF_MESHFILED3D,
		D3DPOOL_MANAGED,
		&this->meshinfo_.vertex_buffer,
		NULL))) {
		PostQuitMessage(0);
	}

	this->meshinfo_.vertex_buffer->Lock(0, 0, (void**)&mpv, D3DLOCK_DISCARD);

	/*
	VX = (P+1) - (P-1);
	*/
	/*
	NX.x = -VX.y;
	NX.y = VX.x;
	NX.z = 0.0;
	*/
	D3DXVECTOR3 vx;
	D3DXVECTOR3 vz;
	D3DXVECTOR3 n;
	/*
	フィールドの端は見せない、もし見る場合があるなら、最後の法線を１にセットしておく
	*/

	for (int nZ = 0; nZ < nCy; nZ++) {
		for (int nX = 0; nX < nCx; nX++) {

			mpv[nCount].pos = D3DXVECTOR3(fStartX + (this->meshsize_.size_x * nX), 0.0f , fStartZ - (this->meshsize_.size_z * nZ));

			vx = mpv[nZ * nCy + (nX + 1)].pos - mpv[nZ * nCx + (nX - 1)].pos;
			vz = mpv[(nZ + 1) + nCy * nX].pos - mpv[(nZ - 1)+ nX * nCy].pos;

			D3DXVec3Cross(&n, &vx, &vz);
			D3DXVec3Normalize(&n, &n);
			mpv[nZ * nCx + nX].Normal = n;

			mpv[nCount].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			mpv[nCount].texcoord = D3DXVECTOR2((this->meshsize_.size_x * nX) / 10, (this->meshsize_.size_z * nZ) / 10);

			nCount++;
		}
	}

	this->meshinfo_.vertex_buffer->Unlock();

	LPWORD ppIndex;
	int nS = 0, nF = 0, nC = 2 * nCx, nD = 2 * nCx + 1;

	if (FAILED(device->CreateIndexBuffer(
		sizeof(WORD)* this->number_of_indices_,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&this->meshinfo_.index_buffer,
		NULL))) {
		PostQuitMessage(0);
	}

	this->meshinfo_.index_buffer->Lock(0, 0, (void**)&ppIndex, D3DLOCK_DISCARD);

	for (int nCount = 0; nCount < this->number_of_indices_; nCount++)
	{
		//インデックス偶数番の縮退
		if (nCount == nC)
		{
			ppIndex[nCount] = ppIndex[nCount - 1];
			nC += (2 * (nCx + 1));
			continue;
		}
		//インデックス偶数番
		if (nCount % 2 == 0)
		{
			ppIndex[nCount] = nCx + nS;
			nS++;
		}
		//インデックス奇数番の縮退
		if (nCount == nD)
		{
			ppIndex[nCount] = nCx + nS;
			nD += (2 * (nCx + 1));
			continue;
		}
		//インデックス奇数番
		if (nCount % 2 == 1)
		{
			ppIndex[nCount] = nF;
			if (nCount == ((nC * nCx) - 1))
			{
				ppIndex[nCount] = nF;
				continue;
			}
			else
			{
				nF++;
			}
		}
	}

	this->meshinfo_.index_buffer->Unlock();

	D3DXCreateTextureFromFile(device, TEXTURES::FLOOR.c_str(), &texture);

	D3DXMatrixIdentity(&this->world_);
}

void MeshFiled::Update(LPDIRECT3DDEVICE9 device)
{
}

void MeshFiled::Draw(LPDIRECT3DDEVICE9 device, D3DXMATRIX view, D3DXMATRIX proj, D3DXVECTOR3 eye, D3DXVECTOR3 light)
{
	device->SetFVF(FVF_MESHFILED3D);
	device->SetStreamSource(0, this->meshinfo_.vertex_buffer, 0, sizeof(MESHFILED_VERTEX3D));
	device->SetIndices(this->meshinfo_.index_buffer);
	device->SetTexture(0, texture);
	//device->SetMaterial(&this->MeshMaterial_);
	device->SetRenderState(D3DRS_LIGHTING, true);
	device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	device->SetTransform(D3DTS_WORLD, &this->world_);
	device->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, this->number_of_vertices_, 0, this->number_of_primities_);
}

void MeshFiled::Uninit()
{
	if (meshinfo_.vertex_buffer != NULL)
	{
		meshinfo_.vertex_buffer->Release();
		meshinfo_.vertex_buffer = NULL;
	}
	if (meshinfo_.index_buffer != NULL)
	{
		meshinfo_.index_buffer->Release();
		meshinfo_.index_buffer = NULL;
	}
	if (texture != NULL)
	{
		texture->Release();
		texture = NULL;
	}
}
