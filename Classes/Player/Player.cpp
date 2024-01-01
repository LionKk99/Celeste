#include "Player.h"
#include"Trap/jumpTable.h"
#include"Trap/brick.h"
USING_NS_CC;
const float Player::DASH_DURATION = 0.5f;//�����ʱ�䣬������ҡ
 
int Player::currentLevel = 1;//��ʼ��ȫ�ֱ���currentLevelΪ1
Player::Player()
    : keyStates{
        {PlayerKey::LEFT, false},
        {PlayerKey::RIGHT, false},
        {PlayerKey::UP, false},
        {PlayerKey::DOWN, false},
        {PlayerKey::JUMP, false},
        {PlayerKey::TALK, false},
        {PlayerKey::DASH, false},
        {PlayerKey::CLIMB, false}
    }
{
    
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //��ʼ��������λ��
   
    
}

Player* Player::create(const std::string& filename) {
    Player* player = new (std::nothrow) Player();
    if (player && player->init(filename)) {
        player->autorelease();
        return player;
    }
    CC_SAFE_DELETE(player);
    return nullptr;
}

bool Player::init(const std::string& filename) {//��ʼ����ɫ
    if (!Sprite::initWithFile(filename)) {
        return false;
    }
    //currentState = PlayerState::IDLE;
    //previousState = PlayerState::IDLE;    
    isDashing = false;
    dashTimer = 0.0f;
    //��ʼ����������
    _debugDrawNode = cocos2d::DrawNode::create();
    this->addChild(_debugDrawNode);


    this->setName("player");        

    float scale_factor = 0.8f;  // �������ӣ������Ը�����Ҫ����
    this->setScale(scale_factor);

    

    // ʹ��lambda�������schedule_selector��ÿ֡����update����
    this->schedule([this](float delta) {
        this->update(delta);
        }, "update_key");

    // ������ҵ�������
    auto playerBodySize = this->getContentSize() * 0.5;  // ������������
    auto playerShape = PhysicsShapeBox::create(playerBodySize);
    playerShape->setFriction(1.0f);  // ����Ħ��ϵ��
    playerShape->setRestitution(0.0f);  // ���÷���ϵ��Ϊ0��ȷ��û���غ�
    auto playerBody = PhysicsBody::create();
    playerBody->addShape(playerShape);
    playerBody->setDynamic(true);
    playerBody->setRotationEnable(false);  // ��ֹ��ת
    playerBody->setLinearDamping(0.5f); // ����ˮƽ����
    // ��������������ײ������ȷ���ص�����������
    playerBody->setCategoryBitmask(0x02);
    playerBody->setCollisionBitmask(0x01);  // ���������Ϊ0x01�����巢����ײ
    playerBody->setContactTestBitmask(0xFFFFFFFF);
    
    this->setPhysicsBody(playerBody);   
   
    return true;
}

/*
bool Player::isOnSolidGround() {
    float rayLength = 1.0f; // ���߳��ȹ̶�Ϊ1����
    Vec2 centerPoint = this->getPosition();

    // ��ȡ��ɫ�Ĵ�С
    cocos2d::Size playerSize = this->getContentSize();

    // �������ߵ���ʼ��
    Vec2 leftStartPoint = centerPoint - Vec2(playerSize.width * 0.5f-42, playerSize.height * 0.5f - 47);
    Vec2 middleStartPoint = centerPoint - Vec2(0, playerSize.height * 0.5f - 47);
    Vec2 rightStartPoint = centerPoint + Vec2(playerSize.width * 0.5f-42, -playerSize.height * 0.5f + 47);

    std::vector<cocos2d::Vec2> startPoints = { leftStartPoint, middleStartPoint, rightStartPoint };

    bool onSolidGround = false; // Ĭ������¼�����Ҳ��ڵ�����

    // ��ȡ�򴴽�һ�� DrawNode
    auto drawNode = dynamic_cast<cocos2d::DrawNode*>(this->getChildByName("RayDrawNode"));
    if (!drawNode) {
        drawNode = cocos2d::DrawNode::create();
        drawNode->setName("RayDrawNode");
        this->addChild(drawNode);
    }
    drawNode->clear();  // ���֮ǰ�Ļ���

    for (const auto& startPoint : startPoints) {
        cocos2d::Vec2 endPoint = startPoint - cocos2d::Vec2(0, rayLength); // ��startPoint�����ӳ�1����

        // ��������
        drawNode->drawLine(startPoint, endPoint, cocos2d::Color4F::BLUE);

        auto func = [&onSolidGround](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data) -> bool {
            if (info.shape->getBody()->getNode()->getName() == "ground") {
                onSolidGround = true;
                return false; // ֹͣ���߼��
            }
            return true; // �������߼��
            };

        Director::getInstance()->getRunningScene()->getPhysicsWorld()->rayCast(func, startPoint, endPoint, nullptr);

        if (onSolidGround) {
            break; // �����⵽����ڵ����ϣ�����ѭ��
        }
    }

    return onSolidGround;
}
*/

bool Player::isOnSolidGround() {//�Ż����ж�
    float rayLength = 1.0f; // ���߳��ȹ̶�Ϊ1����
    Vec2 centerPoint = this->getPosition();

    // ��ȡ��ɫ�Ĵ�С
    cocos2d::Size playerSize = this->getContentSize();

    // �������ߵ���ʼ��
    Vec2 leftStartPoint = centerPoint - Vec2(playerSize.width * 0.5f - 42, playerSize.height * 0.5f - 47);
    Vec2 middleStartPoint = centerPoint - Vec2(0, playerSize.height * 0.5f - 47);
    Vec2 rightStartPoint = centerPoint + Vec2(playerSize.width * 0.5f - 42, -playerSize.height * 0.5f + 47);

    std::vector<cocos2d::Vec2> startPoints = { leftStartPoint, middleStartPoint, rightStartPoint };

    bool onSolidGround = false; // Ĭ������¼�����Ҳ��ڵ�����

    for (const auto& startPoint : startPoints) {
        cocos2d::Vec2 endPoint = startPoint - cocos2d::Vec2(0, rayLength); // ��startPoint�����ӳ�1����

        auto func = [&onSolidGround](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data) -> bool {
            if (info.shape->getBody()->getNode()->getName() == "ground"|| info.shape->getBody()->getNode()->getName() == "brick") {
                onSolidGround = true;
                return false; // ֹͣ���߼��
            }
            return true; // �������߼��
            };

        Director::getInstance()->getRunningScene()->getPhysicsWorld()->rayCast(func, startPoint, endPoint, nullptr);

        if (onSolidGround) {
            break; // �����⵽����ڵ����ϣ�����ѭ��
        }
    }

    return onSolidGround;
}


void Player::changeState(PlayerState newState) {
    if (currentState == newState) return;
    previousState = currentState;
    currentState = newState;
    //��Ƶ����
    if (previousState== PlayerState::MOVING_RIGHT|| previousState == PlayerState::MOVING_LEFT) {
        cocos2d::AudioEngine::stop(_walkMusicId);
    }
    //״̬����
    switch (currentState) {
    case PlayerState::CROUCH:
        playCrouchAnimation();    
        break;  
    case PlayerState::LOOKUP:
        playLookUpAnimation();
        break;
    case PlayerState::IDLE:
        if (previousState == PlayerState::CROUCH)
        {
            playCrouchToIdleAnimation();//���ȶ������ڶ���������ɺ����״̬��            
        }
        else {
            playIdleAnimation_1();//Ĭ��״̬
        }        
        break;
    case PlayerState::MOVING_LEFT:
        playMoveAnimation();
        this->setScaleX(-1.0f); // ���񶯻��Ա�ʾ����
        facingDirection = -1;//�������
        break;
    case PlayerState::MOVING_RIGHT:
        playMoveAnimation();
        this->setScaleX(1.0f);  // ����������ʾ����
        facingDirection = 1;//�����ұ�
        break;
    case PlayerState::MOVING_TURN_RL:
        playMoveTurnAnimation();
        this->setScaleX(-1.0f); // ���񶯻��Ա�ʾ��������
        break;
    case PlayerState::MOVING_TURN_LR:
        playMoveTurnAnimation();
        this->setScaleX(1.0f);  // ����������ʾ����������
        break;                    
    case PlayerState::JUMPING:
        if (canDash) {
            playJumpUpAnimation();
        }
        else {
            playBJumpUpAnimation();
        }        
        break;
    case PlayerState::DROP:
        if (canDash) {
            playDropAnimation();
        }
        else {
            playBDropAnimation();
        }        
        break;
    case PlayerState::PUSHWALL:
        playPushWallAnimation();        
        break;
    case PlayerState::LANDING:
        playLandingAnimation();     
        break;
    case PlayerState::HOLDWALL:
        if (canDash) {
            playHoldWallAnimation();
        }
        else {
            playBHoldWallAnimation();
        }        
        break;
    case PlayerState::HOLDWALLUP:
        if (canDash) {
            playHoldWallUpAnimation();
        }
        else {
            playBHoldWallUpAnimation();
        }        
        break;
    case PlayerState::HOLDWALLDOWN:
        if (canDash) {
            playHoldWallDownAnimation();
        }
        else {
            playBHoldWallDownAnimation();
        }        
        break;
    case PlayerState::HOLDWALLJUMP:
        if (canDash) {
            playHoldWallJumpAnimation();
        }
        else {
            playBHoldWallJumpAnimation();
        }        
        break;
    case PlayerState::DASH:
        this->setScaleX(facingDirection);
        if (getDashDirection() == cocos2d::Vec2(0, 1) || getDashDirection() == cocos2d::Vec2(0, -1))
        {
            playDashUpAndDownAnimation();
        }
        else {
            playDashAnimation();

        }      
        break;
    case PlayerState::DYING:
        if (canDash) {
            playDeathAnimation();
        }
        else {
            playBDeathAnimation();
        }        
        break;
        // ��������״̬...
    }
}

