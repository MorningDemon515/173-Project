
#include "Image.h"

#include <SDL.h>
#include <windows.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

SDL_Surface* surface;
SDL_Texture* image;

int Image::DrawImage(SDL_Renderer* rend, const char* file, int x, int y)
{
	Sint32 width = 0;
	Sint32 height = 0;
	Sint32 bytes_per_pixel =0;

	Uint8* data = stbi_load(file, &width, &height, &bytes_per_pixel, 0);

	Sint32 dot_pitch = width * bytes_per_pixel;
	dot_pitch = (dot_pitch + 3) & ~3;

	Sint32 red_mask;
	Sint32 green_mask;
	Sint32 blue_mask;
	Sint32 alpha_mask;

	red_mask = 0x000000FF;
	green_mask = 0x0000FF00;
	blue_mask = 0x00FF0000;
	alpha_mask = (bytes_per_pixel == 4) ? 0xFF000000 : 0;

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = width;
	rect.h = height;

	surface = SDL_CreateRGBSurfaceFrom(
		data, width, height, bytes_per_pixel * 8, dot_pitch, red_mask, green_mask,
		blue_mask, alpha_mask);

    image = SDL_CreateTextureFromSurface(rend, surface);

	//SDL_RenderClear(rend);
	SDL_RenderCopy(rend, image, nullptr, &rect);
	SDL_RenderPresent(rend);

	SDL_FreeSurface(surface);
    SDL_free(data);

    return 0;
}

SDL_Surface* Image::LoadICON(const char* icon)
{
    // Load the icon using LoadImage function
    HICON hIcon = (HICON)LoadImageA(NULL, icon, IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
    if (!hIcon) {
        SDL_Log("Failed to load icon: %s", icon);
        return nullptr;
    }

    // Get icon information
    ICONINFO iconInfo;
    if (!GetIconInfo(hIcon, &iconInfo)) {
        SDL_Log("Failed to get icon info: %s", icon);
        DestroyIcon(hIcon);
        return nullptr;
    }

    BITMAP bmpColor, bmpMask;
    GetObject(iconInfo.hbmColor, sizeof(BITMAP), &bmpColor);
    GetObject(iconInfo.hbmMask, sizeof(BITMAP), &bmpMask);

    int width = bmpColor.bmWidth;
    int height = bmpColor.bmHeight;

    // Create SDL surface with alpha channel
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_ARGB8888);
    if (!surface) {
        SDL_Log("Failed to create SDL surface");
        DestroyIcon(hIcon);
        DeleteObject(iconInfo.hbmColor);
        DeleteObject(iconInfo.hbmMask);
        return nullptr;
    }

    // Create a compatible DC and select the bitmap into it
    HDC hdcColor = CreateCompatibleDC(NULL);
    HDC hdcMask = CreateCompatibleDC(NULL);

    SelectObject(hdcColor, iconInfo.hbmColor);
    SelectObject(hdcMask, iconInfo.hbmMask);

    // Copy the bitmap data to the SDL surface, taking into account the mask
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Get pixel from color bitmap
            COLORREF color = GetPixel(hdcColor, x, y);

            // Get pixel from mask bitmap (inverted: 0 = transparent, 1 = opaque)
            BOOL isTransparent = GetPixel(hdcMask, x, y) == RGB(255, 255, 255);

            Uint32* pixels = (Uint32*)surface->pixels + y * surface->pitch / 4 + x;
            *pixels = (isTransparent ? 0x00 : 0xFF000000) | color;
        }
    }

    // Clean up
    DeleteDC(hdcColor);
    DeleteDC(hdcMask);
    DestroyIcon(hIcon);
    DeleteObject(iconInfo.hbmColor);
    DeleteObject(iconInfo.hbmMask);

    return surface;
   
}