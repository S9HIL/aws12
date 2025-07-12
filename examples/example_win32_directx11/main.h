#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui_internal.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <dwmapi.h>
#include "imgui_settings.h"
#include "imgui_edited.hpp"
#include "Auth/auth.hpp"
#include "Auth/json.hpp"
#include "Auth/skStr.h"
#include "Auth/utils.hpp"
#include "font_defines.h"
#include "icon_font.h"
#include "poppins_font.h"
#include "roboto_font.h"
#include "jupiter_mission.h"
#include "Auth/Header.h"

#include "imgui_freetype.h"

#include <iostream>
#include <Windows.h>
#include "DiscordSDK/src/discord_register.h"
#include "DiscordSDK/src/discord_rpc.h"
#include "Notifications.h"
#include <string>

#include "PicoMem.h"
#include "snow.hpp"


#include <D3DX11tex.h>
#pragma comment(lib, "D3DX11.lib")

#include "circle_load.h"
#include "minecraft_bg.h"
#include "logo.h"


bool show_particles = false;
bool hide_login = false;

namespace fonts {
    ImFont* roboto = nullptr;
    ImFont* icon = nullptr;
    ImFont* jupiter = nullptr;
}

std::string FormatExpiryDate(const std::string& timestampStr) {

    if (timestampStr.empty() || !std::all_of(timestampStr.begin(), timestampStr.end(), ::isdigit)) {
        return "Invalid timestamp";
    }

    std::time_t expiryTime = std::stoll(timestampStr);
    std::tm* tm = std::localtime(&expiryTime);

    if (!tm) {
        return "Invalid time";
    }

    char buffer[64];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", tm);
    return std::string(buffer);
}

namespace ImGui
{
    int rotation_start_index;
    void ImRotateStart()
    {
        rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
    }

    ImVec2 ImRotationCenter()
    {
        ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX);

        const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
        for (int i = rotation_start_index; i < buf.Size; i++)
            l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

        return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2);
    }


    void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter())
    {
        float s = sin(rad), c = cos(rad);
        center = ImRotate(center, s, c) - center;

        auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
        for (int i = rotation_start_index; i < buf.Size; i++)
            buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
    }
}


void ParticlesV()
{

    ImVec2 screen_size = { (float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN) };

    static ImVec2 partile_pos[100];
    static ImVec2 partile_target_pos[100];
    static float partile_speed[100];
    static float partile_size[100];
    static float partile_radius[100];
    static float partile_rotate[100];

    for (int i = 1; i < 30; i++)
    {
        if (partile_pos[i].x == 0 || partile_pos[i].y == 0)
        {
            partile_pos[i].x = rand() % (int)screen_size.x + 1;
            partile_pos[i].y = -15.f;
            partile_speed[i] = 1 + rand() % 25;
            partile_radius[i] = rand() % 3;
            partile_size[i] = rand() % 3;

            partile_target_pos[i].x = rand() % (int)screen_size.x;
            partile_target_pos[i].y = screen_size.y * 2;
        }

        partile_pos[i] = ImLerp(partile_pos[i], partile_target_pos[i], ImGui::GetIO().DeltaTime * (partile_speed[i] / 60));
        partile_rotate[i] += ImGui::GetIO().DeltaTime;

        if (partile_pos[i].y > screen_size.y)
        {
            partile_pos[i].x = 0;
            partile_pos[i].y = 0;
            partile_rotate[i] = 0;
        }

        ImGui::GetWindowDrawList()->AddCircleFilled(partile_pos[i], partile_size[i], ImGui::GetColorU32(ImStyle::general_color), 20);
        ImGui::GetWindowDrawList()->AddShadowCircle(partile_pos[i], 2.5f, ImGui::GetColorU32(ImStyle::general_color), 40.f + partile_size[i], ImVec2(0, 0), 0, 20);
    }
}

