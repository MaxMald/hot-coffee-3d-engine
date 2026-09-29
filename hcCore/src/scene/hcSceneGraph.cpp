#include "hc/scene/hcSceneGraph.h"
#include "hc/scene/gameObject/hcIGameObjectFactory.h"
#include "hc/graphics/hcDrawCommand.h"

namespace hc
{
  static constexpr UInt32 SCENE_GRAPH_VERSION = 2;

  SceneGraph::SceneGraph() :
    m_root(nullptr),
    m_gameObjectFactory(nullptr)
  {
  }

  SceneGraph::~SceneGraph()
  {
  }

  void SceneGraph::serialize(io::BinaryWriter& writer) const
  {
    assertIsInitialized();
    writer.startWritingObject(static_cast<UInt32>(0), SCENE_GRAPH_VERSION);
    m_root->serialize(writer);
    writer.finishWritingObject();
  }

  void SceneGraph::deserialize(io::BinaryReader& reader)
  {
    assertIsInitialized();
    m_root->clear();

    io::ObjectHeader header = reader.startReadingObject();
    if (header.version == 1)
    {
      SizeT rootCount = reader.readSizeT();
      for (SizeT i = 0; i < rootCount; ++i)
      {
        UniquePtr<GameObject> rootGameObject = m_gameObjectFactory->create("_toDeserialize");
        rootGameObject->deserialize(reader);
        m_root->addChild(std::move(rootGameObject));
      }
    }
    if (header.version == 2)
    {
      m_root->deserialize(reader);
    }

    reader.finishReadingObject();
    return;
  }

  void SceneGraph::draw(
    const RenderContext& renderContext,
    Vector<DrawCommand>& outDrawCommands
  ) const
  {
    if (m_root != nullptr)
      m_root->draw(renderContext, outDrawCommands);
  }

  void SceneGraph::update(const Time& elapsedTime)
  {
    if (m_root != nullptr)
    {
      m_root->preUpdate(elapsedTime);
      m_root->update(elapsedTime);
      m_root->postUpdate(elapsedTime);
    }
  }

  void SceneGraph::addGameObject(UniquePtr<GameObject> go)
  {
    if (go == nullptr)
      throw InvalidArgumentException("Cannot add a null GameObject as root.");

    assertIsInitialized();
    m_root->addChild(std::move(go));
  }

  UniquePtr<GameObject> SceneGraph::removeGameObject(const String& name)
  {
    assertIsInitialized();
    GameObject* go = m_root->getDescendant(name);
    if (go == nullptr)
      return nullptr;

    GameObject* parent = go->getParent();
    if (parent == nullptr)
      throw RuntimeErrorException("Cannot remove the GameObject. GameObject does not have a parent.");

    return parent->removeChild(go->getUUID());
  }

  UniquePtr<GameObject> SceneGraph::removeGameObject(const UUID& uuid)
  {
    assertIsInitialized();
    return m_root->removeDescendant(uuid);
  }

  GameObject* SceneGraph::getGameObject(const String& name) const
  {
    assertIsInitialized();
    return m_root->getDescendant(name);
  }

  GameObject* SceneGraph::getGameObject(const UUID& uuid) const
  {
    assertIsInitialized();
    return m_root->getDescendant(uuid);
  }

  void SceneGraph::getAllGameObjects(Vector<GameObject*>& outGameObjects) const
  {
    assertIsInitialized();
    m_root->getDescendants(outGameObjects);
  }

  void SceneGraph::clear()
  {
    if (m_root != nullptr)
      m_root->clear();
  }

  void SceneGraph::initialize(IGameObjectFactory* gameObjectFactory)
  {
    m_root = gameObjectFactory->create("Root");
    m_gameObjectFactory = gameObjectFactory;
  }
}
