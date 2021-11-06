#include "brick_rectangle2x1.hpp"
#include "game_interface.hpp"
#include <drawable_helpers.hpp>
#include <math_helper.hpp>

BrickRectangle2x1::BrickRectangle2x1(
    std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def)
    : BrickInterface(world, def)
{
}

void BrickRectangle2x1::doCreate()
{
    m_shape = jt::dh::createRectShape(jt::Vector2 { 32, 16 }, jt::colors::Black);
    m_shape->setOrigin(jt::Vector2 { 16, 8 });
    b2FixtureDef fixtureDef;
    fixtureDef.density = 0.01f;
    fixtureDef.friction = 1.1f;
    fixtureDef.restitution = -0.1f;

    b2PolygonShape boxCollider {};
    boxCollider.SetAsBox(16, 8);
    fixtureDef.shape = &boxCollider;
    getB2Body()->CreateFixture(&fixtureDef);
}

void BrickRectangle2x1::doUpdate(float const elapsed)
{
    m_shape->setPosition(getPosition());
    m_shape->setRotation(-jt::MathHelper::rad2deg(getRotation()));
    m_shape->update(elapsed);
}
void BrickRectangle2x1::doDraw() const { m_shape->draw(getGame()->getRenderTarget()); }