//ԭʼ����
/*

cocos2d::Vec2 Player::adjustMovePosition(const cocos2d::Vec2& desiredPosition) {//�˺��������Ż�λ�����÷�ֹ��ģ��Ҳ�����ж�canClimb
    cocos2d::Vec2 adjustedPosition = desiredPosition;
    float rayLength = 40.0f;

    std::vector<cocos2d::Vec2> directions = {
        cocos2d::Vec2(-1, 0),
        cocos2d::Vec2(1, 0),
        cocos2d::Vec2(0, 1)
    };

    cocos2d::Vec2 centerPoint = this->getPosition();

    for (const auto& dir : directions) {
        cocos2d::Vec2 endPoint = centerPoint + dir * rayLength;

        bool collisionDetected = false;
        std::string collidedObjectName = "";  // Store the name of the collided object

        auto rayCallback = [&collisionDetected, &collidedObjectName](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data) -> bool {
            if (info.shape->getBody()->getNode()->getName() != "player") {
                collisionDetected = true;
                collidedObjectName = info.shape->getBody()->getNode()->getName();
                return false;
            }
            return true;
            };

        Director::getInstance()->getRunningScene()->getPhysicsWorld()->rayCast(rayCallback, centerPoint, endPoint, nullptr);

        if (collisionDetected) {
            if (dir == cocos2d::Vec2(-1, 0) && desiredPosition.x < this->getPositionX()) {
                adjustedPosition.x = this->getPositionX();
                if (facingDirection == -1 && collidedObjectName == "ground") {
                    canClimb = 1;
                }
            }
            else if (dir == cocos2d::Vec2(1, 0) && desiredPosition.x > this->getPositionX()) {
                adjustedPosition.x = this->getPositionX();
                if (facingDirection == 1 && collidedObjectName == "ground") {
                    canClimb = 1;
                }
            }
            else if (dir == cocos2d::Vec2(0, 1)) {
                adjustedPosition.y = this->getPositionY();
            }
        }
        else {
            if ((dir == cocos2d::Vec2(-1, 0) && facingDirection == -1) || (dir == cocos2d::Vec2(1, 0) && facingDirection == 1)) {
                canClimb = 0;
            }
        }
    }
    CCLOG("canClimb: %d", canClimb);
    return adjustedPosition;
}

*/


cocos2d::Vec2 Player::adjustMovePosition(const cocos2d::Vec2& desiredPosition) {//�˺��������Ż�λ�����÷�ֹ��ģ��Ҳ�����ж�canClimb
    cocos2d::Vec2 adjustedPosition = desiredPosition;                           //�����֮ǰ�ĵ����ߣ�����λ�ã����Ż���˫���ߣ����ϣ����£�
    float rayLength = 40.0f;

    std::vector<cocos2d::Vec2> directions = {
        cocos2d::Vec2(-1, 0),
        cocos2d::Vec2(1, 0),
        cocos2d::Vec2(0, 1)
    };
    // ���ĵ�
    cocos2d::Vec2 centerPoint = this->getPosition();

    // ��ȡ��ɫ�Ĵ�С
    cocos2d::Size playerSize = this->getContentSize();

    // �������ߵ����
    cocos2d::Vec2 bottomCenterPoint = centerPoint - cocos2d::Vec2(0, playerSize.height * 0.5f) + cocos2d::Vec2(0, 49.0f);
    cocos2d::Vec2 topCenterPoint = centerPoint + cocos2d::Vec2(0, playerSize.height * 0.5f) - cocos2d::Vec2(0, 49.0f);

    // ��������ʼ����ӵ�һ���б���
    std::vector<cocos2d::Vec2> startPoints = { bottomCenterPoint, centerPoint, topCenterPoint };

    for (const auto& dir : directions) {
        bool collisionDetected = false;
        std::string collidedObjectName = "";

        for (const auto& startPoint : startPoints) {
            if (dir.x != 0 && startPoint == centerPoint) continue; // Ϊ���ĵ�������ֱ���������

            cocos2d::Vec2 endPoint = startPoint + dir * rayLength;

            auto rayCallback = [&collisionDetected, &collidedObjectName](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data) -> bool {
                if (info.shape->getBody()->getNode()->getName() != "player") {
                    collisionDetected = true;
                    collidedObjectName = info.shape->getBody()->getNode()->getName();
                    return false;
                }
                return true;
                };

            Director::getInstance()->getRunningScene()->getPhysicsWorld()->rayCast(rayCallback, startPoint, endPoint, nullptr);

            // �����⵽��ײ�������ڲ�ѭ��
            if (collisionDetected) break;
        }

        if (collisionDetected) {
            if (dir == cocos2d::Vec2(-1, 0) && desiredPosition.x < this->getPositionX()) {
                adjustedPosition.x = this->getPositionX();
                if (facingDirection == -1 && collidedObjectName == "ground") {
                    canClimb = 1;
                }
            }
            else if (dir == cocos2d::Vec2(1, 0) && desiredPosition.x > this->getPositionX()) {
                adjustedPosition.x = this->getPositionX();
                if (facingDirection == 1 && collidedObjectName == "ground") {
                    canClimb = 1;
                }
            }
            else if (dir == cocos2d::Vec2(0, 1)) {
                adjustedPosition.y = this->getPositionY();
            }
        }
        else {
            if ((dir == cocos2d::Vec2(-1, 0) && facingDirection == -1) || (dir == cocos2d::Vec2(1, 0) && facingDirection == 1)) {
                canClimb = 0;
            }
        }
    }

    //CCLOG("canClimb: %d", canClimb);
    return adjustedPosition;
}


bool Player::checkForSpikeweedCollision() {
    float rayLength = 40.0f;  // The same ray length as in adjustMovePosition
    std::vector<cocos2d::Vec2> directions = {
        cocos2d::Vec2(-1, 0),  // Left
        cocos2d::Vec2(1, 0),   // Right
        cocos2d::Vec2(0, -1)   // Bottom
    };

    cocos2d::Vec2 centerPoint = this->getPosition();
    cocos2d::Size playerSize = this->getContentSize();
    cocos2d::Vec2 bottomCenterPoint = centerPoint - cocos2d::Vec2(0, playerSize.height * 0.5f) + cocos2d::Vec2(0, 49.0f);
    cocos2d::Vec2 topCenterPoint = centerPoint + cocos2d::Vec2(0, playerSize.height * 0.5f) - cocos2d::Vec2(0, 49.0f);
    std::vector<cocos2d::Vec2> startPoints = { bottomCenterPoint, centerPoint, topCenterPoint };

    for (const auto& dir : directions) {
        for (const auto& startPoint : startPoints) {
            if (dir.x != 0 && startPoint == centerPoint) continue; // Skip vertical rays for the center point

            cocos2d::Vec2 endPoint = startPoint + dir * rayLength;
            bool collisionDetected = false;

            auto rayCallback = [this, &collisionDetected](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data) -> bool {
                if (info.shape->getBody()->getNode()->getName() == "Spikeweed") {
                    collisionDetected = true;
                    return false;  // Stop detecting further as we found Spikeweed
                }
                return true;
                };

            Director::getInstance()->getRunningScene()->getPhysicsWorld()->rayCast(rayCallback, startPoint, endPoint, nullptr);

            if (collisionDetected) {
                this->changeState(PlayerState::DYING);  // Change state to DYING if Spikeweed is detected
                return true;  // Return true if collision with Spikeweed is detected
            }
        }
    }
    return false;  // Return false if no Spikeweed is detected
}

bool Player::checkForJumpTableInteraction() {
    float rayLength = 40.0f; // ��������߳��ȣ����Ը�����Ҫ���е���

    // ֻ�����·�����
    cocos2d::Vec2 direction = cocos2d::Vec2(0, -1); // Down

    // ��ȡ��ɫ�����ĵ�͵ײ����ĵ�
    cocos2d::Vec2 centerPoint = this->getPosition();
    cocos2d::Size playerSize = this->getContentSize();
    cocos2d::Vec2 bottomCenterPoint = centerPoint - cocos2d::Vec2(0, playerSize.height * 0.5f);

    // ���ߵĽ�����
    cocos2d::Vec2 endPoint = bottomCenterPoint + direction * rayLength;
    bool collisionDetected = false;

    auto rayCallback = [this, &collisionDetected](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data) -> bool {
        if (info.shape->getBody()->getNode()->getName() == "JumpTable") {
            collisionDetected = true;
            // ��ȡ JumpTable ���󲢵������Ĳ��Ŷ�������
            auto jumpTable = dynamic_cast<JumpTable*>(info.shape->getBody()->getNode());
            if (jumpTable) {
                if(!jumpTable->_canBeActivated){ return false; }
                jumpTable->playAnimation(); // ���� JumpTable �� playAnimation ����
                jumpTable->deactivateTemporarily();//����ʱ����
                CCLOG("���ɶ�������");
             }
            return false;  // ֹͣ��һ�����
        }
        return true; // �������
        };

    Director::getInstance()->getRunningScene()->getPhysicsWorld()->rayCast(rayCallback, bottomCenterPoint, endPoint, nullptr);

    return collisionDetected; // �����Ƿ��⵽ JumpTable
}

