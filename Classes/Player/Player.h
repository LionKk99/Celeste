#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"

enum class PlayerState {
    IDLE,             // վ��
    MOVING_LEFT,      // ����
    MOVING_RIGHT,     // ����
    MOVING_TURN_RL,   // ��������ת�� 
    MOVING_TURN_LR,   // ��������ת�� 
    CROUCH,           // �¶�
    LOOKUP,           // ���Ͽ�
    JUMPING,          // ��Ծ
    LANDING,          // ���
    DROP,             // ׹��
    PUSHWALL,         // ��ǽ       
    HOLDWALL,         // ��ǽ
    HOLDWALLUP,       // ��ǽ����
    HOLDWALLDOWN,     // ��ǽ����
    HOLDWALLJUMP,    // ��ǽ��Ծ  
    DASH,             // ���
    DYING,            // ����
    EXHAUSTED         // ƣ�͡�����״̬�»��ǽ���ϻ���
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

    Player();//���캯��

    void update(float delta); // ÿ֡�ĸ��·���
    static Player* create(const std::string& filename);
    virtual bool init(const std::string& filename); // ֱ�Ӵ���filename��init

    // ��������
    cocos2d::Vec2 position;//λ��    
    cocos2d::Vec2 velocity;//�ٶ�  
    float accelerationX = 500.0f;//ˮƽ���ٶ�

    // ״̬�ж�
    bool isOnGround=0;    
    bool isClimbing=0;
    bool isWallSliding=0;
    bool canDash=0;//Ҳ��Ϊ��ɫ�Ƿ�blue���ж�
    bool canClimb=0;//��Ϊ��ɫ�ܷ���ǽ���ж�
    

    // ����״̬   
    PlayerState currentState;
    PlayerState previousState;

    // ����
    float stamina=100.0f;
    const float MAX_STAMINA = 100.0f;

    bool isJumping = false; // ���ڸ�������Ƿ�������Ծ

    // ���
    float dashDistance = 220;
    bool isDashing;
    float dashTimer;
    static const float DASH_DURATION;
    cocos2d::Vec2 getDashDirection();
    // ��������         
    float CLINMB_MAXSPEED = 100;
    
    const float JUMP_SPEED = 300; // ��Ծ���ٶȣ�����500����/��
    float WALK_MAXSPEED = 200;//����ٶ�

    

    // ��ײ�뻷��
    cocos2d::Rect collisionBox;
   
    //����ɫ�Ƿ��ڵ���ĺ����ڳ�����ʵ��
    void setOnGround(bool value);//����״̬ 
    //bool wallCheck();//���ǰ���Ƿ���ǽ��

    // ��������
    void playIdleAnimation_1(); // ����վ������1
    void playIdleAnimation_2(); // ����վ������2
    void playMoveAnimation();//�ƶ�����    
    void playCrouchAnimation();//���˶���  
    void playLookUpAnimation();//���Ͽ�����
    void playJumpUpAnimation();//��Ծ����
    void playJumpMoveAnimation();//��Ծ�ƶ�����
    void playDropAnimation();//׹�䶯��  
    void playPushWallAnimation();//��ǽ����
    void playHoldWallAnimation();//��ǽ
    void playHoldWallUpAnimation();//��ǽ����
    void playHoldWallDownAnimation();//��ǽ����
    void playHoldWallJumpAnimation();//��ǽ��Ծ
    void playDashAnimation();//��̶���
    void playDashUpAndDownAnimation();//����������µ����⶯��
    void playDeathAnimation();//����
    void playRespawnAnimation();//����
    //���ɶ���
    void playMoveTurnAnimation();//ת�򶯻�
    void playCrouchToIdleAnimation();//���˵���ֹ
    void playLandingAnimation();//��ض���
    //ת������
    void playBlackAnimation();//��Ļ����
    //
    int facingDirection=1;//����������Ϊ1������Ϊ-1��

    bool isAlive=1;

    void startDashing(const cocos2d::Vec2& dashDirection);

    std::map<PlayerKey, bool> keyStates;//����û�����
    
    bool Player::checkPlayerInput();//����û��Ƿ��а�������

    cocos2d::Vec2 Player::adjustMovePosition(const cocos2d::Vec2& desiredPosition);//����λ�ü��

private:    
    cocos2d::DrawNode* _debugDrawNode;//����������
    bool isOnSolidGround(); // �������Ƿ��ڼ�ʵ�ĵ�����

    void changeState(PlayerState newState);//״̬ת��
    cocos2d::EventListenerKeyboard* keyboardListener; // ���̼�����

    // �����¼��ص�����
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
};

#endif // __PLAYER_H__
