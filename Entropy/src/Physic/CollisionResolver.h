#pragma once
#include "Entity/Entity.h"

struct Collision;

using CollisionCallback = void (*)(Collision& collision);

extern CollisionCallback Dispatch[static_cast<int>(EntityType::COUNT)][static_cast<int>(EntityType::COUNT)];

Collision Solve(Entity* entityA, Entity* entityB);
void CircleToCircle(Collision& collision);
void CircleToPolygon(Collision& collision);
void PolygonToCircle(Collision& collision);
void PolygonToPolygon(Collision& collision);

float FindAxisLeastPenetration(size_t* faceIdx, entity::Polygon* polygonA, entity::Polygon* polygonB);
void FindIncidentFace(Vector2 incidentFace[], entity::Polygon* polyRef, entity::Polygon* polyInc, size_t idxRef);
int Clip(Vector2 n, float c, Vector2 face[]);