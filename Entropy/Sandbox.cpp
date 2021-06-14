#include <ctime>

#include "Precompiled.h"

constexpr float MAX_ACCUMULATOR_TIME = 0.2f;

Sandbox* Sandbox::instance = nullptr;

Sandbox::Sandbox():
    m_collisionManager(ENTROPY_NEW(CollisionManager, this))
    , m_entities()
    , m_updating(false)
    , m_fps(0)
    , m_lastLoopTime(0)
    , m_accumulatorTime(0)
#ifdef ENTROPY_DEBUG
    , m_debugMode()
#endif // ENTROPY_DEBUG
{
    instance = this;
}

Sandbox::~Sandbox()
{
    ENTROPY_DELETE(m_collisionManager);
    instance = nullptr;
}

void Sandbox::Start()
{
	m_updating = true;
    m_lastLoopTime = glutGet(GLUT_ELAPSED_TIME);

    glutDisplayFunc(&OnLoopWrapper);
    glutVisibilityFunc(&OnVisibleWrapper);
    glutMouseFunc(&OnMouseWrapper);
    glutKeyboardFunc(&OnKeyboardWrapper);
#ifdef ENTROPY_DEBUG
    glutSpecialFunc(&OnSpecialKeyboardWrapper);
#endif // ENTROPY_DEBUG
}

void Sandbox::Stop()
{
	m_updating = false;

    glutDisplayFunc(nullptr);
    glutVisibilityFunc(nullptr);
    glutMouseFunc(nullptr);
    glutKeyboardFunc(nullptr);
    glutIdleFunc(nullptr);
#ifdef ENTROPY_DEBUG
    glutSpecialFunc(nullptr);
#endif // ENTROPY_DEBUG
}

void Sandbox::Update(float deltaTime)
{
	for (Entity* entity : m_entities)
	{
		entity->Update(deltaTime);
	}

    m_collisionManager->Update();
}

void Sandbox::Repaint() const
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 0.0f);
    for (Entity* entity : m_entities)
    {
        entity->Paint();
    }

#ifdef ENTROPY_DEBUG
    RepaintDebug();
#endif // ENTROPY_DEBUG

    glutSwapBuffers();
}

#ifdef ENTROPY_DEBUG
static char s_strBuffer[64];
static std::vector<const char*> texts = {
    "F1: Show FPS",
    "F2: Show quadtree",
    "F3: Show AABB",
    "F4: Show velocity",
    "Left click: Spawn circle",
    "Right click: Spawn rectangle"
};
void Sandbox::RepaintDebug() const
{
    glColor3f(0.5f, 0.5f, 0.9f);
    for (int i = 0; i < texts.size(); ++i)
    {
        RenderText(5.0f, (i + 1) * 15.0f, texts[i]);
    }

    if (m_debugMode.IsEnabled(DebugOption::PERFORMANCE_INFO))
    {
        glColor3f(0.5f, 0.5f, 0.9f);
        glColor3f(0.0f, 1.0f, 0.0f);
        sprintf_s(s_strBuffer, "FPS: %.0f", round(m_fps));
        RenderText(5.0f, HEIGHT - 20.0f, s_strBuffer);
        sprintf_s(s_strBuffer, "RAM: %zu bytes", s_allocatedMemory);
        RenderText(5.0f, HEIGHT - 5.0f, s_strBuffer);
    }

    if (m_debugMode.IsEnabled(DebugOption::SHOW_QUADTREE))
    {
        glColor3f(0.0f, 1.0f, 1.0f);
        RenderQuadTree(m_collisionManager->GetRootQuadTree());
    }

    if (m_debugMode.IsEnabled(DebugOption::SHOW_AABB))
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        for (Entity* entity : m_entities)
        {
            RenderAABB(entity->GetAABB());
        }
    }

    if (m_debugMode.IsEnabled(DebugOption::SHOW_VELOCITY))
    {
        for (Entity* entity : m_entities)
        {
            RenderLine(
                entity->position.x, 
                entity->position.y,
                entity->position.x + entity->velocity.x,
                entity->position.y + entity->velocity.y 
            );
        }
    }
}
#endif // ENTROPY_DEBUG

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

static const float alpha = 0.95f;
void Sandbox::OnLoop()
{
    int now = glutGet(GLUT_ELAPSED_TIME);
    float elapsed = (float) (now - m_lastLoopTime) / CLOCKS_PER_SEC;

#ifdef ENTROPY_DEBUG
    if (!IsZero(elapsed))
    {
        m_fps = alpha * m_fps + (1.0f - alpha) * (1.0f / elapsed);
    }
#endif

    m_accumulatorTime += elapsed;
    m_lastLoopTime = now;

    if (m_accumulatorTime > MAX_ACCUMULATOR_TIME)
    {
        m_accumulatorTime = MAX_ACCUMULATOR_TIME;
    }

    while (m_accumulatorTime >= DELTA_TIME)
    {
        Update(DELTA_TIME);
        m_accumulatorTime -= DELTA_TIME;
    }

    Repaint();
}

void Sandbox::OnIdle()
{
    glutPostRedisplay();
}

void Sandbox::OnVisible(int visibility)
{
    if (visibility == GLUT_VISIBLE)
    {
        glutIdleFunc(&OnIdleWrapper);
    }
    else
    {
        glutIdleFunc(nullptr);
    }
}

void Sandbox::OnMouse(int button, int state, int x, int y)
{
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_UP)
        {
            entity::Circle* circle = ENTROPY_NEW(entity::Circle, (float) x, (float) y, (float) Rand(10, 40));
            circle->AddComponent(ENTROPY_NEW(RigidbodyComponent, circle, MATERIAL_ROCK));
            circle->AddComponent(ENTROPY_NEW(GravityComponent, circle));
            AddEntity(circle);
        }
        break;
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

#ifdef ENTROPY_DEBUG
void Sandbox::OnSpecialKeyboard(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_F1:
    {
        m_debugMode.Enable(DebugOption::PERFORMANCE_INFO);
        break;
    }
    case GLUT_KEY_F2:
    {
        m_debugMode.Enable(DebugOption::SHOW_QUADTREE);
        break;
    }
    case GLUT_KEY_F3:
    {
        m_debugMode.Enable(DebugOption::SHOW_AABB);
        break;
    }
    case GLUT_KEY_F4:
        m_debugMode.Enable(DebugOption::SHOW_VELOCITY);
        break;
    }
}
#endif // ENTROPY_DEBUG

/* 
 * Static functions which are passed to Glut function callbacks 
 */

void Sandbox::OnLoopWrapper()
{
    instance->OnLoop();
}

void Sandbox::OnIdleWrapper()
{
    instance->OnIdle();
}

void Sandbox::OnVisibleWrapper(int visibility)
{
    instance->OnVisible(visibility);
}

void Sandbox::OnMouseWrapper(int button, int state, int x, int y)
{
    instance->OnMouse(button, state, x, y);
}

void Sandbox::OnKeyboardWrapper(unsigned char key, int x, int y)
{
    instance->OnKeyboard(key, x, y);
}

#ifdef ENTROPY_DEBUG
void Sandbox::OnSpecialKeyboardWrapper(int key, int x, int y)
{
    instance->OnSpecialKeyboard(key, x, y);
}
#endif // ENTROPY_DEBUG
