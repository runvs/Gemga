#include "brick_droplet.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"

BrickDroplet::BrickDroplet(std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def)
    : BrickImpl(world, def)
{
}

void BrickDroplet::doCreate()
{
    m_drawable = std::make_shared<jt::Animation>();
    m_drawable->add("assets/gems/ruby.png", "idle", jt::Vector2u { 12, 16 }, { 0 }, 0.1f);
    m_drawable->add(
        "assets/gems/ruby.png", "shine", jt::Vector2u { 12, 16 }, { 0, 1, 2, 3, 4, 5, 6, 0 }, 0.1f);
    m_drawable->play("idle");
    m_drawable->setLooping(false);

    b2FixtureDef fixtureDef;
    fixtureDef.density = GP::PhysicsBrickDensity() * 2.5f;
    fixtureDef.friction = 1.1f;
    fixtureDef.restitution = GP::PhysicsBrickRestitution();

    b2PolygonShape boxCollider {};
    b2Vec2 vertices[8];
    vertices[0].Set(5.0f, 0.0f);
    vertices[1].Set(0.0f, 8.0f);
    vertices[2].Set(0.0f, 12.0f);
    vertices[3].Set(3.0f, 15.0f);
    vertices[4].Set(8.0f, 15.0f);
    vertices[5].Set(11.0f, 12.0f);
    vertices[6].Set(11.0f, 8.0f);
    vertices[7].Set(6.0f, 0.0f);
    int32 count = 8;
    boxCollider.Set(vertices, count);

    fixtureDef.shape = &boxCollider;
    getB2Body()->CreateFixture(&fixtureDef);
}
