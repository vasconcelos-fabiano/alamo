#include "Layout.h"

Layout createLayout(int screenWidth, int screenHeight)
{
    Layout layout;

    layout.screenWidth = screenWidth;
    layout.screenHeight = screenHeight;

    // proporção fixa 16:9
    float targetRatio = 16.0f / 9.0f;

    int viewportWidth = screenWidth;
    int viewportHeight = screenWidth / targetRatio;

    if (viewportHeight > screenHeight)
    {
        viewportHeight = screenHeight;
        viewportWidth = screenHeight * targetRatio;
    }

    int viewportX = (screenWidth - viewportWidth) / 2;
    int viewportY = (screenHeight - viewportHeight) / 2;

    // viewport do jogo
    layout.gameViewport.x = viewportX;
    layout.gameViewport.y = viewportY;
    layout.gameViewport.w = viewportWidth;
    layout.gameViewport.h = viewportHeight;

    // painel esquerdo (tudo que sobra à esquerda)
    layout.leftPanel.x = 0;
    layout.leftPanel.y = 0;
    layout.leftPanel.w = viewportX;
    layout.leftPanel.h = screenHeight;

    // painel direito (tudo que sobra à direita)
    layout.rightPanel.x = viewportX + viewportWidth;
    layout.rightPanel.y = 0;
    layout.rightPanel.w = screenWidth - (viewportX + viewportWidth);
    layout.rightPanel.h = screenHeight;

    return layout;
}