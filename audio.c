#include "audio.h"

void CALLBACK WaveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
   WAVEHDR *whdr;
	switch(uMsg)
	{
	case WOM_OPEN:
		break;
	case WOM_DONE:
			whdr = (WAVEHDR*)dwParam1;
			if(whdr->dwUser)
				waveOutWrite(hwo, whdr, sizeof(WAVEHDR));
			else
			{
				waveOutUnprepareHeader(hwo, whdr, sizeof(WAVEHDR));
				free(whdr);
			}
		break;
	case WOM_CLOSE:
		break;
	}
}


void openAudioFile(LPSTR szFileName, Audio *audio)
{
   FILE *f; 

   f = fopen(szFileName, "rb");
   if(!f)
      return;

   fseek(f, 0, SEEK_END);
   audio->file_size = ftell(f);
   fseek(f, 0, SEEK_SET);

   audio->lpfile = (char *)malloc(sizeof(char)* audio->file_size );
   fread(audio->lpfile, 1, audio->file_size , f);
   fclose(f);

   audio->wfex.wFormatTag         = *((WORD* )(audio->lpfile + OFFSET_FORMATTAG     ));
   audio->wfex.nChannels          = *((WORD* )(audio->lpfile + OFFSET_CHANNELS      ));
   audio->wfex.nSamplesPerSec     = *((DWORD*)(audio->lpfile + OFFSET_SAMPLESPERSEC ));
   audio->wfex.nAvgBytesPerSec    = *((DWORD*)(audio->lpfile + OFFSET_AVGBYTESPERSEC));
   audio->wfex.nBlockAlign        = *((WORD* )(audio->lpfile + OFFSET_BLOCKALIGN    ));
   audio->wfex.wBitsPerSample     = *((WORD* )(audio->lpfile + OFFSET_BITSPERSAMPLE ));

   audio->loop = 0;
   
   return;
}


void playAudio(Audio *audio)
{

   UINT devid;
   
	WAVEHDR* whdr = (WAVEHDR*)malloc(sizeof(WAVEHDR));
	ZeroMemory((void*)whdr, sizeof(WAVEHDR));

	whdr->lpData = audio->lpfile+HEADER_SIZE;
	whdr->dwBufferLength = audio->file_size-HEADER_SIZE;
	whdr->dwUser = (DWORD) audio->loop;


	for(devid = 0; devid < waveOutGetNumDevs(); devid++)
	{
		if(devid == waveOutGetNumDevs()) {
         audio->whdr = NULL;
			return;
      }
		if(waveOutOpen(&(audio->hwo), WAVE_MAPPER, &(audio->wfex), (DWORD)WaveOutProc, 0, CALLBACK_FUNCTION) == MMSYSERR_NOERROR)
			break;
	}

	if(waveOutPrepareHeader(audio->hwo, whdr, sizeof(WAVEHDR)) != MMSYSERR_NOERROR) {
      audio->whdr = NULL;
		return;
   }
	if(waveOutWrite(audio->hwo, whdr, sizeof(WAVEHDR)) != MMSYSERR_NOERROR) {
      audio->whdr = NULL;
		return;
   }

	audio->whdr = whdr;
}



void pauseAudio(Audio *audio)
{
   waveOutPause(audio->hwo);
}

void restartAudio(Audio *audio)
{
   waveOutRestart(audio->hwo);
}


