
#include "MainScence.h"
#include "Launcher.h"

#include <SDL.h>
#include <iostream>

#include <dxgi.h>

#include "Text.h"
#include "Image.h"

#include <math.h>
#include <cmath>
#include <string.h>
#include <stdlib.h>
#include <iomanip>
#include <cstring>
#include <memory>
#include <sstream>
#include <stdexcept>

Scence* scen;
Text* text;
Image* img;
SDL_Event event;

int mx, my;

const char* font = "C:/Windows/Fonts/simfang.ttf";

int Menu::Launcher()
{
    SDL_Window* window = SDL_CreateWindow("SCP-Containment Breach Launcher",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer* render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //窗口和渲染器
    SDL_Surface* icon = img->LoadICON("logo.ico");
    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);

    SDL_SetWindowTitle(window, "SCP-Containment Breach Launcher");
    //设置窗口图标和标题

    img->DrawImage(render, "GFX/menu/launcher.jpg", 0, 0);

    SelectR(render);
    //背景图片
    SDL_Rect GRct;
    GRct.x = 20;
    GRct.y = 369;
    GRct.w = 340;
    GRct.h = 95;

    SDL_RenderDrawRect(render, &GRct);
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    SDL_RenderFillRect(render, &GRct);
    //矩形
    text->Draw_Text(render, font, 20, 360, 20, "你支持的设备:", 255, 255, 255, 255);

    text->Draw_Text(render, font, 25, 190, 20, "游戏简介:", 255, 255, 255, 255);
     
    // Main loop
    bool running = true;
    while (running)
    {
        // Handle events
        while (SDL_PollEvent(&event) != 0)
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
                break;
            case SDL_MOUSEMOTION:
                mx = event.motion.x;
                my = event.motion.y;
                break;
             
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    SDL_Rect BRect;
                    BRect.x = 640 - 30 - 90;
                    BRect.y = 480 - 50;
                    BRect.w = 100;
                    BRect.h = 30;

                    if (IsDown(BRect))
                    {
                        running = false;
                    }

                    SDL_Rect QRect;
                    QRect.x = 640 - 30 - 90;
                    QRect.y = 480 - 50 - 55;
                    QRect.w = 100;
                    QRect.h = 30;

                    if (IsDown(QRect))
                    {
                        //MessageBoxA(NULL,"没做","SCP-Containment Breach", MB_OK);

                        running = false;
                        SDL_DestroyRenderer(render);
                        SDL_DestroyWindow(window);
                        SDL_Quit();

                        SDL_Delay(1000);

                        scen->MainWorld();
                        
                    }
                    SelectDriver(render);
                }
                break;
            }
        }
        
        PrintDevInf(render, 30, 369 + 35);

        Button(render, 640 - 30 - 90, 480 - 50 - 55, 100, 30, "启动");
        Button2(render, 640 - 30 - 90, 480 - 50, 100, 30, L"\u9000\u51fa");

        //text->Draw_Text(render,font,20,20,20,"游戏启动器还没做好,先凑合一下",0,0,0,255);
        SDL_RenderPresent(render);

    }

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void Menu::Button(SDL_Renderer* rend, int x, int y, int w, int h, std::string t)
{
    SDL_Rect rect = { x, y, w, h };

    // Check if the mouse is over the button
    bool is_hovered = mx >= rect.x && mx <= rect.x + rect.w &&
        my >= rect.y && my <= rect.y + rect.h;

    // Draw the button
    if (is_hovered) {
        SDL_RenderDrawRect(rend, &rect);
        SDL_SetRenderDrawColor(rend, 160, 160, 160, 255);
        SDL_RenderFillRect(rend, &rect);
        text->Draw_Text(rend, font, x + w / 2 - 25, y + h / 2 + 10, 25, t.c_str(), 255, 255, 255, 255);
        
    }
    else {
        SDL_RenderDrawRect(rend, &rect);
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderFillRect(rend, &rect);
        text->Draw_Text(rend, font, x + w / 2 - 25, y + h / 2 + 10, 25, t.c_str(), 255, 255, 255, 255);

    }

}

