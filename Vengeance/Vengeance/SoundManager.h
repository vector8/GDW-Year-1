#pragma once

#include <string>

enum SoundEffects
{
	RIFLE_SHOT = 0,
	PISTOL_SHOT,
	PISTOL_PICKUP,
	PIPE_SWING,
	GRENADE_TOSS,
	GRENADE_EXPLODE,
	HAMBURGER_EAT,
	SOUND_EFFECTS_END
};

enum MusicTracks
{
	MAIN_THEME = 0,
	ACREWOOD_THEME,
	DOWNTOWN_THEME,
	INDUSTRIAL_THEME,
	UNDERCITY_THEME,
	WAREHOUSE_THEME,
	SUB_BOSS_THEME,
	BOSS_THEME,
	CREDITS_THEME,
	MUSIC_TRACKS_END
};

class SoundManager
{
private:
	std::string activeMusic;

	SoundManager();

	std::string getSoundEffectFileNameBasedOnEnum(SoundEffects sound);

	std::string getMusicTrackFileNameBasedOnEnum(MusicTracks track);

public:
	~SoundManager();

	static SoundManager* SoundManager::getInstance();

	void playSoundEffect(SoundEffects sound);

	void playMusic(MusicTracks track, bool loop);

	void stopMusic();
};