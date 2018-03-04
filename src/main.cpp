#define _USE_MATH_DEFINES
#include <math.h>
#include <ctype.h>
#include <thread>
#include <chrono>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "spdlog/spdlog.h"

#include "learnopengl/shader.h"
#include "RaycasterEngine.h"
#include "Window.h"
#include "Texture.h"
#include "ImageRenderer.h"

const int WIDTH = 1024;
const int HEIGHT = 768;
auto console = spdlog::stdout_color_st("console");

const char *VERTEX_SHADER = "shader.vert";
const char *FRAGMENT_SHADER = "shader.frag";
const int TEX1_WIDTH = 320;
const int TEX1_HEIGHT = 280;
const int BOARD_WIDTH = 16;
const int BOARD_HEIGHT = 16;


void glfwErrorCallback(int error, const char *desc)
{
    console->error(desc);
}

// wrap angle into [-M_PI, M_PI] interval
float wrapAngle(float angle)
{
    if (angle > M_PI)
        angle -= M_PI + M_PI;
    else if (angle < -M_PI)
        angle += M_PI + M_PI;
    return angle;
}

inline bool inB(int v, int b)
{
    return 0 <= v && v < b;
}

inline int get1dIndex(int i, int j, int width)
{
    return i*width + j;
}

class Player
{
public:
    raycaster::vec2<float> pos;
    float angle;
    float fov;
public:
    Player(float x = 0, float y = 0, float angle = 0, float fov=75.f)
    : pos(x,y), angle(angle), fov(fov)
    {}
    
    void moveForward(float factor)
    {
        pos += raycaster::vec2<float>(cosf(angle), sinf(angle)) * factor;
    }
    void strafeRight(float factor)
    {
        pos += raycaster::vec2<float>(cosf(angle+M_PI_2), sinf(angle+M_PI_2)) * factor;
    }
    void rotate(float onAngleRad)
    {
        angle = wrapAngle(angle + onAngleRad);
    }
    
    float lAngle() const
    {
        return wrapAngle(angle - fov / 2.f);
    }
    float rAngle() const
    {
        return wrapAngle(angle + fov / 2.f);
    }
    
