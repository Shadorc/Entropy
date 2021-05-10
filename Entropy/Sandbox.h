#pragma once

#include "Common.h"
#include <vector>

class Sandbox
{
private:
	std::vector<Entity*> m_entities;
	bool m_updating;
	float m_fps;
	int m_lastLoopTime;
	CollisionManager* m_collisionManager;

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

	/* Static functions which are passed to Glut function callbacks */
	static void OnIdleWrapper();
	static void OnVisibleWrapper(int visibility);
	static void OnLoopWrapper();
	static void OnKeyboardWrapper(unsigned char key, int x, int y);
};