bool Player::checkForBrickInteraction() {
    float rayLength = 20.0f; // ��������߳��ȣ����Ը�����Ҫ���е���

    // ֻ�����·�����
    cocos2d::Vec2 direction = cocos2d::Vec2(0, -1); // Down

    // ��ȡ��ɫ�����ĵ�͵ײ����ĵ�
    cocos2d::Vec2 centerPoint = this->getPosition();
    cocos2d::Size playerSize = this->getContentSize();
    cocos2d::Vec2 bottomCenterPoint = centerPoint - cocos2d::Vec2(0, playerSize.height * 0.5f);

    // ���ߵĽ�����
    cocos2d::Vec2 endPoint = bottomCenterPoint + direction * rayLength;
    bool collisionDetected = false;

    auto rayCallback = [this, &collisionDetected](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data) -> bool {
        if (info.shape->getBody()->getNode()->getName() == "brick") {
            collisionDetected = true;
            // ��ȡ brick ���󲢵������Ĳ��Ŷ�������
            auto brick = dynamic_cast<Brick*>(info.shape->getBody()->getNode());
            if (brick) {
                if (!brick->_isNormal) { return false; }
                brick->toggleVisibility();                
                CCLOG("ש������");
            }
            return false;  // ֹͣ��һ�����
        }
        return true; // �������
        };

    Director::getInstance()->getRunningScene()->getPhysicsWorld()->rayCast(rayCallback, bottomCenterPoint, endPoint, nullptr);

    return collisionDetected; // �����Ƿ��⵽ JumpTable
}

//����������
/*
cocos2d::Vec2 Player::adjustMovePosition(const cocos2d::Vec2& desiredPosition) {
    cocos2d::Vec2 adjustedPosition = desiredPosition;
    float rayLength = 40.0f;

    std::vector<cocos2d::Vec2> horizontalDirections = {
        cocos2d::Vec2(-1, 0),
        cocos2d::Vec2(1, 0)
    };

    cocos2d::Vec2 centerPoint = this->getPosition();
    cocos2d::Size playerSize = this->getContentSize();
    cocos2d::Vec2 bottomCenterPoint = centerPoint - cocos2d::Vec2(0, playerSize.height * 0.5f) + cocos2d::Vec2(0, 49.0f);
    cocos2d::Vec2 topCenterPoint = centerPoint + cocos2d::Vec2(0, playerSize.height * 0.5f) - cocos2d::Vec2(0, 49.0f);

    std::vector<cocos2d::Vec2> startPoints = {
        bottomCenterPoint,
        centerPoint,
        topCenterPoint
    };

    // Horizontal direction checks
    for (const auto& dir : horizontalDirections) {
        for (const auto& startPoint : startPoints) {
            cocos2d::Vec2 endPoint = startPoint + dir * rayLength;

            bool collisionDetected = false;
            std::string collidedObjectName = "";

            auto rayCallback = [&collisionDetected, &collidedObjectName](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data) -> bool {
                if (info.shape->getBody()->getNode()->getName() != "player") {
                    collisionDetected = true;
                    collidedObjectName = info.shape->getBody()->getNode()->getName();
                    return false;
                }
                return true;
                };

            Director::getInstance()->getRunningScene()->getPhysicsWorld()->rayCast(rayCallback, startPoint, endPoint, nullptr);

            if (collisionDetected) {
                if ((dir == cocos2d::Vec2(-1, 0) && desiredPosition.x < this->getPositionX()) ||
                    (dir == cocos2d::Vec2(1, 0) && desiredPosition.x > this->getPositionX())) {
                    adjustedPosition.x = this->getPositionX();
                    if (collidedObjectName == "ground") {
                        canClimb = 1;
                    }
                }
            }
        }
    }

    // Vertical direction check
    cocos2d::Vec2 verticalDir = cocos2d::Vec2(0, 1);
    cocos2d::Vec2 endPoint = centerPoint + verticalDir * rayLength;

    bool collisionDetected = false;
    std::string collidedObjectName = "";

    auto rayCallback = [&collisionDetected, &collidedObjectName](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data) -> bool {
        if (info.shape->getBody()->getNode()->getName() != "player") {
            collisionDetected = true;
            collidedObjectName = info.shape->getBody()->getNode()->getName();
            return false;
        }
        return true;
        };

    Director::getInstance()->getRunningScene()->getPhysicsWorld()->rayCast(rayCallback, centerPoint, endPoint, nullptr);

    if (collisionDetected && collidedObjectName != "player") {
        adjustedPosition.y = this->getPositionY();
    }

    CCLOG("canClimb: %d", canClimb);
    return adjustedPosition;
}
*/

