// JumpTable.h
#include "cocos2d.h"
#include "Trap.h"
#include "AudioEngine.h"

class JumpTable : public Trap {
public:
    static JumpTable* create(const cocos2d::Vec2& position); // 现在不再需要size参数
    virtual bool init() override;
    void deactivateTemporarily();//避免重复播放
    void playAnimation();
    void activate() override;
    void setCanBeActivated(bool canBeActivated);
    bool canBeActivated() const;
    virtual ~JumpTable();
    void initDefaultTexture();
    void initPhysicsBody(); // 移除size参数
    bool _canBeActivated=1;
    int _jumpTableMusicId;
    cocos2d::AudioEngine::AudioState _jumpTableMusicState;
};