void ParticlesWhite()
{

    ImVec2 screen_size = { (float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN) };

    static ImVec2 partile_pos[100];
    static ImVec2 partile_target_pos[100];
    static float partile_speed[100];
    static float partile_size[100];
    static float partile_radius[100];
    static float partile_rotate[100];

    for (int i = 1; i < 30; i++)
    {
        if (partile_pos[i].x == 0 || partile_pos[i].y == 0)
        {
            partile_pos[i].x = rand() % (int)screen_size.x + 1;
            partile_pos[i].y = -15.f;
            partile_speed[i] = 1 + rand() % 25;
            partile_radius[i] = rand() % 3;
            partile_size[i] = rand() % 3;

            partile_target_pos[i].x = rand() % (int)screen_size.x;
            partile_target_pos[i].y = screen_size.y * 2;
        }

        partile_pos[i] = ImLerp(partile_pos[i], partile_target_pos[i], ImGui::GetIO().DeltaTime * (partile_speed[i] / 60));
        partile_rotate[i] += ImGui::GetIO().DeltaTime;

        if (partile_pos[i].y > screen_size.y)
        {
            partile_pos[i].x = 0;
            partile_pos[i].y = 0;
            partile_rotate[i] = 0;
        }

        ImGui::GetWindowDrawList()->AddCircleFilled(partile_pos[i], partile_size[i], ImColor(1.f,1.f,1.f,1.f), 20);
        ImGui::GetWindowDrawList()->AddShadowCircle(partile_pos[i], 2.5f, ImColor(1.f, 1.f, 1.f, 1.f), 40.f + partile_size[i], ImVec2(0, 0), 0, 20);
    }
}


void Hexagons()
{
    ImVec2 screen_size = { (float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN) };

    static ImVec2 partile_pos[100];
    static ImVec2 partile_target_pos[100];
    static float partile_speed[100];
    static float partile_size[100];
    static float partile_radius[100];
    static float partile_rotate[100];

    for (int i = 1; i < 30; i++)
    {
        if (partile_pos[i].x == 0 || partile_pos[i].y == 0)
        {
            partile_pos[i].x = rand() % (int)screen_size.x + 1;
            partile_pos[i].y = -15.f;
            partile_speed[i] = 1 + rand() % 25;
            partile_radius[i] = rand() % 4;
            partile_size[i] =  rand() % 8;

            partile_target_pos[i].x = rand() % (int)screen_size.x;
            partile_target_pos[i].y = screen_size.y * 2;
        }

        partile_pos[i] = ImLerp(partile_pos[i], partile_target_pos[i], ImGui::GetIO().DeltaTime * (partile_speed[i] / 60));
        partile_rotate[i] += ImGui::GetIO().DeltaTime;

        if (partile_pos[i].y > screen_size.y)
        {
            partile_pos[i].x = 0;
            partile_pos[i].y = 0;
            partile_rotate[i] = 0;
        }

        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        float angle_offset = partile_rotate[i];
        float radius = partile_size[i];

        ImVec2 hex[6];
        for (int j = 0; j < 6; j++)
        {
            float angle = angle_offset + j * IM_PI / 3.0f;
            hex[j] = ImVec2(partile_pos[i].x + cosf(angle) * radius * 0.7f,
                partile_pos[i].y + sinf(angle) * radius * 0.7f);
        }

        draw_list->AddConvexPolyFilled(hex, 6, ImGui::GetColorU32(ImStyle::general_color));
        draw_list->AddShadowCircle(partile_pos[i], 6.f, ImGui::GetColorU32(ImStyle::general_color), 30.f + partile_size[i], ImVec2(0, 0), 0, 12);
    }
}


