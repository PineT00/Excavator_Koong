#include "pch.h"
#include "TileMap.h"



TileMap::TileMap(const std::string& name)
    :GameObject(name)
{
}

sf::FloatRect TileMap::GetLocalBounds()
{
    sf::FloatRect bounds = va.getBounds();
    bounds.left = origin.x;
    bounds.top = origin.y;
    return bounds;
}

sf::FloatRect TileMap::GetGlobalBounds()
{
    sf::FloatRect bounds = va.getBounds();
    return transform.transformRect(bounds);
}

void TileMap::SetLevel(const std::string& filename)
{
    std::ifstream file(filename);
    std::string line;

    //int value = std::stoi(line);

    if (file.is_open())
    {
        while (getline(file, line))
        {
            std::istringstream iss(line);
            int value;

            while (iss >> value) //iss의 값을 value로 옮기는데 성공하면
            {
                level.push_back(value);

                if (iss.peek() == ',') //다음 읽을 문자 확인
                    iss.ignore();
                else
                    break;
            }
        }
        file.close();
    }
    else
    {
        std::cout << "파일을 열 수 없습니다." << std::endl;
    }
}


void TileMap::Set(sf::Vector2i& count, sf::Vector2f& size)
{
    cellCount = count;
    cellSize = size;

    va.clear();
    va.setPrimitiveType(sf::Quads);
    va.resize(count.x * count.y * 4);

    posOffset[0] = { 0, 0 };
    posOffset[1] = { size.x, 0 };
    posOffset[2] = { size.x, size.y };
    posOffset[3] = { 0, size.y };

    texCoord0[0] = { 0, 0 };
    texCoord0[1] = { 42.f, 0 };
    texCoord0[2] = { 42.f, 42.f };
    texCoord0[3] = { 0, 42.f };


    //level =
    //{
    //    70,70,1,1,1,5,5,1,1,1,5,1,1,1,2,2,1,1,70,70,
    //    70,70,1,1,1,5,5,1,1,1,5,1,1,1,1,2,1,1,70,70,
    //    70,70,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,70,70,
    //    70,70,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,70,70,
    //    70,70,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,70,70,
    //    70,70,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,70,70,
    //    70,70,1,1,1,5,5,1,1,1,5,1,1,1,2,2,1,1,70,70,
    //    70,70,1,1,1,5,5,1,1,1,5,1,2,1,2,1,1,1,70,70,
    //    70,70,1,1,1,5,5,1,1,1,5,2,2,2,2,1,1,1,70,70,
    //    70,70,1,1,1,5,5,1,1,1,5,1,1,1,1,1,1,1,70,70,
    //};

    SetLevel("tables/MapTable.csv");

    for (int i = 0; i < count.y; ++i)
    {
        for (int j = 0; j < count.x; ++j)
        {

            int texIndexY = level[i * count.x + j];
            int texIndexX = 0;

            if (texIndexY > 5)
            {
                if(texIndexY % 5 == 0)
                {
                    texIndexX = (texIndexY / 5) - 1;
                    texIndexY = 5;
                }
                else
                {
                    texIndexX = texIndexY / 5;
                    texIndexY = texIndexY % 5;
                }
            }

            int quadIndex = i * count.x + j;
            sf::Vector2f quadPos(size.x * j, size.y * i);

            for (int k = 0; k < 4; k++)
            {
                int vertexIndex = ((quadIndex * 4) + k);
                va[vertexIndex].position = quadPos + posOffset[k];
                va[vertexIndex].texCoords = texCoord0[k] ;

                va[vertexIndex].texCoords.x += texIndexX * 42.f;
                va[vertexIndex].texCoords.y += texIndexY * 42.f;
            }
        }
    }



}

sf::FloatRect TileMap::GetTileBound(int x, int y)
{
    sf::FloatRect bound;
    bound.left = position.x + cellSize.x * x;
    bound.top = position.y + cellSize.y * y;
    bound.width = cellSize.x;
    bound.height = cellSize.y;
    return bound;
}

