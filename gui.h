#ifndef _IMGUIHOOK_
#define _IMGUIHOOK_

extern bool bShowMenu;
extern bool show_app_console;
extern bool show_menu_key;
extern bool loadtexture;
extern bool loadtexturemodel;
extern bool keys[256];
extern bool show_demo;
extern bool channelchange;

extern int MenuTab;

extern GLuint texture_id[1024];

extern DWORD stream;

bool CalcScreen(float* pflOrigin, float* pflVecScreen);

ImVec2 ImRotationCenter();

typedef std::basic_string<TCHAR> tstring;
tstring VirtualKeyCodeToString(UCHAR virtualKey);

void KeyBinds();
void InitImGui(HDC hdc);
void ImRotateStart();
void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter());
void InitBass();
void GetTextureModel();

#endif