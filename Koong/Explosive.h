#pragma once
#include "SpriteGo.h"

class Monster;

class Explosive : public SpriteGo
{

protected:
    sf::Sprite bomb;
    Animator animatorBomb;

    sf::Sprite Dynamite;
    Animator animatorDynamite;

    float bombTime = 0.f;
    float bombTimer = 2.f;

public:
    Monster* monster;

    Explosive(const std::string& name = "");
    ~Explosive() = default;

    void SetBomb(sf::Vector2i pos);
    void SetNuke(sf::Vector2i pos);
    void SetDynamite(sf::Vector2i pos);

    void Init() override;
    void Reset() override;
    void Update(float dt) override;
    void Draw(sf::RenderWindow& window) override;


};