void Player::update(float dt) {
    if (!isAlive) { return; }//��ɫ����ֱ�ӷ���
    if (isAlive) { if (checkForSpikeweedCollision()) {} }
    float deathThreshold = 75; // ����Ҫ��������ֵ
    if (this->getPositionY() < deathThreshold) {//����ɫ�߶�
        // ��������״̬
        isAlive = 0;
        changeState(PlayerState::DYING);
    }
    if (isDashing) {
        dashTimer += dt;
        if (dashTimer >= DASH_DURATION) {
            isDashing = false;
            dashTimer = 0.0f;
        }
        return;  // ���������ڳ�̣���������������״̬����
    }
   
    
   // CCLOG("canDash:%d",canDash);
    if ( previousState == PlayerState::DASH) {
        this->getPhysicsBody()->setGravityEnable(true);
    }
    if (canClimb == 0 && (currentState == PlayerState::HOLDWALL || currentState == PlayerState::HOLDWALLUP || currentState == PlayerState::HOLDWALLDOWN)&&currentState!= PlayerState::DASH) {
        this->getPhysicsBody()->setGravityEnable(true);
        velocity.y = -1;
        changeState(PlayerState::DROP);
    }
    
    if((previousState==PlayerState::HOLDWALL ||previousState == PlayerState::HOLDWALLUP || previousState == PlayerState::HOLDWALLDOWN)&&(currentState != PlayerState::HOLDWALL && currentState != PlayerState::HOLDWALLUP && currentState != PlayerState::HOLDWALLDOWN) && currentState != PlayerState::DASH)
    {
        velocity.y = -1;
        this->getPhysicsBody()->setGravityEnable(true);
    }

    float verticalVelocity = this->getPhysicsBody()->getVelocity().y;//���������е�vy
    float horizontalVelocity = this->getPhysicsBody()->getVelocity().x;//���������е�vx

    
    //�·���
    Vec2 desiredPosition = this->getPosition() + velocity * dt;
    Vec2 adjustedPosition = adjustMovePosition(desiredPosition);

    this->setPosition(adjustedPosition);
    //�ɷ���
    //this->setPositionX(this->getPositionX() + velocity.x * dt);
    //this->setPositionY(this->getPositionY() + velocity.y * dt);
    
    //�������Ƿ��ڼ�ʵ�ĵ�����
    bool onGround = isOnSolidGround();
    setOnGround(onGround);
    if (onGround) {
        canDash = 1;
    }

   
    /* ���ֱ�������ٶȵĻ�Ҫ�õ��������
    // �������ڵ����ϣ����ô�ֱ�ٶ�Ϊ0
    if (onGround && !keyStates[PlayerKey::JUMP]) {
        velocity.y = velocity.y - 15;
        if (velocity.y < 0)
        {
            velocity.y = 0;
        }
    }
    */
    // �����־���
   // CCLOG("isOnGround: %d, verticalVelocity: %f, jumpKeyState: %d", onGround, verticalVelocity, keyStates[PlayerKey::JUMP]);
   

    //����
    if (keyStates[PlayerKey::DOWN] && isOnGround&&!keyStates[PlayerKey::DASH] && !keyStates[PlayerKey::CLIMB]&&velocity.x==0){
        changeState(PlayerState::CROUCH);
        CCLOG("Player state changed to CROUCH");
        return;
    }
    //���Ͽ�
    if (keyStates[PlayerKey::UP] && isOnGround && !keyStates[PlayerKey::DASH]&&!keyStates[PlayerKey::DOWN]&&!keyStates[PlayerKey::CLIMB] && velocity.x == 0) {
        changeState(PlayerState::LOOKUP);
        CCLOG("Player state changed to LOOKUP");
        return;
    }    

    //���(Ĭ��)
    if (keyStates[PlayerKey::DASH] && canDash  ) {
        this->getPhysicsBody()->setGravityEnable(false);
        canDash = 0;
        velocity.x = 0; velocity.y = 0;
        this->getPhysicsBody()->setVelocity(Vec2(0, 0));
        changeState(PlayerState::DASH);
        CCLOG("Player state changed to DASH");
        return;
    }


    //���
    if ((previousState == PlayerState::DROP && isOnGround)||currentState== PlayerState::LANDING) {
        changeState(PlayerState::LANDING);        
        //CCLOG("Player state changed to LANDING");
        return;
    }
    //��ǽ
    if (((keyStates[PlayerKey::RIGHT] && velocity.x > 0) || (keyStates[PlayerKey::LEFT] && velocity.x < 0)) && isOnGround && canClimb &&!keyStates[PlayerKey::CLIMB]) {
        changeState(PlayerState::PUSHWALL);
        CCLOG("Player state changed to PUSHWALL");
        return;
    }
    //����

    if (!isOnGround && verticalVelocity < 0) {
        changeState(PlayerState::DROP);
        return;
    }
    //����
    
    if (checkForJumpTableInteraction()) {
        this->getPhysicsBody()->setGravityEnable(true);
        //velocity.y = 0;
        this->getPhysicsBody()->applyImpulse(Vec2(0, 100));//ʹ�ó���
    }
    //ש��
    checkForBrickInteraction();
    //��ǽ��Ծ
    if ((currentState == PlayerState::HOLDWALL || currentState == PlayerState::HOLDWALLUP || currentState == PlayerState::HOLDWALLDOWN) && keyStates[PlayerKey::JUMP]) {
        this->getPhysicsBody()->setGravityEnable(true);
        velocity.y = 0;
        this->getPhysicsBody()->applyImpulse(Vec2(-30, 90) * facingDirection);//ʹ�ó���
        changeState(PlayerState::HOLDWALLJUMP);
        CCLOG("Player state changed to HOLDWALLJUMP");
        return;
    }

    //��ǽ
    if (keyStates[PlayerKey::CLIMB] && canClimb &&!keyStates[PlayerKey::UP] && !keyStates[PlayerKey::DOWN])
    {
        this->getPhysicsBody()->setGravityEnable(false);
        changeState(PlayerState::HOLDWALL);
        CCLOG("Player state changed to HOLDWALL");
        return;
    }
    if (keyStates[PlayerKey::CLIMB] && canClimb && keyStates[PlayerKey::UP] && !keyStates[PlayerKey::DOWN])
    {
        this->getPhysicsBody()->setGravityEnable(false);
        velocity.y = CLINMB_MAXSPEED;
        changeState(PlayerState::HOLDWALLUP);
        CCLOG("Player state changed to HOLDWALLUP");
        return;
    }
    if (keyStates[PlayerKey::CLIMB] && canClimb && !keyStates[PlayerKey::UP] && keyStates[PlayerKey::DOWN])
    {
        this->getPhysicsBody()->setGravityEnable(false);
        velocity.y = -CLINMB_MAXSPEED;
        changeState(PlayerState::HOLDWALLDOWN);
        CCLOG("Player state changed to HOLDWALLDOWN");
        return;
    }
  
    //ˮƽ�ƶ�
    if (keyStates[PlayerKey::LEFT] && velocity.x > -WALK_MAXSPEED) {
        velocity.x -= accelerationX * dt;
        if (velocity.x < -WALK_MAXSPEED) {
            velocity.x = -WALK_MAXSPEED;
        }
    }
    else if (keyStates[PlayerKey::RIGHT] && velocity.x < WALK_MAXSPEED) {
        velocity.x += accelerationX * dt;
        if (velocity.x > WALK_MAXSPEED) {
            velocity.x = WALK_MAXSPEED;
        }
    }
    else {
        // ���û�а��������£����ٵ�0
        if (velocity.x > 0) {
            velocity.x -= accelerationX * dt;
            if (velocity.x < 0) velocity.x = 0;
        }
        else if (velocity.x < 0) {
            velocity.x += accelerationX * dt;
            if (velocity.x > 0) velocity.x = 0;
        }
    }

   
    if (keyStates[PlayerKey::JUMP] && isOnGround ) {
        //velocity.y = JUMP_SPEED;//ֱ�������ٶ�
        this->getPhysicsBody()->applyImpulse(Vec2(0, 350));//ʹ�ó���
        isOnGround = false;        

        changeState(PlayerState::JUMPING);
        return;
    }

    
    

    //״̬ת��
    if (isOnGround) {
        if (velocity.x == 0) {
            changeState(PlayerState::IDLE);
            //CCLOG("Player state changed to IDLE");
            return;
        }
        else if (velocity.x > 0) {            
            if (keyStates[PlayerKey::RIGHT]) { 
                changeState(PlayerState::MOVING_RIGHT); 
                facingDirection = 1;
                return;
            }
            else if(keyStates[PlayerKey::LEFT]){ 
                changeState(PlayerState::MOVING_TURN_RL);   
                facingDirection = -1;
                return;
            }
        }
        else if (velocity.x < 0) {            
            if (keyStates[PlayerKey::LEFT]) {
                changeState(PlayerState::MOVING_LEFT);
                facingDirection = -1;
                return;
            }
            else if (keyStates[PlayerKey::RIGHT]) {
                changeState(PlayerState::MOVING_TURN_LR);  
                facingDirection = 1;
                return;
            }
        }
    }
    
   

    // ... �������� ...
}





void Player::setOnGround(bool a) {
    this->isOnGround = a;//����isOnGround״̬
    if (a)
    {
        //CCLOG("on groud");
    }
    else {
        //CCLOG("not on groud");
    }
}



bool Player::checkPlayerInput() {//�ް���ʱ����0���а���ʱ����1
    // ���û�м�������
    if (!keyStates[PlayerKey::LEFT] && !keyStates[PlayerKey::RIGHT] && !keyStates[PlayerKey::UP] &&
        !keyStates[PlayerKey::DOWN] && !keyStates[PlayerKey::JUMP] && !keyStates[PlayerKey::DASH] &&
        !keyStates[PlayerKey::TALK] && !keyStates[PlayerKey::CLIMB]) {
        return 0;
    }
    return 1;
}



void Player::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        keyStates[PlayerKey::LEFT] = true;
        break;
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        keyStates[PlayerKey::RIGHT] = true;
        break;
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
        keyStates[PlayerKey::UP] = true;
        break;
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        keyStates[PlayerKey::DOWN] = true;
        break;
        // ����ʹ�ÿո��������Ծ
    case EventKeyboard::KeyCode::KEY_SPACE:
        //CCLOG("press space");
        keyStates[PlayerKey::JUMP] = true;
        break;
        // ����ʹ��J������̸
    case EventKeyboard::KeyCode::KEY_J:
        keyStates[PlayerKey::TALK] = true;
        break;
        // ����ʹ��SHIFT��������
    case EventKeyboard::KeyCode::KEY_SHIFT:
        keyStates[PlayerKey::DASH] = true;
        break;
        // ����ʹ��K����������
    case EventKeyboard::KeyCode::KEY_K:
        keyStates[PlayerKey::CLIMB] = true;
        break;
    default:
        break;
    }
}

void Player::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        keyStates[PlayerKey::LEFT] = false;
        break;
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        keyStates[PlayerKey::RIGHT] = false;
        break;
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
        keyStates[PlayerKey::UP] = false;
        break;
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        keyStates[PlayerKey::DOWN] = false;
        break;
    case EventKeyboard::KeyCode::KEY_SPACE:
        //CCLOG("release space");
        keyStates[PlayerKey::JUMP] = false;    
        isJumping = false; // �������ͷ�ʱ��������һ����Ծ
        break;
    case EventKeyboard::KeyCode::KEY_J:
        keyStates[PlayerKey::TALK] = false;
        break;
    case EventKeyboard::KeyCode::KEY_SHIFT:
        keyStates[PlayerKey::DASH] = false;
        break;
    case EventKeyboard::KeyCode::KEY_K:
        keyStates[PlayerKey::CLIMB] = false;
        CCLOG("KEY_K RELESE");
        break;
    default:
        break;
    }
}

//��Ƶ�Ͷ������

void Player::playIdleAnimation_1() {//վ��1    
    // ֹͣ�����������еĶ�����ȷ������������������ͻ��
    this->stopAllActions();
    //CCLOG("Starting IDLE animation");
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 6; i++) {
        std::string frameName = StringUtils::format("Idle_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }

    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.2f);
    auto animate = Animate::create(animation);
    // ʹ�� RepeatForever ����ʹ��������ѭ������
    auto repeatForever = RepeatForever::create(animate);

    this->runAction(repeatForever); // ʹ�� repeatForever ���ж���
   // CCLOG("Finished setting up IDLE animation");
}

void Player::playIdleAnimation_2() {//վ��2
    // ֹͣ�����������еĶ�����ȷ������������������ͻ��
    this->stopAllActions();
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 9; i++) {
        std::string frameName = StringUtils::format("Idle_01-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }

    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.2f);
    auto animate = Animate::create(animation);
    this->runAction(animate);

}

