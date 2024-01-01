#include "Player.h"
#include"Trap/jumpTable.h"
#include"Trap/brick.h"
USING_NS_CC;
const float Player::DASH_DURATION = 0.5f;//冲刺总时间，包括后摇
 
int Player::currentLevel = 1;//初始化全局变量currentLevel为1
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
    //初始化重生点位置
   
    
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

bool Player::init(const std::string& filename) {//初始化角色
    if (!Sprite::initWithFile(filename)) {
        return false;
    }
    //currentState = PlayerState::IDLE;
    //previousState = PlayerState::IDLE;    
    isDashing = false;
    dashTimer = 0.0f;
    //初始化测试射线
    _debugDrawNode = cocos2d::DrawNode::create();
    this->addChild(_debugDrawNode);


    this->setName("player");        

    float scale_factor = 0.8f;  // 缩放因子，您可以根据需要调整
    this->setScale(scale_factor);

    

    // 使用lambda函数替代schedule_selector来每帧调用update方法
    this->schedule([this](float delta) {
        this->update(delta);
        }, "update_key");

    // 创建玩家的物理体
    auto playerBodySize = this->getContentSize() * 0.5;  // 考虑缩放因子
    auto playerShape = PhysicsShapeBox::create(playerBodySize);
    playerShape->setFriction(1.0f);  // 设置摩擦系数
    playerShape->setRestitution(0.0f);  // 设置反弹系数为0，确保没有重合
    auto playerBody = PhysicsBody::create();
    playerBody->addShape(playerShape);
    playerBody->setDynamic(true);
    playerBody->setRotationEnable(false);  // 禁止旋转
    playerBody->setLinearDamping(0.5f); // 减少水平滑动
    // 设置类别掩码和碰撞掩码以确保回调方法被调用
    playerBody->setCategoryBitmask(0x02);
    playerBody->setCollisionBitmask(0x01);  // 可以与分类为0x01的物体发生碰撞
    playerBody->setContactTestBitmask(0xFFFFFFFF);
    
    this->setPhysicsBody(playerBody);   
   
    return true;
}

/*
bool Player::isOnSolidGround() {
    float rayLength = 1.0f; // 射线长度固定为1像素
    Vec2 centerPoint = this->getPosition();

    // 获取角色的大小
    cocos2d::Size playerSize = this->getContentSize();

    // 定义射线的起始点
    Vec2 leftStartPoint = centerPoint - Vec2(playerSize.width * 0.5f-42, playerSize.height * 0.5f - 47);
    Vec2 middleStartPoint = centerPoint - Vec2(0, playerSize.height * 0.5f - 47);
    Vec2 rightStartPoint = centerPoint + Vec2(playerSize.width * 0.5f-42, -playerSize.height * 0.5f + 47);

    std::vector<cocos2d::Vec2> startPoints = { leftStartPoint, middleStartPoint, rightStartPoint };

    bool onSolidGround = false; // 默认情况下假设玩家不在地面上

    // 获取或创建一个 DrawNode
    auto drawNode = dynamic_cast<cocos2d::DrawNode*>(this->getChildByName("RayDrawNode"));
    if (!drawNode) {
        drawNode = cocos2d::DrawNode::create();
        drawNode->setName("RayDrawNode");
        this->addChild(drawNode);
    }
    drawNode->clear();  // 清除之前的绘制

    for (const auto& startPoint : startPoints) {
        cocos2d::Vec2 endPoint = startPoint - cocos2d::Vec2(0, rayLength); // 从startPoint向下延长1像素

        // 绘制射线
        drawNode->drawLine(startPoint, endPoint, cocos2d::Color4F::BLUE);

        auto func = [&onSolidGround](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data) -> bool {
            if (info.shape->getBody()->getNode()->getName() == "ground") {
                onSolidGround = true;
                return false; // 停止射线检测
            }
            return true; // 继续射线检测
            };

        Director::getInstance()->getRunningScene()->getPhysicsWorld()->rayCast(func, startPoint, endPoint, nullptr);

        if (onSolidGround) {
            break; // 如果检测到玩家在地面上，跳出循环
        }
    }

    return onSolidGround;
}
*/

bool Player::isOnSolidGround() {//优化后判定
    float rayLength = 1.0f; // 射线长度固定为1像素
    Vec2 centerPoint = this->getPosition();

    // 获取角色的大小
    cocos2d::Size playerSize = this->getContentSize();

    // 定义射线的起始点
    Vec2 leftStartPoint = centerPoint - Vec2(playerSize.width * 0.5f - 42, playerSize.height * 0.5f - 47);
    Vec2 middleStartPoint = centerPoint - Vec2(0, playerSize.height * 0.5f - 47);
    Vec2 rightStartPoint = centerPoint + Vec2(playerSize.width * 0.5f - 42, -playerSize.height * 0.5f + 47);

    std::vector<cocos2d::Vec2> startPoints = { leftStartPoint, middleStartPoint, rightStartPoint };

    bool onSolidGround = false; // 默认情况下假设玩家不在地面上

    for (const auto& startPoint : startPoints) {
        cocos2d::Vec2 endPoint = startPoint - cocos2d::Vec2(0, rayLength); // 从startPoint向下延长1像素

        auto func = [&onSolidGround](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data) -> bool {
            if (info.shape->getBody()->getNode()->getName() == "ground"|| info.shape->getBody()->getNode()->getName() == "brick") {
                onSolidGround = true;
                return false; // 停止射线检测
            }
            return true; // 继续射线检测
            };

        Director::getInstance()->getRunningScene()->getPhysicsWorld()->rayCast(func, startPoint, endPoint, nullptr);

        if (onSolidGround) {
            break; // 如果检测到玩家在地面上，跳出循环
        }
    }

    return onSolidGround;
}


