#pragma once

#include <vector>

DEBUG(class DebugMode;)
class CollisionManager;
class Entity;
class Sandbox
{
private:
	CollisionManager* m_collisionManager;
	std::vector<Entity*> m_entities;
	bool m_updating;
	float m_fps;
	int m_lastLoopTime;
	float m_accumulatorTime;
	DEBUG(DebugMode m_debugMode;)

	DEBUG(void RepaintDebug() const;)

protected:
	static Sandbox* instance;

public:
	Sandbox();
	~Sandbox();

	std::vector<Entity*> GetEntities() const;

	void Start();
	void Stop();
	void Update(float deltaTime);
	void Repaint() const;
	void AddEntity(Entity* entity);
	void RemoveEntity(Entity* entity);

	void OnLoop();
	void OnIdle();
	void OnVisible(int visibility);
	void OnMouse(int button, int state, int x, int y);
	void OnKeyboard(unsigned char key, int x, int y);
	DEBUG(void OnSpecialKeyboard(int key, int x, int y);)

	/* Static functions which are passed to Glut function callbacks */
	static void OnLoopWrapper();
	static void OnIdleWrapper();
	static void OnVisibleWrapper(int visibility);
	static void OnMouseWrapper(int button, int state, int x, int y);
	static void OnKeyboardWrapper(unsigned char key, int x, int y);
	DEBUG(static void OnSpecialKeyboardWrapper(int key, int x, int y);)
};

