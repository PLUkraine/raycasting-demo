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

const int WIDTH = 800;
const int HEIGHT = 600;
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
    raycaster::vec2<float> m_pos;
	float m_angle;
	float m_fov;
public:
	Player(float x = 0, float y = 0, float angle = 0, float fov=75.f)
		: m_pos(x,y), m_angle(angle), m_fov(fov)
	{}
    
    void moveForward(float factor)
    {
        m_pos += raycaster::vec2<float>(cosf(m_angle), sinf(m_angle)) * factor;
    }
    void strafeRight(float factor)
    {
        m_pos += raycaster::vec2<float>(cosf(m_angle+M_PI_2), sinf(m_angle+M_PI_2)) * factor;
    }
    void rotate(float onAngleRad)
    {
        m_angle = wrapAngle(m_angle + onAngleRad);
    }

	float lAngle() const
	{
		return wrapAngle(m_angle - m_fov / 2.f);
	}
	float rAngle() const
	{
		return wrapAngle(m_angle + m_fov / 2.f);
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
	Player p(BOARD_WIDTH/2, BOARD_HEIGHT/2, 0.f, glm::radians(45.f));
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
        // rotations
		if (window.getKeyState(GLFW_KEY_LEFT) == GLFW_PRESS)
            p.rotate(-1.15f*dt);
		if (window.getKeyState(GLFW_KEY_RIGHT) == GLFW_PRESS)
            p.rotate(1.15f*dt);
        // walking forwards/backwards
		if (window.getKeyState(GLFW_KEY_DOWN)|window.getKeyState(GLFW_KEY_UP)
            |window.getKeyState(GLFW_KEY_W)|window.getKeyState(GLFW_KEY_S) == GLFW_PRESS)
		{
            vec2<float> previous = p.m_pos;
            float moveFactor = (window.getKeyState(GLFW_KEY_DOWN)|window.getKeyState(GLFW_KEY_S) == GLFW_PRESS ? -dt : dt) * 3.0f;
            p.moveForward(moveFactor);
            
            if (board[(int)p.m_pos.y][(int)p.m_pos.x])
            {
                p.m_pos = previous;
            }
		}
        // strafing left/right
        if (window.getKeyState(GLFW_KEY_A)|window.getKeyState(GLFW_KEY_D) == GLFW_PRESS)
        {
            vec2<float> previous = p.m_pos;
            float moveFactor = (window.getKeyState(GLFW_KEY_A) == GLFW_PRESS ? -dt : dt) * 3.0f;
            p.strafeRight(moveFactor);
            
            if (board[(int)p.m_pos.y][(int)p.m_pos.x])
            {
                p.m_pos = previous;
            }
        }

		// update texture here ...
		tex1->clearTexture();
		// raycast here!
		const float MAX_DIST = 16.f;
		const float DIST_STEP = 0.05f;
		for (int x = 0; x < tex1->width(); ++x)
		{
            // [-pov/2; +pov/2]
			float rayDisplacementAngle = -p.m_fov / 2.f + (1.f * x / tex1->width()) * p.m_fov;
			float rayAngle = p.m_angle + rayDisplacementAngle;
			float wallDist = 0.f;
            vec2<float> projVec = {cosf(rayAngle), sinf(rayAngle)};
            
			bool wallHit = false;
            //
            vec2<float> uvTextureSample(0.f, 0.f);
			while (!wallHit && wallDist < MAX_DIST)
			{
				wallDist += DIST_STEP;
                vec2<float> hitCoord = p.m_pos + projVec*wallDist;
                vec2<int> blockCoord = vec2<int>(hitCoord.x, hitCoord.y);

				if (!inB(blockCoord.y, BOARD_WIDTH) || !inB(blockCoord.x, BOARD_HEIGHT))
				{
					wallHit = true;
					wallDist = MAX_DIST;
				}
				else
				{
					if (board[blockCoord.y][blockCoord.x])
					{
						wallHit = true;
                        
                        vec2<float> wallCenter = vec2<float>(blockCoord.x+0.5f, blockCoord.y+0.5f);
                        vec2<float> dirFromCenter = hitCoord - wallCenter;
                        float angle = atan2(dirFromCenter.y, dirFromCenter.x);
                        int octant = raycaster::getOctant((double)angle);
                        
                        if (octant==4||octant==5||octant==8||octant==1)
                            uvTextureSample.x = raycaster::getFraction(hitCoord.y);
                        if (octant==6||octant==7||octant==2||octant==3)
                            uvTextureSample.x = raycaster::getFraction(hitCoord.x);
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
                else if (y < ceilingYBorder) {
                    uvTextureSample.y = ((float)y - floorYBorder) / (ceilingYBorder - floorYBorder);
                    vec2<int> coord = uvTextureSample.multPerCoord(vec2<float>(brickTexture.width(), brickTexture.height()));
                    tex1->setPixel(x, y,
                                   colorMult*brickTexture.getData(coord.x, coord.y, 0),
                                   colorMult*brickTexture.getData(coord.x, coord.y, 1),
                                   colorMult*brickTexture.getData(coord.x, coord.y, 2)
                                   );
                }
                // ceiling
                else {
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

	tex1->dispose();
	renderer.dispose();
	window.dispose();
	glfwTerminate();

	return 0;
}
