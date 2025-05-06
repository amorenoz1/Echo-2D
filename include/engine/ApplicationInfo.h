#ifndef APPLICATIONINFO_H
#define APPLICATIONINFO_H

namespace Echo2D{

struct ApplicationInfo {
   int ScreenHeight = {0};
   int ScreenWidth = {0};
   const char* Title = {0};
};

extern ApplicationInfo g_AppInfo;

}

#endif