void Menu::Button2(SDL_Renderer* rend, int x, int y, int w, int h, const wchar_t* t)
{
    SDL_Rect rect = { x, y, w, h };

    // Check if the mouse is over the button
    bool is_hovered = mx >= rect.x && mx <= rect.x + rect.w &&
        my >= rect.y && my <= rect.y + rect.h;

    // Draw the button
    if (is_hovered) {
        SDL_RenderDrawRect(rend, &rect);
        SDL_SetRenderDrawColor(rend, 160, 160, 160, 255);
        SDL_RenderFillRect(rend, &rect);
        text->Draw_Text2(rend, font, x + w / 2 - 25, y + h / 2 + 10, 25, t, 255, 255, 255, 255);

    }
    else {
        SDL_RenderDrawRect(rend, &rect);
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderFillRect(rend, &rect);
        text->Draw_Text2(rend, font, x + w / 2 - 25, y + h / 2 + 10, 25, t, 255, 255, 255, 255);

    }
}

bool Menu::IsDown(SDL_Rect rectt)
{
    int x = event.button.x;
    int y = event.button.y;

    return x >= rectt.x && x <= rectt.x + rectt.w &&
        y >= rectt.y && y <= rectt.y + rectt.h;
}

std::string wideCharToMultiByte(wchar_t* pWCStrKey)
{
    //第一次调用确认转换后单字节字符串的长度，用于开辟空间
    int pSize = WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), NULL, 0, NULL, NULL);
    char* pCStrKey = new char[pSize + 1];
    //第二次调用将双字节字符串转换成单字节字符串
    WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), pCStrKey, pSize, NULL, NULL);
    pCStrKey[pSize] = '\0';
    std::string pKey = pCStrKey;
    return pKey;

    //如果想要转换成string，直接赋值即可
    //string pKey = pCStrKey;
}

int Menu::PrintDevInf(SDL_Renderer* rend,int x, int y)
{
    
    // 创建DXGI工厂
    IDXGIFactory* pFactory = nullptr;
    CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);

    // 枚举适配器
    IDXGIAdapter* pAdapter = nullptr;

    /*
    for (UINT i = 0; pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i) {
        DXGI_ADAPTER_DESC adapterDesc;
        pAdapter->GetDesc(&adapterDesc);

        // 输出适配器信息
        //wprintf(L"Adapter %d: %s\n", i, adapterDesc.Description);
        //wprintf(L"Dedicated Video Memory: %llu MB\n", adapterDesc.DedicatedVideoMemory / 1024 / 1024);

        text->Draw_Text(rend,font,x,y,28, wideCharToMultiByte(adapterDesc.Description).c_str(),0,0,0,255);

        // 释放适配器
        pAdapter->Release();
    }*/

    //设备1
    if (pFactory->EnumAdapters(0, &pAdapter) != DXGI_ERROR_NOT_FOUND) {
        DXGI_ADAPTER_DESC adapterDesc;
        pAdapter->GetDesc(&adapterDesc);

        // 输出适配器名称
        //wprintf(L"%s\n", adapterDesc.Description);
        text->Draw_Text2(rend, font, x, y, 20, adapterDesc.Description, 0, 0, 0, 255);
     
        // 释放适配器
        pAdapter->Release();
    }
    //设备2
    if (pFactory->EnumAdapters(2, &pAdapter) != DXGI_ERROR_NOT_FOUND) {
        DXGI_ADAPTER_DESC adapterDesc;
        pAdapter->GetDesc(&adapterDesc);

        // 输出适配器名称
        //wprintf(L"%s\n", adapterDesc.Description);
        text->Draw_Text2(rend, font, x, y + 25, 20, adapterDesc.Description, 0, 0, 0, 255);
     
        // 释放适配器
        pAdapter->Release();
    }

    // 释放工厂
    pFactory->Release();

    return 0;
}


