#include "Figure.h"
#include "EllipseFigure.h"
#include "Triangle.h"
#include "RectangleFigure.h"
#include "Service.h"
#include "FileManager.h"

#define WND_WIDTH 800
#define WND_HEIGHT 700
#define WND_CLASSNAME "MainWindow"
#define WND_NAME "Paint"

#define BLACK_PEN RGB(0, 0, 0)
#define WHITE_PEN RGB(255, 255, 255)
#define GRAY_PEN RGB(169, 169, 169)

#define NO_COMMAND 0
#define CIRCLE_DRAW_COMMAND 1
#define RECTANGLE_DRAW_COMMAND 2
#define TRIANGLE_DRAW_COMMAND 3
#define CONNECTOR_LINE_COMMAND 4
#define MOVE_FIGURES_COMMAND 5
#define SAVE_FILE_COMMAND 6
#define OPEN_FILE_COMMAND 7

#define ID_CIRCLE 666
#define ID_RECTANGLE 667
#define ID_TRIANGLE 668
#define ID_CONNECTOR_LINE 669
#define ID_MOVE_OBJECTS 670
#define ID_SAVE_FILE 671
#define ID_OPEN_FILE 672

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);