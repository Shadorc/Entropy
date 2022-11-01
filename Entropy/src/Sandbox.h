#pragma once

#include <vector>
#include "Physic/CollisionManager.h"
#include "Entity/Entity.h"

#ifdef ENTROPY_DEBUG
#include "Debug/DebugMode.h"
#endif // ENTROPY_DEBUG

class Sandbox
{
public:
	static Sandbox* INSTANCE;

	Sandbox();
	~Sandbox();

	const std::vector<std::unique_ptr<Entity>>& GetEntities() const;

	void Start();
	void Stop();
	void Update(float deltaTime);
	void Repaint() const;
	void AddEntity(std::unique_ptr<Entity> entity);
	bool RemoveEntity(const uint id);

	void OnLoop();
	void OnIdle();
	void OnVisible(int visibility);
	void OnMouse(int button, int state, int x, int y);
	void OnKeyboard(unsigned char key, int x, int y);
#ifdef ENTROPY_DEBUG
	void OnSpecialKeyboard(int key, int x, int y);
#endif // ENTROPY_DEBUG

	/* Static functions which are passed to Glut function callbacks */
	static void OnLoopWrapper();
	static void OnIdleWrapper();
	static void OnVisibleWrapper(int visibility);
	static void OnMouseWrapper(int button, int state, int x, int y);
	static void OnKeyboardWrapper(unsigned char key, int x, int y);
#ifdef ENTROPY_DEBUG
	static void OnSpecialKeyboardWrapper(int key, int x, int y);
#endif // ENTROPY_DEBUG

private:
	CollisionManager m_CollisionManager;
	std::vector<std::unique_ptr<Entity>> m_Entities;
	bool m_Updating;
	float m_Fps;
	int m_LastLoopTime;
	float m_AccumulatorTime;
#ifdef ENTROPY_DEBUG
	DebugMode m_DebugMode;
#endif // ENTROPY_DEBUG

#ifdef ENTROPY_DEBUG
	void RepaintDebug() const;
#endif // ENTROPY_DEBUG
};

