#include "pch.h"
#include "UiHud.h"
#include "PlayerBody.h"

UiHud::UiHud(const std::string& name)
{
}

void UiHud::SetResolution(const sf::Vector2f resolution)
{
}

void UiHud::SetScore(int s)
{
}

void UiHud::SetMessage(const std::string& msg)
{
	textMessage.SetString(msg);
	textMessage.SetOrigin(Origins::MC);
}

void UiHud::SetMessageActive(bool active)
{
	if (active)
		textMessage.SetActive(true);
	else
		textMessage.SetActive(false);
}

void UiHud::SetHpBar(int hp, int max)
{
	float value = (float)hp / max;
	hpBar.SetScale({ hpBarSize.x * value, hpBarSize.y });
}

void UiHud::SetAirBar(int air, int maxair)
{
	float value = (float)air / maxair;

	// 1.0�϶� 0, 0.0�϶� 180
	float r = (1.f - value) * 180.0f;

	airBarColor.SetRotation(r);

	if (air > 60.f)
	{
		airBarColor.SetTexture(airBarBlue);
	}
	if (air < 60.f && air > 40.f)
	{
		airBarColor.SetTexture(airBarYellow);
	}
	if (air < 40.f)
	{
		airBarColor.SetTexture(airBarRed);
	}
}

void UiHud::SetInventory(sf::Vector2i& count, sf::Vector2f& size)
{
	cellCount = count;
	cellSize = size;

	invenVa.clear();
	invenVa.setPrimitiveType(sf::Quads);
	invenVa.resize(count.x * count.y * 4);

	posOffset[0] = { 0, 0 };
	posOffset[1] = { size.x, 0 };
	posOffset[2] = { size.x, size.y };
	posOffset[3] = { 0, size.y };

	texCoord0[0] = { 0, 0 };
	texCoord0[1] = { 42.f, 0 };
	texCoord0[2] = { 42.f, 42.f };
	texCoord0[3] = { 0, 42.f };


	invenMap =
	{
	    1,1,1,1,1,1,
		1,1,1,1,1,1,
		1,1,1,1,1,1,
		1,1,1,1,1,1,
		1,1,1,1,1,1,
		1,1,1,1,1,1,
	};

	for (int i = 0; i < count.y; ++i)
	{
		for (int j = 0; j < count.x; ++j)
		{

			int texIndexX = invenMap[i * count.x + j];
			int texIndexY = 0;

			int quadIndex = i * count.x + j;
			sf::Vector2f quadPos(size.x * j, size.y * i);

			for (int k = 0; k < 4; k++)
			{
				int vertexIndex = ((quadIndex * 4) + k);
				invenVa[vertexIndex].position = quadPos + posOffset[k];
				invenVa[vertexIndex].texCoords = texCoord0[k];

				invenVa[vertexIndex].texCoords.x += texIndexX * 42.f;
				invenVa[vertexIndex].texCoords.y += texIndexY * 42.f;
			}

		}
	}
}

void UiHud::SetInventoryActive()
{
	
}


void UiHud::Init()
{
	hud.SetTexture("graphics/ui/FSADIGBOY19-412.png");

	energy.SetTexture("graphics/ui/FSADIGBOY19-411.png");
	hpBar.SetTexture("graphics/ui/FSADIGBOY19-413.png");

	airText.SetTexture("graphics/ui/FSADIGBOY19-378.png");
	airBarColor.SetTexture("graphics/ui/FSADIGBOY19-375.png");
	airBarFrame1.SetTexture("graphics/ui/FSADIGBOY19-372.png"); //���� ���
	airBarFrame2.SetTexture("graphics/ui/FSADIGBOY19-377.png"); //����

	inventory.SetTexture("graphics/ui/FSADIGBOY19-319.png");
	invenSprite.SetTexture("graphics/ui/FSADIGBOY19-322.png");
	invenEmpty.SetTexture("graphics/ui/FSADIGBOY19-305.png");
	invenBlocked.SetTexture("graphics/ui/FSADIGBOY19-321.png");

	quickMenu.SetTexture("graphics/ui/FSADIGBOY19-470.png");
	quickItem1.SetTexture("graphics/shop/FSADIGBOY19-48.png");
	quickItem2.SetTexture("graphics/shop/FSADIGBOY19-47.png");
	quickItem3.SetTexture("graphics/shop/FSADIGBOY19-51.png");
}

