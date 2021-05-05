#include "Sandbox.h"
#include "GraphicUtil.cpp"
#include <GL/glut.h>
#include <ctime>

Sandbox* Sandbox::instance = nullptr;

Sandbox::Sandbox():
    m_fps(0),
    m_lastLoopTime(0),
    m_updating(false)
{
    instance = this;
}

void Sandbox::Start()
{
	m_updating = true;
    m_lastLoopTime = glutGet(GLUT_ELAPSED_TIME);

    glutDisplayFunc(&OnLoopWrapper);
    glutVisibilityFunc(&OnVisibleWrapper);
    glutKeyboardFunc(&OnKeyboardWrapper);
}

void Sandbox::Stop()
{
	m_updating = false;

    glutDisplayFunc(nullptr);
    glutVisibilityFunc(nullptr);
    glutKeyboardFunc(nullptr);
    glutIdleFunc(nullptr);
}

void Sandbox::Update(float delta)
{
	m_fps = 1.0f / delta;
	for (Entity* entity : m_entities)
	{
		entity->Update(delta);
	}
}

char fps[32];
void Sandbox::Repaint() const
{
    glColor3f(1.0f, 1.0f, 0.0f);
    for (Entity* entity : m_entities)
    {
        entity->Paint();
    }
    
    glColor3f(0.0f, 1.0f, 0.0f);
    sprintf_s(fps, "%.1f", m_fps);
    DisplayText(5, 20, 3.5f, fps);
}

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
    /* Exit on escape key press */
    case '\x1B':
    {
        exit(EXIT_SUCCESS);
        break;
    }
    }
}

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
    float elapsed = static_cast<float>((now - m_lastLoopTime) / CLOCKS_PER_SEC);
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
