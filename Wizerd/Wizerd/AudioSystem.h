#pragma once

#include <io.h>
#include <iostream>
#include <string>
#include <xaudio2.h>
#include <mmsystem.h>
#pragma comment (lib, "XAudio2.lib")
#pragma comment (lib, "winmm.lib")

struct WaveData
{
  WAVEFORMATEX m_wavFormat;
  char* m_soundBuffer = nullptr;
  DWORD m_size = 0;

  ~WaveData() { delete[] m_soundBuffer; }
};

class AudioSystem
{
public:
  AudioSystem()
    : mXAudio2(nullptr), mMasteringVoice(nullptr), mSourceVoice(nullptr)
  {
  }

  ~AudioSystem()
  {
    if (mSourceVoice) mSourceVoice->DestroyVoice();
    if (mMasteringVoice) mMasteringVoice->DestroyVoice();
    if (mXAudio2) mXAudio2->Release();
    CoUninitialize();
  }

  bool Initialize()
  {
    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr)) {
      std::cout << "COMの初期化に失敗しました。" << std::endl;
      return false;
    }
    hr = XAudio2Create(&mXAudio2);
    if (FAILED(hr)) {
      std::cout << "XAudio2の初期化に失敗しました。" << std::endl;
      return false;
    }
    hr = mXAudio2->CreateMasteringVoice(&mMasteringVoice);
    if (FAILED(hr)) {
      std::cout << "マスターボイスの作成に失敗しました。" << std::endl;
      return false;
    }
    return true;
  }

  bool LoadWaveFile(const std::wstring& wFilePath, WaveData& outData)
  {
    delete[] outData.m_soundBuffer;
    outData.m_soundBuffer = nullptr;
    outData.m_size = 0;

    HMMIO mmioHandle = nullptr;
    MMCKINFO chunkInfo{};
    MMCKINFO riffChunkInfo{};

    mmioHandle = mmioOpen(
      (LPWSTR)wFilePath.data(),
      nullptr,
      MMIO_READ
    );

    if (!mmioHandle) {
      std::cout << "WAVファイルのオープンに失敗しました。" << std::endl;
      return false;
    }

    riffChunkInfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
    if (mmioDescend(mmioHandle, &riffChunkInfo, nullptr, MMIO_FINDRIFF) != MMSYSERR_NOERROR) {
      std::cout << "RIFFチャンクの侵入に失敗しました。" << std::endl;
      mmioClose(mmioHandle, 0);
      return false;
    }

    chunkInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
    if (mmioDescend(mmioHandle, &chunkInfo, &riffChunkInfo, MMIO_FINDCHUNK) != MMSYSERR_NOERROR) {
      std::cout << "fmtチャンクの侵入に失敗しました。" << std::endl;
      mmioClose(mmioHandle, 0);
      return false;
    }

    DWORD readSize = mmioRead(
      mmioHandle,
      (HPSTR)&outData.m_wavFormat,
      chunkInfo.cksize
    );
    if (readSize != chunkInfo.cksize) {
      std::cout << "fmtチャンクの読み込みに失敗しました。" << std::endl;
      mmioClose(mmioHandle, 0);
      return false;
    }

    if (outData.m_wavFormat.wFormatTag != WAVE_FORMAT_PCM) {
      std::cout << "WAVEフォーマットがPCMではありません。" << std::endl;
      mmioClose(mmioHandle, 0);
      return false;
    }

    if (mmioAscend(mmioHandle, &chunkInfo, 0) != MMSYSERR_NOERROR) {
      std::cout << "fmtチャンクの退出に失敗しました。" << std::endl;
      mmioClose(mmioHandle, 0);
      return false;
    }

    chunkInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
    if (mmioDescend(mmioHandle, &chunkInfo, &riffChunkInfo, MMIO_FINDCHUNK) != MMSYSERR_NOERROR) {
      std::cout << "dataチャンクの侵入に失敗しました。" << std::endl;
      mmioClose(mmioHandle, 0);
      return false;
    }
    outData.m_size = chunkInfo.cksize;

    outData.m_soundBuffer = new char[chunkInfo.cksize];
    readSize = mmioRead(mmioHandle, (HPSTR)outData.m_soundBuffer, chunkInfo.cksize);
    if (readSize != chunkInfo.cksize) {
      std::cout << "dataチャンクの読み込みに失敗しました。" << std::endl;
      mmioClose(mmioHandle, 0);
      delete[] outData.m_soundBuffer;
      outData.m_soundBuffer = nullptr;
      return false;
    }

    mmioClose(mmioHandle, 0);
    return true;
  }

  bool PlayWaveSound(const std::wstring& fileName, WaveData& outData, bool loop)
  {
    if (!LoadWaveFile(fileName, outData)) {
      std::cout << "Waveファイルの読み込みに失敗しました。" << std::endl;
      return false;
    }

    if (mSourceVoice) {
      mSourceVoice->DestroyVoice();
      mSourceVoice = nullptr;
    }

    WAVEFORMATEX waveFormat{};
    memcpy(&waveFormat, &outData.m_wavFormat, sizeof(outData.m_wavFormat));
    waveFormat.wBitsPerSample = outData.m_wavFormat.nBlockAlign * 8 / outData.m_wavFormat.nChannels;

    HRESULT result = mXAudio2->CreateSourceVoice(&mSourceVoice, &waveFormat);
    if (FAILED(result)) {
      std::cout << "ソースボイスの作成に失敗しました。" << std::endl;
      return false;
    }

    XAUDIO2_BUFFER xAudio2Buffer{};
    xAudio2Buffer.pAudioData = (BYTE*)outData.m_soundBuffer;
    xAudio2Buffer.Flags = XAUDIO2_END_OF_STREAM;
    xAudio2Buffer.AudioBytes = outData.m_size;
    xAudio2Buffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;

    mSourceVoice->SubmitSourceBuffer(&xAudio2Buffer);
    mSourceVoice->Start();

    return true;
  }

  bool IsPlaying() const
  {
    if (!mSourceVoice) return false;
    XAUDIO2_VOICE_STATE state;
    mSourceVoice->GetState(&state);
    return state.BuffersQueued > 0;
  }

private:
  IXAudio2* mXAudio2;
  IXAudio2MasteringVoice* mMasteringVoice;
  IXAudio2SourceVoice* mSourceVoice;
};
