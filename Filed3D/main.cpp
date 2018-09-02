#include "main.h"
#include "Renderer\Renderer.h"
//�E�B���h�E�Y�̉�ʂɒ����閼�O
const std::string CLASS_NAME = "Enemy Using Behaviortree";
const std::string WINDOW_NAME = "FrameWork";

//�O���[�o���֐�
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow, int nWindowWidth, int nWindowHeight);
void Uninit(void);
void Update(void);
void Draw(void);

CDX9Renderer* dx9render;
//main�֐�
//��ʂ̍쐬���I�u�W�F�N�g���̏������A�X�V�A�`��A�폜��������֐�
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevinstance, LPSTR lpCmdLine, int nCmdShow)
{
	//���p���Ȃ�����������
	lpCmdLine = lpCmdLine;
	hPrevinstance = hPrevinstance;

	//�������[���[�N�̊m�F
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);

	//�E�B���h�E�N���X�̓o�^
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = CLASS_NAME.c_str();
	wcex.hIconSm = NULL;

	//WNDCLASSEX�^�̒��g��o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�T�C�Y�̐ݒ�
	DWORD WStyle = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
	RECT wr = { 0, 0, WINDOWS_RECT::SCREEN_WIDTH, WINDOWS_RECT::SCREEN_HEIGHT };	// ��`�f�[�^

	AdjustWindowRect(&wr, WStyle, FALSE);

	int nWindowWidth = wr.right - wr.left;
	int nWindowHeight = wr.bottom - wr.top;

	// �E�B���h�E�̏ꏊ�𒆉��ɕύX
	// �f�X�N�g�b�v�T�C�Y�̎擾
	RECT DesktopRect;

	GetWindowRect(GetDesktopWindow(), &DesktopRect);
	int nWindowPosX = (DesktopRect.right - nWindowWidth) / 2;
	int nWindowPosY = (DesktopRect.bottom - nWindowHeight) / 2;

	(DesktopRect.right - DesktopRect.left) < nWindowPosX ? nWindowPosX = 0 : nWindowPosX;
	(DesktopRect.bottom - DesktopRect.top) < nWindowPosY ? nWindowPosY = 0 : nWindowPosY;


	// �E�B���h�E�̍쐬
	HWND hWnd = CreateWindowEx(
		0,
		CLASS_NAME.c_str(),
		WINDOW_NAME.c_str(),
		WStyle,
		nWindowPosX,
		nWindowPosY,
		nWindowWidth,
		nWindowHeight,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// ���b�Z�[�W&�Q�[�����[�v
	MSG msg;
	DWORD dwTime = timeGetTime();
	DWORD dwOldTime = timeGetTime();

	// �^�C�}�[�̐��\���グ��
	timeBeginPeriod(1);

	// ������(�`�F�b�N����)
	if (!Init(hInstance, hWnd, true, nWindowWidth, nWindowHeight))
	{
		MessageBox(hWnd, "�������Ɏ��s���܂���!!", "�x��", MB_OK);

		return -1;
	}
	//���C�����[�v
	//������Update,Draw�֐���ǂ�Ń��[�v������B
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{// ���b�Z�[�W����
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{// �Q�[������
			dwTime = timeGetTime();

			if ((dwTime - dwOldTime) >= (1000 / 60))
			{
				// �X�V
				Update();

				// �`��
				Draw();

				// �O��̎����̍X�V
				dwOldTime = dwTime;
			}
		}
	}

	// �I��
	Uninit();

	timeEndPeriod(1);

	return (int)msg.wParam;

}

//================================================================================
//
// �E�B���h�E�v���V�[�W���֐�
//
//================================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY: // �E�B���h�E�j�����b�Z�[�W
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
		{// �I�����b�Z�[�W

			int nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO | MB_DEFBUTTON2);

			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
			}
			else
			{
				return 0;
			}
		}
		break;

		}
		break;

	case WM_CLOSE:
	{
		int nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO | MB_DEFBUTTON2);

		if (nID == IDYES)
		{
			DestroyWindow(hWnd);
		}
		break;
	}

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow, int nWindowWidth, int nWindowHeight)
{
	bool bSetWindow;
	// �g���Ă��Ȃ������̏���
	hInstance = hInstance;

	bWindow >= 1 ? bSetWindow = true : bSetWindow = false;

	dx9render = new CDX9Renderer();
	dx9render->Init(hWnd, true, nWindowWidth, nWindowHeight);

	return true;
}

void Uninit(void)
{
	dx9render->Uninit();
	delete dx9render;
	dx9render = NULL;
}

void Update(void)
{
	dx9render->Update();
}

void Draw(void)
{
	dx9render->Draw();
}