#include "Camera.h"
#include "../main.h"

void Camera::Update(LPDIRECT3DDEVICE9 device)
{
	//�r���[�s��̍쐬
	D3DXMatrixLookAtLH(
		&view_,
		&eye_,
		&at_,
		&up_
	);

	//�v���W�F�N�V�����s��̍쐬
	D3DXMatrixPerspectiveFovLH(&proj_, D3DX_PI / 3 /*D3DXToRadian(60),*/, (float)WINDOWS_RECT::SCREEN_WIDTH / WINDOWS_RECT::SCREEN_HEIGHT, 0.1f, 10000.0f);

	//�e��ލs��̐ݒ�
	device->SetTransform(D3DTS_VIEW, &view_);
	device->SetTransform(D3DTS_PROJECTION, &proj_);
}
