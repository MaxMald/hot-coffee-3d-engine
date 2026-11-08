#include "game.h"

Game::Game(hc::HotCoffeeEngine* engine)
  : m_engine(engine),
  m_sphere(nullptr),
  m_velocity(0.0f, 0.0f, 0.0f)
{
}

Game::~Game()
{
}

void Game::onLoad()
{
  hc::SkyboxSettings skyboxSettings;
  skyboxSettings.sourcePath = hc::Path("small_apartment_4K.hdr");
  skyboxSettings.cubeMapGeneratorSettings.invert.y = -1.0f;
  getSceneSkybox().initialize(
    m_engine->getAssetManager(),
    m_engine->getGraphicsManager(),
    skyboxSettings
  );

  m_sphere = getSceneGraph().getRoot()->createChild("Sphere");
  hc::MeshComponent* meshComponent = m_sphere->createComponent<hc::MeshComponent>();

  hc::Path base = m_engine->getAssetManager().getRootPath();
  base /= "highres-icosphere-matPBR.fbx";

  hc::SharedPtr<hc::IMesh> mesh = m_engine->getGraphicsManager()
    .getMeshManager()
    .createMeshFromPath(base);

  meshComponent->setMesh(mesh);
  m_sphere->setPosition(hc::Vector3f(0.0f, 0.0f, -10.0f));

  getCameraManager().getActiveCamera()->setPosition(hc::Vector3f(0.0f, 0.0f, 0.0f));
  getCameraManager().getActiveCamera()->setDirection(hc::Vector3f(0.0f, 0.0f, -1.0f));
}

void Game::onBeforeUpdate(const hc::Time& delta)
{
  if (m_sphere)
  {
    float enabled = 0.0f;
    if (m_engine->getInputManager().getKeyboardKeyState(hc::keyboardKey::Space).isPressed())
      enabled = 1.0f;

    hc::Vector3f position = m_sphere->getPosition();

    // Update Velocity
    const float acceleration = 0.5f;
    const float maxSpeed = 1.0f;
    m_velocity.x += acceleration * delta.toSeconds() * enabled;
    if (m_velocity.length() > maxSpeed)
      m_velocity = m_velocity.normalized() * maxSpeed;

    // Update Friction
    const float friction = 0.5f;
    hc::Vector3f frictionVector = m_velocity.normalized()
      * delta.toSeconds()
      * friction
      * m_velocity.length()
      * -1.0f;

    m_velocity += frictionVector;

    position += m_velocity * delta.toSeconds();
    m_sphere->setPosition(position);
  }
}