void UiHud::Release()
{
}

void UiHud::Reset()
{
	sf::Vector2f windowSize = (sf::Vector2f)FRAMEWORK.GetWindowSize();

	player = dynamic_cast<PlayerBody*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));

	//ü�°� ��Ұ�����
	SetHpBar(player->GetHP(), player->GetMaxHP());
	SetAirBar(player->GetAir(), player->GetMaxAir());

	hud.SetOrigin(Origins::TR);
	hud.SetPosition({ windowSize.x + 10.f, -10.f });
	//hud.SetPosition({ 1280.f, 0.f });

	energy.SetOrigin(Origins::TR);
	energy.SetPosition({ windowSize.x * 0.945f, windowSize.y * 0.28f });

	hpBar.SetOrigin(Origins::TR);
	hpBar.SetPosition({ 1242.f, 160.f });
	hpBar.SetPosition({ windowSize.x * 0.955f, windowSize.y * 0.315f });

	airText.SetOrigin(Origins::TR);
	airText.SetPosition({ 1223.f, 170.f });
	airText.SetPosition({ windowSize.x * 0.926f, windowSize.y * 0.34f });

	airBarFrame1.SetOrigin(Origins::TR);
	airBarFrame1.SetPosition({ windowSize.x * 0.975f, windowSize.y * 0.34f });

	airBarColor.SetOrigin(Origins::TC);
	airBarColor.SetPosition({ 1198.f, 184.f });
	airBarColor.SetPosition({ windowSize.x * 0.887f, windowSize.y * 0.36f });

	airBarFrame2.SetOrigin(Origins::TR);
	airBarFrame2.SetPosition({ 1253.f, 173.f });
	airBarFrame2.SetPosition({ windowSize.x * 0.975f, windowSize.y * 0.34f });

	textMessage.SetW(fontK, L"�ѱ�����׽�Ʈ", 40, sf::Color::White);
	textMessage.SetPosition({ 0.f, 0.f });

	//�κ��丮
	inventory.SetOrigin(Origins::TL);
	inventory.SetPosition(invenClose);

	quickMenu.SetOrigin(Origins::TC);
	quickMenu.SetPosition({ windowSize.x * 0.5f, 0.f });

	quickItem1.SetPosition({ windowSize.x * 0.336f, 3.f});
	quickItem2.SetPosition({ windowSize.x * 0.394f, 3.f });
	quickItem3.SetPosition({ windowSize.x * 0.446f, 3.f });
}

void UiHud::Update(float dt)
{
	SetHpBar(player->GetHP(), player->GetMaxHP());
	SetAirBar(player->GetAir(), player->GetMaxAir());

	if (InputMgr::GetKeyDown(sf::Keyboard::I))
	{
		if (inventory.GetPosition() == invenClose)
		{
			inventory.SetPosition(invenOpen);

		}
		else if (inventory.GetPosition() == invenOpen)
		{
			inventory.SetPosition(invenClose);
		}
		
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Num1))
	{
		player->UseItem(1);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Num2))
	{
		player->UseItem(2);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Num3))
	{
		player->UseItem(3);
	}
}

void UiHud::Draw(sf::RenderWindow& window)
{
	airBarFrame1.Draw(window);
	airBarColor.Draw(window);

	airBarFrame2.Draw(window);

	hud.Draw(window);

	airText.Draw(window);

	hpBar.Draw(window);
	energy.Draw(window);

	inventory.Draw(window);

	textMessage.Draw(window);

	quickMenu.Draw(window);
	quickItem1.Draw(window);
	quickItem2.Draw(window);
	quickItem3.Draw(window);
}