    void update(float dt, core::Window& window, bool board[][BOARD_WIDTH])
    {
        // rotations
        if (window.getKeyState(GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            rotate(-1.15f*dt);
        }
        if (window.getKeyState(GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            rotate(1.15f*dt);
        }
        // walking forwards/backwards
        if (window.getKeyState(GLFW_KEY_DOWN)|window.getKeyState(GLFW_KEY_UP)
            |window.getKeyState(GLFW_KEY_W)|window.getKeyState(GLFW_KEY_S) == GLFW_PRESS)
        {
            raycaster::vec2<float> previous = pos;
            float moveFactor = (window.getKeyState(GLFW_KEY_DOWN)|window.getKeyState(GLFW_KEY_S) == GLFW_PRESS ? -dt : dt) * 3.0f;
            moveForward(moveFactor);
            
            if (board[(int)pos.y][(int)pos.x])
            {
                pos = previous;
            }
        }
        // strafing left/right
        if (window.getKeyState(GLFW_KEY_A)|window.getKeyState(GLFW_KEY_D) == GLFW_PRESS)
        {
            raycaster::vec2<float> previous = pos;
            float moveFactor = (window.getKeyState(GLFW_KEY_A) == GLFW_PRESS ? -dt : dt) * 3.0f;
            strafeRight(moveFactor);
            
            if (board[(int)pos.y][(int)pos.x])
            {
                pos = previous;
            }
        }
    }
};

template <typename T>
T clamp(T a, T b, T value)
{
    if (value < a) return a;
    else if (value > b) return b;
    else return value;
}
using raycaster::vec2;

int main()
{
    console->set_level(spdlog::level::debug);
    
    glfwInit();
    glfwSetErrorCallback(glfwErrorCallback);
    
    // init
    core::Window window;
    window.createGlContext(WIDTH, HEIGHT);
    
    Shader shader("shader.vert", "shader.frag");
    shader.use();
    
    // tex1 doesn't have to be a pointer, just a legacy I'm too lasy to get rid of
    core::Texture *tex1 = new core::Texture();
    tex1->createGlTexture(TEX1_WIDTH, TEX1_HEIGHT);
    core::Image brickTexture;
    brickTexture.loadFromFile("brick.png");
    core::ImageRenderer renderer;
    
    bool board[BOARD_HEIGHT][BOARD_WIDTH] =
    {
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1 },
        { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1 },
        { 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    };
    
    
    // Game Loop
    double start = glfwGetTime();
    Player p(BOARD_WIDTH/2+0.1f, BOARD_HEIGHT/2+0.1f, 0.f, glm::radians(45.f));
    while (!window.mustClose())
    {
        double end = glfwGetTime();
        double dt = end - start;
        start = end;
        // make processor sleep if we are getting our job done in time
        if (dt < (1. / 60.))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds((1000 / 60 - (int)dt*1000)));
        }
        
        // input processing ...
        p.update(dt, window, board);
        
        // update texture here ...
        tex1->clearTexture();
        // raycast here!
        const float MAX_DIST = 16.f;
        for (int x = 0; x < tex1->width(); ++x)
        {
            // [-pov/2; +pov/2]
            float rayDisplacementAngle = -p.fov / 2.f + (1.f * x / tex1->width()) * p.fov;
            float rayAngle = p.angle + rayDisplacementAngle;
            
            float wallDist = 0.f;
            bool wallHit = false;
            vec2<float> uvTextureSample(0.f, 0.f);
            // to create "shadow" effect we divide final color by this value
            int side = 1;
            
            vec2<int> curBrick(floorf(p.pos.x), floorf(p.pos.y));
            vec2<int> brickStep = raycaster::getDeltaBrick(rayAngle);
            vec2<float> hitCoord = p.pos;
            vec2<float> initDelta = vec2<float>(raycaster::modSgn(brickStep.x), raycaster::modSgn(brickStep.y)) + vec2<float>(curBrick) - p.pos;
            
            float tanRayAngle = fabsf(tanf(rayAngle));
            float dx = brickStep.x * 1.f / tanRayAngle;
            float dy = brickStep.y * tanRayAngle;
            
            vec2<float> advanceX = hitCoord + vec2<float>(initDelta.x, brickStep.y * fabsf(initDelta.x * tanRayAngle));
            vec2<float> advanceY = hitCoord + vec2<float>(brickStep.x * fabsf(initDelta.y / tanRayAngle), initDelta.y);
            
            while (!wallHit)
            {
                if (board[curBrick.y][curBrick.x] || (hitCoord-p.pos).sqrLen() >= MAX_DIST*MAX_DIST)
                {
                    wallHit = true;
                    wallDist = std::min(MAX_DIST, (hitCoord-p.pos).len());
                    
                    // calculating texture sampling u coordinate
                    vec2<float> wallCenter = vec2<float>(curBrick.x+0.5f, curBrick.y+0.5f);
                    vec2<float> dirFromCenter = hitCoord - wallCenter;
                    float angle = atan2(dirFromCenter.y, dirFromCenter.x);
                    int octant = raycaster::getOctant((double)angle);
                    
                    if (octant==4||octant==5)
                    {
                        uvTextureSample.x = raycaster::getFraction(hitCoord.y);
                        side = 1;
                    }
                    if(octant==8||octant==1)
                    {
                        uvTextureSample.x = 1.0f-raycaster::getFraction(hitCoord.y);
                        side = 1;
                    }
                    if (octant==6||octant==7)
                    {
                        uvTextureSample.x = 1.0f-raycaster::getFraction(hitCoord.x);
                        side = 2;
                    }
                    if (octant==2||octant==3)
                    {
                        uvTextureSample.x = raycaster::getFraction(hitCoord.x);
                        side = 2;
                    }
                }
                else
                {
                    // find intersection with the closest cell
                    
                    // if x advanced less then y
                    if ((advanceX - p.pos).sqrLen() < (advanceY - p.pos).sqrLen())
                    {
                        // move in X
                        hitCoord = advanceX;
                        advanceX += vec2<float>(brickStep.x, dy);
                        curBrick.x += brickStep.x;
                    }
                    else
                    {
                        // move in Y
                        hitCoord = advanceY;
                        advanceY += vec2<float>(dx, brickStep.y);
                        curBrick.y += brickStep.y;
                    }
                }
            }
            
            // z is a distance to a wall
            // this line prevents the Fisheye Effect
            float z = wallDist * cosf(rayDisplacementAngle);
            int floorYBorder = tex1->height() / 2.f - tex1->height() / z;
            int ceilingYBorder = tex1->height() - floorYBorder;
            
            // white near us, black when far
            float colorMult = 1.0 * (1 - z / MAX_DIST);
            
            // paint the texture
            // TODO do it in shader one day
            for (int y = 0; y < tex1->height(); ++y)
            {
                // floor
                if (y <= floorYBorder) {
                    tex1->setPixel(x, y, 0x55, 0x55, 0x55);
                }
                // wall
                else if (y < ceilingYBorder)
                {
                    uvTextureSample.y = ((float)y - floorYBorder) / (ceilingYBorder - floorYBorder);
                    vec2<int> coord = uvTextureSample.multPerCoord(vec2<float>(brickTexture.width(), brickTexture.height()));
                    tex1->setPixel(x, y,
                                   colorMult*brickTexture.getData(coord.x, coord.y, 0)/side,
                                   colorMult*brickTexture.getData(coord.x, coord.y, 1)/side,
                                   colorMult*brickTexture.getData(coord.x, coord.y, 2)/side
                                   );
                }
                // ceiling
                else
                {
                    tex1->setPixel(x, y, 0x55, 0x55, 0xff);
                }
            }
            
        }
        tex1->loadToVRAM();
        
        // rendering texture ...
        renderer.render(tex1, &shader);
        
        // polling events and swapping buffers ...
        window.update();
    }
    
    brickTexture.dispose();
    tex1->dispose();
    renderer.dispose();
    window.dispose();
    glfwTerminate();
    
    return 0;
}
