#include <GL/glut.h>
#include "Sandbox.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Config.h"
#include "RigidBodyComponent.h"
#include "GravityComponent.h"
#include <stdlib.h>
#include <time.h>

#define RAND_INT(max) rand() % max + 1

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Entropy");

	Sandbox sandbox;

	const int wallSize = 25;
	entity::Rectangle* floor = new entity::Rectangle(WIDTH / 2.0f, HEIGHT - wallSize, WIDTH, wallSize);
	sandbox.AddEntity(floor);
	entity::Rectangle* leftWall = new entity::Rectangle(wallSize / 2.0f, HEIGHT / 2.0f, wallSize, HEIGHT);
	sandbox.AddEntity(leftWall);
	entity::Rectangle* rightWall = new entity::Rectangle(WIDTH - wallSize / 2.0f, HEIGHT / 2.0f, wallSize, HEIGHT);
	sandbox.AddEntity(rightWall);

	entity::Rectangle* rectangle = new entity::Rectangle(25.0f, 25.0f, 50.0f, 50.0f);
	rectangle->AddComponent(new RigidBodyComponent(rectangle, 1));
	rectangle->AddComponent(new GravityComponent(rectangle));
	sandbox.AddEntity(rectangle);

	srand(time(NULL));
	for (int i = 0; i < 100; ++i)
	{
		entity::Circle* circle = new entity::Circle(RAND_INT(WIDTH), RAND_INT(HEIGHT / 5.0f), 15.0f);
		circle->AddComponent(new RigidBodyComponent(circle, 1));
		circle->AddComponent(new GravityComponent(circle));
		sandbox.AddEntity(circle);
	}

	sandbox.Start();
	
	glutMainLoop();

	return EXIT_SUCCESS;
}
