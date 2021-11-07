﻿#include "state_menu.hpp"
#include "color.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "input_manager.hpp"
#include "key_codes.hpp"
#include "lerp.hpp"
#include "math_helper.hpp"
#include "shape.hpp"
#include "sprite.hpp"
#include "state_game.hpp"
#include "text.hpp"
#include "tweens/tween_alpha.hpp"
#include "tweens/tween_position.hpp"
#include "tweens/tween_scale.hpp"
#include <algorithm>

StateMenu::StateMenu() = default;

void StateMenu::doInternalCreate()
{
    createMenuText();
    createShapes();
    createBackground();
    createVignette();

    createTweens();

    getGame()->getMusicPlayer()->PlayMusic("assets/music/complete.ogg");
}

void StateMenu::createVignette()
{
    m_vignette = jt::dh::createVignette(GP::GetScreenSize());
    m_vignette->setColor({ 255, 255, 255, 110 });
}

void StateMenu::createShapes()
{
    m_overlay = jt::dh::createRectShape(GP::GetScreenSize(), jt::colors::Black);
}

void StateMenu::createBackground()
{
    m_backgroundSprites = {};
    std::vector<std::string> backgroundSpriteList = { "assets/menu_bg/01_background.png",
        "assets/menu_bg/02_trees_back.png", "assets/menu_bg/03_trees_mid.png",
        "assets/menu_bg/04_trees_front.png", "assets/menu_bg/05_bushes.png",
        "assets/menu_bg/06_grass.png", "assets/menu_bg/07_flies_dust.png" };

    for (auto backgroundSprite : backgroundSpriteList) {
        auto sprite = std::make_shared<jt::Sprite>();
        sprite->loadSprite(backgroundSprite);
        m_backgroundSprites.emplace_back(sprite);
    }
}

void StateMenu::createMenuText()
{
    createLogo();
    createTextExplanation();
    createTextCredits();
}

void StateMenu::createTextCredits()
{
    m_text_Credits = jt::dh::createText(getGame()->getRenderTarget(),
        "Created by " + GP::AuthorName() + "for " + GP::JamName()
            + "\nThis game uses OpenAl, licensed under\nLGPL v2. Please see https://openal.org",
        10U, GP::PaletteColor5());
    m_text_Credits->setTextAlign(jt::Text::TextAlign::LEFT);
    m_text_Credits->setPosition({ 5, GP::GetScreenSize().y() - 70 });
    m_text_Credits->setShadow(GP::PaletteFontShadow(), jt::Vector2 { 1, 1 });
}

void StateMenu::createTextExplanation()
{
    float half_width = GP::GetScreenSize().x() / 2;
    m_text_Explanation = jt::dh::createText(
        getGame()->getRenderTarget(), "Press Space to start the game", 12U, GP::PaletteColor3());
    m_text_Explanation->setPosition({ half_width, 150 });
    m_text_Explanation->setShadow(jt::colors::Black, jt::Vector2 { 1, 1 });
}

void StateMenu::createLogo()
{
    float half_width = GP::GetScreenSize().x() / 2;

    m_logo = std::make_shared<jt::Animation>();
    m_logo->add("assets/logo.png", "idle", jt::Vector2u { 125, 50 }, { 0, 1, 2, 3, 4, 5, 6 }, 0.2f);
    m_logo->play("idle");
    m_logo->setPosition({ half_width - 62.5f, 20 });


    m_logoGlow = std::make_shared<jt::Sprite>();
    m_logoGlow->loadSprite("assets/logo_blur.png");
    m_logoGlow->setPosition({ half_width - 71.5f, 10 });
}

void StateMenu::createTweens()
{
    createTweenOverlayAlpha();
    createTweenCreditsPosition();
    createTweenExplanationScale();
}

