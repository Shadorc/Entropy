#include <ctime>

#include "Precompiled.h"

constexpr float MAX_ACCUMULATOR_TIME = 0.2f;
constexpr size_t DEFAULT_ENTITIES_CAPACITY= 100;

Sandbox* Sandbox::INSTANCE = nullptr;

Sandbox::Sandbox() 
	: m_CollisionManager(this)
	, m_Entities()
	, m_Updating(false)
	, m_Fps(0)
	, m_LastLoopTime(0)
	, m_AccumulatorTime(0)
#ifdef ENTROPY_DEBUG
	, m_DebugMode()
#endif // ENTROPY_DEBUG
{
	INSTANCE = this;
	m_Entities.reserve(DEFAULT_ENTITIES_CAPACITY);
}

Sandbox::~Sandbox()
{
	INSTANCE = nullptr;
}

void Sandbox::Start()
{
	m_Updating = true;
	m_LastLoopTime = glutGet(GLUT_ELAPSED_TIME);

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
	m_Updating = false;

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
	for (const auto& entity : m_Entities)
	{
		entity->Update(deltaTime);
	}

	m_CollisionManager.Update();
}

void Sandbox::Repaint() const
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0f, 1.0f, 0.0f);
	for (const auto& entity : m_Entities)
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
	"F1: Show commands",
	"F2: Show FPS",
	"F3: Show quadtree",
	"F4: Show AABB",
	"F5: Show velocity",
	"F6: Enable/disable physic steps",
	"Space: Next physic step",
	"Left click: Spawn circle",
	"Right click: Spawn polygon"
};
void Sandbox::RepaintDebug() const
{
	glColor3f(0.5f, 0.5f, 0.9f);
	if (m_DebugMode.IsEnabled(DebugOption::SHOW_COMMANDS))
	{
		for (int i = 0; i < texts.size(); ++i)
		{
			RenderText(5.0f, (i + 1) * 15.0f, texts[i]);
		}
	}
	else
	{
		RenderText(5.0f, 15.0f, texts[0]);
	}

	if (m_DebugMode.IsEnabled(DebugOption::PERFORMANCE_INFO))
	{
		glColor3f(0.5f, 0.5f, 0.9f);
		sprintf_s(s_strBuffer, "FPS: %.0f", round(m_Fps));
		RenderText(5.0f, HEIGHT - 20.0f, s_strBuffer);
	}

	if (m_DebugMode.IsEnabled(DebugOption::SHOW_QUADTREE))
	{
		glColor3f(0.0f, 1.0f, 1.0f);
		RenderQuadTree(m_CollisionManager.GetRootQuadTree());
	}

	if (m_DebugMode.IsEnabled(DebugOption::SHOW_AABB))
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		for (const auto& entity : m_Entities)
		{
			RenderAABB(entity->GetAABB());
		}
	}

	if (m_DebugMode.IsEnabled(DebugOption::SHOW_VELOCITY))
	{
		glColor3f(0.0f, 0.0f, 1.0f);
		for (const auto& entity : m_Entities)
		{
			RenderLine(
				entity->GetPosition().x,
				entity->GetPosition().y,
				entity->GetPosition().x + entity->velocity.x,
				entity->GetPosition().y + entity->velocity.y
			);
		}
	}
}
#endif // ENTROPY_DEBUG

const std::vector<std::unique_ptr<Entity>>& Sandbox::GetEntities() const
{
	return m_Entities;
}

void Sandbox::AddEntity(std::unique_ptr<Entity> entity)
{
	m_Entities.emplace_back(std::move(entity));
}

bool Sandbox::RemoveEntity(const uint id)
{
	for (const auto& entity : m_Entities)
	{
		if (entity->GetId() == id)
		{
			const auto& it = std::find(m_Entities.begin(), m_Entities.end(), entity);
			m_Entities.erase(it);
			return true;
		}
	}
	return false;

}

