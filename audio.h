#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef _AUDIO_H
#define _AUDIO_H
#define OFFSET_FORMATTAG			20
#define OFFSET_CHANNELS				22
#define OFFSET_SAMPLESPERSEC		24
#define OFFSET_AVGBYTESPERSEC		28
#define OFFSET_BLOCKALIGN			32
#define OFFSET_BITSPERSAMPLE		34
#define OFFSET_WAVEDATA				44
#define HEADER_SIZE	            OFFSET_WAVEDATA


struct t_audio {
   WAVEFORMATEX wfex;
   WAVEHDR *whdr; 
   HWAVEOUT hwo;  
   int loop;
   int file_size;
   char *lpfile;
};
typedef struct t_audio Audio;


extern void playAudio(Audio *audio);
extern void pauseAudio(Audio *audio);
extern void openAudioFile(LPSTR szFileName, Audio *audio);
extern void restartAudio(Audio *audio);

#endif
