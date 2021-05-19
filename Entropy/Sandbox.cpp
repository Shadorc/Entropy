#include "CollisionManager.h"
#include "Entity.h"
#include "Sandbox.h"
#include <GL/glut.h>
#include <ctime>
#include "GraphicUtil.h"

#ifdef _DEBUG
#include "DebugMode.h"
#endif // _DEBUG

Sandbox* Sandbox::instance = nullptr;

Sandbox::Sandbox():
    m_collisionManager(new CollisionManager(this))
    , m_fps(0)
    , m_lastLoopTime(0)
    , m_updating(false)
#ifdef _DEBUG
    , m_debugMask(0)
#endif // _DEBUG
{
    instance = this;
}

Sandbox::~Sandbox()
{
    delete m_collisionManager;
    instance = nullptr;
}

void Sandbox::Start()
{
	m_updating = true;
    m_lastLoopTime = glutGet(GLUT_ELAPSED_TIME);

    glutDisplayFunc(&OnLoopWrapper);
    glutVisibilityFunc(&OnVisibleWrapper);
    glutKeyboardFunc(&OnKeyboardWrapper);
#ifdef _DEBUG
    glutSpecialFunc(&OnSpecialKeyboardWrapper);
#endif // _DEBUG
}

void Sandbox::Stop()
{
	m_updating = false;

    glutDisplayFunc(nullptr);
    glutVisibilityFunc(nullptr);
    glutKeyboardFunc(nullptr);
#ifdef _DEBUG
    glutSpecialFunc(nullptr);
#endif // _DEBUG
    glutIdleFunc(nullptr);
}

void Sandbox::Update(float delta)
{
	m_fps = 1.0f / delta;
	for (Entity* entity : m_entities)
	{
		entity->Update(delta);
	}

    m_collisionManager->Update(delta);
}

void Sandbox::Repaint() const
{
    glColor3f(1.0f, 1.0f, 0.0f);
    for (Entity* entity : m_entities)
    {
        entity->Paint();
    }

#ifdef _DEBUG
    RepaintDebug();
#endif // _DEBUG
}

#ifdef _DEBUG
#define DEBUG_MODE_ENABLED(mode) m_debugMask & 1 << static_cast<int>(mode)
void Sandbox::RepaintDebug() const
{
    static char fps[16];
    if (DEBUG_MODE_ENABLED(DebugMode::PERFORMANCE_INFO))
    {
        glColor3f(0.0f, 1.0f, 0.0f);
        sprintf_s(fps, "%.1f", m_fps);
        DrawText(5, 20, 3.5f, fps);
    }

    if (DEBUG_MODE_ENABLED(DebugMode::SHOW_QUADTREE))
    {
        DrawQuadTree(m_collisionManager->GetRootQuadTree());
    }

    if (DEBUG_MODE_ENABLED(DebugMode::SHOW_AABB))
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        for (Entity* entity : m_entities)
        {
            DrawAABB(entity->GetAABB());
        }
    }

    if (DEBUG_MODE_ENABLED(DebugMode::SHOW_VELOCITY))
    {
    }
}
#endif // _DEBUG

std::vector<Entity*> Sandbox::GetEntities() const
{
    return m_entities;
}

void Sandbox::AddEntity(Entity* entity)
{
	m_entities.push_back(entity);
}

void Sandbox::RemoveEntity(Entity* entity)
{
    auto it = std::find(m_entities.begin(), m_entities.end(), entity);
    if (it != m_entities.end())
    {
		m_entities.erase(it);
    }
}

void Sandbox::OnKeyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    // Exit on escape key press
    case '\x1B':
    {
        exit(EXIT_SUCCESS);
        break;
    }
    }
}

#ifdef _DEBUG
#define ENABLE_DEBUG_MODE(mode) m_debugMask ^= 1 << static_cast<int>(mode)
void Sandbox::OnSpecialKeyboard(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_F1:
    {
        ENABLE_DEBUG_MODE(DebugMode::PERFORMANCE_INFO);
        break;
    }
    case GLUT_KEY_F2:
    {
        ENABLE_DEBUG_MODE(DebugMode::SHOW_QUADTREE);
        break;
    }
    case GLUT_KEY_F3:
    {
        ENABLE_DEBUG_MODE(DebugMode::SHOW_AABB);
        break;
    }
    case GLUT_KEY_F4:
    {
        ENABLE_DEBUG_MODE(DebugMode::SHOW_VELOCITY);
        break;
    }
    }
}
#endif // _DEBUG

void Sandbox::OnVisible(int visibility)
{
    if (visibility == GLUT_VISIBLE)
        glutIdleFunc(&OnIdleWrapper);
    else
        glutIdleFunc(nullptr);
}

void Sandbox::OnIdle()
{
    glutPostRedisplay();
}

void Sandbox::OnLoop()
{
    int now = glutGet(GLUT_ELAPSED_TIME);
    float elapsed = static_cast<float>(now - m_lastLoopTime) / CLOCKS_PER_SEC;
    m_lastLoopTime = now;

    glClear(GL_COLOR_BUFFER_BIT);

    Update(elapsed);
    Repaint();

    glutSwapBuffers();
}

/* 
 * Static functions which are passed to Glut function callbacks 
 */

void Sandbox::OnIdleWrapper()
{
    instance->OnIdle();
}

void Sandbox::OnVisibleWrapper(int visibility)
{
    instance->OnVisible(visibility);
}

void Sandbox::OnLoopWrapper()
{
    instance->OnLoop();
}

void Sandbox::OnKeyboardWrapper(unsigned char key, int x, int y)
{
    instance->OnKeyboard(key, x, y);
}

#ifdef _DEBUG
void Sandbox::OnSpecialKeyboardWrapper(int key, int x, int y)
{
    instance->OnSpecialKeyboard(key, x, y);
}
#endif // _DEBUG
