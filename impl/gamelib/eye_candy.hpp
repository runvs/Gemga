#ifndef GEMGA_EYE_CANDY_HPP
#define GEMGA_EYE_CANDY_HPP

#include "game_object.hpp"
#include <animation.hpp>

class EyeCandy : public jt::GameObject {
private:
    std::shared_ptr<jt::Animation> m_frog;
    std::shared_ptr<jt::Animation> m_miniBirds;
    std::shared_ptr<jt::Animation> m_singleBird1;
    std::shared_ptr<jt::Animation> m_singleBird2;
    std::shared_ptr<jt::Animation> m_trickyHeart;
    std::shared_ptr<jt::Animation> m_sparklyStar1;
    std::shared_ptr<jt::Animation> m_sparklyStar2;
    std::shared_ptr<jt::Animation> m_sparklyStar3;
    std::shared_ptr<jt::Sprite> m_shootingStar;

    virtual void doCreate();
    virtual void doUpdate(float const elapsed);
    virtual void doDraw() const;
};

#endif // GEMGA_EYE_CANDY_HPP