void Player::changeState(PlayerState newState) {
    if (currentState == newState) return;
    previousState = currentState;
    currentState = newState;
    //音频控制
    if (previousState== PlayerState::MOVING_RIGHT|| previousState == PlayerState::MOVING_LEFT) {
        cocos2d::AudioEngine::stop(_walkMusicId);
    }
    //状态控制
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
            playCrouchToIdleAnimation();//过度动画（在动画调用完成后更新状态）            
        }
        else {
            playIdleAnimation_1();//默认状态
        }        
        break;
    case PlayerState::MOVING_LEFT:
        playMoveAnimation();
        this->setScaleX(-1.0f); // 镜像动画以表示左走
        facingDirection = -1;//面向左边
        break;
    case PlayerState::MOVING_RIGHT:
        playMoveAnimation();
        this->setScaleX(1.0f);  // 正常动画表示右走
        facingDirection = 1;//面向右边
        break;
    case PlayerState::MOVING_TURN_RL:
        playMoveTurnAnimation();
        this->setScaleX(-1.0f); // 镜像动画以表示左向右走
        break;
    case PlayerState::MOVING_TURN_LR:
        playMoveTurnAnimation();
        this->setScaleX(1.0f);  // 正常动画表示从右向左走
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
        // 处理其他状态...
    }
}

