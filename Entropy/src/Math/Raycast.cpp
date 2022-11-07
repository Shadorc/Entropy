#include "Precompiled.h"

// Source: https://stackoverflow.com/questions/14307158/how-do-you-check-for-intersection-between-a-line-segment-and-a-line-ray-emanatin
Vector2 Raycast(const Vector2& origin, const Vector2& direction, const std::vector<Edge>& edges)
{
	ENTROPY_ASSERT_WITH_REASON(Equal(direction.LengthSq(), 1.0f), "Direction should be normalized");

	float t1Min = FLT_MAX;
	for (const Edge& edge : edges)
	{
		const Vector2& ray = origin - edge.start;
		const Vector2& face = edge.end - edge.start;
		const Vector2& tangent = direction.Tangent();

		float dot = face.Dot(tangent);
		if (!IsZero(dot))
		{
			float t1 = face.Cross(ray) / dot;
			float t2 = ray.Dot(tangent) / dot;

			if (t1 >= 0.0f && t2 >= 0.0f && t2 <= 1.0f)
			{
				// Check if this intersect point is closest to source. 
				// If it is, then store this point and reject others.
				if (t1 < t1Min)
				{
					t1Min = t1;
				}
			}
		}
	}

	return t1Min == FLT_MAX ? Vector2::ZERO : (origin + direction * t1Min);
}
