#pragma once

#include <vector>

#ifdef ENTROPY_DEBUG
class DebugMode;
#endif // ENTROPY_DEBUG

class CollisionManager;
class Entity;
class Sandbox
{
private:
	CollisionManager* m_CollisionManager;
	std::vector<Entity*> m_Entities;
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

public:
	static Sandbox* INSTANCE;

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
};