void Player::playMoveAnimation() {//�ƶ� 
    // ֹͣ�����������еĶ�����ȷ������������������ͻ��
    //��Ƶ
    _walkMusicId = cocos2d::AudioEngine::play2d("music/walk.mp3", true);//���������
    cocos2d::AudioEngine::setVolume(_walkMusicId, 1.0f);  // ��������
    //����
    this->stopAllActions();
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 24; i++) {
        std::string frameName = StringUtils::format("Move_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }

    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.04f);
    auto animate = Animate::create(animation);
    // ʹ�� RepeatForever ����ʹ��������ѭ������
    auto repeatForever = RepeatForever::create(animate);
    this->runAction(repeatForever);

}

void Player::playMoveTurnAnimation() {//ת�� 10֡ 
    // ֹͣ�����������еĶ�����ȷ������������������ͻ��
    this->stopAllActions();
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 9; i++) {
        std::string frameName = StringUtils::format("moveturn_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }

    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.1f);
    auto animate = Animate::create(animation);
    this->runAction(animate);

}

void Player::playCrouchAnimation() {
   
    // ֹͣ�����������еĶ�����ȷ������������������ͻ��
    this->stopAllActions();

    CCLOG("Starting Crouch animation");
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 6; i++) {
        std::string frameName = StringUtils::format("crouch_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }             
    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.1f);
    auto animate = Animate::create(animation);

    this->runAction(animate);   
    this->setSpriteFrame(idleFrames.at(6));   //��һ�ζ����͹���
    
    CCLOG("Finished setting up Crouch animation");
}

void Player::playCrouchToIdleAnimation() {

    // ֹͣ�����������еĶ�����ȷ������������������ͻ��
    this->stopAllActions();

    CCLOG("Starting CrouchToIdle animation");
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 2; i++) {
        std::string frameName = StringUtils::format("CrouchToIdle_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }
    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.1f);
    auto animate = Animate::create(animation);

    // ����lambda��Ϊ�ص�
    auto callback = [this]() {
        previousState = PlayerState::IDLE;//����״̬��֤���ظ����Ź��ɶ���
        playIdleAnimation_1();//����״̬�����ƣ��ֶ�����
        CCLOG("Animation completed!");
        // ����������״̬���µȲ���
        };

    // �����ص�����
    auto callbackAction = CallFunc::create(callback);

    // ʹ�� Sequence �����������ͻص��������
    auto sequence = Sequence::create(animate, callbackAction, nullptr);

    this->runAction(sequence);

    CCLOG("Finished setting up CrouchToIdle animation");
}

void Player::playLookUpAnimation() {//���Ͽ� 
    // ֹͣ�����������еĶ�����ȷ������������������ͻ��
    this->stopAllActions();
    CCLOG("Starting LookUp animation");
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 5; i++) {
        std::string frameName = StringUtils::format("LookUp_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }
  
    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.1f);
    auto animate = Animate::create(animation);  

    this->runAction(animate); 
    this->setSpriteFrame(idleFrames.at(5));//��һ�ζ����͹���
    CCLOG("Finished setting up LookUp animation");
}

