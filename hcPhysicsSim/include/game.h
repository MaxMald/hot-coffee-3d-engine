#include "hc/hcHotCoffeeCore.h"

class Game : public hc::Scene
{
public:
  Game(hc::HotCoffeeEngine* engine);
  ~Game() override;

  void onLoad() override;
  void onBeforeUpdate(const hc::Time& delta) override;

private:
  hc::HotCoffeeEngine* m_engine;
  hc::GameObject* m_sphere;
  hc::Vector3f m_velocity;
};