//原始方法
/*

cocos2d::Vec2 Player::adjustMovePosition(const cocos2d::Vec2& desiredPosition) {//此函数用于优化位置设置防止穿模，也用于判定canClimb
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


cocos2d::Vec2 Player::adjustMovePosition(const cocos2d::Vec2& desiredPosition) {//此函数用于优化位置设置防止穿模，也用于判定canClimb
    cocos2d::Vec2 adjustedPosition = desiredPosition;                           //相比于之前的单射线（中心位置），优化成双射线（最上，最下）
    float rayLength = 40.0f;

    std::vector<cocos2d::Vec2> directions = {
        cocos2d::Vec2(-1, 0),
        cocos2d::Vec2(1, 0),
        cocos2d::Vec2(0, 1)
    };
    // 中心点
    cocos2d::Vec2 centerPoint = this->getPosition();

    // 获取角色的大小
    cocos2d::Size playerSize = this->getContentSize();

    // 定义射线的起点
    cocos2d::Vec2 bottomCenterPoint = centerPoint - cocos2d::Vec2(0, playerSize.height * 0.5f) + cocos2d::Vec2(0, 49.0f);
    cocos2d::Vec2 topCenterPoint = centerPoint + cocos2d::Vec2(0, playerSize.height * 0.5f) - cocos2d::Vec2(0, 49.0f);

    // 将三个起始点添加到一个列表中
    std::vector<cocos2d::Vec2> startPoints = { bottomCenterPoint, centerPoint, topCenterPoint };

    for (const auto& dir : directions) {
        bool collisionDetected = false;
        std::string collidedObjectName = "";

        for (const auto& startPoint : startPoints) {
            if (dir.x != 0 && startPoint == centerPoint) continue; // 为中心点跳过垂直方向的射线

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

            // 如果检测到碰撞，跳出内部循环
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
    float rayLength = 40.0f; // 假设的射线长度，可以根据需要进行调整

    // 只在向下方向检测
    cocos2d::Vec2 direction = cocos2d::Vec2(0, -1); // Down

    // 获取角色的中心点和底部中心点
    cocos2d::Vec2 centerPoint = this->getPosition();
    cocos2d::Size playerSize = this->getContentSize();
    cocos2d::Vec2 bottomCenterPoint = centerPoint - cocos2d::Vec2(0, playerSize.height * 0.5f);

    // 射线的结束点
    cocos2d::Vec2 endPoint = bottomCenterPoint + direction * rayLength;
    bool collisionDetected = false;

    auto rayCallback = [this, &collisionDetected](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data) -> bool {
        if (info.shape->getBody()->getNode()->getName() == "JumpTable") {
            collisionDetected = true;
            // 获取 JumpTable 对象并调用它的播放动画方法
            auto jumpTable = dynamic_cast<JumpTable*>(info.shape->getBody()->getNode());
            if (jumpTable) {
                if(!jumpTable->_canBeActivated){ return false; }
                jumpTable->playAnimation(); // 调用 JumpTable 的 playAnimation 方法
                jumpTable->deactivateTemporarily();//设置时间间隔
                CCLOG("弹簧动画播放");
             }
            return false;  // 停止进一步检测
        }
        return true; // 继续检测
        };

    Director::getInstance()->getRunningScene()->getPhysicsWorld()->rayCast(rayCallback, bottomCenterPoint, endPoint, nullptr);

    return collisionDetected; // 返回是否检测到 JumpTable
}

bool Player::checkForBrickInteraction() {
    float rayLength = 20.0f; // 假设的射线长度，可以根据需要进行调整

    // 只在向下方向检测
    cocos2d::Vec2 direction = cocos2d::Vec2(0, -1); // Down

    // 获取角色的中心点和底部中心点
    cocos2d::Vec2 centerPoint = this->getPosition();
    cocos2d::Size playerSize = this->getContentSize();
    cocos2d::Vec2 bottomCenterPoint = centerPoint - cocos2d::Vec2(0, playerSize.height * 0.5f);

    // 射线的结束点
    cocos2d::Vec2 endPoint = bottomCenterPoint + direction * rayLength;
    bool collisionDetected = false;

    auto rayCallback = [this, &collisionDetected](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data) -> bool {
        if (info.shape->getBody()->getNode()->getName() == "brick") {
            collisionDetected = true;
            // 获取 brick 对象并调用它的播放动画方法
            auto brick = dynamic_cast<Brick*>(info.shape->getBody()->getNode());
            if (brick) {
                if (!brick->_isNormal) { return false; }
                brick->toggleVisibility();                
                CCLOG("砖块悬空");
            }
            return false;  // 停止进一步检测
        }
        return true; // 继续检测
        };

    Director::getInstance()->getRunningScene()->getPhysicsWorld()->rayCast(rayCallback, bottomCenterPoint, endPoint, nullptr);

    return collisionDetected; // 返回是否检测到 JumpTable
}

//备用三射线
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
    if (!isAlive) { return; }//角色死亡直接返回
    if (isAlive) { if (checkForSpikeweedCollision()) {} }
    float deathThreshold = 75; // 你想要的死亡阈值
    if (this->getPositionY() < deathThreshold) {//检查角色高度
        // 进入死亡状态
        isAlive = 0;
        changeState(PlayerState::DYING);
    }
    if (isDashing) {
        dashTimer += dt;
        if (dashTimer >= DASH_DURATION) {
            isDashing = false;
            dashTimer = 0.0f;
        }
        return;  // 如果玩家正在冲刺，跳过所有其他的状态更新
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

    float verticalVelocity = this->getPhysicsBody()->getVelocity().y;//物理引擎中的vy
    float horizontalVelocity = this->getPhysicsBody()->getVelocity().x;//物理引擎中的vx

    
    //新方法
    Vec2 desiredPosition = this->getPosition() + velocity * dt;
    Vec2 adjustedPosition = adjustMovePosition(desiredPosition);

    this->setPosition(adjustedPosition);
    //旧方法
    //this->setPositionX(this->getPositionX() + velocity.x * dt);
    //this->setPositionY(this->getPositionY() + velocity.y * dt);
    
    //检查玩家是否在坚实的地面上
    bool onGround = isOnSolidGround();
    setOnGround(onGround);
    if (onGround) {
        canDash = 1;
    }

   
    /* 如果直接设置速度的话要用到下面代码
    // 如果玩家在地面上，设置垂直速度为0
    if (onGround && !keyStates[PlayerKey::JUMP]) {
        velocity.y = velocity.y - 15;
        if (velocity.y < 0)
        {
            velocity.y = 0;
        }
    }
    */
    // 添加日志输出
   // CCLOG("isOnGround: %d, verticalVelocity: %f, jumpKeyState: %d", onGround, verticalVelocity, keyStates[PlayerKey::JUMP]);
   

    //蹲姿
    if (keyStates[PlayerKey::DOWN] && isOnGround&&!keyStates[PlayerKey::DASH] && !keyStates[PlayerKey::CLIMB]&&velocity.x==0){
        changeState(PlayerState::CROUCH);
        CCLOG("Player state changed to CROUCH");
        return;
    }
    //向上看
    if (keyStates[PlayerKey::UP] && isOnGround && !keyStates[PlayerKey::DASH]&&!keyStates[PlayerKey::DOWN]&&!keyStates[PlayerKey::CLIMB] && velocity.x == 0) {
        changeState(PlayerState::LOOKUP);
        CCLOG("Player state changed to LOOKUP");
        return;
    }    

    //冲刺(默认)
    if (keyStates[PlayerKey::DASH] && canDash  ) {
        this->getPhysicsBody()->setGravityEnable(false);
        canDash = 0;
        velocity.x = 0; velocity.y = 0;
        this->getPhysicsBody()->setVelocity(Vec2(0, 0));
        changeState(PlayerState::DASH);
        CCLOG("Player state changed to DASH");
        return;
    }


    //落地
    if ((previousState == PlayerState::DROP && isOnGround)||currentState== PlayerState::LANDING) {
        changeState(PlayerState::LANDING);        
        //CCLOG("Player state changed to LANDING");
        return;
    }
    //推墙
    if (((keyStates[PlayerKey::RIGHT] && velocity.x > 0) || (keyStates[PlayerKey::LEFT] && velocity.x < 0)) && isOnGround && canClimb &&!keyStates[PlayerKey::CLIMB]) {
        changeState(PlayerState::PUSHWALL);
        CCLOG("Player state changed to PUSHWALL");
        return;
    }
    //下落

    if (!isOnGround && verticalVelocity < 0) {
        changeState(PlayerState::DROP);
        return;
    }
    //弹簧
    
    if (checkForJumpTableInteraction()) {
        this->getPhysicsBody()->setGravityEnable(true);
        //velocity.y = 0;
        this->getPhysicsBody()->applyImpulse(Vec2(0, 100));//使用冲量
    }
    //砖块
    checkForBrickInteraction();
    //爬墙跳跃
    if ((currentState == PlayerState::HOLDWALL || currentState == PlayerState::HOLDWALLUP || currentState == PlayerState::HOLDWALLDOWN) && keyStates[PlayerKey::JUMP]) {
        this->getPhysicsBody()->setGravityEnable(true);
        velocity.y = 0;
        this->getPhysicsBody()->applyImpulse(Vec2(-30, 90) * facingDirection);//使用冲量
        changeState(PlayerState::HOLDWALLJUMP);
        CCLOG("Player state changed to HOLDWALLJUMP");
        return;
    }

    //爬墙
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
  
    //水平移动
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
        // 如果没有按键被按下，减速到0
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
        //velocity.y = JUMP_SPEED;//直接设置速度
        this->getPhysicsBody()->applyImpulse(Vec2(0, 350));//使用冲量
        isOnGround = false;        

        changeState(PlayerState::JUMPING);
        return;
    }

    
    

    //状态转换
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
    
   

    // ... 其他代码 ...
}





