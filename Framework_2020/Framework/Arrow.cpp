#include "stdafx.h"
#include "Arrow.h"
#include "InputManager.h"

Arrow::Arrow(AnimationInfo* animInfo) 
	: GameObject(animInfo), state(eArrow::Right), posVec(Vector2(0, -1))
{
	this->animRenderer = animInfo;

	for (int i = 0; i < 4; i++)
		arrowShape.push_back(new ListAnimation(i, 1.0f));	// state : 0~3, �ʴ� 1������
	
	arrowShape[0]->PushBackSprite(L"resources/image/arrow_R.png");
	arrowShape[1]->PushBackSprite(L"resources/image/arrow_D.png");
	arrowShape[2]->PushBackSprite(L"resources/image/arrow_RD.png");
	arrowShape[3]->PushBackSprite(L"resources/image/arrow_RU.png");

	for (int i = 0; i < 4; i++) {
		arrowShape[i]->SetAnimateOnce(true);
		animRenderer->PushBackAnimation(arrowShape[i]);
	}
}

Arrow::~Arrow()
{
}

void Arrow::Update()
{
	Move();
	ChangeSprite();

	if (InputManager::GetKeyDown(VK_SPACE))
		SwitchCoins();
}

void Arrow::Move()
{
	if (InputManager::GetKeyDown(VK_UP))
	{
		posVec.y += posVec.y < 0 ? 1 : 0;
	}
	if (InputManager::GetKeyDown(VK_DOWN))
	{
		posVec.x = 0;
		posVec.y -= posVec.y > limitPos.y ? 1 : 0;
	}
	if (InputManager::GetKeyDown(VK_RIGHT))
	{
		posVec.y = 0;
		posVec.x += posVec.x < limitPos.x ? 1 : 0;
	}
	if (InputManager::GetKeyDown(VK_LEFT))
	{
		posVec.x -= posVec.x > 0 ? 1 : 0;
	}

	transform->SetPosition(posVec * 75 + offset);
}

void Arrow::ChangeSprite()
{
	// Set arrow state
	if (posVec.x == 0) {
		if (posVec.y == 0)
			state = eArrow::RD;
		else if (posVec.y == limitPos.y)
			state = eArrow::RU;
		else
			state = eArrow::Right;
	}
	else {
		state = eArrow::Down;
	}

	animRenderer->ChangeAnimation((int)state);
	/*switch (state)
	{
	case eArrow::Right:
		animRenderer->ChangeAnimation(0);
		break;
	case eArrow::Down:
		animRenderer->ChangeAnimation(1);
		break;
	case eArrow::RD:
		animRenderer->ChangeAnimation(2);
		break;
	case eArrow::RU:
		animRenderer->ChangeAnimation(3);
		break;
	default:
		break;
	}*/
}

void Arrow::SwitchCoins()
{
	switch (state)
	{
	case eArrow::Right:
		for (int i = (-posVec.y - 1) * 3; i < -posVec.y * 3; i++)
			coins[i]->SwitchCoin();

		std::cout << "Right " << posVec.y << std::endl;
		break;
	case eArrow::Down:
		for (int i = posVec.x - 1; i < coins.size(); i += 3)
			coins[i]->SwitchCoin();

		std::cout << "Down " << posVec.x << std::endl;
		break;
	case eArrow::RD:
		for (int i = 0; i < coins.size(); i += 4)
			coins[i]->SwitchCoin();
		std::cout << "RD " << posVec.y << std::endl;
		break;
	case eArrow::RU:
		for (int i = 6; i > 0; i -= 2)
			coins[i]->SwitchCoin();
		std::cout << "RU " << posVec.y << std::endl;
		break;
	default:
		break;
	}
}

void Arrow::PutCoins(std::vector<Coin*> c)
{
	coins = c;
}