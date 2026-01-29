#include "SDL3/SDL.h"
#include <iostream>

int main(int argc, char *argv[])
{
   SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s", "Hello, world!");

   return 0;
}
