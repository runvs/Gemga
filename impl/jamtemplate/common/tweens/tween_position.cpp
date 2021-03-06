#include "tween_position.hpp"
#include "linterp.hpp"

namespace jt {

std::shared_ptr<Tween> TweenPosition::create(
    std::weak_ptr<DrawableInterface> obj, float time, jt::Vector2 valueStart, jt::Vector2 valueEnd)
{
    return std::make_shared<TweenPosition>(obj, time, valueStart, valueEnd);
}

TweenPosition::TweenPosition(
    std::weak_ptr<DrawableInterface> obj, float time, jt::Vector2 valueStart, jt::Vector2 valueEnd)
    : Tween { obj, time }
    , m_initialValue { valueStart }
    , m_finalValue { valueEnd }
{
}

void TweenPosition::doUpdateObject(
    std::shared_ptr<DrawableInterface> const& sptr, float agePercent) const
{
    auto pos = sptr->getPosition();

    pos.x() = Lerp::linear(
        static_cast<float>(m_initialValue.x()), static_cast<float>(m_finalValue.x()), agePercent);
    pos.y() = Lerp::linear(
        static_cast<float>(m_initialValue.y()), static_cast<float>(m_finalValue.y()), agePercent);

    sptr->setPosition(pos);
}
} // namespace jt
