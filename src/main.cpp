#include <SDL2/SDL.h>
#include <iostream>
using namespace std;
#include <complex>
#include <random>
#include <vector>
#include <utility>
#include <cmath>

const int WIDTH = 800;
const int HEIGHT = 600;

const double MIN_X = -2.0;
const double MAX_X = 2.0;
const double MIN_Y = -1.5;
const double MAX_Y = 1.5;

const int MAX_ITERATIONS = 100;
const double BOUNDARY = 2.0;

SDL_Window *gWindow = nullptr;
SDL_Renderer *gRenderer = nullptr;

double centerX = 0.0;
double centerY = 0.0;
double scale = 0.9;

std::complex<double> c(-0.8, 0.156); // Change this constant to visualize different Julia sets

void initSDL()
{
    SDL_Init(SDL_INIT_VIDEO);
    gWindow = SDL_CreateWindow("Fractals", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
}

void closeSDL()
{
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

void drawMandelbrot()
{
    for (int x = 0; x < WIDTH; ++x)
    {
        for (int y = 0; y < HEIGHT; ++y)
        {
            double zx = (x - WIDTH / 2.0) / (0.5 * scale * WIDTH) + centerX;
            double zy = (y - HEIGHT / 2.0) / (0.5 * scale * HEIGHT) + centerY;

            double cx = zx;
            double cy = zy;

            int iter = 0;
            const int maxIter = 100;
            while (zx * zx + zy * zy < 4 && iter < maxIter)
            {
                double temp = zx * zx - zy * zy + cx;
                zy = 2.0 * zx * zy + cy;
                zx = temp;
                ++iter;
            }

            int color = (255 * iter) / maxIter;
            SDL_SetRenderDrawColor(gRenderer, color, color, color, 255);
            SDL_RenderDrawPoint(gRenderer, x, y);
        }
    }
    SDL_RenderPresent(gRenderer);
}

void drawJuliaSet()
{
    for (int x = 0; x < WIDTH; ++x)
    {
        for (int y = 0; y < HEIGHT; ++y)
        {
            double zx = (MIN_X + (MAX_X - MIN_X) * x / WIDTH);
            double zy = (MIN_Y + (MAX_Y - MIN_Y) * y / HEIGHT);

            complex<double> z(zx, zy);

            int iter = 0;
            while (abs(z) < BOUNDARY && iter < MAX_ITERATIONS)
            {
                z = z * z + c;
                ++iter;
            }

            int color = (255 * iter) / MAX_ITERATIONS;
            SDL_SetRenderDrawColor(gRenderer, color, color, color, 255);
            SDL_RenderDrawPoint(gRenderer, x, y);
        }
    }
    SDL_RenderPresent(gRenderer);
}

void drawKochLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int depth)
{
    x1 = (x1 - centerX);
    y1 = (y1 - centerY);
    x2 = (x2 - centerX);
    y2 = (y2 - centerY);

    if (depth == 0)
    {
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        return;
    }

    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    int newX1 = x1 + deltaX / 3;
    int newY1 = y1 + deltaY / 3;
    int newX2 = x1 + 2 * deltaX / 3;
    int newY2 = y1 + 2 * deltaY / 3;

    int tipX = (newX1 + newX2) / 2 - deltaY / 3;
    int tipY = (newY1 + newY2) / 2 + deltaX / 3;

    drawKochLine(renderer, x1, y1, newX1, newY1, depth - 1);
    drawKochLine(renderer, newX1, newY1, tipX, tipY, depth - 1);
    drawKochLine(renderer, tipX, tipY, newX2, newY2, depth - 1);
    drawKochLine(renderer, newX2, newY2, x2, y2, depth - 1);
}

void drawKochSnowflake(SDL_Renderer *renderer, int x, int y, int size, int depth)
{
    int x1 = x + size * std::cos(0);
    int y1 = y + size * std::sin(0);
    int x2 = x + size * std::cos(2 * M_PI / 3);
    int y2 = y + size * std::sin(2 * M_PI / 3);
    int x3 = x + size * std::cos(4 * M_PI / 3);
    int y3 = y + size * std::sin(4 * M_PI / 3);

    drawKochLine(renderer, x1, y1, x2, y2, depth);
    drawKochLine(renderer, x2, y2, x3, y3, depth);
    drawKochLine(renderer, x3, y3, x1, y1, depth);
}

void drawTriangle(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int x3, int y3)
{
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    SDL_RenderDrawLine(renderer, x2, y2, x3, y3);
    SDL_RenderDrawLine(renderer, x3, y3, x1, y1);
}

void drawSierpinski(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int x3, int y3, int depth)
{
    if (depth == 0)
    {
        drawTriangle(renderer, x1, y1, x2, y2, x3, y3);
    }
    else
    {
        x1 = (x1 - centerX);
        y1 = (y1 - centerY);
        x2 = (x2 - centerX);
        y2 = (y2 - centerY);
        x3 = (x3 - centerX);
        y3 = (y3 - centerY);

        int x12 = (x1 + x2) / 2;
        int y12 = (y1 + y2) / 2;
        int x23 = (x2 + x3) / 2;
        int y23 = (y2 + y3) / 2;
        int x31 = (x3 + x1) / 2;
        int y31 = (y3 + y1) / 2;

        drawSierpinski(renderer, x1, y1, x12, y12, x31, y31, depth - 1);
        drawSierpinski(renderer, x12, y12, x2, y2, x23, y23, depth - 1);
        drawSierpinski(renderer, x31, y31, x23, y23, x3, y3, depth - 1);
    }
}

void drawBarnsleyFern()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 100.0);

    double x = 0.0, y = 0.0;
    for (int i = 0; i < 100000; ++i)
    {
        double r = dis(gen);

        double x1, y1;
        if (r <= 1.0)
        {
            x1 = 0.0;
            y1 = 0.16 * y;
        }
        else if (r <= 86.0)
        {
            x1 = 0.85 * x + 0.04 * y;
            y1 = -0.04 * x + 0.85 * y + 1.6;
        }
        else if (r <= 93.0)
        {
            x1 = 0.2 * x - 0.26 * y;
            y1 = 0.23 * x + 0.22 * y + 1.6;
        }
        else
        {
            x1 = -0.15 * x + 0.28 * y;
            y1 = 0.26 * x + 0.24 * y + 0.44;
        }

        x = x1;
        y = y1;

        int px = static_cast<int>(WIDTH / 2.0 + 50.0 * x);
        int py = static_cast<int>(HEIGHT - 50.0 * y);

        SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
        SDL_RenderDrawPoint(gRenderer, px, py);
    }

    SDL_RenderPresent(gRenderer);
}

