#pragma once

#include <vector>

class CollisionManager;
class Entity;
class Sandbox
{
private:
	CollisionManager* m_collisionManager;
	std::vector<Entity*> m_entities;
	bool m_updating;
	float m_targetFps;
	float m_dt;
	float m_fps;
	int m_lastLoopTime;
	float m_accumulatorTime;
#ifdef _DEBUG
	int m_debugMask;
#endif // _DEBUG

#ifdef _DEBUG
	void RepaintDebug() const;
#endif // _DEBUG

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
#ifdef _DEBUG
	void OnSpecialKeyboard(int key, int x, int y);
#endif // _DEBUG

	/* Static functions which are passed to Glut function callbacks */
	static void OnLoopWrapper();
	static void OnIdleWrapper();
	static void OnVisibleWrapper(int visibility);
	static void OnMouseWrapper(int button, int state, int x, int y);
	static void OnKeyboardWrapper(unsigned char key, int x, int y);
#ifdef _DEBUG
	static void OnSpecialKeyboardWrapper(int key, int x, int y);
#endif // _DEBUG
};

