// 基本的な Windows ヘッダー ファイルと Direct3D ヘッダー ファイルをインクルードします
#include <windows.h> 
#include <windowsx.h> 
#include <d3d11.h> 

// Direct3D ライブラリ ファイルをインクルードします
#pragma comment (lib, "d3d11.lib") 

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include <DirectXMath.h>

#include "AudioSystem.h"

// 画面解像度を定義します
#define SCREEN_WIDTH 800 
#define SCREEN_HEIGHT 600 

// グローバル宣言
IDXGISwapChain* swapchain; // スワップ チェーン インターフェイスへのポインター
ID3D11Device* dev; // Direct3D デバイス インターフェイスへのポインター
ID3D11DeviceContext* devcon; // Direct3D デバイス コンテキストへのポインター
ID3D11RenderTargetView* backbuffer; // バック バッファーへのポインター
ID3D11InputLayout* pLayout; // 入力レイアウトへのポインター
ID3D11VertexShader* pVS; // 頂点シェーダーへのポインター
ID3D11PixelShader* pPS; // ピクセル シェーダーへのポインター
ID3D11Buffer* pVBuffer; // 頂点バッファーへのポインター

// 単一の頂点を定義する構造体
struct VERTEX { FLOAT X, Y, Z; DirectX::XMFLOAT4 Color; };

// 関数プロトタイプ
void InitD3D(HWND hWnd); // Direct3D を設定して初期化します
void RenderFrame(void); // 単一のフレームをレンダリングします
void CleanD3D(void); // Direct3D を閉じてメモリを解放します
void InitGraphics(void); // レンダリングする図形を作成します
void InitPipeline(void); // シェーダーを読み込んで準備します

// WindowProc 関数のプロトタイプ
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


// 任意の Windows プログラムのエントリ ポイント
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

  // Direct3D をセットアップして初期化します
  InitD3D(hWnd);

	// ----------- AudioSystemの使用例 -----------
  AudioSystem audio;
  if (!audio.Initialize()) return 1;

  WaveData wave;
  if (!audio.PlayWaveSound(L"Assets/Sounds/pikopiko2.wav", wave, false)) return 1;

  // 再生が終わるまで待機
  while (audio.IsPlaying()) {
    Sleep(10);
  }
  // AudioSystemのデストラクタで自動的に解放されます
	// ----------- AudioSystemの使用例 -----------

  // メイン ループに入ります: 

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

  // DirectX と COM をクリーンアップします
  CleanD3D();

  return msg.wParam;
}


// これはプログラムのメイン メッセージ ハンドラーです。
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


// この関数は Direct3D を初期化し、使用できるように準備します。
void InitD3D(HWND hWnd)
{
  // スワップ チェーンに関する情報を保持する構造体を作成します。
  DXGI_SWAP_CHAIN_DESC scd;

  // 使用できるように構造体をクリアします。
  ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

  // スワップ チェーンの説明を入力します。 struct 
  scd.BufferCount = 1; // バック バッファー 1 つ
  scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32 ビット カラーを使用します
  scd.BufferDesc.Width = SCREEN_WIDTH; // バック バッファーの幅を設定します
  scd.BufferDesc.Height = SCREEN_HEIGHT; // バック バッファーの高さを設定します
  scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // スワップ チェーンの使用方法
  scd.OutputWindow = hWnd; // 使用するウィンドウ
  scd.SampleDesc.Count = 4; // マルチサンプルの数
  scd.Windowed = TRUE; // ウィンドウ/フル スクリーン モード
  scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // フル スクリーンの切り替えを許可します

  // scd 構造体の情報を使用して、デバイス、デバイス コンテキスト、およびスワップ チェーンを作成します。 
  D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE
    , NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &scd, &swapchain, &dev, NULL, &devcon);     
  // バック バッファーのアドレスを取得します。    
  ID3D11Texture2D *pBackBuffer;
  swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
  // バック バッファー アドレスを使用して、レンダー ターゲットを作成します。 
  dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
  pBackBuffer->Release();
  // レンダー ターゲットをバック バッファーとして設定します。
  devcon->OMSetRenderTargets(1, &backbuffer, NULL);
  // ビューポートを設定します 
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
// これは、1 つのフレームをレンダリングするために使用される関数です
void RenderFrame(void) {
  // バック バッファーを濃い青にクリアします
  const float clearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
  devcon->ClearRenderTargetView(backbuffer, clearColor);
  // 表示する頂点バッファーを選択します
  UINT stride = sizeof(VERTEX);
  UINT offset = 0;
  devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
  // 使用しているプリミティブタイプを選択します  
  devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  // 頂点バッファをバックバッファに描画します
  devcon->Draw(3, 0);
  // バックバッファとフロントバッファを切り替えます
  swapchain->Present(0, 0);
}
// これは Direct3D と COM をクリーンアップする関数です
void CleanD3D(void) {
  swapchain->SetFullscreenState(FALSE,NULL);
  // ウィンドウ モードに切り替える 
  // 既存の COM オブジェクトをすべて閉じて解放する
  pLayout->Release();
  pVS->Release();
  pPS->Release();
  pVBuffer->Release();
  swapchain->Release();
  backbuffer->Release();
  dev->Release();
  devcon->Release();
}


// これはレンダリングする図形を作成する関数です
void InitGraphics()
{
  // VERTEX 構造体を使用して三角形を作成します
  VERTEX OurVertices[] =
  {
      {0.0f, 0.5f, 0.0f, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
      {0.45f, -0.5, 0.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
      {-0.45f, -0.5f, 0.0f, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)}
  };


  // 頂点バッファーを作成します
  D3D11_BUFFER_DESC bd;
  ZeroMemory(&bd, sizeof(bd));

  bd.Usage = D3D11_USAGE_DYNAMIC; // CPU と GPU による書き込みアクセス
  bd.ByteWidth = sizeof(VERTEX) * 3; // サイズは VERTEX 構造体 * 3 
  bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // 頂点バッファーとして使用します
  bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU がバッファーに書き込むことを可能にします

  dev->CreateBuffer(&bd, NULL, &pVBuffer); // バッファーを作成します


  // 頂点をバッファーにコピーします
  D3D11_MAPPED_SUBRESOURCE ms;
  devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); // バッファーをマップします
  memcpy(ms.pData, OurVertices, sizeof(OurVertices)); // データをコピーします
  devcon->Unmap(pVBuffer, NULL); // バッファーのマップを解除します
}


// この関数はシェーダーを読み込んで準備します
void InitPipeline()
{
  // 2 つのシェーダーを読み込んでコンパイルします
  ID3D10Blob* VS, * PS;
  ID3DBlob* errorBlob = nullptr;

  HRESULT hr = D3DCompileFromFile(
    L"shaders.shader",         // ファイル名
    nullptr,                   // マクロ
    nullptr,                   // インクルード
    "VShader",                 // エントリポイント
    "vs_4_0",                  // シェーダーモデル
    0, 0,
    &VS, &errorBlob
  );
  if (FAILED(hr)) {
    if (errorBlob) {
      OutputDebugStringA((char*)errorBlob->GetBufferPointer());
      errorBlob->Release();
    }
    // エラー処理
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
    // エラー処理
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
