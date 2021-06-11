#include "Precompiled.h"

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Entropy");

	Sandbox sandbox;

	const int wallSize = 25;
	entity::Rectangle* floor = ENTROPY_NEW(entity::Rectangle, WIDTH / 2.0f, HEIGHT - wallSize / 2.0f, WIDTH - wallSize * 2.0f, wallSize);
	floor->AddComponent(ENTROPY_NEW(RigidbodyComponent, floor, MATERIAL_STATIC));
	sandbox.AddEntity(floor);

	entity::Rectangle* leftWall = ENTROPY_NEW(entity::Rectangle, wallSize / 2.0f, HEIGHT / 2.0f, wallSize, HEIGHT);
	leftWall->AddComponent(ENTROPY_NEW(RigidbodyComponent, leftWall, MATERIAL_STATIC));
	sandbox.AddEntity(leftWall);
	
	entity::Rectangle* rightWall = ENTROPY_NEW(entity::Rectangle, WIDTH - wallSize / 2.0f, HEIGHT / 2.0f, wallSize, HEIGHT);
	rightWall->AddComponent(ENTROPY_NEW(RigidbodyComponent, rightWall, MATERIAL_STATIC));
	sandbox.AddEntity(rightWall);

	sandbox.Start();
	
	glutMainLoop();

	return EXIT_SUCCESS;
}
