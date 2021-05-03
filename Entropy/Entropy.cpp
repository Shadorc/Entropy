#include <GL/glut.h>
#include "Sandbox.h"
#include "Rectangle.h"
#include "Config.h"
#include "RigidBodyComponent.h"
#include "GravityComponent.h"

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Entropy");

	Sandbox sandbox;

	entity::Rectangle* rectangle = new entity::Rectangle(25.0f, 25.0f, 50.0f, 50.0f);
	rectangle->AddComponent(new RigidBodyComponent(rectangle, 1));
	rectangle->AddComponent(new GravityComponent(rectangle));
	sandbox.AddEntity(rectangle);

	sandbox.Start();
	
	glutMainLoop();

	return EXIT_SUCCESS;
}
