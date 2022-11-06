#include "Precompiled.h"

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Entropy");

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, WIDTH, HEIGHT, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	Sandbox sandbox;

	const float WALL_SIZE = 25.0f;
	auto floor = std::make_shared<entity::Polygon>(WIDTH / 2.0f, HEIGHT - WALL_SIZE / 2.0f, static_cast<float>(WIDTH), WALL_SIZE);
	floor->AddComponent<RigidbodyComponent>(floor, MATERIAL_STATIC, FRICTION_NORMAL);
	sandbox.AddEntity(floor);

	sandbox.Start();

	glutMainLoop();

	return EXIT_SUCCESS;
}
