// JumpTable.h
#include "cocos2d.h"
#include "Trap.h"

class JumpTable : public Trap {
public:
    static JumpTable* create(const cocos2d::Vec2& position); // ���ڲ�����Ҫsize����
    virtual bool init() override;
    void deactivateTemporarily();//�����ظ�����
    void playAnimation();
    void activate() override;
    void setCanBeActivated(bool canBeActivated);
    bool canBeActivated() const;
    virtual ~JumpTable();
    void initDefaultTexture();
    void initPhysicsBody(); // �Ƴ�size����
    bool _canBeActivated=1;
};