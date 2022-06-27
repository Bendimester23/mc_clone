//
// Created by bendi on 2022. 06. 27..
//

#ifndef MC_CLONE_CONFIG_H
#define MC_CLONE_CONFIG_H

//Buildelés előtt ki kell szedni a kommentet
//#define RELEASE

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#ifdef RELEASE
//Ide kerülnek azok a makrók, amik Release módban kellenek
#define WINDOW_TITLE "Minecraft clone"

#else
//És ide pedig azok, amik fejlesztés közben kellenek (pl: splash screen megkerülése)
#define WINDOW_TITLE "Minecraft clone Debug"

#endif

#endif //MC_CLONE_CONFIG_H
