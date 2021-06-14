#pragma once
#include "Entity.h"

struct Collision;

typedef void (*CollisionCallback)(Collision& collision);

extern CollisionCallback Dispatch[(int) EntityType::COUNT][(int) EntityType::COUNT];

Collision Solve(Entity* entityA, Entity* entityB);
void CircleToCircle(Collision& collision);
void CircleToPolygon(Collision& collision);
void PolygonToCircle(Collision& collision);
void PolygonToPolygon(Collision& collision);

float FindAxisLeastPenetration(unsigned int* faceIndex, entity::Polygon* polygonA, entity::Polygon* polygonB);
void FindIncidentFace(Vector2 incidentFace[], entity::Polygon* polyRef, entity::Polygon* polyInc, unsigned int indexRef);
int Clip(Vector2 n, float c, Vector2 face[]);