#include "Renderer.h"
#include "../Object/Camera.h"
#include "../Object/MeshFiled.h"
#include "../Object/Light.h"
bool CDX9Renderer::Init(HWND hWnd, bool bWindow, int nWindowWidth, int nWindowHeight)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);

	DX9Init(hWnd, bWindow, nWindowWidth, nWindowHeight);

	TargetObject();
	
	return true;
}

void CDX9Renderer::TargetObject()
{
	camera_ = new Camera();
	light_ = new Light();
	light_->Init(device_);
	meshfiled_ = new MeshFiled();
	meshfiled_->Init(device_);
}

void CDX9Renderer::Uninit()
{
	if (pD3D_ != NULL)
	{
		pD3D_->Release();
		pD3D_ = NULL;
	}
	if (device_ != NULL)
	{
		device_->Release();
		device_ = NULL;
	}
	if (light_ != NULL)
	{
		delete light_;
		light_ = NULL;
	}
	if (camera_ != NULL)
	{
		delete camera_;
		camera_ = NULL;
	}
	if (meshfiled_ != NULL)
	{
		meshfiled_->Uninit();
		delete meshfiled_;
		meshfiled_ = NULL;
	}

}

void CDX9Renderer::Update()
{
	camera_->Update(device_);
	meshfiled_->Update(device_);
}

void CDX9Renderer::Draw()
{
	device_->Clear(0,										// RECT構造体配列の矩形の数
		NULL,									// RECT構造体の先頭アドレス(画面全体はNULL)
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),	// TARGETは色のクリア、ZBUFFERはZバッファのクリア
		D3DCOLOR_RGBA(30, 64, 192, 255),		// クリアカラ―(TARGETがあるとき)
		1.0f,									// Zバッファのクリア値
		0);									// ステンシル値のクリア値

	SUCCEEDED(device_->BeginScene());

	meshfiled_->Draw(device_,camera_->GetView(),camera_->GetProj(),camera_->GetEye(), light_->GetDirection());

	device_->EndScene();

	device_->Present(NULL, NULL, NULL, NULL);
}

bool CDX9Renderer::DX9Init(HWND hWnd, bool bWindow, int nWindowWidth, int nWindowHeight)
{
	pD3D_ = Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3D_ == NULL)
	{
		MessageBox(hWnd, "Direct3Dインターフェースの取得に失敗しました。", NULL, MB_OK);
		return false;
	}

	D3DDISPLAYMODE d3ddm;

	if (FAILED(pD3D_->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		MessageBox(hWnd, "ディスプレイモードの取得に失敗しました。", NULL, MB_OK);
		return false;
	}

	// デバイスのプレゼンテーションパラメータ(デバイスを作成するためのデータ)
	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));							//　中のものを全てを初期化する(初期化する先頭アドレス, 初期化するサイズ)
	d3dpp.BackBufferWidth = nWindowWidth;				//　裏画面(バックバッファ)の幅
	d3dpp.BackBufferHeight = nWindowHeight;				//	裏画面(バックバッファ)の高さ
	d3dpp.BackBufferFormat = d3ddm.Format;				//	色数
	d3dpp.BackBufferCount = 1;							//	バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		//	映像信号の同期(DISCARDは垂直同期を気にする)
	d3dpp.EnableAutoDepthStencil = TRUE;						//	TRUEで良い
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					//	今はこれで良い上のフォーマット
	d3dpp.Windowed = bWindow;						//	TRUEでウィンドウモード,FALSEでフルスクリーンモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//	フルスクリーンモードの時のリフレッシュレートのHz数を変更
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//	モニターのHz数に合わせて描画される(IMMEDIATEだと垂直同期を待たずに描画する)

	if (FAILED(pD3D_->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&device_
	)))
	{
		MessageBox(hWnd, "Direct3DDeviceの作成に失敗しました", "エラー", MB_OK);
		return false;
	}
	device_->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device_->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device_->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	device_->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device_->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device_->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	device_->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device_->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device_->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return true;
}