void Player::setOnGround(bool a) {
    this->isOnGround = a;//设置isOnGround状态
    if (a)
    {
        //CCLOG("on groud");
    }
    else {
        //CCLOG("not on groud");
    }
}



bool Player::checkPlayerInput() {//无按键时返回0，有按键时返回1
    // 如果没有键被按下
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
        // 假设使用空格键代表跳跃
    case EventKeyboard::KeyCode::KEY_SPACE:
        //CCLOG("press space");
        keyStates[PlayerKey::JUMP] = true;
        break;
        // 假设使用J键代表交谈
    case EventKeyboard::KeyCode::KEY_J:
        keyStates[PlayerKey::TALK] = true;
        break;
        // 假设使用SHIFT键代表冲刺
    case EventKeyboard::KeyCode::KEY_SHIFT:
        keyStates[PlayerKey::DASH] = true;
        break;
        // 假设使用K键代表攀爬
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
        isJumping = false; // 当按键释放时，允许下一次跳跃
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

//音频和动画结合

void Player::playIdleAnimation_1() {//站立1    
    // 停止所有正在运行的动画（确保不会与其他动画冲突）
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
    // 使用 RepeatForever 动作使动画无限循环播放
    auto repeatForever = RepeatForever::create(animate);

    this->runAction(repeatForever); // 使用 repeatForever 运行动画
   // CCLOG("Finished setting up IDLE animation");
}

void Player::playIdleAnimation_2() {//站立2
    // 停止所有正在运行的动画（确保不会与其他动画冲突）
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

void Player::playMoveAnimation() {//移动 
    // 停止所有正在运行的动画（确保不会与其他动画冲突）
    //音频
    _walkMusicId = cocos2d::AudioEngine::play2d("music/walk.mp3", true);//声音很奇怪
    cocos2d::AudioEngine::setVolume(_walkMusicId, 1.0f);  // 设置音量
    //动画
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
    // 使用 RepeatForever 动作使动画无限循环播放
    auto repeatForever = RepeatForever::create(animate);
    this->runAction(repeatForever);

}

void Player::playMoveTurnAnimation() {//转向 10帧 
    // 停止所有正在运行的动画（确保不会与其他动画冲突）
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
   
    // 停止所有正在运行的动画（确保不会与其他动画冲突）
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
    this->setSpriteFrame(idleFrames.at(6));   //用一次动画就够了
    
    CCLOG("Finished setting up Crouch animation");
}

void Player::playCrouchToIdleAnimation() {

    // 停止所有正在运行的动画（确保不会与其他动画冲突）
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

    // 定义lambda作为回调
    auto callback = [this]() {
        previousState = PlayerState::IDLE;//更新状态保证不重复播放过渡动画
        playIdleAnimation_1();//由于状态机机制，手动调用
        CCLOG("Animation completed!");
        // 这里是您的状态更新等操作
        };

    // 创建回调动作
    auto callbackAction = CallFunc::create(callback);

    // 使用 Sequence 动作将动画和回调结合起来
    auto sequence = Sequence::create(animate, callbackAction, nullptr);

    this->runAction(sequence);

    CCLOG("Finished setting up CrouchToIdle animation");
}

void Player::playLookUpAnimation() {//向上看 
    // 停止所有正在运行的动画（确保不会与其他动画冲突）
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
    this->setSpriteFrame(idleFrames.at(5));//用一次动画就够了
    CCLOG("Finished setting up LookUp animation");
}

void Player::playJumpUpAnimation() {//跳跃
    
     // 检查音乐的状态
    _jumpMusicState = cocos2d::AudioEngine::getState(_jumpMusicId);

    // 如果音乐没有播放或者播放已经完成，那么开始播放音乐(mp3格式)
    if (_jumpMusicState != cocos2d::AudioEngine::AudioState::PLAYING) {
        _jumpMusicId = cocos2d::AudioEngine::play2d("music/jump.mp3", false);
    }
    // 停止所有正在运行的动画（确保不会与其他动画冲突）

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

void Player::playJumpMoveAnimation() {//跳跃移动
    // 停止所有正在运行的动画（确保不会与其他动画冲突）
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

void Player::playDropAnimation() {//坠落
    // 停止所有正在运行的动画（确保不会与其他动画冲突）
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

void Player::playPushWallAnimation() {//推墙    
    // 停止所有正在运行的动画（确保不会与其他动画冲突）
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
    // 使用 RepeatForever 动作使动画无限循环播放
    auto repeatForever = RepeatForever::create(animate);

    this->runAction(repeatForever); // 使用 repeatForever 运行动画
    // CCLOG("Finished setting up PushWall animation");
}

void Player::playLandingAnimation() {//过渡动画 落地

    _landingMusicState = cocos2d::AudioEngine::getState(_landingMusicId);

    // 如果音乐没有播放或者播放已经完成，那么开始播放音乐(mp3格式)
    if (_landingMusicState != cocos2d::AudioEngine::AudioState::PLAYING) {
        _landingMusicId = cocos2d::AudioEngine::play2d("music/landing.mp3", false);
    }

    // 停止所有正在运行的动画（确保不会与其他动画冲突）
    this->stopAllActions();
    //限制落地速度
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

    // 定义lambda作为回调
    auto callback = [this]() {
        previousState = PlayerState::LANDING;
        currentState = PlayerState::IDLE;//更新状态保证不重复播放过渡动画
        playIdleAnimation_1();//由于状态机机制，手动调用
        CCLOG("Animation completed!");
        
        };

    // 创建回调动作
    auto callbackAction = CallFunc::create(callback);

    // 使用 Sequence 动作将动画和回调结合起来
    auto sequence = Sequence::create(animate, callbackAction, nullptr);

    this->runAction(sequence);

    CCLOG("Finished setting up CrouchToIdle animation");
}

void Player::playHoldWallAnimation(){//爬墙
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
    // 使用 RepeatForever 动作使动画无限循环播放
    auto repeatForever = RepeatForever::create(animate);

    this->runAction(repeatForever); // 使用 repeatForever 运行动画
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
    // 使用 RepeatForever 动作使动画无限循环播放
    auto repeatForever = RepeatForever::create(animate);

    this->runAction(repeatForever); // 使用 repeatForever 运行动画
    // CCLOG("Finished setting up HoldWall animation");




}//爬墙向上

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
    // 使用 RepeatForever 动作使动画无限循环播放
    auto repeatForever = RepeatForever::create(animate);

    this->runAction(repeatForever); // 使用 repeatForever 运行动画
    // CCLOG("Finished setting up HoldWall animation");





}//爬墙向下

void Player::playHoldWallJumpAnimation() {//爬墙跳跃
    // 停止所有正在运行的动画（确保不会与其他动画冲突）
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

    //音频
    
    // 检查音乐的状态
    _dashMusicState = cocos2d::AudioEngine::getState(_dashMusicId);

    // 如果音乐没有播放或者播放已经完成，那么开始播放音乐(mp3格式)
    if (_dashMusicState != cocos2d::AudioEngine::AudioState::PLAYING) {
        _dashMusicId = cocos2d::AudioEngine::play2d("music/dash.mp3", false);
    }
    
    //动画

    isDashing = true;
    this->stopAllActions();

    // 获取冲刺方向
    cocos2d::Vec2 dashDirection = getDashDirection();

    // 开始冲刺
    this->startDashing(dashDirection);  // 传递冲刺方向到 startDashing 函数

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

    // 创建一个CallFunc动作，在动画达到特定帧时生成shadow效果
    auto generateShadow = CallFunc::create([this, cache]() {
        auto shadow = Sprite::createWithSpriteFrameName("shadow_00.png");
        shadow->setScale(0.80);
        // 根据facingDirection翻转shadow
        if (this->facingDirection == -1) {
            shadow->setFlippedX(true);
        }

        shadow->setPosition(this->getPosition());
        this->getParent()->addChild(shadow, this->getLocalZOrder() - 1);

        // 设置shadow的持续时间和消失效果
        auto fadeOut = FadeOut::create(0.7f);
        auto remove = CallFunc::create([shadow]() {
            shadow->removeFromParent();
            });
        auto shadowSequence = Sequence::create(fadeOut, remove, nullptr);
        shadow->runAction(shadowSequence);
        });

    // 在动画的特定时间调用generateShadow
    auto delayForFirstShadow = DelayTime::create(0.2f);  // 0.05 * 4
    auto delayForSecondShadow = DelayTime::create(0.1f);  // 0.05 * 2
    auto shadowSequence = Sequence::create(delayForFirstShadow, generateShadow->clone(), delayForSecondShadow, generateShadow->clone(), nullptr);

    // 结束冲刺后重启重力
    auto enableGravity = CallFunc::create([this]() {
        this->getPhysicsBody()->setGravityEnable(true);
       // this->isDashing = false;
        });
    auto sequence = Sequence::create(animate, enableGravity, nullptr);

    // 运行两个动作
    this->runAction(sequence);
    this->runAction(shadowSequence);
}
//新方法，去除多段检测，先一次性计算出合适的移动距离
void Player::startDashing(const cocos2d::Vec2& dashDirection) {
    float rayLength = 220.0f;  // 冲刺的射线长度

    // 中心点
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
        // 计算玩家与碰撞点之间的距离
        float distance = centerPoint.distance(collisionPoint) - 40; // 减少40像素以防止碰撞
        endPoint = centerPoint + dashDirection.getNormalized() * distance;
    }

    // 计算所需的时间
    float time = rayLength / 500.0f;  // 500像素每秒
    auto moveAction = cocos2d::MoveTo::create(time, endPoint);
   
    // 确保总的冲刺时间为0.4秒
    float remainingTime = 0.4f - time;
    auto delayAction = cocos2d::DelayTime::create(remainingTime > 0 ? remainingTime : 0);

    // 创建一个动作序列
    auto sequence = cocos2d::Sequence::create(moveAction, delayAction, nullptr);
    this->runAction(sequence);
}

/*
void Player::startDashing(const cocos2d::Vec2& dashDirection) {
    int segments = 11;  // 冲刺段数
    float segmentLength = 20.0f;  // 每段的长度

    // 使用schedule方法进行冲刺
    this->schedule([this, segments, segmentLength, dashDirection](float dt) mutable {  // 这里添加了dashDirection
        // 计算期望的位置
        cocos2d::Vec2 desiredPosition = this->getPosition() + dashDirection * segmentLength;  // 使用dashDirection

        // 使用adjustMovePosition函数来检查是否有障碍物
        cocos2d::Vec2 adjustedPosition = this->adjustMovePosition(desiredPosition);

        // 计算移动的差值
        cocos2d::Vec2 moveByValue = adjustedPosition - this->getPosition();

        // 使用MoveBy来平滑移动玩家
        auto moveAction = cocos2d::MoveBy::create(0.1f, moveByValue);
        this->runAction(moveAction);

        // 递减segments
        --segments;

        // 如果检测到障碍物或冲刺已完成，则停止冲刺
        if (segments <= 0) {
            this->unschedule("DashingKey");
        }
        }, 0.04f, "DashingKey");
}
*/

cocos2d::Vec2 Player::getDashDirection() {
    // 使用成员变量来检查按键状态
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
        // 如果没有按下其他方向键，则使用默认方向
        direction.x = facingDirection;
    }

    return direction.getNormalized();  // 返回归一化的方向向量
}

void Player::playDashUpAndDownAnimation() {

    // 检查音乐的状态
    _dashMusicState = cocos2d::AudioEngine::getState(_dashMusicId);

    // 如果音乐没有播放或者播放已经完成，那么开始播放音乐(mp3格式)
    if (_dashMusicState != cocos2d::AudioEngine::AudioState::PLAYING) {
        _dashMusicId = cocos2d::AudioEngine::play2d("music/dash.mp3", false);
    }

    isDashing = true;
    this->stopAllActions();

    // 获取冲刺方向
    cocos2d::Vec2 dashDirection = getDashDirection();

    // 开始冲刺
    this->startDashing(dashDirection);  // 传递冲刺方向到 startDashing 函数

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

    // 创建一个CallFunc动作，在动画达到特定帧时生成shadow效果
    auto generateShadow = CallFunc::create([this, cache]() {
        auto shadow = Sprite::createWithSpriteFrameName("shadow_00.png");
        // 根据facingDirection翻转shadow
        if (this->facingDirection == -1) {
            shadow->setFlippedX(true);
        }

        shadow->setPosition(this->getPosition());
        this->getParent()->addChild(shadow, this->getLocalZOrder() - 1);

        // 设置shadow的持续时间和消失效果
        auto fadeOut = FadeOut::create(0.5f);
        auto remove = CallFunc::create([shadow]() {
            shadow->removeFromParent();
            });
        auto shadowSequence = Sequence::create(fadeOut, remove, nullptr);
        shadow->runAction(shadowSequence);
        });

    // 在动画的特定时间调用generateShadow
    auto delayForFirstShadow = DelayTime::create(0.20f);  // 0.05 * 4
    auto delayForSecondShadow = DelayTime::create(0.10f);  // 0.05 * 2
    auto shadowSequence = Sequence::create(delayForFirstShadow, generateShadow->clone(), delayForSecondShadow, generateShadow->clone(), nullptr);

    // 结束冲刺后重启重力
    auto enableGravity = CallFunc::create([this]() {
        this->getPhysicsBody()->setGravityEnable(true);
        //this->isDashing = false;
        });
    auto sequence = Sequence::create(animate, enableGravity, nullptr);

    // 运行两个动作
    this->runAction(sequence);
    this->runAction(shadowSequence);
}

void Player::playRespawnAnimation() {//复活动画   
  
    // 检查音乐的状态
    _reviveMusicState = cocos2d::AudioEngine::getState(_reviveMusicId);
    
    // 如果音乐没有播放或者播放已经完成，那么开始播放音乐(mp3格式)
    if (_reviveMusicState != cocos2d::AudioEngine::AudioState::PLAYING) {
        _reviveMusicId = cocos2d::AudioEngine::play2d("music/revive.mp3", false);
    }

    CCLOG("Starting Respawn animation");
    
    // 创建一个新的Sprite对象用于播放重生动画
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
        this->setVisible(true); // 角色重新可见
        this->getPhysicsBody()->setEnabled(true); // 重新启用物理体
        this->isAlive = true; // 设置角色状态为活着
        respawnSprite->removeFromParent();
        //最后放玩家
        // 设置缩放因子为0.80
        this->setScale(0.80);
        velocity = Vec2::ZERO;
        this->getPhysicsBody()->setVelocity(Vec2::ZERO);
        // 设置玩家为可见    
        this->getPhysicsBody()->setGravityEnable(true);
        // 开启碰撞
        this->getPhysicsBody()->setEnabled(true);
        // 设置角色的位置
        cocos2d::Vec2 respawnPosition = respawnPoints[currentLevel]; // 重生点位置 这里需改为关卡的重生点
        this->setPosition(respawnPosition);
        });

    auto sequence = Sequence::create(animate, setPlayerAlive, nullptr);
    respawnSprite->runAction(sequence);

    CCLOG("Finished setting up Respawn animation");

    
}

