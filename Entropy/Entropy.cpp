#include <stdlib.h>
#include <time.h>

#include "Precompiled.h"

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Entropy");

	Sandbox sandbox;

	const int wallSize = 25;
	entity::Rectangle* floor = new entity::Rectangle(WIDTH / 2.0f, HEIGHT - wallSize / 2.0f, WIDTH - wallSize * 2.0f, wallSize);
	floor->AddComponent(new RigidbodyComponent(floor, STATIC));
	sandbox.AddEntity(floor);

	entity::Rectangle* leftWall = new entity::Rectangle(wallSize / 2.0f, HEIGHT / 2.0f, wallSize, HEIGHT);
	leftWall->AddComponent(new RigidbodyComponent(leftWall, STATIC));
	sandbox.AddEntity(leftWall);
	
	entity::Rectangle* rightWall = new entity::Rectangle(WIDTH - wallSize / 2.0f, HEIGHT / 2.0f, wallSize, HEIGHT);
	rightWall->AddComponent(new RigidbodyComponent(rightWall, STATIC));
	sandbox.AddEntity(rightWall);

	sandbox.Start();
	
	glutMainLoop();

	return EXIT_SUCCESS;
}