void StateMenu::createInstructionTweenScaleUp()
{
    auto ts = jt::TweenScale::create(
        m_text_Explanation, 0.75f, jt::Vector2 { 1.0f, 1.0f }, jt::Vector2 { 1.05f, 1.05f });
    ts->setAgePercentConversion([](float age) {
        return jt::Lerp::cosine(0.0f, 1.0f, jt::MathHelper::clamp(age, 0.0f, 1.0f));
    });
    ts->addCompleteCallback([this]() { createInstructionTweenScaleDown(); });
    add(ts);
}
void StateMenu::createInstructionTweenScaleDown()
{
    auto ts = jt::TweenScale::create(
        m_text_Explanation, 0.75f, jt::Vector2 { 1.05f, 1.05f }, jt::Vector2 { 1.0f, 1.0f });
    ts->setAgePercentConversion([](float age) {
        return jt::Lerp::cosine(0.0f, 1.0f, jt::MathHelper::clamp(age, 0.0f, 1.0f));
    });
    ts->addCompleteCallback([this]() { createInstructionTweenScaleUp(); });
    add(ts);
}

void StateMenu::createTweenExplanationScale()
{
    auto s2 = m_text_Explanation->getPosition() + jt::Vector2 { -1000, 0 };
    auto e2 = m_text_Explanation->getPosition();

    auto tween = jt::TweenPosition::create(m_text_Explanation, 0.5f, s2, e2);
    tween->setStartDelay(0.3f);
    tween->setSkipFrames();

    tween->addCompleteCallback([this]() { createInstructionTweenScaleUp(); });
    add(tween);
}

void StateMenu::createTweenOverlayAlpha()
{
    auto tween = jt::TweenAlpha::create(m_overlay, 0.5f, std::uint8_t { 255 }, std::uint8_t { 0 });
    tween->setSkipFrames();
    add(tween);
}

void StateMenu::createTweenCreditsPosition()
{
    auto s3 = m_text_Credits->getPosition() + jt::Vector2 { 0, 100 };
    auto e3 = m_text_Credits->getPosition();

    auto tween = jt::TweenPosition::create(m_text_Credits, 0.35f, s3, e3);
    tween->setStartDelay(0.8f);
    tween->setSkipFrames();
    add(tween);
}

void StateMenu::doInternalUpdate(float const elapsed)
{
    updateDrawables(elapsed);
    checkForTransitionToStateGame();
}

void StateMenu::updateDrawables(const float& elapsed)
{
    for (int i = 0; i < m_backgroundSprites.size(); ++i) {
        const auto sprite = m_backgroundSprites.at(i);
        const jt::Vector2 currentPosition = sprite->getPosition();

        float newX = currentPosition.x() - 0.1f * i;
        newX = newX <= -240.0f ? newX + 240.0f : newX;
        sprite->setPosition(jt::Vector2 { newX, currentPosition.y() });
        sprite->update(elapsed);
    }

    m_logo->update(elapsed);
    m_logoGlow->update(elapsed);

    m_text_Explanation->update(elapsed);
    m_text_Credits->update(elapsed);
    m_overlay->update(elapsed);
    m_vignette->update(elapsed);
}

void StateMenu::checkForTransitionToStateGame()
{
    auto const keysToTriggerTransition = { jt::KeyCode::Space, jt::KeyCode::Enter };

    if (std::any_of(keysToTriggerTransition.begin(), keysToTriggerTransition.end(),
            [this](auto const k) { return getGame()->input()->keyboard()->justPressed(k); })) {
        startTransitionToStateGame();
    }
}

void StateMenu::startTransitionToStateGame()
{
    if (!m_started) {
        m_started = true;

        createTweenTransition();
    }
}

void StateMenu::createTweenTransition()
{
    auto tw = jt::TweenAlpha::create(m_overlay, 0.5f, std::uint8_t { 0 }, std::uint8_t { 255 });
    tw->setSkipFrames();
    tw->addCompleteCallback([this]() { getGame()->switchState(std::make_shared<StateGame>()); });
    add(tw);
}

void StateMenu::doInternalDraw() const
{
    for (auto backgroundSprite : m_backgroundSprites) {
        backgroundSprite->draw(getGame()->getRenderTarget());
    }

    m_logoGlow->draw(getGame()->getRenderTarget());
    m_logo->draw(getGame()->getRenderTarget());

    m_text_Explanation->draw(getGame()->getRenderTarget());
    m_text_Credits->draw(getGame()->getRenderTarget());

    m_overlay->draw(getGame()->getRenderTarget());
    m_vignette->draw(getGame()->getRenderTarget());
}