void HexagonsW()
{
    ImVec2 screen_size = { (float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN) };

    static ImVec2 partile_pos[100];
    static ImVec2 partile_target_pos[100];
    static float partile_speed[100];
    static float partile_size[100];
    static float partile_radius[100];
    static float partile_rotate[100];

    for (int i = 1; i < 30; i++)
    {
        if (partile_pos[i].x == 0 || partile_pos[i].y == 0)
        {
            partile_pos[i].x = rand() % (int)screen_size.x + 1;
            partile_pos[i].y = -15.f;
            partile_speed[i] = 1 + rand() % 25;
            partile_radius[i] = rand() % 4;
            partile_size[i] = rand() % 8;

            partile_target_pos[i].x = rand() % (int)screen_size.x;
            partile_target_pos[i].y = screen_size.y * 2;
        }

        partile_pos[i] = ImLerp(partile_pos[i], partile_target_pos[i], ImGui::GetIO().DeltaTime * (partile_speed[i] / 60));
        partile_rotate[i] += ImGui::GetIO().DeltaTime;

        if (partile_pos[i].y > screen_size.y)
        {
            partile_pos[i].x = 0;
            partile_pos[i].y = 0;
            partile_rotate[i] = 0;
        }

        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        float angle_offset = partile_rotate[i];
        float radius = partile_size[i];

        ImVec2 hex[6];
        for (int j = 0; j < 6; j++)
        {
            float angle = angle_offset + j * IM_PI / 3.0f;
            hex[j] = ImVec2(partile_pos[i].x + cosf(angle) * radius * 0.7f,
                partile_pos[i].y + sinf(angle) * radius * 0.7f);
        }

        draw_list->AddConvexPolyFilled(hex, 6, ImGui::GetColorU32(ImStyle::white));
        draw_list->AddShadowCircle(partile_pos[i], 6.f, ImGui::GetColorU32(ImStyle::white), 30.f + partile_size[i], ImVec2(0, 0), 0, 12);
    }
}

float EaseOutExpo(float t) {
    return (t >= 1.0f) ? 1.0f : 1.0f - powf(2.0f, -10.0f * t);
}

void RenderAnimatedText(const char* finalText, float startX, float startY, ImFont* font, float revealSpeed = 0.0035f, int scrambleFrames = 6) {
    static int currentChar = 0;
    static float timer = 0.0f;
    static int scrambleCounter = 0;
    static char currentScramble = ' ';
    static std::string displayed = "";

    static float shadowFadeTimer = 0.0f;
    static float shadowAlpha = 0.0f;

    static const float shadowDelay = 0.4f;
    static const float shadowFadeDuration = 0.6f;

    static const float slideDelay = 0.4f;
    static const float slideDuration = 0.6f;
    static float textMoveTimer = 0.0f;
    static float textMoveAlpha = 0.0f;

    static const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-=<>?";

    ImGuiIO& io = ImGui::GetIO();
    timer += io.DeltaTime;

    const int len = strlen(finalText);

    if (currentChar < len) {
        if (timer >= revealSpeed) {
            timer = 0.0f;
            scrambleCounter++;

            if (scrambleCounter >= scrambleFrames) {
                displayed += finalText[currentChar];
                currentChar++;
                scrambleCounter = 0;
            }
            else {
                currentScramble = charset[rand() % (sizeof(charset) - 1)];
            }
        }
    }

    if (currentChar >= len) {
        shadowFadeTimer += io.DeltaTime;
        if (shadowFadeTimer > shadowDelay) {
            float fadeProgress = (shadowFadeTimer - shadowDelay) / shadowFadeDuration;
            shadowAlpha = ImClamp(fadeProgress, 0.0f, 1.0f);
        }

        if (shadowAlpha >= 1.0f) {
            textMoveTimer += io.DeltaTime;
            if (textMoveTimer >= slideDelay) {
                float slideProgress = (textMoveTimer - slideDelay) / slideDuration;
                textMoveAlpha = ImClamp(slideProgress, 0.0f, 1.0f);
            }
        }
    }

    std::string renderText = displayed;
    if (currentChar < len && scrambleCounter > 0) {
        renderText += currentScramble;
    }

    ImGui::PushFont(font);

    ImVec2 fullTextSize = ImGui::CalcTextSize(finalText);
    ImVec2 renderTextSize = ImGui::CalcTextSize(renderText.c_str());
    float baseY = startY;
    float targetY = 10.0f;

    float easedAlpha = EaseOutExpo(textMoveAlpha);
    float animatedY = ImLerp(baseY, targetY, easedAlpha);
    float startScale = 1.0f;
    float targetScale = 0.6f;
    float currentScale = ImLerp(startScale, targetScale, easedAlpha);

    ImVec2 scaledFullTextSize = ImVec2(fullTextSize.x * currentScale, fullTextSize.y * currentScale);
    ImVec2 pos = ImVec2((ImGui::GetWindowSize().x - scaledFullTextSize.x) * 0.5f + startX, animatedY);
    ImU32 textColor = ImGui::GetColorU32(ImStyle::text::text_active);

    ImVec4 baseShadowColor = ImGui::ColorConvertU32ToFloat4(textColor);
    baseShadowColor.w *= shadowAlpha;
    ImU32 shadowColor = ImGui::ColorConvertFloat4ToU32(baseShadowColor);

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 winPos = ImGui::GetWindowPos();
    ImVec2 winSize = ImGui::GetWindowSize();
    ImVec2 circleCenter = ImVec2(winPos.x + winSize.x * 0.5f, winPos.y + winSize.y + 180);

    float revealProgress = (float)currentChar / (float)len;
    float fadeInAlpha = ImClamp(revealProgress * 2, 0.0f, 1.0f);

    ImVec4 circleColor = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImStyle::general_color));
    circleColor.w = fadeInAlpha * 0.35f;
    ImU32 circleColU32 = ImGui::ColorConvertFloat4ToU32(circleColor);
    drawList->AddShadowCircle(circleCenter, 200.f, circleColU32, 400.f, ImVec2(0, 0), 0, 64);

    font->Scale = currentScale;
    ImGui::PushFont(font);
    ImGui::ShadowText(renderText.c_str(), textColor, shadowColor, 60.f, pos);
    ImGui::PopFont();
    font->Scale = 1.0f;
    ImGui::PopFont();

    if (fabsf(animatedY - targetY) < 0.5f) {
        show_particles = true;
    }
}



