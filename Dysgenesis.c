#include <stdio.h>
#include "SDL.h"

int main() {
	SDL_SetError("Hello world!");
	printf(SDL_GetError());
	return 0;
}