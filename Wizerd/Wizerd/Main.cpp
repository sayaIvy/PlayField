// ��{�I�� Windows �w�b�_�[ �t�@�C���� Direct3D �w�b�_�[ �t�@�C�����C���N���[�h���܂�
#include <windows.h> 
#include <windowsx.h> 
#include <d3d11.h> 

// Direct3D ���C�u���� �t�@�C�����C���N���[�h���܂�
#pragma comment (lib, "d3d11.lib") 

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include <DirectXMath.h>

#include "AudioSystem.h"

// ��ʉ𑜓x���`���܂�
#define SCREEN_WIDTH 800 
#define SCREEN_HEIGHT 600 

// �O���[�o���錾
IDXGISwapChain* swapchain; // �X���b�v �`�F�[�� �C���^�[�t�F�C�X�ւ̃|�C���^�[
ID3D11Device* dev; // Direct3D �f�o�C�X �C���^�[�t�F�C�X�ւ̃|�C���^�[
ID3D11DeviceContext* devcon; // Direct3D �f�o�C�X �R���e�L�X�g�ւ̃|�C���^�[
ID3D11RenderTargetView* backbuffer; // �o�b�N �o�b�t�@�[�ւ̃|�C���^�[
ID3D11InputLayout* pLayout; // ���̓��C�A�E�g�ւ̃|�C���^�[
ID3D11VertexShader* pVS; // ���_�V�F�[�_�[�ւ̃|�C���^�[
ID3D11PixelShader* pPS; // �s�N�Z�� �V�F�[�_�[�ւ̃|�C���^�[
ID3D11Buffer* pVBuffer; // ���_�o�b�t�@�[�ւ̃|�C���^�[

// �P��̒��_���`����\����
struct VERTEX { FLOAT X, Y, Z; DirectX::XMFLOAT4 Color; };

// �֐��v���g�^�C�v
void InitD3D(HWND hWnd); // Direct3D ��ݒ肵�ď��������܂�
void RenderFrame(void); // �P��̃t���[���������_�����O���܂�
void CleanD3D(void); // Direct3D ����ă�������������܂�
void InitGraphics(void); // �����_�����O����}�`���쐬���܂�
void InitPipeline(void); // �V�F�[�_�[��ǂݍ���ŏ������܂�

// WindowProc �֐��̃v���g�^�C�v
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


// �C�ӂ� Windows �v���O�����̃G���g�� �|�C���g
int WINAPI WinMain(HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR lpCmdLine,
  int nCmdShow)
{
  HWND hWnd;
  WNDCLASSEX wc;

  ZeroMemory(&wc, sizeof(WNDCLASSEX));

  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hInstance;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.lpszClassName = L"WindowClass";

  RegisterClassEx(&wc);

  RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
  AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

  hWnd = CreateWindowEx(NULL,
    L"WindowClass",
    L"Our First Direct3D Program",
    WS_OVERLAPPEDWINDOW,
    300,
    300,
    wr.right - wr.left,
    wr.bottom - wr.top,
    NULL,
    NULL,
    hInstance,
    NULL);

  ShowWindow(hWnd, nCmdShow);

  // Direct3D ���Z�b�g�A�b�v���ď��������܂�
  InitD3D(hWnd);

	// ----------- AudioSystem�̎g�p�� -----------
  AudioSystem audio;
  if (!audio.Initialize()) return 1;

  WaveData wave;
  if (!audio.PlayWaveSound(L"Assets/Sounds/pikopiko2.wav", wave, false)) return 1;

  // �Đ����I���܂őҋ@
  while (audio.IsPlaying()) {
    Sleep(10);
  }
  // AudioSystem�̃f�X�g���N�^�Ŏ����I�ɉ������܂�
	// ----------- AudioSystem�̎g�p�� -----------

  // ���C�� ���[�v�ɓ���܂�: 

  MSG msg;

  while (TRUE)
  {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);

      if (msg.message == WM_QUIT)
        break;
    }

    RenderFrame();
  }

  // DirectX �� COM ���N���[���A�b�v���܂�
  CleanD3D();

  return msg.wParam;
}


// ����̓v���O�����̃��C�� ���b�Z�[�W �n���h���[�ł��B
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
  case WM_DESTROY:
  {
    PostQuitMessage(0);
    return 0;
  } break;
  }

  return DefWindowProc(hWnd, message, wParam, lParam);
}


