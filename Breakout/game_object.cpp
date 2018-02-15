#include "game_object.h"

GameObject::GameObject()
	: Position(0, 0), Size(1, 1), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite() {}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 posIdx, glm::vec3 color, glm::vec2 velocity)
	: Position(pos), Size(size), Velocity(velocity), PosIdx(posIdx), Color(color), Rotation(0.0f), Sprite(sprite) {}

void GameObject::Draw(SpriteRenderer &renderer)
{
	renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}