/*
void Player::playDeathAnimation() {//死亡
    
    //取消碰撞体积
    this->getPhysicsBody()->setEnabled(false);
    // 设置角色的位置
    cocos2d::Vec2 respawnPosition = respawnPoints[currentLevel]; // 重生点位置 这里需改为关卡的重生点
    this->setPosition(respawnPosition);
    // 检查音乐的状态
    _deathMusicState = cocos2d::AudioEngine::getState(_deathMusicId);

    // 如果音乐没有播放或者播放已经完成，那么开始播放音乐(mp3格式)
    if (_deathMusicState != cocos2d::AudioEngine::AudioState::PLAYING) {
        _deathMusicId = cocos2d::AudioEngine::play2d("music/death.mp3", false);
    }
   
    
    
    // Set the player's velocity to zero
    this->getPhysicsBody()->setVelocity(Vec2::ZERO);
    velocity = Vec2::ZERO;
    // Disable gravity for the player
    this->getPhysicsBody()->setGravityEnable(false);
    // Set the player's Y position to 100    
    if (this->getPositionY() < 100) {this->setPositionY(getPositionY()+100);}//动画看得清
    //this->setPositionX(500);//动画看得清

    // 设置缩放因子为0.80
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

    // 在Death动画结束后调用playBlackAnimation
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
    // 获取player当前的位置
    cocos2d::Vec2 currentPosition = this->getPosition();
    // 设置玩家为不可见
    this->setVisible(false);
    // 禁用物理体以防止碰撞
    this->getPhysicsBody()->setEnabled(false);
    //
    velocity = Vec2::ZERO;
    this->getPhysicsBody()->setVelocity(Vec2::ZERO);
    this->getPhysicsBody()->setGravityEnable(false);
    // 设置角色的位置
    cocos2d::Vec2 respawnPosition = respawnPoints[currentLevel]; // 重生点位置 这里需改为关卡的重生点
    this->setPosition(respawnPosition);
    // 创建一个新的Sprite对象用于播放死亡动画
    auto deathSprite = Sprite::create();
    deathSprite->setPosition(currentPosition);
    deathSprite->setScale(0.80);
    this->getParent()->addChild(deathSprite);

    // 加载死亡动画的所有帧
    Vector<SpriteFrame*> deathFrames;
    auto frameCache = SpriteFrameCache::getInstance();
    for (int i = 0; i <= 23; ++i) {
        auto frameName = StringUtils::format("death_00-%d.png", i);
        auto frame = frameCache->getSpriteFrameByName(frameName);
        if (frame) {
            deathFrames.pushBack(frame);
        }
    }

    // 创建动画并播放
    auto animation = Animation::createWithSpriteFrames(deathFrames, 0.05f);
    auto animate = Animate::create(animation);

    // 在动画播放完成后移除Sprite,打开黑幕
    auto callBlackAnimation = CallFunc::create([this, deathSprite]() {
        this->playBlackAnimation();
        deathSprite->removeFromParent();
        });

    // 播放死亡音乐
    _deathMusicState = cocos2d::AudioEngine::getState(_deathMusicId);
    if (_deathMusicState != cocos2d::AudioEngine::AudioState::PLAYING) {
        _deathMusicId = cocos2d::AudioEngine::play2d("music/death.mp3", false);
    }

    // 执行动画和移除自身的序列动作
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

    auto blackSprite = Sprite::createWithSpriteFrame(idleFrames.front());  // 使用第一帧初始化
    blackSprite->setPosition(Director::getInstance()->getVisibleSize() / 2);  // 设置为屏幕中心

    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.05f);
    auto animate = Animate::create(animation);
    auto reverseAnimate = animate->reverse();

    // 在Black动画结束后调用playRespawnAnimation
    auto callRespawnAnimation = CallFunc::create([this]() {
        this->playRespawnAnimation();
        });

    auto removeBlackSprite = CallFunc::create([blackSprite]() {
        blackSprite->removeFromParent();
        });

    auto finalSequence = Sequence::create(reverseAnimate, animate->clone(), callRespawnAnimation, removeBlackSprite, nullptr);

    blackSprite->runAction(finalSequence);
    this->getParent()->addChild(blackSprite, this->getLocalZOrder() + 1);  // 添加到父场景，确保其位于玩家之上
    CCLOG("Finished setting up Black animation");
}
//B动作
/*
void Player::playBDeathAnimation() {//死亡

    _deathMusicState = cocos2d::AudioEngine::getState(_deathMusicId);

    // 如果音乐没有播放或者播放已经完成，那么开始播放音乐(mp3格式)
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
    this->setPositionY(300);//动画看得清
    //this->setPositionX(500);//动画看得清

    // 设置缩放因子为0.80
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

    // 在Death动画结束后调用playBlackAnimation
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
    // 获取player当前的位置
    cocos2d::Vec2 currentPosition = this->getPosition();
    // 设置玩家为不可见
    this->setVisible(false);
    // 禁用物理体以防止碰撞
    this->getPhysicsBody()->setEnabled(false);
    //
    velocity = Vec2::ZERO;
    this->getPhysicsBody()->setVelocity(Vec2::ZERO);
    this->getPhysicsBody()->setGravityEnable(false);
    // 设置角色的位置
    cocos2d::Vec2 respawnPosition = respawnPoints[currentLevel]; // 重生点位置 这里需改为关卡的重生点
    this->setPosition(respawnPosition);
    // 创建一个新的Sprite对象用于播放死亡动画
    auto deathSprite = Sprite::create();
    deathSprite->setPosition(currentPosition);
    deathSprite->setScale(0.80);
    this->getParent()->addChild(deathSprite);

    // 加载B版本死亡动画的所有帧
    Vector<SpriteFrame*> deathFrames;
    auto frameCache = SpriteFrameCache::getInstance();
    for (int i = 0; i <= 23; ++i) {
        auto frameName = StringUtils::format("Bdeath_00-%d.png", i);
        auto frame = frameCache->getSpriteFrameByName(frameName);
        if (frame) {
            deathFrames.pushBack(frame);
        }
    }

    // 创建动画并播放
    auto animation = Animation::createWithSpriteFrames(deathFrames, 0.05f);
    auto animate = Animate::create(animation);

    // 播放死亡音乐
    _deathMusicState = cocos2d::AudioEngine::getState(_deathMusicId);
    if (_deathMusicState != cocos2d::AudioEngine::AudioState::PLAYING) {
        _deathMusicId = cocos2d::AudioEngine::play2d("music/death.mp3", false);
    }

    // 在动画播放完成后移除Sprite,打开黑幕
    auto callBlackAnimation = CallFunc::create([this, deathSprite]() {
        this->playBlackAnimation();
        deathSprite->removeFromParent();
        });

    // 执行动画和移除自身的序列动作
    auto sequence = Sequence::create(animate, callBlackAnimation, nullptr);
    deathSprite->runAction(sequence);
}

void Player::playBDropAnimation() {//坠落
    // 停止所有正在运行的动画（确保不会与其他动画冲突）
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

void Player::playBHoldWallAnimation() {//爬墙
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
    // 使用 RepeatForever 动作使动画无限循环播放
    auto repeatForever = RepeatForever::create(animate);

    this->runAction(repeatForever); // 使用 repeatForever 运行动画
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
    // 使用 RepeatForever 动作使动画无限循环播放
    auto repeatForever = RepeatForever::create(animate);

    this->runAction(repeatForever); // 使用 repeatForever 运行动画
    // CCLOG("Finished setting up HoldWall animation");




}//爬墙向上

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
    // 使用 RepeatForever 动作使动画无限循环播放
    auto repeatForever = RepeatForever::create(animate);

    this->runAction(repeatForever); // 使用 repeatForever 运行动画
    // CCLOG("Finished setting up HoldWall animation");





}//爬墙向下

void Player::playBJumpUpAnimation() {//跳跃
    
     // 检查音乐的状态
    _jumpMusicState = cocos2d::AudioEngine::getState(_jumpMusicId);

    // 如果音乐没有播放或者播放已经完成，那么开始播放音乐(mp3格式)
    if (_jumpMusicState != cocos2d::AudioEngine::AudioState::PLAYING) {
        _jumpMusicId = cocos2d::AudioEngine::play2d("music/jump.mp3", false);
    }
    // 停止所有正在运行的动画（确保不会与其他动画冲突）
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

void Player::playBJumpMoveAnimation() {//跳跃移动
    // 停止所有正在运行的动画（确保不会与其他动画冲突）
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

void Player::playBHoldWallJumpAnimation() {//爬墙跳跃
    // 停止所有正在运行的动画（确保不会与其他动画冲突）
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

//特效
void Player::playFloorLandingAshAnimation() {
    CCLOG("Starting FloorLandingAsh animation");

    // 1. 创建烟尘动画的帧
    Vector<SpriteFrame*> ashFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 13; i++) {
        std::string frameName = StringUtils::format("floorlandingash_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            ashFrames.pushBack(frame);
        }
    }

    // 2. 创建动画和动作
    auto animation = Animation::createWithSpriteFrames(ashFrames, 0.05f);
    auto animate = Animate::create(animation);

    // 3. 创建一个新的精灵来播放烟尘动画
    auto ashSprite = Sprite::createWithSpriteFrame(ashFrames.front());

    // 4. 设置精灵的位置在玩家的脚下
    ashSprite->setPosition(this->getPosition() + Vec2(0, -this->getContentSize().height * 0.02f));

    // 5. 添加到当前的父节点并播放动画
    this->getParent()->addChild(ashSprite, this->getLocalZOrder() - 1);  // 保证烟尘在玩家下面
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

    // 1. 创建烟尘动画的帧
    Vector<SpriteFrame*> ashFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 9; i++) {
        std::string frameName = StringUtils::format("floorjumpash_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            ashFrames.pushBack(frame);
        }
    }

    // 2. 创建动画和动作
    auto animation = Animation::createWithSpriteFrames(ashFrames, 0.05f);
    auto animate = Animate::create(animation);

    // 3. 创建一个新的精灵来播放烟尘动画
    auto ashSprite = Sprite::createWithSpriteFrame(ashFrames.front());

    // 4. 设置精灵的位置在玩家的脚下
    ashSprite->setPosition(this->getPosition() + Vec2(0, 20));

    // 5. 添加到当前的父节点并播放动画
    this->getParent()->addChild(ashSprite, this->getLocalZOrder() - 1);  // 保证烟尘在玩家下面
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

    // 1. 创建烟尘动画的帧
    Vector<SpriteFrame*> ashFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 9; i++) {
        std::string frameName = StringUtils::format("walljumpash_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            ashFrames.pushBack(frame);
        }
    }

    // 2. 创建动画和动作
    auto animation = Animation::createWithSpriteFrames(ashFrames, 0.05f);
    auto animate = Animate::create(animation);

    // 3. 创建一个新的精灵来播放烟尘动画
    auto ashSprite = Sprite::createWithSpriteFrame(ashFrames.front());

    // 4. 设置精灵的位置在玩家的面前
    ashSprite->setPosition(this->getPosition() + facingDirection * Vec2(40, 0));

    // 5. 添加到当前的父节点并播放动画
    this->getParent()->addChild(ashSprite, this->getLocalZOrder() - 1);  // 保证烟尘在玩家下面
    ashSprite->runAction(Sequence::create(
        animate,
        CallFunc::create([ashSprite]() {
            ashSprite->removeFromParentAndCleanup(true);
            }),
        nullptr
    ));

    CCLOG("Finished setting up FloorWallJumpAsh animation");
}