int Menu::SelectDriver(SDL_Renderer* rend)
{
    
    SDL_Rect rect1;
    SDL_Rect rect2;

    rect1.x = 30;
    rect1.y = 369+20;
    rect1.w = 300; 
    rect1.h = 20;

    rect2.x = 30;
    rect2.y = 369+45;
    rect2.w = 300;
    rect2.h = 20;

    bool is_hovered1 = mx >= rect1.x && mx <= rect1.x + rect1.w &&
        my >= rect1.y && my <= rect1.y + rect1.h;

    bool is_hovered2 = mx >= rect2.x && mx <= rect2.x + rect2.w &&
        my >= rect2.y && my <= rect2.y + rect2.h;

    if (IsDown(rect1))
    {
        SDL_Rect rrrr;
        rrrr.x = 30;
        rrrr.y = 369 + 20;
        rrrr.w = 300;
        rrrr.h = 20;

        SDL_SetRenderDrawColor(rend, 160, 160, 160, 255);
        SDL_RenderDrawRect(rend, &rrrr);
        SDL_RenderFillRect(rend, &rrrr);
    }
    else
    {
    SDL_Rect rrrr;
    rrrr.x = 30;
    rrrr.y = 369+20;
    rrrr.w = 300;
    rrrr.h = 20;

    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    SDL_RenderDrawRect(rend,&rrrr);
    SDL_RenderFillRect(rend,&rrrr);
    }
   
    if (IsDown(rect2))
    {
        SDL_Rect rrrr2;
        rrrr2.x = 30;
        rrrr2.y = 369 + 45;
        rrrr2.w = 300;
        rrrr2.h = 20;

        SDL_SetRenderDrawColor(rend, 160,160, 160, 255);
        SDL_RenderDrawRect(rend, &rrrr2);
        SDL_RenderFillRect(rend, &rrrr2);
    }
    else{
    SDL_Rect rrrr2;
    rrrr2.x = 30;
    rrrr2.y = 369 + 45;
    rrrr2.w = 300;
    rrrr2.h = 20;

    SDL_SetRenderDrawColor(rend, 255,255, 255, 255);
    SDL_RenderDrawRect(rend, &rrrr2);
    SDL_RenderFillRect(rend,&rrrr2);
    }

//----------------------------------------------------------------------------

    return 0;
}

int Menu::SelectR(SDL_Renderer* rend)
{
    /*
    SDL_Rect rect;

    text->Draw_Text2(rend,font,25,220,20,L"640x480",0,0,0,255);
    text->Draw_Text2(rend, font, 25, 240, 20, L"720x480", 0, 0, 0, 255);
    text->Draw_Text2(rend, font, 25, 260, 20, L"720x576", 0, 0, 0, 255);
    text->Draw_Text2(rend, font, 25, 280, 20, L"800x600", 0, 0, 0, 255);
    text->Draw_Text2(rend, font, 25, 300, 20, L"1024x768", 0, 0, 0, 255);
    text->Draw_Text2(rend, font, 25, 320, 20, L"1152x864", 0, 0, 0, 255);

    text->Draw_Text2(rend, font, 25+85, 220, 20, L"1280x720", 0, 0, 0, 255);
    text->Draw_Text2(rend, font, 25 + 85, 240, 20, L"1280x768", 0, 0, 0, 255);
    text->Draw_Text2(rend, font, 25 + 85, 260, 20, L"1280x800", 0, 0, 0, 255);
    text->Draw_Text2(rend, font, 25 + 85, 280, 20, L"1360x768", 0, 0, 0, 255);
    text->Draw_Text2(rend, font, 25 + 85, 300, 20, L"1366x768", 0, 0, 0, 255);
    text->Draw_Text2(rend, font, 25 + 85, 320, 20, L"1440x900", 0, 0, 0, 255);

    text->Draw_Text2(rend, font, 25 + 85 + 85, 220, 20, L"1920x1080", 0, 0, 0, 255);
    text->Draw_Text2(rend, font, 25 + 85 + 85, 240, 20, L"2560x1440", 0, 0, 0, 255);
    text->Draw_Text2(rend, font, 25 + 85 + 85, 260, 20, L"3840x2160", 0, 0, 0, 255);
    */

    text->Draw_Text2(rend, font, 25, 220, 15, L"\u8fd9\u662fSCP-Containment Breach,\u4e5f\u5c31\u662fSCP-\u6536\u5bb9\u5931\u6548\u7684\u91cd\u5236\u7248", 0, 0, 0, 255);
    text->Draw_Text(rend, font, 25, 240, 20, "目前只是测试版,希望你喜欢!", 0, 0, 0, 255);

    text->Draw_Text(rend, font, 25, 320, 20, "游戏目前只支持1280x720分辨率", 0, 0, 0, 255);

    return 0;
}