void drawTree(SDL_Renderer *renderer, int x, int y, double length, double angle, int depth)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    if (depth == 0)
    {
        return;
    }

    int x2 = x + static_cast<int>(length * cos(angle));
    int y2 = y - static_cast<int>(length * sin(angle));

    SDL_RenderDrawLine(renderer, x + centerX, y + centerY, x2, y2);

    double newLength1 = length * 0.7;
    double newLength2 = length * 0.7;

    double newAngle1 = angle + M_PI / 6;
    double newAngle2 = angle - M_PI / 6;

    drawTree(renderer, x2, y2, newLength1, newAngle1, depth - 1);
    drawTree(renderer, x2, y2, newLength2, newAngle2, depth - 1);
}

int main()
{

    bool quit = false;
    SDL_Event e;

    cout << "Choose a fractale to visualize:" << endl;
    cout << "1. Mandelbrot Set" << endl;
    cout << "2. Julia Set" << endl;
    cout << "3. Koch Snowflake" << endl;
    cout << "4. Sierpinski Triangle" << endl;
    cout << "5. Barnsley Fern" << endl;
    cout << "6. Tree" << endl;
    cout << "7. Exit" << endl;

    int choice;
    cin >> choice;

    int initialX = WIDTH / 2;
    int initialY = HEIGHT;
    double initialLength = HEIGHT / 5.0;
    double initialAngle = M_PI / 2;

    initSDL();

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
            SDL_RenderClear(gRenderer);
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                if (e.button.button == SDL_BUTTON_LEFT)
                {
                    double mouseX = ((2.0 * e.button.x / WIDTH - 1.0) - centerX) * scale;
                    double mouseY = ((2.0 * e.button.y / HEIGHT - 1.0) - centerY) * scale;

                    centerX += mouseX;
                    centerY += mouseY;
                    scale *= 1.2;
                }
                else if (e.button.button == SDL_BUTTON_RIGHT)
                {
                    double mouseX = ((2.0 * e.button.x / WIDTH - 1.0) - centerX) * scale;
                    double mouseY = ((2.0 * e.button.y / HEIGHT - 1.0) - centerY) * scale;

                    centerX += mouseX;
                    centerY += mouseY;
                    scale *= 0.7;
                }
            }
        }

        switch (choice)
        {
        case 1:
            drawMandelbrot();
            break;

        case 2:
            drawJuliaSet();
            break;

        case 3:
            drawKochSnowflake(gRenderer, WIDTH / 2, HEIGHT / 2, 200, 5);
            break;

        case 4:
            drawSierpinski(gRenderer, WIDTH / 2, 100, 100, HEIGHT - 100, WIDTH - 100, HEIGHT - 100, 6);
            break;

        case 5:
            drawBarnsleyFern();
            break;
        case 6:

            drawTree(gRenderer, initialX, initialY, initialLength, initialAngle, 10);
            break;

        case 7:
            closeSDL();
            return 0;
        }

        SDL_RenderPresent(gRenderer);
        SDL_Delay(1000 / 60);
    }
    closeSDL();
    return 0;
}