static const float alpha = 0.95f;
void Sandbox::OnLoop()
{
	int now = glutGet(GLUT_ELAPSED_TIME);
	float elapsed = static_cast<float>(now - m_LastLoopTime) / CLOCKS_PER_SEC;

#ifdef ENTROPY_DEBUG
	if (!IsZero(elapsed))
	{
		m_Fps = alpha * m_Fps + (1.0f - alpha) * (1.0f / elapsed);
	}
#endif

	m_AccumulatorTime += elapsed;
	m_LastLoopTime = now;

	if (m_AccumulatorTime > MAX_ACCUMULATOR_TIME)
	{
		m_AccumulatorTime = MAX_ACCUMULATOR_TIME;
	}

	while (m_AccumulatorTime >= DELTA_TIME)
	{
#ifdef ENTROPY_DEBUG
		if (m_DebugMode.ShouldContinuePhysicStep())
#endif
		{
			Update(DELTA_TIME);
		}
		m_AccumulatorTime -= DELTA_TIME;
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
			std::unique_ptr<Entity> circle = std::make_unique<entity::Circle>(static_cast<float>(x), static_cast<float>(y), static_cast<float>(Rand(10, 40)));
			circle->AddComponent(std::make_unique<RigidbodyComponent>(circle.get()));
			circle->AddComponent(std::make_unique<GravityComponent>(circle.get()));
			AddEntity(std::move(circle));
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_UP)
		{
			std::vector<Vector2> vertices;
			const int size = Rand(50, 100);
			const uint vertexCount = Rand(3, 25);
			for (uint i = 0; i < vertexCount; ++i)
			{
				vertices.emplace_back(Rand(-size, size), Rand(-size, size));
			}
			std::unique_ptr<Entity> polygon = std::make_unique<entity::Polygon>(static_cast<float>(x), static_cast<float>(y), vertices);
			polygon->AddComponent(std::make_unique<RigidbodyComponent>(polygon.get()));
			polygon->AddComponent(std::make_unique<GravityComponent>(polygon.get()));
			AddEntity(std::move(polygon));
		}
		break;
	}
}

void Sandbox::OnKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
#ifdef ENTROPY_DEBUG
	case ' ':
	{
		m_DebugMode.ContinuePhysicStep();
		break;
	}
#endif // ENTROPY_DEBUG
	// Exit on escape key press
	case '\x1B':
	{
		exit(EXIT_SUCCESS);
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
		m_DebugMode.Toggle(DebugOption::SHOW_COMMANDS);
		break;
	}
	case GLUT_KEY_F2:
	{
		m_DebugMode.Toggle(DebugOption::PERFORMANCE_INFO);
		break;
	}
	case GLUT_KEY_F3:
	{
		m_DebugMode.Toggle(DebugOption::SHOW_QUADTREE);
		break;
	}
	case GLUT_KEY_F4:
	{
		m_DebugMode.Toggle(DebugOption::SHOW_AABB);
		break;
	}
	case GLUT_KEY_F5:
	{
		m_DebugMode.Toggle(DebugOption::SHOW_VELOCITY);
		break;
	}
	case GLUT_KEY_F6:
	{
		m_DebugMode.TogglePhysicStep();
		break;
	}
	}
}
#endif // ENTROPY_DEBUG

/*
 * Static functions which are passed to Glut function callbacks
 */

void Sandbox::OnLoopWrapper()
{
	INSTANCE->OnLoop();
}

void Sandbox::OnIdleWrapper()
{
	INSTANCE->OnIdle();
}

void Sandbox::OnVisibleWrapper(int visibility)
{
	INSTANCE->OnVisible(visibility);
}

void Sandbox::OnMouseWrapper(int button, int state, int x, int y)
{
	INSTANCE->OnMouse(button, state, x, y);
}

void Sandbox::OnKeyboardWrapper(unsigned char key, int x, int y)
{
	INSTANCE->OnKeyboard(key, x, y);
}

#ifdef ENTROPY_DEBUG
void Sandbox::OnSpecialKeyboardWrapper(int key, int x, int y)
{
	INSTANCE->OnSpecialKeyboard(key, x, y);
}
#endif // ENTROPY_DEBUG