ID3D11ShaderResourceView* circle_loading = nullptr;
ID3D11ShaderResourceView* minecraft_pic = nullptr;
ID3D11ShaderResourceView* lg = nullptr;

static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

using namespace ImStyle;

#include <thread>
#include <chrono>
#include <random>

HWND hwnd;
RECT rc;

static DWORD tick_count = GetTickCount();

int ProcId = 0;

static int findMyProc(const char* procname) {
    if (procname == NULL)
        return 0;
    DWORD pid = 0;
    DWORD threadCount = 0;

    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe;

    pe.dwSize = sizeof(PROCESSENTRY32);
    Process32First(hSnap, &pe);
    while (Process32Next(hSnap, &pe)) {
        if (_tcsicmp(pe.szExeFile, procname) == 0) {
            if ((int)pe.cntThreads > threadCount) {
                threadCount = pe.cntThreads;

                pid = pe.th32ProcessID;

            }
        }
    }
    return pid;
}

static int GetPid(const char* procname) {

    if (procname == NULL)
        return 0;
    DWORD pid = 0;
    DWORD threadCount = 0;

    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe;

    pe.dwSize = sizeof(PROCESSENTRY32);
    Process32First(hSnap, &pe);
    while (Process32Next(hSnap, &pe)) {
        if (_tcsicmp(pe.szExeFile, procname) == 0) {
            if ((int)pe.cntThreads > threadCount) {
                threadCount = pe.cntThreads;

                pid = pe.th32ProcessID;

            }
        }
    }
    return pid;
}

const char* GetEmulatorRunning() {
    if (GetPid("HD-Player.exe") != 0)
        return "HD-Player.exe";

    else if (GetPid("MEmuHeadless.exe") != 0)
        return "MEmuHeadless.exe";

    else if (GetPid("LdVBoxHeadless.exe") != 0)
        return "LdVBoxHeadless.exe";

    else if (GetPid("AndroidProcess.exe") != 0)
        return "AndroidProcess.exe";

    else if (GetPid("aow_exe.exe") != 0)
        return "aow_exe.exe";

    else if (GetPid("Nox.exe") != 0)
        return "Nox.exe";
}

DWORD processId = findMyProc(GetEmulatorRunning());


