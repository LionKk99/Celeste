#include "Player.h"
USING_NS_CC;

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

bool Player::isOnSolidGround() {
    float rayLength = 1.0f; // ���߳��ȹ̶�Ϊ1����
    Vec2 startPoint = this->getPosition() - Vec2(0, this->getContentSize().height * 0.5f); // ��ɫ�ĵײ����ĵ�
    Vec2 endPoint = startPoint - Vec2(0, rayLength); // ��startPoint�����ӳ�1����
    bool onSolidGround = false; // Ĭ������¼�����Ҳ��ڵ�����

    auto func = [&onSolidGround](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data) -> bool {
        if (info.shape->getBody()->getNode()->getName() == "ground") {
            onSolidGround = true;
            return false; // ֹͣ���߼��
        }
        return true; // �������߼��
        };

    Director::getInstance()->getRunningScene()->getPhysicsWorld()->rayCast(func, startPoint, endPoint, nullptr);

    return onSolidGround;
}



void Player::changeState(PlayerState newState) {
    if (currentState == newState) return;
    previousState = currentState;
    currentState = newState;
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
        playJumpUpAnimation();
        break;
    case PlayerState::DROP:
        playDropAnimation();
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

    CCLOG("canClimb: %d", canClimb);
    return adjustedPosition;
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
    float verticalVelocity = this->getPhysicsBody()->getVelocity().y;//���������е�vy
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
    if (keyStates[PlayerKey::DOWN] && isOnGround&&!keyStates[PlayerKey::DASH]) {
        changeState(PlayerState::CROUCH);
        CCLOG("Player state changed to CROUCH");
        return;
    }
    if (keyStates[PlayerKey::UP] && isOnGround && !keyStates[PlayerKey::DASH]&&!keyStates[PlayerKey::DOWN]){
        changeState(PlayerState::LOOKUP);
        CCLOG("Player state changed to LOOKUP");
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
        this->getPhysicsBody()->applyImpulse(Vec2(0, 40));//ʹ�ó���
        isOnGround = false;        

        changeState(PlayerState::JUMPING);
        return;
    }

    //����
    
    if (!isOnGround && verticalVelocity < 0) {
        changeState(PlayerState::DROP);
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
                return;
            }
            else if(keyStates[PlayerKey::LEFT]){ 
                changeState(PlayerState::MOVING_TURN_RL);
                return;
            }
        }
        else if (velocity.x < 0) {
            if (keyStates[PlayerKey::LEFT]) {
                changeState(PlayerState::MOVING_LEFT);
                return;
            }
            else if (keyStates[PlayerKey::RIGHT]) {
                changeState(PlayerState::MOVING_TURN_LR);
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
        CCLOG("on groud");
    }
    else {
        CCLOG("not on groud");
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
    case EventKeyboard::KeyCode::KEY_T:
        keyStates[PlayerKey::TALK] = false;
        break;
    case EventKeyboard::KeyCode::KEY_D:
        keyStates[PlayerKey::DASH] = false;
        break;
    case EventKeyboard::KeyCode::KEY_C:
        keyStates[PlayerKey::CLIMB] = false;
        break;
    default:
        break;
    }
}



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














