#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"

enum class PlayerState {
    IDLE,             // 站立
    MOVING_LEFT,      // 左移
    MOVING_RIGHT,     // 右移
    MOVING_TURN_RL,   // 从右向左转向 
    MOVING_TURN_LR,   // 从左向右转向 
    CROUCH,           // 下蹲
    LOOKUP,           // 向上看
    JUMPING,          // 跳跃
    CLIMBING,         // 攀爬
    DROP,             // 坠落
    HOLDWALL,         // 爬墙
    WALL_JUMPING,     // 墙壁跳跃
    DASH,             // 冲刺
    DYING,            // 死亡
    EXHAUSTED         // 疲劳——此状态下会从墙壁上滑落
};

enum class PlayerKey {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    JUMP,
    TALK,
    DASH,
    CLIMB
};

class Player : public cocos2d::Sprite {
public:

    Player();//构造函数

    void update(float delta); // 每帧的更新方法
    static Player* create(const std::string& filename);
    virtual bool init(const std::string& filename); // 直接传递filename给init

    // 基础属性
    cocos2d::Vec2 position;//位置    
    cocos2d::Vec2 velocity;//速度  
    float accelerationX = 200.0f;//水平加速度

    // 状态判定
    bool isOnGround=0;
    bool isInAir=0;
    bool isClimbing=0;
    bool isWallSliding=0;
    bool canDash=1;//也作为角色是否blue的判断
    bool canClimb=0;//作为角色能否爬墙的判断
    

    // 动作状态   
    PlayerState currentState;
    PlayerState previousState;

    // 体力
    float stamina=100.0f;
    const float MAX_STAMINA = 100.0f;

    bool isJumping = false; // 用于跟踪玩家是否正在跳跃

    // 冲刺
    float dashDistance = 200;

    // 物理属性         
    float CLINMB_MAXSPEED = 100;
    
    const float JUMP_SPEED = 300; // 跳跃初速度，例如500像素/秒
    float WALK_MAXSPEED = 200;//最大速度

    

    // 碰撞与环境
    cocos2d::Rect collisionBox;
   
    //检测角色是否在地面的函数在场景中实现
    void setOnGround(bool value);//设置状态 
    //bool wallCheck();//检测前方是否有墙壁

    // 动画播放
    void playIdleAnimation_1(); // 播放站立动画1
    void playIdleAnimation_2(); // 播放站立动画2
    void playMoveAnimation();//移动动画    
    void playCrouchAnimation();//蹲姿动画  
    void playLookUpAnimation();//向上看动画
    void playJumpUpAnimation();//跳跃动画
    void playJumpMoveAnimation();//跳跃移动动画
    void playDropAnimation();//坠落动画  
    

    //过渡动画
    void playMoveTurnAnimation();//转向动画
    void Player::playCrouchToIdleAnimation();//蹲姿到静止

    //
    int facingDirection=1;//面向方向（向右为1，向左为-1）

    bool isAlive=1;

    std::map<PlayerKey, bool> keyStates;//检测用户输入
    
    bool Player::checkPlayerInput();//检测用户是否有按键输入

    cocos2d::Vec2 Player::adjustMovePosition(const cocos2d::Vec2& desiredPosition);//完善位置检测

private:    
    cocos2d::DrawNode* _debugDrawNode;//测试用射线
    bool isOnSolidGround(); // 检查玩家是否在坚实的地面上

    void changeState(PlayerState newState);//状态转换
    cocos2d::EventListenerKeyboard* keyboardListener; // 键盘监听器

    // 键盘事件回调函数
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
};

#endif // __PLAYER_H__
