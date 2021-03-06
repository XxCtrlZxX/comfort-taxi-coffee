#include "stdafx.h"
#include "Arrow.h"
#include "InputManager.h"
#include "GameManager.h"

Arrow::Arrow(AnimationInfo* animInfo) 
	: GameObject(animInfo), state(eArrow::Right), posVec(Vector2(0, -1))
{
	this->animRenderer = animInfo;

	for (int i = 0; i < 4; i++)
		arrowShape.push_back(new ListAnimation(i, 1.0f));	// state : 0~3, 초당 1프레임
	
	arrowShape[0]->PushBackSprite(L"resources/image/arrow_R.png");
	arrowShape[1]->PushBackSprite(L"resources/image/arrow_D.png");
	arrowShape[2]->PushBackSprite(L"resources/image/arrow_RD.png");
	arrowShape[3]->PushBackSprite(L"resources/image/arrow_RU.png");

	for (int i = 0; i < 4; i++) {
		arrowShape[i]->SetAnimateOnce(true);
		animRenderer->PushBackAnimation(arrowShape[i]);
	}

	coins = GameManager::GetInstance()->coins;
}

Arrow::~Arrow()
{
}

void Arrow::Update()
{
	GameManager::GetInstance()->Update();

	Move();
	ChangeSprite();

	if (InputManager::GetKeyDown(VK_SPACE)) {
		SwitchCoins();
		GameManager::GetInstance()->TurnUpdate();
	}
}

void Arrow::Move()
{
	if (InputManager::GetKeyDown(VK_UP) || InputManager::GetKeyDown('W'))
	{
		GameManager::GetInstance()->AddScore(-1);
		posVec.y += posVec.y < 0 ? 1 : 0;
	}
	if (InputManager::GetKeyDown(VK_DOWN) || InputManager::GetKeyDown('S'))
	{
		GameManager::GetInstance()->AddScore(-1);
		posVec.x = 0;
		posVec.y -= posVec.y > limitPos.y ? 1 : 0;
	}
	if (InputManager::GetKeyDown(VK_RIGHT) || InputManager::GetKeyDown('D'))
	{
		GameManager::GetInstance()->AddScore(-1);
		posVec.y = 0;
		posVec.x += posVec.x < limitPos.x ? 1 : 0;
	}
	if (InputManager::GetKeyDown(VK_LEFT) || InputManager::GetKeyDown('A'))
	{
		GameManager::GetInstance()->AddScore(-1);
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
	GameManager::GetInstance()->AddScore(-5);

	std::list<int> num = CreateList();
	std::list<int>::iterator num_iter = num.begin();
	
	for (int i = 0; i < num.size(); i++) {
		coins[*num_iter]->FlipWithDelay(0.1f * i);
		num_iter++;
	}

	
	// 게임이 클리어 됐는지 확인해주는 마지막 코인
	if (!GameManager::GetInstance()->isHintTime) {
		coins[num.back()]->SetLastCoin();

		std::cout << "마지막 코인 " << num.back() << std::endl;
	}
}

std::list<int> Arrow::CreateList()
{
	std::list<int> num;

	switch (state)
	{
	case eArrow::Right:	// ex) 0, 1, 2
		for (int i = (-posVec.y - 1) * 3; i < -posVec.y * 3; i++)
			num.push_back(i);
		break;
	case eArrow::Down:	// ex) 0, 3, 6
		for (int i = posVec.x - 1; i < coins.size(); i += 3)
			num.push_back(i);
		break;
	case eArrow::RD:	// ex) 0, 4, 8
		for (int i = 0; i < coins.size(); i += 4)
			num.push_back(i);
		break;
	case eArrow::RU:	// ex) 6, 4, 2
		for (int i = 6; i > 0; i -= 2)
			num.push_back(i);
		break;
	default:
		break;
	}
	return num;
}
