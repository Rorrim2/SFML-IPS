#include "DebugDraw.h"

DebugDraw::DebugDraw(Window &window)
{
   this->window = &window;
}

DebugDraw::~DebugDraw()
{

}

sf::Color DebugDraw::B2COL2SF(const b2Color & color, sf::Uint8 alpha)
{
   return sf::Color(color.r * 255, color.g * 255, color.b * 255, alpha );
}

void DebugDraw::DrawPolygon(const b2Vec2 * vertices, int32 vertexCount, const b2Color & color)
{
   sf::ConvexShape polygon;
   polygon.setPointCount(vertexCount);
   for (int32 i = 0; i < vertexCount; i++)
   {
      b2Vec2 vertex = vertices[i];
      polygon.setPoint(i, sf::Vector2f(vertex.x * PIXELS_PER_METER, vertex.y * PIXELS_PER_METER));// this->B2SFColor(color, 50), this->B2SFColor(color, 255));
   }
   polygon.setFillColor(sf::Color::Transparent);
   polygon.setOutlineColor(B2COL2SF(color));
   polygon.setOutlineThickness(1.0f);
   //sf::RectangleShape 
   this->window->draw(polygon);
}

void DebugDraw::DrawSolidPolygon(const b2Vec2 * vertices, int32 vertexCount, const b2Color & color)
{
   sf::ConvexShape polygon;
   polygon.setPointCount(vertexCount);
   for (int32 i = 0; i < vertexCount; i++)
   {
      b2Vec2 vertex = vertices[i];
      polygon.setPoint(i, sf::Vector2f(vertex.x * PIXELS_PER_METER, vertex.y * PIXELS_PER_METER));// this->B2SFColor(color, 50), this->B2SFColor(color, 255));
   }
   polygon.setFillColor(sf::Color::Transparent);
   polygon.setOutlineColor(B2COL2SF(color));
   polygon.setOutlineThickness(1.0f);
   //sf::RectangleShape 
   this->window->draw(polygon);
}

void DebugDraw::DrawCircle(const b2Vec2 & center, float32 radius, const b2Color & color)
{
   sf::CircleShape circle;
   circle.setRadius(radius * PIXELS_PER_METER);
   circle.setPosition(center.x * PIXELS_PER_METER, center.y * PIXELS_PER_METER);
   circle.setFillColor(sf::Color::Transparent);
   circle.setOutlineColor(B2COL2SF(color));
   circle.setOutlineThickness(1.0f);
   this->window->draw(circle);
}

void DebugDraw::DrawSolidCircle(const b2Vec2 & center, float32 radius, const b2Vec2 & axis, const b2Color & color)
{
   sf::CircleShape circle;
   circle.setRadius(radius * PIXELS_PER_METER);
   circle.setPosition(center.x * PIXELS_PER_METER, center.y * PIXELS_PER_METER);
   circle.setFillColor(sf::Color::Transparent);
   circle.setOutlineColor(B2COL2SF(color));
   circle.setOutlineThickness(1.0f);

   b2Vec2 pos = center + (radius * axis);
   sf::Vertex line[] =
   {
      sf::Vertex(sf::Vector2f(center.x * PIXELS_PER_METER, center.y * PIXELS_PER_METER)),
      sf::Vertex(sf::Vector2f(pos.x * PIXELS_PER_METER, pos.y * PIXELS_PER_METER))
   };

   this->window->draw(circle);
   this->window->draw(line, 2, sf::Lines);
}

void DebugDraw::DrawSegment(const b2Vec2 & p1, const b2Vec2 & p2, const b2Color & color)
{
}

void DebugDraw::DrawTransform(const b2Transform & xf)
{
}

void DebugDraw::DrawPoint(const b2Vec2 & p, float32 size, const b2Color & color)
{
   sf::RectangleShape rec;
   rec.setPosition(p.x * PIXELS_PER_METER, p.y * PIXELS_PER_METER);
   //rec.setSize(sf::Vector2f(size * PIXELS_PER_METER, size * PIXELS_PER_METER));
   sf::Color color2(28, 38, 234);
   rec.setSize(sf::Vector2f(3,3));
   rec.setFillColor(color2);
   //rec.setFillColor(sf::Color::Transparent);
   rec.setOutlineColor(color2);
   //rec.setOutlineColor(B2COL2SF(color));
   rec.setOutlineThickness(1.0f);
   this->window->draw(rec);
}
