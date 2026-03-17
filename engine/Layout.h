#ifndef ALAMO_LAYOUT_H
#define ALAMO_LAYOUT_H

struct Rect {
    int x;
    int y;
    int width;
    int height;
};

struct Layout {
    int screenWidth;
    int screenHeight;

    int leftPanelWidth;
    int gameViewportWidth;
    int rightPanelWidth;

    Rect leftPanel;
    Rect gameViewport;
    Rect rightPanel;
};

Layout createLayout(int screenWidth, int screenHeight);

#endif
