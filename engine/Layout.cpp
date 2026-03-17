#include "Layout.h"

Layout createLayout(int screenWidth, int screenHeight)
{
    Layout layout;

    layout.screenWidth = screenWidth;
    layout.screenHeight = screenHeight;

    layout.leftPanelWidth = screenWidth * 0.15;
    layout.rightPanelWidth = screenWidth * 0.15;
    layout.gameViewportWidth = screenWidth * 0.70;

    layout.leftPanel = {
        0,
        0,
        layout.leftPanelWidth,
        screenHeight
    };

    layout.gameViewport = {
        layout.leftPanelWidth,
        0,
        layout.gameViewportWidth,
        screenHeight
    };

    layout.rightPanel = {
        layout.leftPanelWidth + layout.gameViewportWidth,
        0,
        layout.rightPanelWidth,
        screenHeight
    };

    return layout;
}