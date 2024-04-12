//
//  GUI_theme.cpp
//  GR_PLOTS_100
//
//  Created by Guillem Ramisa de Soto on 14/2/24.
//

#include "GUI_theme.hpp"

//ofColor getRandomColor()
//{
//    return ofColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255), 255);
//}

void GUI_theme::setup()
{
    // global text size
    // ImGui::SetWindowFontScale(1.5);
    
    // init style object to pass to the gui
    ImGuiStyle* style                                       = &ImGui::GetStyle();
    
    style->WindowTitleAlign                                 = ImVec2(1,0);

    // ImVec4 bgColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);  // Adjust the color as needed
    // style->Colors[ImGuiCol_DockingEmptyBg] = bgColor;
    // style->Colors[ImGuiCol_WindowBg] = bgColor;
    
    
    // global window constrol
    style->WindowMinSize                                    = ImVec2(300, 300);
    style->WindowRounding                                   = 3.0f;
    style->FrameRounding                                    = 2.0f;

    style->Alpha                                            = 1.0f;

    // items global control spacing
    style->ItemSpacing                                      = ImVec2(20, 20);
    style->ItemInnerSpacing                                 = ImVec2(10, 10);
    style->IndentSpacing                                    = 6.0f;
        
    // main window
    style->Colors[ImGuiCol_TitleBg]                         = ImVec4(0.2,0.2,0.2,1);
    style->Colors[ImGuiCol_TitleBgActive]                   = ImVec4(0.4,0.4,0.4,1);
    style->Colors[ImGuiCol_TitleBgCollapsed]                = ImVec4(0.3,0.3,0.3,0.75);
        
    style->Colors[ImGuiCol_WindowBg]                        = ImVec4(0.1,0.1,0.1,0.64);
    style->Colors[ImGuiCol_Border]                          = ImVec4(0.4,0.4,0.4,1);


    // ITEMS COLOR CONTROLS
    
    // windows popup for IsItemHovered()
    style->Colors[ImGuiCol_PopupBg]                         = ImVec4(0.1,0.1,0.1,0.9);
    
    // grip button resize
    style->Colors[ImGuiCol_ResizeGrip]                      = ImVec4(0.3,0.3,0.3,1);
    style->Colors[ImGuiCol_ResizeGripHovered]               = ImVec4(0.35,0.35,0.35,1);
    style->Colors[ImGuiCol_ResizeGripActive]                = ImVec4(0.4,0.4,0.4,1);
    
    // text
    style->Colors[ImGuiCol_Text]                            = ImVec4(0.9,0.9,0.9,1.0);
    
    // Background
    style->Colors[ImGuiCol_FrameBg]                         = ImVec4(0.3,0.3,0.3,1.0);
    style->Colors[ImGuiCol_FrameBgHovered]                  = ImVec4(0.35,0.35,0.35,1.0);
    style->Colors[ImGuiCol_FrameBgActive]                   = ImVec4(0.4,0.4,0.4,1.0);
    
    // checkbox tick
    style->Colors[ImGuiCol_CheckMark]                       = ImVec4(0.65, 0.65, 0.65, 1.0);
    
    // sliders control grab
    style->Colors[ImGuiCol_SliderGrab]                      = ImVec4(0.4,0.4,0.4,1.0);
    style->Colors[ImGuiCol_SliderGrabActive]                = ImVec4(0.5,0.5,0.5,1.0);
    
    // tabs
    style->Colors[ImGuiCol_Tab]                             = ImVec4(0.3,0.3,0.3,1.0);
    style->Colors[ImGuiCol_TabHovered]                      = ImVec4(0.4,0.4,0.4,1.0);
    style->Colors[ImGuiCol_TabActive]                       = ImVec4(0.45,0.45,0.45,1.0);
    style->Colors[ImGuiCol_TabUnfocused]                    = ImVec4(0.2,0.2,0.2,1.0);
    style->Colors[ImGuiCol_TabUnfocusedActive]              = ImVec4(0.3,0.3,0.3,1.0);
    
    // docking
    style->Colors[ImGuiCol_DockingPreview]                  = ImVec4(0.6, 0.6, 0.6, 1.0);
    style->Colors[ImGuiCol_DockingEmptyBg]                  = ImVec4(0.75,0.75,0.75,1.0);
    
    // scrollbar
    style->Colors[ImGuiCol_ScrollbarBg]                     = ImVec4(0.2, 0.2, 0.2, 0.75);
    style->Colors[ImGuiCol_ScrollbarGrab]                   = ImVec4(0.3, 0.3, 0.3, 0.75);
    style->Colors[ImGuiCol_ScrollbarGrabHovered]            = ImVec4(0.4, 0.4, 0.4, 0.75);
    style->Colors[ImGuiCol_ScrollbarGrabActive]             = ImVec4(0.45,0.45,0.45, 0.75);
    
    // separator, when 2 windows docked together
    style->Colors[ImGuiCol_Separator]                       = ImVec4(0.3,0.3,0.3, 1.0);
    style->Colors[ImGuiCol_SeparatorHovered]                = ImVec4(0.6,0.6,0.6, 1.0);
    style->Colors[ImGuiCol_SeparatorActive]                 = ImVec4(0.7,0.7,0.7, 1.0);

    // button
    style->Colors[ImGuiCol_Button]                          = ImVec4(0.35, 0.35, 0.35, 1.0);
    style->Colors[ImGuiCol_ButtonHovered]                   = ImVec4(0.4, 0.4, 0.4, 1.0);
    style->Colors[ImGuiCol_ButtonActive]                    = ImVec4(0.45, 0.45, 0.45, 1.0);

    // dropdown comobo-box items
    style->Colors[ImGuiCol_Header]                          = ImVec4(0.3, 0.3, 0.3, 1.0);
    style->Colors[ImGuiCol_HeaderHovered]                   = ImVec4(0.2, 0.2, 0.2, 1.0);
    style->Colors[ImGuiCol_HeaderActive]                    = ImVec4(0.4, 0.4, 0.4, 1.0);
    
}
