#include "Precompiled.h"

entity::Polygon::Polygon(float x, float y, float width, float height)
	: Entity(x, y)
    , m_vertices()
    , m_normals()
    , m_orientationMatrix(0)
{
    const float halfW = width / 2.0f;
    const float halfH = height / 2.0f;

    m_vertices.reserve(4);
    m_vertices.emplace_back(-halfW, -halfH);
    m_vertices.emplace_back(halfW, -halfH);
    m_vertices.emplace_back(halfW, halfH);
    m_vertices.emplace_back(-halfW, halfH);

    m_normals.reserve(4);
    m_normals.emplace_back(0.0f, -1.0f);
    m_normals.emplace_back(1.0f, 0.0f);
    m_normals.emplace_back(0.0f, 1.0f);
    m_normals.emplace_back(-1.0f, 0.0f);
}

entity::Polygon::Polygon(float x, float y, std::vector<Vector2>& vertices)
    : Entity(x, y)
    , m_vertices()
    , m_normals()
    , m_orientationMatrix(0)
{
    ENTROPY_ASSERT(vertices.size() > 2, "Number of polygon vertices cannot be less than 2");

    // Find the right most point on the hull
    size_t rightMostIdx = 0;
    float highestXCoord = vertices[0].x;
    for (size_t i = 1; i < vertices.size(); ++i)
    {
        const Vector2& vertex = vertices[i];
        if (vertex.x > highestXCoord)
        {
            highestXCoord = vertex.x;
            rightMostIdx = i;
        }
        // If matching x then take farthest negative y
        else if (x == highestXCoord && vertex.y < vertices[rightMostIdx].y)
        {
            rightMostIdx = i;
        }
    }

    size_t hull[24];
    size_t outCount = 0;
    size_t hullIdx = rightMostIdx;
    for (;;)
    {
        hull[outCount] = hullIdx;

        // Search for next index that wraps around the hull by computing cross products to find the
        // most counter-clockwise vertex in the set, given the previos hull index
        size_t nextHullIdx = 0;
        for (size_t i = 1; i < vertices.size(); ++i)
        {
            // Skip if same coordinate as we need three unique
            // points in the set to perform a cross product
            if (nextHullIdx == hullIdx)
            {
                nextHullIdx = i;
                continue;
            }

            // Cross every set of three unique vertices
            // Record each counter clockwise third vertex and add to the output hull
            const Vector2& e1 = vertices[nextHullIdx] - vertices[hull[outCount]];
            const Vector2& e2 = vertices[i] - vertices[hull[outCount]];
            float c = e1.Cross(e2);
            if (c < 0.0f)
            {
                nextHullIdx = i;
            }

            // Cross product is zero then e vectors are on same line
            // therefor want to record vertex farthest along that line
            if (c == 0.0f && e2.LengthSq() > e1.LengthSq())
            {
                nextHullIdx = i;
            }
        }

        ++outCount;
        hullIdx = nextHullIdx;

        // Conclude algorithm upon wrap-around
        if (nextHullIdx == rightMostIdx)
        {
            break;
        }
    }

    // Copy vertices into shape's vertices
    for (size_t i = 0; i < outCount; ++i)
    {
        m_vertices.push_back(vertices[hull[i]]);
    }

    // Compute face normals
    for (size_t i1 = 0; i1 < outCount; ++i1)
    {
        size_t i2 = (i1 + 1) % outCount;
        const Vector2& face = m_vertices[i2] - m_vertices[i1];

        ENTROPY_ASSERT(!IsZero(face.LengthSq()), "Zero-length edge");

        Vector2 normal = -face.Tangent();
        normal.Normalize();
        m_normals.push_back(normal);
    }
}

Matrix22 entity::Polygon::GetOrientationMatrix() const
{
    return m_orientationMatrix;
}

size_t entity::Polygon::GetVertexCount() const
{
    return m_vertices.size();
}

Vector2 entity::Polygon::GetVertex(size_t index) const
{
    ENTROPY_ASSERT(index < m_vertices.size(), "Vertex index " << index << "out of bound (" << m_vertices.size() << ")");
    return m_vertices[index];
}

Vector2 entity::Polygon::GetNormal(size_t index) const
{
    ENTROPY_ASSERT(index < m_normals.size(), "Normal index " << index << "out of bound (" << m_normals.size() << ")");
    return m_normals[index];
}

AABB entity::Polygon::GetAABB() const
{
    Vector2 start(FLT_MAX, FLT_MAX);
    Vector2 end(-FLT_MAX, -FLT_MAX);
    for (const Vector2& vertex : m_vertices) {
        const Vector2& vector = position + m_orientationMatrix * vertex;
        if (vector.x < start.x)
        {
            start.x = vector.x;
        }
        if (vector.y < start.y)
        {
            start.y = vector.y;
        }
        if (vector.x > end.x)
        {
            end.x = vector.x;
        }
        if (vector.y > end.y)
        {
            end.y = vector.y;
        }
    }
	return AABB(start, end);
}

EntityType entity::Polygon::GetType() const
{
	return EntityType::POLYGON;
}

// The extreme point along a direction within a polygon
Vector2 entity::Polygon::GetSupport(const Vector2& dir) const
{
    float bestProjection = -FLT_MAX;
    Vector2 bestVertex;

    for (const Vector2& vertex : m_vertices)
    {
        const float projection = vertex.Dot(dir);
        if (projection > bestProjection)
        {
            bestVertex = vertex;
            bestProjection = projection;
        }
    }

    return bestVertex;
}

void entity::Polygon::Paint() const
{
    glBegin(GL_LINE_LOOP);
    for (const Vector2& vertex : m_vertices)
    {
        const Vector2& vector = position + m_orientationMatrix * vertex;
        glVertex2f(vector.x, vector.y);
    }
    glEnd();
}
