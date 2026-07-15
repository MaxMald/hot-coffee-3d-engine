#include "hc/scene/hcSceneGraph.h"
#include "hc/scene/gameObject/hcIGameObjectFactory.h"

namespace hc
{
  SceneGraph::SceneGraph() :
    m_roots(),
    m_gameObjectFactory(nullptr)
  {
  }

  SceneGraph::~SceneGraph()
  {
  }

  void SceneGraph::serialize(BinaryWriter& writer) const
  {
    writer.writeSizeT(m_roots.size());
    for (const UniquePtr<GameObject>& root : m_roots)
      root->serialize(writer);
  }

  void SceneGraph::deserialize(BinaryReader& reader)
  {
    if (m_gameObjectFactory == nullptr)
    {
      throw RuntimeErrorException(
        "GameObjectFactory is not initialized. Cannot deserialize SceneGraph."
      );
    }

    SizeT rootCount = reader.readSizeT();

    m_roots.clear();
    m_roots.reserve(rootCount);
    for (SizeT i = 0; i < rootCount; ++i)
    {
      UniquePtr<GameObject> root = m_gameObjectFactory->create("_toDeserialize");
      root->deserialize(reader);
      addRoot(std::move(root));
    }
  }

  void SceneGraph::draw(const RenderContext& renderContext)
  {
    for (const UniquePtr<GameObject>& root : m_roots)
    {
      if (root)
        root->draw(renderContext);
    }
  }

  void SceneGraph::update(const Time& elapsedTime)
  {
    for (const UniquePtr<GameObject>& root : m_roots)
    {
      if (root)
        root->preUpdate(elapsedTime);
    }

    for (const UniquePtr<GameObject>& root : m_roots)
    {
      if (root)
        root->update(elapsedTime);
    }

    for (const UniquePtr<GameObject>& root : m_roots)
    {
      if (root)
        root->postUpdate(elapsedTime);
    }
  }

  void SceneGraph::addRoot(UniquePtr<GameObject> root)
  {
    if (!root)
    {
      throw RuntimeErrorException(
        "Cannot add a null root GameObject to the SceneGraph."
      );
    }

    if (root->getParent())
    {
      throw RuntimeErrorException(
        String::Format(
          "Cannot set GameObject with name '%s' because it already has a parent.",
          root->getName().c_str()
        )
      );
    }

    m_roots.push_back(std::move(root));
  }

  UniquePtr<GameObject> SceneGraph::removeRoot(const String& name)
  {
    auto it = std::find_if(
      m_roots.begin(), m_roots.end(),
      [&name](const UniquePtr<GameObject>& root) 
      {
        return root && root->getName() == name;
      }
    );

    if (it != m_roots.end())
    {
      UniquePtr<GameObject> removedRoot = std::move(*it);
      m_roots.erase(it);
      return removedRoot;
    }

    return nullptr;
  }

  GameObject* SceneGraph::getRoot(const String& name) const
  {
    auto it = std::find_if(
      m_roots.begin(), m_roots.end(),
      [&name](const UniquePtr<GameObject>& root) 
      {
        return root && root->getName() == name;
      }
    );

    if (it != m_roots.end())
      return it->get();

    return nullptr;
  }

  const Vector<UniquePtr<GameObject>>& SceneGraph::getRoots() const
  {
    return m_roots;
  }

  void SceneGraph::getAllGameObjects(Vector<GameObject*>& outGameObjects) const
  {
    for (const UniquePtr<GameObject>& root : m_roots)
    {
      if (root)
        root->getAllDescendants(outGameObjects);
    }
  }

  void SceneGraph::clear()
  {
    m_roots.clear();
  }

  void SceneGraph::initialize(IGameObjectFactory* gameObjectFactory)
  {
    m_gameObjectFactory = gameObjectFactory;
  }
}
