#ifndef AUDIO_LIB_H
#define AUDIO_LIB_H

void AudioLibInit();

void AudioLibPlaySound(const char *filename, bool looping);
void AudioLibStopSound(const char *filename);

void AudioLibPlayMusic(const char *filename);
void AudioLibStopMusic();

void AudioLibShutdown();

#endif