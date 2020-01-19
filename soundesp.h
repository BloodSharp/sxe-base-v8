#ifndef _SOUNDHOOK_H_
#define _SOUNDHOOK_H_

struct svc_entry_s
{
	DWORD dwId;
	char  *szName;
	DWORD pfnHandler;
};

struct player_sound_t
{
	Vector origin;
	DWORD timestamp;
};

struct my_sound_t
{
	Vector origin;
	DWORD timestamp;
};

typedef void (*sound_t)(int, int, char*, float*, float, float, int, int);
void InitSound();
extern deque<my_sound_t> mySounds;

#endif