#include "LEpch.h"
#include "LE/Scene/Entity.h"

namespace LE
{
	Entity::Entity(entt::entity Handle, Scene* Scene)
		: m_EntityHandle(Handle), ScenePtr(Scene)
	{
	}
}