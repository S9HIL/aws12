
#include "main.h"

using namespace std;
using namespace KeyAuth;
#define SNOW_LIMIT 20

Memory elite;

CNotifications p_notif;

bool authenticated = false;


bool aimbot_external = false;
bool aimbot_drag = false;
bool sniper_scope = false;
bool sniper_tracking = false;
bool sniper_aim = false;
bool sniper_switch = false;
bool sniper_fix = false;
bool chams_menu = false;
bool chams_hologram = false;
bool chams_box = false;
bool chams_wukong = false;
bool load_speed = false;
bool activate_speed = false;
bool load_camera = false;
bool activate_camera = false;
bool load_wallhack = false;
bool activate_wallhack = false;


static int aimbot_e_key;
static int aimbot_d_key;
static int speed_key;
static int camera_key;
static int wallhack_key;



DWORD picker_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview;



std::vector<Snowflake::Snowflake> snow;



int tabs = 0;

float timer_loading = 0.f;

DWORD win_flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;


void LoginManual()
{
    std::string KeyAuth_USER_user_str(KeyAuth_USER_user_char);
    std::string KeyAuth_USER_pass_str(KeyAuth_USER_pass_char);

    std::string KeyAuth_login_URL =
        "https://prod.keyauth.com/api/1.3/?type=login&username=" + KeyAuth_USER_user_str +
        "&pass=" + KeyAuth_USER_pass_str +
        "&hwid=" + KeyAuth_USER_HWID +
        "&sessionid=" + KeyAuth_sessionid +
        "&name=" + name + "&ownerid=" + ownerid;

    wrap::Response r1 = wrap::HttpsRequest(wrap::Url{ KeyAuth_login_URL.c_str() }, wrap::Method{ "POST" });

    json data1;
    try {
        data1 = json::parse(r1.text);
    }
    catch (...) {
        p_notif.AddMessage("Failed to parse JSON Response", ICON_NOTIFICATION_FILL, ImColor(89, 0, 255));
        return;
    }

    KeyAuth_message = data1.value("message", "Not Found");

    // Only attempt to extract further data if login was successful
    if (KeyAuth_message == "Logged in!" && data1.contains("info")) {

        auto info = data1["info"];

        KeyAuth_username = info.value("username", "Not Found");
        KeyAuth_ip = info.value("ip", "Not Found");
        KeyAuth_createdate = info.value("createdate", "Not Found");
        KeyAuth_lastlogin = info.value("lastlogin", "Not Found");

        if (info.contains("subscriptions") && info["subscriptions"].is_array() && !info["subscriptions"].empty()) {
            KeyAuth_expiry = info["subscriptions"][0].value("expiry", "Not Found");
        }
        else {
            KeyAuth_expiry = "Not Found";
        }

        p_notif.AddMessage("Successfully Logged In", "b", ImGui::GetColorU32(ImStyle::general_color));

        tabs = 0;
        authenticated = true;
        hide_login = true;

        std::time_t currentTime = std::time(0);
        std::time_t expiryTime = std::atoi(KeyAuth_expiry.c_str());

        std::string usernameDisplay = "User: " + KeyAuth_username;
        std::string expiryDisplay = "Expiry: " + FormatExpiryDate(KeyAuth_expiry);

        static char usernameBuf[64];
        static char expiryBuf[64];
        strncpy(usernameBuf, usernameDisplay.c_str(), sizeof(usernameBuf));
        strncpy(expiryBuf, expiryDisplay.c_str(), sizeof(expiryBuf));

        DiscordEventHandlers handle;
        memset(&handle, 0, sizeof(handle));
        Discord_Initialize("1371392284440006656", &handle, 1, NULL);

        DiscordRichPresence discordPresence;
        memset(&discordPresence, 0, sizeof(discordPresence));
        discordPresence.details = usernameBuf;
        discordPresence.state = expiryBuf;
        discordPresence.startTimestamp = currentTime;
        discordPresence.largeImageKey = "https://i.ibb.co/qL3XJb2P/Chat-GPT-Image-May-5-2025-11-09-07-PM.png";
        discordPresence.largeImageText = "Pico's Museum";
        discordPresence.smallImageKey = "https://i.gifer.com/3OWpa.gif";
        discordPresence.button1_label = "Pico's Museum";
        discordPresence.button1_url = "https://discord.gg/SXXN9nKxR9";
        discordPresence.button2_label = "Join Discord";
        discordPresence.button2_url = "https://discord.gg/SXXN9nKxR9";

        Discord_UpdatePresence(&discordPresence);

    }
    else {
        p_notif.AddMessage(KeyAuth_message.c_str(), "b", ImColor(255, 0, 0));
        authenticated = false;
    }

    std::cout << "Message: " << KeyAuth_message << std::endl;
    std::cout << "Expiry: " << KeyAuth_expiry << std::endl;
}

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

    
    const char* targetProcessName = "HD-Player.exe";

    if (FirstStart == true)
    {
        wrap::Response r = wrap::HttpsRequest(wrap::Url{ KeyAuth_init_URL }, wrap::Method{ "POST" });

        json data;
        try {
            data = json::parse(r.text);
        }
        catch (...) {
            std::cerr << "Failed to parse init JSON response!" << std::endl;

            FirstStart = false;
            return 0;
        }

        KeyAuth_message = data.value("message", "Not Found");
        KeyAuth_sessionid = data.value("sessionid", "Not Found");
        KeyAuth_version = data["appinfo"].value("version", "Not Found");

        if (KeyAuth_message != "Initialized") {

        }

        FirstStart = false;
    }

    WNDCLASSEXW wc;
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = NULL;
    wc.cbWndExtra = NULL;
    wc.hInstance = nullptr;
    wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = L"ImGui";
    wc.lpszClassName = L"Example";
    wc.hIconSm = LoadIcon(0, IDI_APPLICATION);

    RegisterClassExW(&wc);
    hwnd = CreateWindowExW(NULL, wc.lpszClassName, L"Example", WS_POPUP, (GetSystemMetrics(SM_CXSCREEN) / 2) - (window::size.x / 2), (GetSystemMetrics(SM_CYSCREEN) / 2) - (window::size.y / 2), window::size.x, window::size.y, 0, 0, 0, 0);

    //RenderBlur(hwnd);
    
    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    POINT mouse;
    rc = { 0 };
    GetWindowRect(hwnd, &rc);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImFontConfig cfg;
    cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_NoHinting | ImGuiFreeTypeBuilderFlags_LightHinting | ImGuiFreeTypeBuilderFlags_LoadColor;;

    io.Fonts->AddFontFromMemoryTTF(poppins, sizeof(poppins), 23.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

    static const ImWchar icomoon_ranges[] = { 0xF000, 0xF3FF, 0 };

    static ImFontConfig icomoon_config;
    icomoon_config.OversampleH = icomoon_config.OversampleV = 1;
    icomoon_config.MergeMode = true;
    icomoon_config.GlyphOffset.y = 6.5f;
    icomoon_config.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LoadColor;
    

    poppins_reg = io.Fonts->AddFontFromMemoryTTF(&poppins, sizeof poppins, 23, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    fonts::roboto = io.Fonts->AddFontFromMemoryTTF(&roboto, sizeof roboto, 45, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    lexend_reg = io.Fonts->AddFontFromMemoryTTF(&lexend_regular, sizeof lexend_regular, 14, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    lexend_b = io.Fonts->AddFontFromMemoryTTF(lexend_regular, sizeof(lexend_regular), 19, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    icon_widget = io.Fonts->AddFontFromMemoryTTF(icomoon_widget, sizeof(icomoon_widget), 15.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    fonts::icon = io.Fonts->AddFontFromMemoryTTF(&ico, sizeof ico, 35, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    icon_moon2 = io.Fonts->AddFontFromMemoryTTF(&icomoon2, sizeof icomoon2, 35, NULL, io.Fonts->GetGlyphRangesCyrillic());
    icomoon_widget2 = io.Fonts->AddFontFromMemoryTTF(icomoonw2, sizeof(icomoonw2), 16.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    icon_moon = io.Fonts->AddFontFromMemoryTTF(&icomoon, sizeof icomoon, 20, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    inter = io.Fonts->AddFontFromMemoryTTF(inter_semibold, sizeof(inter_semibold), 17.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    notif_font = io.Fonts->AddFontFromMemoryCompressedBase85TTF(icomoon_compressed_data_base85, 25.f, &icomoon_config, icomoon_ranges);
    fonts::jupiter = io.Fonts->AddFontFromMemoryTTF(&jupiter_mission, sizeof jupiter_mission, 40, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    ImFont* jupiter_mini = io.Fonts->AddFontFromMemoryTTF(&jupiter_mission, sizeof jupiter_mission, 30, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    ImFont* jupiter_anim = io.Fonts->AddFontFromMemoryTTF(&jupiter_mission, sizeof jupiter_mission, 48,  &cfg, io.Fonts->GetGlyphRangesCyrillic());
    ImFont* poppins_mini = io.Fonts->AddFontFromMemoryTTF(&poppins, sizeof poppins, 18, &cfg, io.Fonts->GetGlyphRangesCyrillic());

    io.Fonts->Build();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //Snowflake::CreateSnowFlakes(snow, SNOW_LIMIT, 1.5f, 3.f, 0, 0, window::size.x, window::size.y, Snowflake::vec3(0.f, 0.01f), ImGui::GetColorU32(general_color));

    bool done = false;
    while (!done)
    {

        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            ImGuiContext& g = *GImGui;

            D3DX11_IMAGE_LOAD_INFO info; ID3DX11ThreadPump* pump{ nullptr };
            if (circle_loading == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, circle, sizeof(circle), &info, pump, &circle_loading, 0);
            if (minecraft_pic == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, minecraft_bg, sizeof(minecraft_bg), &info, pump, &minecraft_pic, 0);
            if (lg == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, logo, sizeof(logo), &info, pump, &lg, 0);

            ImGui::GetStyle().WindowPadding = ImVec2(0, 0);
            ImGui::GetStyle().WindowBorderSize = 0.f;
            ImGui::GetStyle().ItemSpacing = ImVec2(15, 15);
            ImGui::GetStyle().ScrollbarSize = 7.f;

            ImGui::SetNextWindowSize(ImVec2(window::size.x, window::size.y));
            ImGui::SetNextWindowPos(ImVec2(0, 0));

            ImGui::Begin("Pico", nullptr, win_flags);
            {
                GetWindowRect(hwnd, &rc);

                GetCursorPos(&mouse);

                ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(window::size.x, window::size.y), ImGui::GetColorU32(window::background), window::rounding);

                //Snowflake::Update(snow, Snowflake::vec3(mouse.x, mouse.y), Snowflake::vec3(rc.left, rc.top));

                ImGui::SetCursorPos(ImVec2(window::size.x - (45 * 2), 5));
                ImGui::BeginGroup();
                {

                    if (ImGui::TextButton("B", ImVec2(25, 25))) ShowWindow(hwnd, SW_MINIMIZE);

                    ImGui::SameLine();

                    if (ImGui::TextButton("A", ImVec2(25, 25))) {
                        ShowWindow(hwnd, SW_HIDE);
                        done = true;
                    };

                }
                ImGui::EndGroup();

                static float login_alpha = 0.0f;

                if (!hide_login)
                {
                    login_alpha = ImClamp(login_alpha + ImGui::GetIO().DeltaTime * 6.0f, 0.0f, 1.0f);
                }
                else
                {
                    login_alpha = ImClamp(login_alpha - ImGui::GetIO().DeltaTime * 6.0f, 0.0f, 1.0f);
                }

                ImVec2 tom = ImGui::GetIO().MousePos;

                ImGui::GetWindowDrawList()->AddCircleFilled(tom, 2.f, ImGui::GetColorU32(ImStyle::general_color), 20);
                ImGui::GetWindowDrawList()->AddShadowCircle(tom, 4.f, ImGui::GetColorU32(ImStyle::general_color), 40.f, ImVec2(0, 0), 0, 20);

                if (authenticated == false || login_alpha > 0.0f)
                {
                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, login_alpha * ImGui::GetStyle().Alpha);

                    ImVec2 logo_pos = ImVec2(755 - 160, 40);
                    ImVec2 logo_size = ImVec2(80, 80);
                    ImGui::GetWindowDrawList()->AddImage(lg, logo_pos, ImVec2(logo_pos.x + logo_size.x, logo_pos.y + logo_size.y), ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(ImStyle::general_color));
                    ImGui::GetWindowDrawList()->AddImage(minecraft_pic, ImVec2(0, 0), ImVec2(window::size.x / 2 + 80, window::size.y), ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(window::background_pic));

                    ImGui::PushFont(fonts::jupiter);
                    ImGui::ShadowText("PICO'S ", ImGui::GetColorU32(general_color), ImGui::GetColorU32(general_color), 80.f, ImVec2(((window::size.x / 2 + 80) / 2 - (ImGui::CalcTextSize("PICO'S MUSEUM").x) / 2), 80));
                    ImGui::ShadowText("MUSEUM", ImGui::GetColorU32(ImStyle::text::text_active), ImGui::GetColorU32(ImStyle::text::text_active), 80.f, ImVec2(((window::size.x / 2 + 80) / 2 - (ImGui::CalcTextSize("PICO'S MUSEUM").x) / 2 + ImGui::CalcTextSize("PICO'S ").x), 80));
                    ImGui::PopFont();


                    const char* line1 =  "ImGui Design By Pico's Museum";
                    const char* line2 =  "Minimal Design";
                    const char* line3 =  "Not perfect but good for external cheats";
                    const char* line4 =  "UI + Memory Only";

                    ImGui::PushFont(poppins_mini);
                    ImGui::ShadowText(line1, ImGui::GetColorU32(ImStyle::text::text_active), ImGui::GetColorU32(ImStyle::text::text_active), 25.f, ImVec2(((window::size.x / 2 + 80) / 2 - (ImGui::CalcTextSize(line1).x) / 2), 180));
                    ImGui::ShadowText(line2, ImGui::GetColorU32(ImStyle::text::text_active), ImGui::GetColorU32(ImStyle::text::text_active), 25.f, ImVec2(((window::size.x / 2 + 80) / 2 - (ImGui::CalcTextSize(line2).x) / 2), ImGui::GetCursorPosY()));
                    ImGui::ShadowText(line3, ImGui::GetColorU32(ImStyle::text::text_active), ImGui::GetColorU32(ImStyle::text::text_active), 25.f, ImVec2(((window::size.x / 2 + 80) / 2 - (ImGui::CalcTextSize(line3).x) / 2), ImGui::GetCursorPosY()));
                    ImGui::ShadowText(line4, ImGui::GetColorU32(ImStyle::text::text_active), ImGui::GetColorU32(ImStyle::text::text_active), 25.f, ImVec2(((window::size.x / 2 + 80) / 2 - (ImGui::CalcTextSize(line4).x) / 2), ImGui::GetCursorPosY()));

                    const char* copyright = "Copyright 2025 Pico's Museum";
                    ImGui::ShadowText(copyright, ImGui::GetColorU32(general_color), ImGui::GetColorU32(general_color), 35.f, ImVec2(((window::size.x / 2 + 80) / 2 - (ImGui::CalcTextSize(copyright).x) / 2), window::size.y - 15 - ImGui::CalcTextSize(copyright).y));

                    ImGui::PopFont();


                    ImGui::PushFont(jupiter_mini);
                    ImGui::ShadowText("WELCOME", ImGui::GetColorU32(ImStyle::text::text_active), ImGui::GetColorU32(ImStyle::text::text_active), 80.f, ImVec2(window::size.x - 160 - (ImGui::CalcTextSize("WELCOME").x) / 2, 145));
                    ImGui::PopFont();

                    static bool login_page = true;
                    static float login_page_offset = 0.f;
                    static float animation_speed = 0.1f;

                    login_page_offset = ImLerp(login_page_offset, login_page ? (window::size.y / 2) - (105 / 2) - (ImGui::GetStyle().ItemSpacing.y / 2) * 2 + 30 : -100.f, animation_speed);

                    ImGui::SetCursorPos(ImVec2((window::size.x / 2 + 40) + (window::size.x / 4 - 230 / 2), login_page_offset));
                    ImGui::BeginGroup();
                    {

                        ImGui::InputTextEx("##0", "Username", KeyAuth_USER_user_char, 20, ImVec2(230, 35), ImGuiInputTextFlags_None);

                        ImGui::InputTextEx("##1", "Password", KeyAuth_USER_pass_char, 20, ImVec2(230, 35), ImGuiInputTextFlags_Password);

                        ImGui::Spacing();

                        ImGui::TextColored(ImColor(ImStyle::text::have_account), "I don't have an account");

                        if (ImGui::IsItemClicked()) ShellExecute(NULL, "open", "https://discord.gg/j36BHbPgDv", NULL, NULL, SW_SHOW);

                        if (ImGui::Button("Login", ImVec2(230, 35))) {

                            std::thread([]()
                                {

                                    LoginManual();

                                }).detach();
                        };
                    }
                    ImGui::EndGroup();

                    ImGui::SetCursorPos(ImVec2(755 - 167.5, window::size.y - 65));

                    ImGui::BeginGroup();
                    {

                        if (ImGui::TextButton("C", ImVec2(35, 35)));
                        if (ImGui::IsItemClicked()) ShellExecute(NULL, "open", "https://discord.gg/j36BHbPgDv", NULL, NULL, SW_SHOW);

                        ImGui::SameLine(0, 20);

                        if (ImGui::TextButton("D", ImVec2(35, 35)));
                        if (ImGui::IsItemClicked()) ShellExecute(NULL, "open", "https://discord.gg/j36BHbPgDv", NULL, NULL, SW_SHOW);

                    }
                    ImGui::EndGroup();

                    ImGui::PopStyleVar();
                }
                else if(authenticated == true && login_alpha <= 0.0f)
                {
                    const char* text = "PICO'S MUSEUM";
                    ImGui::PushFont(jupiter_anim);
                    static float height = window::size.y / 2 - ImGui::CalcTextSize(text).y / 2;
                    ImGui::PopFont();
                    RenderAnimatedText(text, 0.0f, height, jupiter_anim);


                    static float tab_alpha = 0.f; /* */ static float tab_add; /* */ static int active_tab = 0;

                    tab_alpha = ImClamp(tab_alpha + (4.f * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f);
                    tab_add = ImClamp(tab_add + (std::round(350.f) * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f);

                    if (tab_alpha == 0.f && tab_add == 0.f) active_tab = tabs;


                    static float item_alpha = 0.0f;

                    if (show_particles)
                        item_alpha = ImClamp(item_alpha + ImGui::GetIO().DeltaTime * 6.0f, 0.0f, 1.0f);

                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, item_alpha* ImGui::GetStyle().Alpha);

                    ImGui::SetCursorPos(ImVec2(0, 50));
                    ImGui::BeginChild("Tabs", ImVec2(ImStyle::window::size.x, 35), false);

                    float childWidth = ImGui::GetContentRegionAvail().x;
                    const int totalTabs = 5;

                    ImGui::SetCursorPosX(20);
                    ImGui::Tab("d", "External", &tabs, 0, totalTabs, childWidth, item_alpha);
                    ImGui::Tab("b", "Sniper", &tabs, 1, totalTabs, childWidth, item_alpha);
                    ImGui::Tab("c", "Visual", &tabs, 2, totalTabs, childWidth, item_alpha);
                    ImGui::Tab("s", "Brutal", &tabs, 3, totalTabs, childWidth, item_alpha);
                    ImGui::Tab("2", "Settings", &tabs, 4, totalTabs, childWidth, item_alpha);

                    ImGui::EndChild();

                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha * ImGui::GetStyle().Alpha);

                    if (show_particles)
                    {
                        /*ParticlesWhite();

                        ParticlesV();*/

                        Hexagons();
                        HexagonsW();
                    }

                    if (active_tab == 0)
                    {
                        ImVec2 blur_pos = ImVec2(160, 100);

                        ImGui::SetCursorPos(blur_pos);

                        ImGui::BeginChild("External", ImVec2(ImStyle::window::size.x - 320, 230), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
                        {

                            if (ImGui::Checkbox("Aimbot External", "Activate Aimbot External", &aimbot_external))
                            {


                            }

                            ImGui::Keybind("Aimbot External - Key Bind", "Assign Key For Aimbot External", &aimbot_e_key);


                            if (ImGui::Checkbox("Aimbot Drag", "Activate Aimbot Drag", &aimbot_drag))
                            {


                            }

                            ImGui::Keybind("Aimbot Drag - Key Bind", "Assign Key For Aimbot Drag", &aimbot_d_key);


                        }
                        ImGui::EndChild();
                        
                    }
                    else if (active_tab == 1) 
                    {

                        ImGui::SetCursorPos(ImVec2(160, 100));

                        ImGui::BeginChild("Sniper", ImVec2(ImStyle::window::size.x - 320, 290), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
                        {

                            if (ImGui::Checkbox("Sniper Scope", "Activate Sniper Scope", &sniper_scope))
                            {


                            }

                            if (ImGui::Checkbox("Sniper Scope Tracking", "Activate Sniper Scope Tracking", &sniper_tracking))
                            {


                            }

                            if (ImGui::Checkbox("Sniper Aim", "Activate Sniper No Scope", &sniper_aim))
                            {


                            }

                            if (ImGui::Checkbox("Sniper Switch", "Activate Sniper Switch", &sniper_switch))
                            {


                            }

                            if (ImGui::Checkbox("Sniper Delay Fix", "Activate Sniper Delay Fix", &sniper_fix))
                            {


                            }


                        }
                        ImGui::EndChild();


                    }
                    else if (active_tab == 2) 
                    {

                        ImGui::SetCursorPos(ImVec2(160, 100));

                        ImGui::BeginChild("Visual", ImVec2(ImStyle::window::size.x - 320, 230), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
                        {

                            if (ImGui::Checkbox("Chams Menu", "Inject Chams Menu", &chams_menu))
                            {


                            }

                            if (ImGui::Checkbox("Chams Hologram", "Activate Hologram Chams", &chams_hologram))
                            {


                            }

                            if (ImGui::Checkbox("Chams Box", "Activate Chams Box", &chams_box))
                            {


                            }

                            if (ImGui::Checkbox("Chams Wukong", "Activate Chams for Wukong", &chams_wukong))
                            {


                            }

                        }
                        ImGui::EndChild();


                    }
                    else if (active_tab == 3) 
                    {

                        ImGui::SetCursorPos(ImVec2(160, 100));

                        ImGui::BeginChild("Brutal", ImVec2(ImStyle::window::size.x - 320, 350), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
                        {

                            if (ImGui::Checkbox("Load Speed Hack", "Load before activating Speed Hack", &load_speed))
                            {


                            }

                            ImGui::Keybind("Speed Hack - Key Bind", "Assign Key For Speed Hack", &speed_key);

                            if (ImGui::Checkbox("Load Camera Hack", "Load before activating Camera Hack", &load_camera))
                            {


                            }

                            ImGui::Keybind("Cameara Hack - Key Bind", "Assign Key For Camera Hack", &camera_key);

                            if (ImGui::Checkbox("Load Wall Hack", "Load before activating Wall Hack", &load_wallhack))
                            {


                            }

                            ImGui::Keybind("Wall Hack - Key Bind", "Assign Key For Wall Hack", &wallhack_key);

                        }
                        ImGui::EndChild();

                    }
                    else if (active_tab == 4) 
                    {
                        ImGui::SetCursorPos(ImVec2(160, 100));


                        ImGui::BeginChild("Brutal", ImVec2(ImStyle::window::size.x - 320, 350), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
                        {

                            static int select = 0;
                            static int previousSelect = -1;
                            const char* items[3]{ "Item 1", "Item 2", "Item 3" };

                            edited::Combo("Example Combo", "Example Demo Combo", &select, items, IM_ARRAYSIZE(items), 3);

                            static float examplefloat;
                            edited::SliderFloat("Example Slider", "Example Demo Slider", &examplefloat, 1.f, 50.f, "%.0f");

                            static float examplefloat2;
                            ImGui::SliderFloat("Example Slider 2", &examplefloat2, 1.f, 50.f, "%.0f");

                            edited::ColorEdit4("Accent Color", "Change Menu Accent Color", (float*)&ImStyle::general_color, picker_flags);

                        }
                        ImGui::EndChild();
                    }

                    ImGui::PopStyleVar(2);

                }
                
                
                move_window();
            }
            p_notif.Render();
            ImGui::End();

        }
        ImGui::Render();
        const float clear_color_with_alpha[4] = { 0 };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}
