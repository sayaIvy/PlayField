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
//IXAudio2SourceVoice* mSourceVoice; // Todo �����g���Ȃ�z��ɂ��Ă���������
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
//	// ���g�����Ă���̗�����A��U������Ƃ�
//	// (����Ȃ��ƁA���Ƃ̒��g�̃T�E���h�o�b�t�@�[������ꍇ�A���������[�N����)
//	if (outData)
//	{
//		free(outData->m_soundBuffer);
//	}
//	// nullptr�������烊�^�[������
//	else
//	{
//		std::cout << "WaveData�̃|�C���^��nullptr�ł��B" << std::endl;
//		return false;
//	}
//
//
//	HMMIO mmioHandle = nullptr;
//
//	// �`�����N���
//	MMCKINFO chunkInfo{};
//
//	// RIFF�`�����N�p
//	MMCKINFO riffChunkInfo{};
//
//
//	// WAV�t�@�C�����J��
//	mmioHandle = mmioOpen(
//		(LPWSTR)wFilePath.data(),
//		nullptr,
//		MMIO_READ
//	);
//
//	if (!mmioHandle)
//	{
//		std::cout << "WAV�t�@�C���̃I�[�v���Ɏ��s���܂����B" << std::endl;
//		return false;
//	}
//
//	// RIFF�`�����N�ɐN�����邽�߂�fccType��WAVE��ݒ������
//	riffChunkInfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
//
//	// RIFF�`�����N�ɐN������
//	if (mmioDescend(
//		mmioHandle,		//MMIO�n���h��
//		&riffChunkInfo,	//�擾�����`�����N�̏��
//		nullptr,		//�e�`�����N
//		MMIO_FINDRIFF	//�擾���̎��
//	) != MMSYSERR_NOERROR)
//	{
//		// ���s
//		std::cout << "RIFF�`�����N�̐N���Ɏ��s���܂����B" << std::endl;
//		mmioClose(mmioHandle, MMIO_FHOPEN);
//		return false;
//	}
//
//	// �N����̃`�����N��"fmt "�Ƃ��Đݒ肷��
//	chunkInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
//	if (mmioDescend(
//		mmioHandle,
//		&chunkInfo,
//		&riffChunkInfo,
//		MMIO_FINDCHUNK
//	) != MMSYSERR_NOERROR)
//	{
//		std::cout << "fmt�`�����N�̐N���Ɏ��s���܂����B" << std::endl;
//		mmioClose(mmioHandle, MMIO_FHOPEN);
//		return false;
//	}
//
//	// fmt�f�[�^�̓ǂݍ���
//	DWORD readSize = mmioRead(
//		mmioHandle,						//�n���h��
//		(HPSTR)&outData->m_wavFormat,	// �ǂݍ��ݗp�o�b�t�@
//		chunkInfo.cksize				//�o�b�t�@�T�C�Y
//	);
//
//	if (readSize != chunkInfo.cksize)
//	{
//		std::cout << "fmt�`�����N�̓ǂݍ��݂Ɏ��s���܂����B" << std::endl;
//		mmioClose(mmioHandle, MMIO_FHOPEN);
//		return false;
//	}
//
//	// �t�H�[�}�b�g�`�F�b�N
//	if (outData->m_wavFormat.wFormatTag != WAVE_FORMAT_PCM)
//	{
//		std::cout << "WAVE�t�H�[�}�b�g��PCM�ł͂���܂���B" << std::endl;
//		mmioClose(mmioHandle, MMIO_FHOPEN);
//		return false;
//	}
//
//	// fmt�`�����N��ޏo����
//	if (mmioAscend(mmioHandle, &chunkInfo, 0) != MMSYSERR_NOERROR)
//	{
//		std::cout << "fmt�`�����N�̑ޏo�Ɏ��s���܂����B" << std::endl;
//		mmioClose(mmioHandle, MMIO_FHOPEN);
//		return false;
//	}
//
//	// data�`�����N�ɐN��
//	chunkInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
//	if (mmioDescend(mmioHandle, &chunkInfo, &riffChunkInfo, MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
//	{
//		std::cout << "data�`�����N�̐N���Ɏ��s���܂����B" << std::endl;
//		mmioClose(mmioHandle, MMIO_FHOPEN);
//		return false;
//	}
//	// �T�C�Y�ۑ�
//	outData->m_size = chunkInfo.cksize;
//
//	// data�`�����N�ǂݍ���
//	outData->m_soundBuffer = new char[chunkInfo.cksize];
//	readSize = mmioRead(mmioHandle, (HPSTR)outData->m_soundBuffer, chunkInfo.cksize);
//	if (readSize != chunkInfo.cksize)
//	{
//		std::cout << "data�`�����N�̓ǂݍ��݂Ɏ��s���܂����B" << std::endl;
//		mmioClose(mmioHandle, MMIO_FHOPEN);
//		delete[] outData->m_soundBuffer;
//		return false;
//	}
//
//	// �t�@�C�������
//	mmioClose(mmioHandle, MMIO_FHOPEN);
//
//	return true;
//}
//
//bool PlayWaveSound(const std::wstring& fileName, WaveData* outData, bool loop)
//{
//	if (!LoadWaveFile(fileName, outData))
//	{
//		std::cout << "Wave�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B" << std::endl;
//		return false;
//	}
//
//	//=======================
//	// SourceVoice�̍쐬
//	//=======================
//	WAVEFORMATEX waveFormat{};
//
//	// �g�`�t�H�[�}�b�g�̐ݒ�
//	memcpy(&waveFormat, &outData->m_wavFormat, sizeof(outData->m_wavFormat));
//
//	// 1�T���v��������̃o�b�t�@�T�C�Y���Z�o
//	waveFormat.wBitsPerSample = outData->m_wavFormat.nBlockAlign * 8 / outData->m_wavFormat.nChannels;
//
//	// �\�[�X�{�C�X�̍쐬 �����ł̓t�H�[�}�b�g�̂ݓn���Ă���
//	HRESULT result = mXAudio2->CreateSourceVoice(&mSourceVoice, (WAVEFORMATEX*)&waveFormat);
//	if (FAILED(result))
//	{
//		std::cout << "�\�[�X�{�C�X�̍쐬�Ɏ��s���܂����B" << std::endl;
//		return false;
//	}
//
//	//================================
//	// �g�`�f�[�^(���f�[�^�{��)���\�[�X�{�C�X�ɓn��
//	//================================
//	XAUDIO2_BUFFER xAudio2Buffer{};
//	xAudio2Buffer.pAudioData = (BYTE*)outData->m_soundBuffer;
//	xAudio2Buffer.Flags = XAUDIO2_END_OF_STREAM;
//	xAudio2Buffer.AudioBytes = outData->m_size;
//
//	// �O�����Z�q��p���āA���[�v���邩�ۂ��̐ݒ������
//	xAudio2Buffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;
//
//	mSourceVoice->SubmitSourceBuffer(&xAudio2Buffer);
//
//	// ���ۂɉ���炷
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
//		std::cout << "COM�̏������Ɏ��s���܂����B" << std::endl;
//		return 0;
//	}
//
//	hr = XAudio2Create(&mXAudio2);
//	if (FAILED(hr))
//	{
//		std::cout << "XAudio2�̏������Ɏ��s���܂����B" << std::endl;
//		return 0;
//	}
//
//	hr = mXAudio2->CreateMasteringVoice(&mMasteringVoice);
//	if (FAILED(hr))
//	{
//		std::cout << "�}�X�^�[�{�C�X�̍쐬�Ɏ��s���܂����B" << std::endl;
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
//	// ��������@�Y�ꂽ��Ƃ�ł��Ȃ����������[�N����
//	mMasteringVoice->DestroyVoice();
//	mXAudio2->Release();
//
//	CoUninitialize();
//}