bool InjectDLL(const char* targetProcessName, const char* dllPath) {

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        std::cerr << "Erro ao criar o snapshot do processo\n";
        return false;
    }

    HANDLE hProcess = nullptr;
    if (Process32First(hSnapshot, &processEntry)) {
        do {
            if (_stricmp(processEntry.szExeFile, targetProcessName) == 0) {
                hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processEntry.th32ProcessID);
                break;
            }
        } while (Process32Next(hSnapshot, &processEntry));
    }

    CloseHandle(hSnapshot);

    if (hProcess == nullptr) {
        std::cerr << "Processo alvo não encontrado\n";
        return false;
    }


    LPVOID pRemoteMemory = VirtualAllocEx(hProcess, nullptr, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (pRemoteMemory == nullptr) {
        std::cerr << "Erro ao alocar memória remota\n";
        CloseHandle(hProcess);
        return false;
    }


    WriteProcessMemory(hProcess, pRemoteMemory, dllPath, strlen(dllPath) + 1, nullptr);


    HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, pRemoteMemory, 0, nullptr);
    if (hThread == nullptr) {
        std::cerr << "Erro ao criar a thread remota\n";
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }


    WaitForSingleObject(hThread, INFINITE);


    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
    CloseHandle(hProcess);

    std::cout << "DLL injetada com sucesso!\n";
    return true;
}

bool EjectDLL(const char* targetProcessName, const char* dllPath) {

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        std::cerr << "Erro ao criar o snapshot do processo\n";
        return false;
    }

    HANDLE hProcess = nullptr;
    if (Process32First(hSnapshot, &processEntry)) {
        do {
            if (_stricmp(processEntry.szExeFile, targetProcessName) == 0) {
                hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processEntry.th32ProcessID);
                break;
            }
        } while (Process32Next(hSnapshot, &processEntry));
    }

    CloseHandle(hSnapshot);

    if (hProcess == nullptr) {
        std::cerr << "Processo alvo não encontrado\n";
        return false;
    }


    HMODULE hModule = GetModuleHandleA(dllPath);
    if (hModule == nullptr) {
        std::cerr << "Erro ao obter o identificador do módulo da DLL\n";
        CloseHandle(hProcess);
        return false;
    }

    // Obter o endereço da função FreeLibrary
    FARPROC pFreeLibrary = GetProcAddress(GetModuleHandleA("kernel32.dll"), "FreeLibrary");
    if (pFreeLibrary == nullptr) {
        std::cerr << "Erro ao obter o endereço de FreeLibrary\n";
        CloseHandle(hProcess);
        return false;
    }


    HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0, (LPTHREAD_START_ROUTINE)pFreeLibrary, hModule, 0, nullptr);
    if (hThread == nullptr) {
        std::cerr << "Erro ao criar a thread remota\n";
        CloseHandle(hProcess);
        return false;
    }


    WaitForSingleObject(hThread, INFINITE);

    // Limpar recursos
    CloseHandle(hThread);
    CloseHandle(hProcess);

    std::cout << "DLL removida com sucesso!\n";

    return true;
}



void move_window() {

    ImGui::SetCursorPos(ImVec2(0, 0));
    if (ImGui::InvisibleButton("Move_detector", ImVec2(window::size.x, window::size.y)));
    if (ImGui::IsItemActive()) {

        GetWindowRect(hwnd, &rc);
        MoveWindow(hwnd, rc.left + ImGui::GetMouseDragDelta().x, rc.top + ImGui::GetMouseDragDelta().y, window::size.x, window::size.y, TRUE);
    }

}

void RenderBlur(HWND hwnd)
{
    struct ACCENTPOLICY
    {
        int na;
        int nf;
        int nc;
        int nA;
    };
    struct WINCOMPATTRDATA
    {
        int na;
        PVOID pd;
        ULONG ul;
    };

    const HINSTANCE hm = LoadLibrary("user32.dll");
    if (hm)
    {
        typedef BOOL(WINAPI* pSetWindowCompositionAttribute)(HWND, WINCOMPATTRDATA*);

        const pSetWindowCompositionAttribute SetWindowCompositionAttribute = (pSetWindowCompositionAttribute)GetProcAddress(hm, "SetWindowCompositionAttribute");
        if (SetWindowCompositionAttribute)
        {
            ACCENTPOLICY policy = { 3, 0, 0, 0 }; // and even works 4,0,155,0 (Acrylic blur)
            WINCOMPATTRDATA data = { 19, &policy,sizeof(ACCENTPOLICY) };
            SetWindowCompositionAttribute(hwnd, &data);
        }
        FreeLibrary(hm);
    }
}


bool CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
