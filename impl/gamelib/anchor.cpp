#include "anchor.hpp"
#include "conversions.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"

using namespace jt::Conversion;

Anchor::Anchor(std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def)
    : Box2DObject(world, def)
{
}

void Anchor::doCreate()
{
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1000.0f;
    fixtureDef.friction = 0.99f;
    fixtureDef.restitution = 0.0f;

    b2PolygonShape boxCollider {};
    fixtureDef.shape = &boxCollider;
    getB2Body()->CreateFixture(&fixtureDef);
}

void Anchor::doUpdate(float const elapsed)
{
    getB2Body()->SetLinearVelocity(vec(jt::Vector2 { 0.0f, 0.0f }));
    auto kbd = getGame()->input()->keyboard();
    if (kbd->pressed(jt::KeyCode::A) || kbd->pressed(jt::KeyCode::Left)) {
        getB2Body()->SetLinearVelocity(vec(jt::Vector2 { -GP::PlatformMovementSpeed(), 0.0f }));
    } else if (kbd->pressed(jt::KeyCode::D) || kbd->pressed(jt::KeyCode::Right)) {
        getB2Body()->SetLinearVelocity(vec(jt::Vector2 { GP::PlatformMovementSpeed(), 0.0f }));
    }
}

void Anchor::doDraw() const { }
void Anchor::doKill() { }
void Anchor::doDestroy() { }