void Player::playJumpUpAnimation() {//��Ծ
    
     // ������ֵ�״̬
    _jumpMusicState = cocos2d::AudioEngine::getState(_jumpMusicId);

    // �������û�в��Ż��߲����Ѿ���ɣ���ô��ʼ��������(mp3��ʽ)
    if (_jumpMusicState != cocos2d::AudioEngine::AudioState::PLAYING) {
        _jumpMusicId = cocos2d::AudioEngine::play2d("music/jump.mp3", false);
    }
    // ֹͣ�����������еĶ�����ȷ������������������ͻ��

    CCLOG("Starting JumpUp animation");
    this->stopAllActions();
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 3; i++) {
        std::string frameName = StringUtils::format("jumpup_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }

    for (int i = 0; i <= 1; i++) {
        std::string frameName = StringUtils::format("Top_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }
    playFloorJumpAshAnimation();
    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.1f);
    auto animate = Animate::create(animation);
    this->runAction(animate);
     CCLOG("Finished setting up JumpUp animation");

}

void Player::playJumpMoveAnimation() {//��Ծ�ƶ�
    // ֹͣ�����������еĶ�����ȷ������������������ͻ��
    this->stopAllActions();
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 3; i++) {
        std::string frameName = StringUtils::format("jumpmove_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }

    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.1f);
    auto animate = Animate::create(animation);
    this->runAction(animate);

}

void Player::playDropAnimation() {//׹��
    // ֹͣ�����������еĶ�����ȷ������������������ͻ��
    this->stopAllActions();
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 9; i++) {
        std::string frameName = StringUtils::format("drop_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }
    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.07f);
    auto animate = Animate::create(animation);
    this->runAction(animate);
    
}

void Player::playPushWallAnimation() {//��ǽ    
    // ֹͣ�����������еĶ�����ȷ������������������ͻ��
    this->stopAllActions();
    //CCLOG("Starting PushWall animation");
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 9; i++) {
        std::string frameName = StringUtils::format("pushwall_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }

    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.1f);
    auto animate = Animate::create(animation);
    // ʹ�� RepeatForever ����ʹ��������ѭ������
    auto repeatForever = RepeatForever::create(animate);

    this->runAction(repeatForever); // ʹ�� repeatForever ���ж���
    // CCLOG("Finished setting up PushWall animation");
}

void Player::playLandingAnimation() {//���ɶ��� ���

    _landingMusicState = cocos2d::AudioEngine::getState(_landingMusicId);

    // �������û�в��Ż��߲����Ѿ���ɣ���ô��ʼ��������(mp3��ʽ)
    if (_landingMusicState != cocos2d::AudioEngine::AudioState::PLAYING) {
        _landingMusicId = cocos2d::AudioEngine::play2d("music/landing.mp3", false);
    }

    // ֹͣ�����������еĶ�����ȷ������������������ͻ��
    this->stopAllActions();
    //��������ٶ�
    if (velocity.x > 30) {
        velocity.x = 30;
    }
    else if (velocity.x < -30) {
        velocity.x = -30;
    }
    CCLOG("Starting Landing animation");
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 5; i++) {
        std::string frameName = StringUtils::format("landing_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }
    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.1f);
    auto animate = Animate::create(animation);

    playFloorLandingAshAnimation();

    // ����lambda��Ϊ�ص�
    auto callback = [this]() {
        previousState = PlayerState::LANDING;
        currentState = PlayerState::IDLE;//����״̬��֤���ظ����Ź��ɶ���
        playIdleAnimation_1();//����״̬�����ƣ��ֶ�����
        CCLOG("Animation completed!");
        
        };

    // �����ص�����
    auto callbackAction = CallFunc::create(callback);

    // ʹ�� Sequence �����������ͻص��������
    auto sequence = Sequence::create(animate, callbackAction, nullptr);

    this->runAction(sequence);

    CCLOG("Finished setting up CrouchToIdle animation");
}

void Player::playHoldWallAnimation(){//��ǽ
    this->stopAllActions();
    //CCLOG("Starting HoldWall animation");
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 7; i++) {
        std::string frameName = StringUtils::format("holdwall_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }

    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.1f);
    auto animate = Animate::create(animation);
    // ʹ�� RepeatForever ����ʹ��������ѭ������
    auto repeatForever = RepeatForever::create(animate);

    this->runAction(repeatForever); // ʹ�� repeatForever ���ж���
    // CCLOG("Finished setting up HoldWall animation");

    }

void Player::playHoldWallUpAnimation() {
    this->stopAllActions();
    //CCLOG("Starting HoldWall animation");
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 9; i++) {
        std::string frameName = StringUtils::format("holdwallup_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }

    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.1f);
    auto animate = Animate::create(animation);
    // ʹ�� RepeatForever ����ʹ��������ѭ������
    auto repeatForever = RepeatForever::create(animate);

    this->runAction(repeatForever); // ʹ�� repeatForever ���ж���
    // CCLOG("Finished setting up HoldWall animation");




}//��ǽ����

void Player::playHoldWallDownAnimation(){

    this->stopAllActions();
    //CCLOG("Starting HoldWall animation");
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 4; i++) {
        std::string frameName = StringUtils::format("holdwalldown_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }

    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.1f);
    auto animate = Animate::create(animation);
    // ʹ�� RepeatForever ����ʹ��������ѭ������
    auto repeatForever = RepeatForever::create(animate);

    this->runAction(repeatForever); // ʹ�� repeatForever ���ж���
    // CCLOG("Finished setting up HoldWall animation");





}//��ǽ����

void Player::playHoldWallJumpAnimation() {//��ǽ��Ծ
    // ֹͣ�����������еĶ�����ȷ������������������ͻ��
    CCLOG("Starting HoldWallJump animation");
    this->stopAllActions();
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 3; i++) {
        std::string frameName = StringUtils::format("jumpmove_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }

    for (int i = 0; i <= 1; i++) {
        std::string frameName = StringUtils::format("Top_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }
    playFloorWallJumpAshAnimation();
    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.1f);
    auto animate = Animate::create(animation);
    this->runAction(animate);
    CCLOG("Finished setting up HoldWallJump animation");

}

void Player::playDashAnimation() {

    //��Ƶ
    
    // ������ֵ�״̬
    _dashMusicState = cocos2d::AudioEngine::getState(_dashMusicId);

    // �������û�в��Ż��߲����Ѿ���ɣ���ô��ʼ��������(mp3��ʽ)
    if (_dashMusicState != cocos2d::AudioEngine::AudioState::PLAYING) {
        _dashMusicId = cocos2d::AudioEngine::play2d("music/dash.mp3", false);
    }
    
    //����

    isDashing = true;
    this->stopAllActions();

    // ��ȡ��̷���
    cocos2d::Vec2 dashDirection = getDashDirection();

    // ��ʼ���
    this->startDashing(dashDirection);  // ���ݳ�̷��� startDashing ����

    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 8; i++) {
        std::string frameName = StringUtils::format("dashmove_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }

    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.05f);
    auto animate = Animate::create(animation);

    // ����һ��CallFunc�������ڶ����ﵽ�ض�֡ʱ����shadowЧ��
    auto generateShadow = CallFunc::create([this, cache]() {
        auto shadow = Sprite::createWithSpriteFrameName("shadow_00.png");
        shadow->setScale(0.80);
        // ����facingDirection��תshadow
        if (this->facingDirection == -1) {
            shadow->setFlippedX(true);
        }

        shadow->setPosition(this->getPosition());
        this->getParent()->addChild(shadow, this->getLocalZOrder() - 1);

        // ����shadow�ĳ���ʱ�����ʧЧ��
        auto fadeOut = FadeOut::create(0.7f);
        auto remove = CallFunc::create([shadow]() {
            shadow->removeFromParent();
            });
        auto shadowSequence = Sequence::create(fadeOut, remove, nullptr);
        shadow->runAction(shadowSequence);
        });

    // �ڶ������ض�ʱ�����generateShadow
    auto delayForFirstShadow = DelayTime::create(0.2f);  // 0.05 * 4
    auto delayForSecondShadow = DelayTime::create(0.1f);  // 0.05 * 2
    auto shadowSequence = Sequence::create(delayForFirstShadow, generateShadow->clone(), delayForSecondShadow, generateShadow->clone(), nullptr);

    // ������̺���������
    auto enableGravity = CallFunc::create([this]() {
        this->getPhysicsBody()->setGravityEnable(true);
       // this->isDashing = false;
        });
    auto sequence = Sequence::create(animate, enableGravity, nullptr);

    // ������������
    this->runAction(sequence);
    this->runAction(shadowSequence);
}
//�·�����ȥ����μ�⣬��һ���Լ�������ʵ��ƶ�����
void Player::startDashing(const cocos2d::Vec2& dashDirection) {
    float rayLength = 220.0f;  // ��̵����߳���

    // ���ĵ�
    cocos2d::Vec2 centerPoint = this->getPosition();
    cocos2d::Vec2 endPoint = centerPoint + dashDirection.getNormalized() * rayLength;

    bool collisionDetected = false;
    cocos2d::Vec2 collisionPoint;

    auto rayCallback = [&collisionDetected, &collisionPoint](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data) -> bool {
        if (info.shape->getBody()->getNode()->getName() != "player") {
            collisionDetected = true;
            collisionPoint = info.contact;
            return false;
        }
        return true;
        };

    Director::getInstance()->getRunningScene()->getPhysicsWorld()->rayCast(rayCallback, centerPoint, endPoint, nullptr);

    if (collisionDetected) {
        // �����������ײ��֮��ľ���
        float distance = centerPoint.distance(collisionPoint) - 40; // ����40�����Է�ֹ��ײ
        endPoint = centerPoint + dashDirection.getNormalized() * distance;
    }

    // ���������ʱ��
    float time = rayLength / 500.0f;  // 500����ÿ��
    auto moveAction = cocos2d::MoveTo::create(time, endPoint);
   
    // ȷ���ܵĳ��ʱ��Ϊ0.4��
    float remainingTime = 0.4f - time;
    auto delayAction = cocos2d::DelayTime::create(remainingTime > 0 ? remainingTime : 0);

    // ����һ����������
    auto sequence = cocos2d::Sequence::create(moveAction, delayAction, nullptr);
    this->runAction(sequence);
}

/*
void Player::startDashing(const cocos2d::Vec2& dashDirection) {
    int segments = 11;  // ��̶���
    float segmentLength = 20.0f;  // ÿ�εĳ���

    // ʹ��schedule�������г��
    this->schedule([this, segments, segmentLength, dashDirection](float dt) mutable {  // ���������dashDirection
        // ����������λ��
        cocos2d::Vec2 desiredPosition = this->getPosition() + dashDirection * segmentLength;  // ʹ��dashDirection

        // ʹ��adjustMovePosition����������Ƿ����ϰ���
        cocos2d::Vec2 adjustedPosition = this->adjustMovePosition(desiredPosition);

        // �����ƶ��Ĳ�ֵ
        cocos2d::Vec2 moveByValue = adjustedPosition - this->getPosition();

        // ʹ��MoveBy��ƽ���ƶ����
        auto moveAction = cocos2d::MoveBy::create(0.1f, moveByValue);
        this->runAction(moveAction);

        // �ݼ�segments
        --segments;

        // �����⵽�ϰ����������ɣ���ֹͣ���
        if (segments <= 0) {
            this->unschedule("DashingKey");
        }
        }, 0.04f, "DashingKey");
}
*/

cocos2d::Vec2 Player::getDashDirection() {
    // ʹ�ó�Ա��������鰴��״̬
    bool isUp = keyStates[PlayerKey::UP];
    bool isDown = keyStates[PlayerKey::DOWN];
    bool isLeft = keyStates[PlayerKey::LEFT];
    bool isRight = keyStates[PlayerKey::RIGHT];

    cocos2d::Vec2 direction(0,0);

    if (isUp) {
        direction.y = 1;
    }
    else if (isDown) {
        direction.y = -1;
    }

    if (isLeft) {
        direction.x = -1;
    }
    else if (isRight) {
        direction.x = 1;
    }
    else if (!isUp && !isDown) {
        // ���û�а����������������ʹ��Ĭ�Ϸ���
        direction.x = facingDirection;
    }

    return direction.getNormalized();  // ���ع�һ���ķ�������
}

void Player::playDashUpAndDownAnimation() {

    // ������ֵ�״̬
    _dashMusicState = cocos2d::AudioEngine::getState(_dashMusicId);

    // �������û�в��Ż��߲����Ѿ���ɣ���ô��ʼ��������(mp3��ʽ)
    if (_dashMusicState != cocos2d::AudioEngine::AudioState::PLAYING) {
        _dashMusicId = cocos2d::AudioEngine::play2d("music/dash.mp3", false);
    }

    isDashing = true;
    this->stopAllActions();

    // ��ȡ��̷���
    cocos2d::Vec2 dashDirection = getDashDirection();

    // ��ʼ���
    this->startDashing(dashDirection);  // ���ݳ�̷��� startDashing ����

    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 8; i++) {
        std::string frameName = StringUtils::format("dashup_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }

    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.05f);
    auto animate = Animate::create(animation);

    // ����һ��CallFunc�������ڶ����ﵽ�ض�֡ʱ����shadowЧ��
    auto generateShadow = CallFunc::create([this, cache]() {
        auto shadow = Sprite::createWithSpriteFrameName("shadow_00.png");
        // ����facingDirection��תshadow
        if (this->facingDirection == -1) {
            shadow->setFlippedX(true);
        }

        shadow->setPosition(this->getPosition());
        this->getParent()->addChild(shadow, this->getLocalZOrder() - 1);

        // ����shadow�ĳ���ʱ�����ʧЧ��
        auto fadeOut = FadeOut::create(0.5f);
        auto remove = CallFunc::create([shadow]() {
            shadow->removeFromParent();
            });
        auto shadowSequence = Sequence::create(fadeOut, remove, nullptr);
        shadow->runAction(shadowSequence);
        });

    // �ڶ������ض�ʱ�����generateShadow
    auto delayForFirstShadow = DelayTime::create(0.20f);  // 0.05 * 4
    auto delayForSecondShadow = DelayTime::create(0.10f);  // 0.05 * 2
    auto shadowSequence = Sequence::create(delayForFirstShadow, generateShadow->clone(), delayForSecondShadow, generateShadow->clone(), nullptr);

    // ������̺���������
    auto enableGravity = CallFunc::create([this]() {
        this->getPhysicsBody()->setGravityEnable(true);
        //this->isDashing = false;
        });
    auto sequence = Sequence::create(animate, enableGravity, nullptr);

    // ������������
    this->runAction(sequence);
    this->runAction(shadowSequence);
}

void Player::playRespawnAnimation() {//�����   
  
    // ������ֵ�״̬
    _reviveMusicState = cocos2d::AudioEngine::getState(_reviveMusicId);
    
    // �������û�в��Ż��߲����Ѿ���ɣ���ô��ʼ��������(mp3��ʽ)
    if (_reviveMusicState != cocos2d::AudioEngine::AudioState::PLAYING) {
        _reviveMusicId = cocos2d::AudioEngine::play2d("music/revive.mp3", false);
    }

    CCLOG("Starting Respawn animation");
    
    // ����һ���µ�Sprite�������ڲ�����������
    auto respawnSprite = Sprite::create();    
    respawnSprite->setScale(0.80);
    respawnSprite->setVisible(true);
    respawnSprite->setPosition(respawnPoints[currentLevel]);
    this->getParent()->addChild(respawnSprite);
  

    this->stopAllActions();
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 17; i++) {
        std::string frameName = StringUtils::format("Respawn_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }

    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.05f);
    auto animate = Animate::create(animation);

    auto setPlayerAlive = CallFunc::create([this, respawnSprite]() {        
        this->setVisible(true); // ��ɫ���¿ɼ�
        this->getPhysicsBody()->setEnabled(true); // ��������������
        this->isAlive = true; // ���ý�ɫ״̬Ϊ����
        respawnSprite->removeFromParent();
        //�������
        // ������������Ϊ0.80
        this->setScale(0.80);
        velocity = Vec2::ZERO;
        this->getPhysicsBody()->setVelocity(Vec2::ZERO);
        // �������Ϊ�ɼ�    
        this->getPhysicsBody()->setGravityEnable(true);
        // ������ײ
        this->getPhysicsBody()->setEnabled(true);
        // ���ý�ɫ��λ��
        cocos2d::Vec2 respawnPosition = respawnPoints[currentLevel]; // ������λ�� �������Ϊ�ؿ���������
        this->setPosition(respawnPosition);
        });

    auto sequence = Sequence::create(animate, setPlayerAlive, nullptr);
    respawnSprite->runAction(sequence);

    CCLOG("Finished setting up Respawn animation");

    
}

/*
void Player::playDeathAnimation() {//����
    
    //ȡ����ײ���
    this->getPhysicsBody()->setEnabled(false);
    // ���ý�ɫ��λ��
    cocos2d::Vec2 respawnPosition = respawnPoints[currentLevel]; // ������λ�� �������Ϊ�ؿ���������
    this->setPosition(respawnPosition);
    // ������ֵ�״̬
    _deathMusicState = cocos2d::AudioEngine::getState(_deathMusicId);

    // �������û�в��Ż��߲����Ѿ���ɣ���ô��ʼ��������(mp3��ʽ)
    if (_deathMusicState != cocos2d::AudioEngine::AudioState::PLAYING) {
        _deathMusicId = cocos2d::AudioEngine::play2d("music/death.mp3", false);
    }
   
    
    
    // Set the player's velocity to zero
    this->getPhysicsBody()->setVelocity(Vec2::ZERO);
    velocity = Vec2::ZERO;
    // Disable gravity for the player
    this->getPhysicsBody()->setGravityEnable(false);
    // Set the player's Y position to 100    
    if (this->getPositionY() < 100) {this->setPositionY(getPositionY()+100);}//����������
    //this->setPositionX(500);//����������

    // ������������Ϊ0.80
    this->setScale(0.80);

    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();
    this->stopAllActions();
    CCLOG("Starting Death animation");
    for (int i = 0; i <= 23; i++) {
        std::string frameName = StringUtils::format("death_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }

    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.05f);
    auto animate = Animate::create(animation);

    // ��Death�������������playBlackAnimation
    auto callBlackAnimation = CallFunc::create([this]() {
        this->setScale(0.80);
        this->playBlackAnimation();
        });

    auto sequence = Sequence::create(animate, callBlackAnimation, nullptr);
    this->runAction(sequence);
    CCLOG("Finished setting up Death animation");
}
*/
void Player::playDeathAnimation() {
    // ��ȡplayer��ǰ��λ��
    cocos2d::Vec2 currentPosition = this->getPosition();
    // �������Ϊ���ɼ�
    this->setVisible(false);
    // �����������Է�ֹ��ײ
    this->getPhysicsBody()->setEnabled(false);
    //
    velocity = Vec2::ZERO;
    this->getPhysicsBody()->setVelocity(Vec2::ZERO);
    this->getPhysicsBody()->setGravityEnable(false);
    // ���ý�ɫ��λ��
    cocos2d::Vec2 respawnPosition = respawnPoints[currentLevel]; // ������λ�� �������Ϊ�ؿ���������
    this->setPosition(respawnPosition);
    // ����һ���µ�Sprite�������ڲ�����������
    auto deathSprite = Sprite::create();
    deathSprite->setPosition(currentPosition);
    deathSprite->setScale(0.80);
    this->getParent()->addChild(deathSprite);

    // ������������������֡
    Vector<SpriteFrame*> deathFrames;
    auto frameCache = SpriteFrameCache::getInstance();
    for (int i = 0; i <= 23; ++i) {
        auto frameName = StringUtils::format("death_00-%d.png", i);
        auto frame = frameCache->getSpriteFrameByName(frameName);
        if (frame) {
            deathFrames.pushBack(frame);
        }
    }

    // ��������������
    auto animation = Animation::createWithSpriteFrames(deathFrames, 0.05f);
    auto animate = Animate::create(animation);

    // �ڶ���������ɺ��Ƴ�Sprite,�򿪺�Ļ
    auto callBlackAnimation = CallFunc::create([this, deathSprite]() {
        this->playBlackAnimation();
        deathSprite->removeFromParent();
        });

    // ������������
    _deathMusicState = cocos2d::AudioEngine::getState(_deathMusicId);
    if (_deathMusicState != cocos2d::AudioEngine::AudioState::PLAYING) {
        _deathMusicId = cocos2d::AudioEngine::play2d("music/death.mp3", false);
    }

    // ִ�ж������Ƴ���������ж���
    auto sequence = Sequence::create(animate, callBlackAnimation, nullptr);
    deathSprite->runAction(sequence);
}

void Player::playBlackAnimation() {
    CCLOG("Starting Black animation");
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 9; i++) {
        std::string frameName = StringUtils::format("black_0%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }

    auto blackSprite = Sprite::createWithSpriteFrame(idleFrames.front());  // ʹ�õ�һ֡��ʼ��
    blackSprite->setPosition(Director::getInstance()->getVisibleSize() / 2);  // ����Ϊ��Ļ����

    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.05f);
    auto animate = Animate::create(animation);
    auto reverseAnimate = animate->reverse();

    // ��Black�������������playRespawnAnimation
    auto callRespawnAnimation = CallFunc::create([this]() {
        this->playRespawnAnimation();
        });

    auto removeBlackSprite = CallFunc::create([blackSprite]() {
        blackSprite->removeFromParent();
        });

    auto finalSequence = Sequence::create(reverseAnimate, animate->clone(), callRespawnAnimation, removeBlackSprite, nullptr);

    blackSprite->runAction(finalSequence);
    this->getParent()->addChild(blackSprite, this->getLocalZOrder() + 1);  // ��ӵ���������ȷ����λ�����֮��
    CCLOG("Finished setting up Black animation");
}
//B����
/*
void Player::playBDeathAnimation() {//����

    _deathMusicState = cocos2d::AudioEngine::getState(_deathMusicId);

    // �������û�в��Ż��߲����Ѿ���ɣ���ô��ʼ��������(mp3��ʽ)
    if (_deathMusicState != cocos2d::AudioEngine::AudioState::PLAYING) {
        _deathMusicId = cocos2d::AudioEngine::play2d("music/death.mp3", false);
    }

    CCLOG("Starting Death animation");
    this->stopAllActions();

    // Set the player's velocity to zero
    this->getPhysicsBody()->setVelocity(Vec2::ZERO);
    velocity = Vec2::ZERO;
    // Disable gravity for the player
    this->getPhysicsBody()->setGravityEnable(false);
    // Set the player's Y position to 100    
    this->setPositionY(300);//����������
    //this->setPositionX(500);//����������

    // ������������Ϊ0.80
    this->setScale(0.80);

    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 23; i++) {
        std::string frameName = StringUtils::format("Bdeath_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }

    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.05f);
    auto animate = Animate::create(animation);

    // ��Death�������������playBlackAnimation
    auto callBlackAnimation = CallFunc::create([this]() {
        this->setScale(0.80);
        this->playBlackAnimation();
        });

    auto sequence = Sequence::create(animate, callBlackAnimation, nullptr);
    this->runAction(sequence);
    CCLOG("Finished setting up Death animation");
}
*/
void Player::playBDeathAnimation() {
    // ��ȡplayer��ǰ��λ��
    cocos2d::Vec2 currentPosition = this->getPosition();
    // �������Ϊ���ɼ�
    this->setVisible(false);
    // �����������Է�ֹ��ײ
    this->getPhysicsBody()->setEnabled(false);
    //
    velocity = Vec2::ZERO;
    this->getPhysicsBody()->setVelocity(Vec2::ZERO);
    this->getPhysicsBody()->setGravityEnable(false);
    // ���ý�ɫ��λ��
    cocos2d::Vec2 respawnPosition = respawnPoints[currentLevel]; // ������λ�� �������Ϊ�ؿ���������
    this->setPosition(respawnPosition);
    // ����һ���µ�Sprite�������ڲ�����������
    auto deathSprite = Sprite::create();
    deathSprite->setPosition(currentPosition);
    deathSprite->setScale(0.80);
    this->getParent()->addChild(deathSprite);

    // ����B�汾��������������֡
    Vector<SpriteFrame*> deathFrames;
    auto frameCache = SpriteFrameCache::getInstance();
    for (int i = 0; i <= 23; ++i) {
        auto frameName = StringUtils::format("Bdeath_00-%d.png", i);
        auto frame = frameCache->getSpriteFrameByName(frameName);
        if (frame) {
            deathFrames.pushBack(frame);
        }
    }

    // ��������������
    auto animation = Animation::createWithSpriteFrames(deathFrames, 0.05f);
    auto animate = Animate::create(animation);

    // ������������
    _deathMusicState = cocos2d::AudioEngine::getState(_deathMusicId);
    if (_deathMusicState != cocos2d::AudioEngine::AudioState::PLAYING) {
        _deathMusicId = cocos2d::AudioEngine::play2d("music/death.mp3", false);
    }

    // �ڶ���������ɺ��Ƴ�Sprite,�򿪺�Ļ
    auto callBlackAnimation = CallFunc::create([this, deathSprite]() {
        this->playBlackAnimation();
        deathSprite->removeFromParent();
        });

    // ִ�ж������Ƴ���������ж���
    auto sequence = Sequence::create(animate, callBlackAnimation, nullptr);
    deathSprite->runAction(sequence);
}

void Player::playBDropAnimation() {//׹��
    // ֹͣ�����������еĶ�����ȷ������������������ͻ��
    this->stopAllActions();
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 9; i++) {
        std::string frameName = StringUtils::format("Bdrop_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }
    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.07f);
    auto animate = Animate::create(animation);
    this->runAction(animate);

}

void Player::playBHoldWallAnimation() {//��ǽ
    this->stopAllActions();
    //CCLOG("Starting HoldWall animation");
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 6; i++) {
        std::string frameName = StringUtils::format("Bholdwall_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }

    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.1f);
    auto animate = Animate::create(animation);
    // ʹ�� RepeatForever ����ʹ��������ѭ������
    auto repeatForever = RepeatForever::create(animate);

    this->runAction(repeatForever); // ʹ�� repeatForever ���ж���
    // CCLOG("Finished setting up HoldWall animation");

}

void Player::playBHoldWallUpAnimation() {
    this->stopAllActions();
    //CCLOG("Starting HoldWall animation");
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 19; i++) {
        std::string frameName = StringUtils::format("Bholdwallup_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }

    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.1f);
    auto animate = Animate::create(animation);
    // ʹ�� RepeatForever ����ʹ��������ѭ������
    auto repeatForever = RepeatForever::create(animate);

    this->runAction(repeatForever); // ʹ�� repeatForever ���ж���
    // CCLOG("Finished setting up HoldWall animation");




}//��ǽ����

void Player::playBHoldWallDownAnimation() {

    this->stopAllActions();
    //CCLOG("Starting HoldWall animation");
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 4; i++) {
        std::string frameName = StringUtils::format("Bholdwalldown_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }

    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.1f);
    auto animate = Animate::create(animation);
    // ʹ�� RepeatForever ����ʹ��������ѭ������
    auto repeatForever = RepeatForever::create(animate);

    this->runAction(repeatForever); // ʹ�� repeatForever ���ж���
    // CCLOG("Finished setting up HoldWall animation");





}//��ǽ����

void Player::playBJumpUpAnimation() {//��Ծ
    
     // ������ֵ�״̬
    _jumpMusicState = cocos2d::AudioEngine::getState(_jumpMusicId);

    // �������û�в��Ż��߲����Ѿ���ɣ���ô��ʼ��������(mp3��ʽ)
    if (_jumpMusicState != cocos2d::AudioEngine::AudioState::PLAYING) {
        _jumpMusicId = cocos2d::AudioEngine::play2d("music/jump.mp3", false);
    }
    // ֹͣ�����������еĶ�����ȷ������������������ͻ��
    CCLOG("Starting JumpUp animation");
    this->stopAllActions();
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 3; i++) {
        std::string frameName = StringUtils::format("Bjumpup_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }

    for (int i = 0; i <= 1; i++) {
        std::string frameName = StringUtils::format("BTop_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }
    playFloorJumpAshAnimation();

    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.1f);
    auto animate = Animate::create(animation);
    this->runAction(animate);
    CCLOG("Finished setting up JumpUp animation");

}

void Player::playBJumpMoveAnimation() {//��Ծ�ƶ�
    // ֹͣ�����������еĶ�����ȷ������������������ͻ��
    this->stopAllActions();
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 3; i++) {
        std::string frameName = StringUtils::format("Bjumpmove_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }

    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.1f);
    auto animate = Animate::create(animation);
    this->runAction(animate);

}

void Player::playBHoldWallJumpAnimation() {//��ǽ��Ծ
    // ֹͣ�����������еĶ�����ȷ������������������ͻ��
    CCLOG("Starting HoldWallJump animation");
    this->stopAllActions();
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 3; i++) {
        std::string frameName = StringUtils::format("Bjumpmove_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }

    for (int i = 0; i <= 1; i++) {
        std::string frameName = StringUtils::format("BTop_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }
    playFloorWallJumpAshAnimation();
    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.1f);
    auto animate = Animate::create(animation);
    this->runAction(animate);
    CCLOG("Finished setting up HoldWallJump animation");

}

//��Ч
void Player::playFloorLandingAshAnimation() {
    CCLOG("Starting FloorLandingAsh animation");

    // 1. �����̳�������֡
    Vector<SpriteFrame*> ashFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 13; i++) {
        std::string frameName = StringUtils::format("floorlandingash_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            ashFrames.pushBack(frame);
        }
    }

    // 2. ���������Ͷ���
    auto animation = Animation::createWithSpriteFrames(ashFrames, 0.05f);
    auto animate = Animate::create(animation);

    // 3. ����һ���µľ����������̳�����
    auto ashSprite = Sprite::createWithSpriteFrame(ashFrames.front());

    // 4. ���þ����λ������ҵĽ���
    ashSprite->setPosition(this->getPosition() + Vec2(0, -this->getContentSize().height * 0.02f));

    // 5. ��ӵ���ǰ�ĸ��ڵ㲢���Ŷ���
    this->getParent()->addChild(ashSprite, this->getLocalZOrder() - 1);  // ��֤�̳����������
    ashSprite->runAction(Sequence::create(
        animate,
        CallFunc::create([ashSprite]() {
            ashSprite->removeFromParentAndCleanup(true);
            }),
        nullptr
    ));

    CCLOG("Finished setting up FloorLandingAsh animation");
}

void Player::playFloorJumpAshAnimation() {
    CCLOG("Starting FloorJumpAsh animation");

    // 1. �����̳�������֡
    Vector<SpriteFrame*> ashFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 9; i++) {
        std::string frameName = StringUtils::format("floorjumpash_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            ashFrames.pushBack(frame);
        }
    }

    // 2. ���������Ͷ���
    auto animation = Animation::createWithSpriteFrames(ashFrames, 0.05f);
    auto animate = Animate::create(animation);

    // 3. ����һ���µľ����������̳�����
    auto ashSprite = Sprite::createWithSpriteFrame(ashFrames.front());

    // 4. ���þ����λ������ҵĽ���
    ashSprite->setPosition(this->getPosition() + Vec2(0, 20));

    // 5. ��ӵ���ǰ�ĸ��ڵ㲢���Ŷ���
    this->getParent()->addChild(ashSprite, this->getLocalZOrder() - 1);  // ��֤�̳����������
    ashSprite->runAction(Sequence::create(
        animate,
        CallFunc::create([ashSprite]() {
            ashSprite->removeFromParentAndCleanup(true);
            }),
        nullptr
    ));

    CCLOG("Finished setting up FloorJumpAsh animation");
}

void Player::playFloorWallJumpAshAnimation() {
    CCLOG("Starting FloorWallJumpAsh animation");

    // 1. �����̳�������֡
    Vector<SpriteFrame*> ashFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 9; i++) {
        std::string frameName = StringUtils::format("walljumpash_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            ashFrames.pushBack(frame);
        }
    }

    // 2. ���������Ͷ���
    auto animation = Animation::createWithSpriteFrames(ashFrames, 0.05f);
    auto animate = Animate::create(animation);

    // 3. ����һ���µľ����������̳�����
    auto ashSprite = Sprite::createWithSpriteFrame(ashFrames.front());

    // 4. ���þ����λ������ҵ���ǰ
    ashSprite->setPosition(this->getPosition() + facingDirection * Vec2(40, 0));

    // 5. ��ӵ���ǰ�ĸ��ڵ㲢���Ŷ���
    this->getParent()->addChild(ashSprite, this->getLocalZOrder() - 1);  // ��֤�̳����������
    ashSprite->runAction(Sequence::create(
        animate,
        CallFunc::create([ashSprite]() {
            ashSprite->removeFromParentAndCleanup(true);
            }),
        nullptr
    ));

    CCLOG("Finished setting up FloorWallJumpAsh animation");
}