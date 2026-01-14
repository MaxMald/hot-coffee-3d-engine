#pragma once

namespace hc
{
  class IRendered
  {
  public:
    virtual ~IRendered() = default;

    virtual void render() = 0;
  };
}
