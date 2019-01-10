#pragma once

#include <Box2D.h>
#include <iostream>
#include <SFML/System.hpp>
#include "Window.h"
#include "Global.h"

class DebugDraw : public b2Draw
{
public:
   DebugDraw(Window &window);
   virtual ~DebugDraw();

   sf::Color B2COL2SF(const b2Color &color, sf::Uint8 alpha = 255);

   void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
   void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
   void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;
   void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;
   void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
   void DrawTransform(const b2Transform& xf) override;
   void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);// override;


private:
   Window *window;
};

