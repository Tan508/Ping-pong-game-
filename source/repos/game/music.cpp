#include <windows.h>
#include <mmsystem.h>        
#pragma comment(lib, "winmm.lib")

void play_music() {
    PlaySound(TEXT("merx-market-song-33936.wav"), NULL, SND_FILENAME | SND_ASYNC);
}