// ���̊֐��� Direct3D �����������A�g�p�ł���悤�ɏ������܂��B
void InitD3D(HWND hWnd)
{
  // �X���b�v �`�F�[���Ɋւ������ێ�����\���̂��쐬���܂��B
  DXGI_SWAP_CHAIN_DESC scd;

  // �g�p�ł���悤�ɍ\���̂��N���A���܂��B
  ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

  // �X���b�v �`�F�[���̐�������͂��܂��B struct 
  scd.BufferCount = 1; // �o�b�N �o�b�t�@�[ 1 ��
  scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32 �r�b�g �J���[���g�p���܂�
  scd.BufferDesc.Width = SCREEN_WIDTH; // �o�b�N �o�b�t�@�[�̕���ݒ肵�܂�
  scd.BufferDesc.Height = SCREEN_HEIGHT; // �o�b�N �o�b�t�@�[�̍�����ݒ肵�܂�
  scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // �X���b�v �`�F�[���̎g�p���@
  scd.OutputWindow = hWnd; // �g�p����E�B���h�E
  scd.SampleDesc.Count = 4; // �}���`�T���v���̐�
  scd.Windowed = TRUE; // �E�B���h�E/�t�� �X�N���[�� ���[�h
  scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // �t�� �X�N���[���̐؂�ւ��������܂�

  // scd �\���̂̏����g�p���āA�f�o�C�X�A�f�o�C�X �R���e�L�X�g�A����уX���b�v �`�F�[�����쐬���܂��B 
  D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE
    , NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &scd, &swapchain, &dev, NULL, &devcon);     
  // �o�b�N �o�b�t�@�[�̃A�h���X���擾���܂��B    
  ID3D11Texture2D *pBackBuffer;
  swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
  // �o�b�N �o�b�t�@�[ �A�h���X���g�p���āA�����_�[ �^�[�Q�b�g���쐬���܂��B 
  dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
  pBackBuffer->Release();
  // �����_�[ �^�[�Q�b�g���o�b�N �o�b�t�@�[�Ƃ��Đݒ肵�܂��B
  devcon->OMSetRenderTargets(1, &backbuffer, NULL);
  // �r���[�|�[�g��ݒ肵�܂� 
  D3D11_VIEWPORT viewport;
  ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
  viewport.TopLeftX = 0;
  viewport.TopLeftY = 0;
  viewport.Width = SCREEN_WIDTH;
  viewport.Height = SCREEN_HEIGHT; 
  devcon->RSSetViewports(1, &viewport);
  InitPipeline();
  InitGraphics();
}
// ����́A1 �̃t���[���������_�����O���邽�߂Ɏg�p�����֐��ł�
void RenderFrame(void) {
  // �o�b�N �o�b�t�@�[��Z���ɃN���A���܂�
  const float clearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
  devcon->ClearRenderTargetView(backbuffer, clearColor);
  // �\�����钸�_�o�b�t�@�[��I�����܂�
  UINT stride = sizeof(VERTEX);
  UINT offset = 0;
  devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
  // �g�p���Ă���v���~�e�B�u�^�C�v��I�����܂�  
  devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  // ���_�o�b�t�@���o�b�N�o�b�t�@�ɕ`�悵�܂�
  devcon->Draw(3, 0);
  // �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@��؂�ւ��܂�
  swapchain->Present(0, 0);
}
// ����� Direct3D �� COM ���N���[���A�b�v����֐��ł�
void CleanD3D(void) {
  swapchain->SetFullscreenState(FALSE,NULL);
  // �E�B���h�E ���[�h�ɐ؂�ւ��� 
  // ������ COM �I�u�W�F�N�g�����ׂĕ��ĉ������
  pLayout->Release();
  pVS->Release();
  pPS->Release();
  pVBuffer->Release();
  swapchain->Release();
  backbuffer->Release();
  dev->Release();
  devcon->Release();
}


// ����̓����_�����O����}�`���쐬����֐��ł�
void InitGraphics()
{
  // VERTEX �\���̂��g�p���ĎO�p�`���쐬���܂�
  VERTEX OurVertices[] =
  {
      {0.0f, 0.5f, 0.0f, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
      {0.45f, -0.5, 0.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
      {-0.45f, -0.5f, 0.0f, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)}
  };


  // ���_�o�b�t�@�[���쐬���܂�
  D3D11_BUFFER_DESC bd;
  ZeroMemory(&bd, sizeof(bd));

  bd.Usage = D3D11_USAGE_DYNAMIC; // CPU �� GPU �ɂ�鏑�����݃A�N�Z�X
  bd.ByteWidth = sizeof(VERTEX) * 3; // �T�C�Y�� VERTEX �\���� * 3 
  bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // ���_�o�b�t�@�[�Ƃ��Ďg�p���܂�
  bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU ���o�b�t�@�[�ɏ������ނ��Ƃ��\�ɂ��܂�

  dev->CreateBuffer(&bd, NULL, &pVBuffer); // �o�b�t�@�[���쐬���܂�


  // ���_���o�b�t�@�[�ɃR�s�[���܂�
  D3D11_MAPPED_SUBRESOURCE ms;
  devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); // �o�b�t�@�[���}�b�v���܂�
  memcpy(ms.pData, OurVertices, sizeof(OurVertices)); // �f�[�^���R�s�[���܂�
  devcon->Unmap(pVBuffer, NULL); // �o�b�t�@�[�̃}�b�v���������܂�
}


// ���̊֐��̓V�F�[�_�[��ǂݍ���ŏ������܂�
void InitPipeline()
{
  // 2 �̃V�F�[�_�[��ǂݍ���ŃR���p�C�����܂�
  ID3D10Blob* VS, * PS;
  ID3DBlob* errorBlob = nullptr;

  HRESULT hr = D3DCompileFromFile(
    L"shaders.shader",         // �t�@�C����
    nullptr,                   // �}�N��
    nullptr,                   // �C���N���[�h
    "VShader",                 // �G���g���|�C���g
    "vs_4_0",                  // �V�F�[�_�[���f��
    0, 0,
    &VS, &errorBlob
  );
  if (FAILED(hr)) {
    if (errorBlob) {
      OutputDebugStringA((char*)errorBlob->GetBufferPointer());
      errorBlob->Release();
    }
    // �G���[����
    return;
  }

  hr = D3DCompileFromFile(
    L"shaders.shader",
    nullptr,
    nullptr,
    "PShader",
    "ps_4_0",
    0, 0,
    &PS, &errorBlob
  );
  if (FAILED(hr)) {
    if (errorBlob) {
      OutputDebugStringA((char*)errorBlob->GetBufferPointer());
      errorBlob->Release();
    }
    // �G���[����
    return;
  }

  dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
  dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

  devcon->VSSetShader(pVS, 0, 0);
  devcon->PSSetShader(pPS, 0, 0);

  D3D11_INPUT_ELEMENT_DESC ied[] =
  {
      {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
      {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
  };

  dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
  devcon->IASetInputLayout(pLayout);

  VS->Release();
  PS->Release();
}
