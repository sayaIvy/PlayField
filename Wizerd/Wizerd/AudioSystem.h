//#pragma once
//class AudioSystem
//{
//};
//
//#include <io.h>      // _waccess
//#include <iostream>
//
//#pragma comment (lib, "XAudio2.lib")
//#include <xaudio2.h>
//
//#pragma comment (lib, "winmm.lib")
//#include <mmsystem.h>
//
//
//#include <string>
//
//IXAudio2* mXAudio2;
//IXAudio2MasteringVoice* mMasteringVoice;
//IXAudio2SourceVoice* mSourceVoice; // Todo 複数使うなら配列にしてもいいかも
//
//struct WaveData
//{
//	WAVEFORMATEX m_wavFormat;
//	char* m_soundBuffer;
//	DWORD m_size;
//
//	~WaveData() { free(m_soundBuffer); }
//};
//WaveData waveData;
//
//
//bool LoadWaveFile(const std::wstring& wFilePath, WaveData* outData)
//{
//	// 中身入ってるもの来たら、一旦解放しとく
//	// (じゃないと、もとの中身のサウンドバッファーがある場合、メモリリークする)
//	if (outData)
//	{
//		free(outData->m_soundBuffer);
//	}
//	// nullptrが来たらリターンする
//	else
//	{
//		std::cout << "WaveDataのポインタがnullptrです。" << std::endl;
//		return false;
//	}
//
//
//	HMMIO mmioHandle = nullptr;
//
//	// チャンク情報
//	MMCKINFO chunkInfo{};
//
//	// RIFFチャンク用
//	MMCKINFO riffChunkInfo{};
//
//
//	// WAVファイルを開く
//	mmioHandle = mmioOpen(
//		(LPWSTR)wFilePath.data(),
//		nullptr,
//		MMIO_READ
//	);
//
//	if (!mmioHandle)
//	{
//		std::cout << "WAVファイルのオープンに失敗しました。" << std::endl;
//		return false;
//	}
//
//	// RIFFチャンクに侵入するためにfccTypeにWAVEを設定をする
//	riffChunkInfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
//
//	// RIFFチャンクに侵入する
//	if (mmioDescend(
//		mmioHandle,		//MMIOハンドル
//		&riffChunkInfo,	//取得したチャンクの情報
//		nullptr,		//親チャンク
//		MMIO_FINDRIFF	//取得情報の種類
//	) != MMSYSERR_NOERROR)
//	{
//		// 失敗
//		std::cout << "RIFFチャンクの侵入に失敗しました。" << std::endl;
//		mmioClose(mmioHandle, MMIO_FHOPEN);
//		return false;
//	}
//
//	// 侵入先のチャンクを"fmt "として設定する
//	chunkInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
//	if (mmioDescend(
//		mmioHandle,
//		&chunkInfo,
//		&riffChunkInfo,
//		MMIO_FINDCHUNK
//	) != MMSYSERR_NOERROR)
//	{
//		std::cout << "fmtチャンクの侵入に失敗しました。" << std::endl;
//		mmioClose(mmioHandle, MMIO_FHOPEN);
//		return false;
//	}
//
//	// fmtデータの読み込み
//	DWORD readSize = mmioRead(
//		mmioHandle,						//ハンドル
//		(HPSTR)&outData->m_wavFormat,	// 読み込み用バッファ
//		chunkInfo.cksize				//バッファサイズ
//	);
//
//	if (readSize != chunkInfo.cksize)
//	{
//		std::cout << "fmtチャンクの読み込みに失敗しました。" << std::endl;
//		mmioClose(mmioHandle, MMIO_FHOPEN);
//		return false;
//	}
//
//	// フォーマットチェック
//	if (outData->m_wavFormat.wFormatTag != WAVE_FORMAT_PCM)
//	{
//		std::cout << "WAVEフォーマットがPCMではありません。" << std::endl;
//		mmioClose(mmioHandle, MMIO_FHOPEN);
//		return false;
//	}
//
//	// fmtチャンクを退出する
//	if (mmioAscend(mmioHandle, &chunkInfo, 0) != MMSYSERR_NOERROR)
//	{
//		std::cout << "fmtチャンクの退出に失敗しました。" << std::endl;
//		mmioClose(mmioHandle, MMIO_FHOPEN);
//		return false;
//	}
//
//	// dataチャンクに侵入
//	chunkInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
//	if (mmioDescend(mmioHandle, &chunkInfo, &riffChunkInfo, MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
//	{
//		std::cout << "dataチャンクの侵入に失敗しました。" << std::endl;
//		mmioClose(mmioHandle, MMIO_FHOPEN);
//		return false;
//	}
//	// サイズ保存
//	outData->m_size = chunkInfo.cksize;
//
//	// dataチャンク読み込み
//	outData->m_soundBuffer = new char[chunkInfo.cksize];
//	readSize = mmioRead(mmioHandle, (HPSTR)outData->m_soundBuffer, chunkInfo.cksize);
//	if (readSize != chunkInfo.cksize)
//	{
//		std::cout << "dataチャンクの読み込みに失敗しました。" << std::endl;
//		mmioClose(mmioHandle, MMIO_FHOPEN);
//		delete[] outData->m_soundBuffer;
//		return false;
//	}
//
//	// ファイルを閉じる
//	mmioClose(mmioHandle, MMIO_FHOPEN);
//
//	return true;
//}
//
//bool PlayWaveSound(const std::wstring& fileName, WaveData* outData, bool loop)
//{
//	if (!LoadWaveFile(fileName, outData))
//	{
//		std::cout << "Waveファイルの読み込みに失敗しました。" << std::endl;
//		return false;
//	}
//
//	//=======================
//	// SourceVoiceの作成
//	//=======================
//	WAVEFORMATEX waveFormat{};
//
//	// 波形フォーマットの設定
//	memcpy(&waveFormat, &outData->m_wavFormat, sizeof(outData->m_wavFormat));
//
//	// 1サンプル当たりのバッファサイズを算出
//	waveFormat.wBitsPerSample = outData->m_wavFormat.nBlockAlign * 8 / outData->m_wavFormat.nChannels;
//
//	// ソースボイスの作成 ここではフォーマットのみ渡っている
//	HRESULT result = mXAudio2->CreateSourceVoice(&mSourceVoice, (WAVEFORMATEX*)&waveFormat);
//	if (FAILED(result))
//	{
//		std::cout << "ソースボイスの作成に失敗しました。" << std::endl;
//		return false;
//	}
//
//	//================================
//	// 波形データ(音データ本体)をソースボイスに渡す
//	//================================
//	XAUDIO2_BUFFER xAudio2Buffer{};
//	xAudio2Buffer.pAudioData = (BYTE*)outData->m_soundBuffer;
//	xAudio2Buffer.Flags = XAUDIO2_END_OF_STREAM;
//	xAudio2Buffer.AudioBytes = outData->m_size;
//
//	// 三項演算子を用いて、ループするか否かの設定をする
//	xAudio2Buffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;
//
//	mSourceVoice->SubmitSourceBuffer(&xAudio2Buffer);
//
//	// 実際に音を鳴らす
//	mSourceVoice->Start();
//
//	return true;
//}
//
//
//int main()
//{
//
//	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
//	if (FAILED(hr))
//	{
//		std::cout << "COMの初期化に失敗しました。" << std::endl;
//		return 0;
//	}
//
//	hr = XAudio2Create(&mXAudio2);
//	if (FAILED(hr))
//	{
//		std::cout << "XAudio2の初期化に失敗しました。" << std::endl;
//		return 0;
//	}
//
//	hr = mXAudio2->CreateMasteringVoice(&mMasteringVoice);
//	if (FAILED(hr))
//	{
//		std::cout << "マスターボイスの作成に失敗しました。" << std::endl;
//		return 0;
//	}
//
//
//	bool isPlay = false;/*PlayWaveSound(L"E:\\Game\\CPPProjects\\PlayField\\Wizerd\\Wizerd\\Assets\\Sounds\\test.wav", &waveData, false);*/
//
//	if (isPlay)
//	{
//		bool isRunning = true;
//
//		XAUDIO2_VOICE_STATE state;
//		mSourceVoice->GetState(&state);
//
//		while ((state.BuffersQueued > 0) != 0)
//		{
//			mSourceVoice->GetState(&state);
//		}
//	}
//
//
//	// 解放処理　忘れたらとんでもないメモリリークする
//	mMasteringVoice->DestroyVoice();
//	mXAudio2->Release();
//
//	CoUninitialize();
//}