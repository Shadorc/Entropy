#pragma once

#include "precomp.h"
#include <vector>

class Sandbox
{
private:
	CollisionManager* m_collisionManager;
	std::vector<Entity*> m_entities;
	bool m_updating;
	float m_fps;
	int m_lastLoopTime;
#ifdef _DEBUG
	int m_debugMask;
#endif // _DEBUG

protected:
	static Sandbox* instance;

public:
	Sandbox();
	~Sandbox();

	std::vector<Entity*> GetEntities() const;

	void Start();
	void Stop();
	void Update(float delta);
	void Repaint() const;
	void AddEntity(Entity* entity);
	void RemoveEntity(Entity* entity);

	void OnIdle();
	void OnVisible(int visibility);
	void OnLoop();
	void OnKeyboard(unsigned char key, int x, int y);
#ifdef _DEBUG
	void OnSpecialKeyboard(int key, int x, int y);
#endif // _DEBUG

	/* Static functions which are passed to Glut function callbacks */
	static void OnIdleWrapper();
	static void OnVisibleWrapper(int visibility);
	static void OnLoopWrapper();
	static void OnKeyboardWrapper(unsigned char key, int x, int y);
#ifdef _DEBUG
	static void OnSpecialKeyboardWrapper(int key, int x, int y);
#endif // _DEBUG
};

