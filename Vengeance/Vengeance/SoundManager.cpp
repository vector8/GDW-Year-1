#include "SoundManager.h"
//#include <windows.h>
#include "AudioLib\AudioLib.h"
#include <assert.h>

SoundManager::SoundManager()
{
	AudioLibInit();	
}

SoundManager::~SoundManager()
{
	AudioLibShutdown();	
}

std::string SoundManager::getSoundEffectFileNameBasedOnEnum(SoundEffects sound)
{
	static const std::string SOUND_FILES[SOUND_EFFECTS_END] = {"shot_rifle_single.wav", "shot_pistol.wav", "pickup_pistol.wav", "pipe_swing.mp3", "grenade_toss.wav", "grenade_explode.wav", "eating.wav"};

	assert(sound < SOUND_EFFECTS_END);

	return "sounds/sound_effects/" + SOUND_FILES[(int)sound];
}

std::string SoundManager::getMusicTrackFileNameBasedOnEnum(MusicTracks track)
{
	static const std::string MUSIC_FILES[MUSIC_TRACKS_END] = {"Hitman.mp3", "Interloper.mp3", "Rocket.mp3", "Harmful_or_Fatal.mp3", "Phantasm.mp3", "In_a_Heartbeat.mp3", "Club_Diver.mp3", "Ropocalypse_2.mp3", "The_Descent.mp3"};

	assert(track < MUSIC_TRACKS_END);

	return "sounds/music/" + MUSIC_FILES[(int)track];
}

SoundManager* SoundManager::getInstance()
{
	static SoundManager* instance = new SoundManager();

	return instance;
}

void SoundManager::playSoundEffect(SoundEffects sound)
{
	std::string fileName = SoundManager::getSoundEffectFileNameBasedOnEnum(sound);

	AudioLibPlaySound(fileName.c_str(), false);
}

void SoundManager::playMusic(MusicTracks track, bool loop)
{
	this->activeMusic = SoundManager::getMusicTrackFileNameBasedOnEnum(track);

	if(loop)
		AudioLibPlaySound(this->activeMusic.c_str(), true);
	else
		AudioLibPlaySound(this->activeMusic.c_str(), false);
}

void SoundManager::stopMusic()
{
	AudioLibStopSound(this->activeMusic.c_str());
}