void TileMap::SetSpriteSheetId(const std::string& id)
{
    spriteSheetId = id;
    texture = &RES_MGR_TEXTURE.Get(spriteSheetId);
}



void TileMap::UpdateTransform()
{
    transform = sf::Transform::Identity;
    float scaleX = isFlipX ? -scale.x : scale.x;
    float scaleY = isFlipY ? -scale.y : scale.y;
    transform.scale(scaleX, scaleY, position.x, position.y);
    transform.rotate(rotation, position.x, position.y);
    transform.translate(position - origin);
}

void TileMap::SetOrigin(Origins preset)
{
    if (preset == Origins::Custom)
        return;

    originPreset = preset;
    sf::FloatRect bound = va.getBounds();
    origin.x = bound.width * ((int)originPreset % 3) * 0.5f;
    origin.y = bound.height * ((int)originPreset / 3) * 0.5f;
    UpdateTransform();

}

void TileMap::SetOrigin(const sf::Vector2f& newOrigin)
{
    originPreset = Origins::Custom;
    origin = newOrigin;
    UpdateTransform();
}

void TileMap::SetPosition(const sf::Vector2f& pos)
{

    GameObject::SetPosition(pos);
    transform.translate(position);
    UpdateTransform();
}

void TileMap::Translate(const sf::Vector2f& delta)
{
    GameObject::Translate(delta);
    transform.translate(position);
    UpdateTransform();
}

void TileMap::SetRotation(float r)
{
    GameObject::SetRotation(r);
    UpdateTransform();
}

void TileMap::SetScale(const sf::Vector2f& scale)
{
    GameObject::SetScale(scale);
    transform.scale(this->scale);
    UpdateTransform();
}

void TileMap::SetFlipX(bool flip)
{
    GameObject::SetFlipX(flip);
    UpdateTransform();
}

void TileMap::SetFlipY(bool flip)
{
    GameObject::SetFlipY(flip);
    UpdateTransform();
}

void TileMap::Init()
{
    GameObject::Init();
    SetSpriteSheetId("graphics/FSADIGBOY19-9.png");
    Set(cellCount, cellSize);
    //SetOrigin(Origins::MC);
}

void TileMap::Release()
{
    GameObject::Release();
}

void TileMap::Reset()
{
    GameObject::Reset();
}

void TileMap::Update(float dt)
{
    GameObject::Update(dt);
    for (int i = 0; i < cellCount.y; ++i)
    {
        for (int j = 0; j < cellCount.x; ++j)
        {
            int tileValue = level[i * cellCount.x + j];
        }
    }
}

void TileMap::Draw(sf::RenderWindow& window)
{
    //GameObject::Draw(window);
    sf::RenderStates state;
    state.texture = texture;
    state.transform = transform;

    window.draw(va, state);

    if (SCENE_MGR.GetDeveloperMode())
    {
        sf::FloatRect rect = GetGlobalBounds();
        sf::RectangleShape t;

        t.setSize({ cellSize.x, cellSize.y });
        t.setOutlineColor(sf::Color::Red);
        t.setOutlineThickness(1.f);
        t.setFillColor(sf::Color::Transparent);

        for (int i = 0; i < cellCount.y; ++i)
        {
            for (int j = 0; j < cellCount.x; ++j)
            {
                // 각 타일의 위치 계산
                sf::Vector2f tilePos(rect.left + j * cellSize.x, rect.top + i * cellSize.y);

                // 각 타일을 그릴 박스의 위치 및 크기 설정
                t.setPosition(tilePos);
                t.setSize({ cellSize.x, cellSize.y });

                int tileValue = level[i * cellCount.x + j];

                

                if (tileValue != 2 || 0)
                {
                    window.draw(t);
                }

            }
        }
    }
}

