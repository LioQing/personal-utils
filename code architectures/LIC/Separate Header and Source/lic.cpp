#include "lic.hpp"

namespace lic
{
	Entity Manager::AddEntity()
	{
		EntityID id;

		if (m_empty_entity.empty())
		{
			id = m_next_entity_id++;
			m_top_id = id;
			m_checklist.emplace_back(false);
		}
		else
		{
			id = m_empty_entity.back();
		}

#ifdef LIC_DEBUG
		std::cout << "Entity " << id << " created." << std::endl;
#endif
		return Entity(this, id);
	}

	void Manager::DestroyEntity(EntityID entity)
	{
		// remove components
		for (ComponentID cid = 0u; cid < MAX_COMPONENT; ++cid)
		{
			if (HasComponent(entity, cid))
				RemoveComponent(entity, cid);
		}

		// reset checklist
		m_checklist.at(entity).reset();

		// destroy entity
		m_empty_entity.push_back(entity);

#ifdef LIC_DEBUG
		std::cout << "Entity " << entity << " destroyed." << std::endl;
#endif
	}

	Entity Manager::GetEntity(EntityID entity)
	{
		return Entity(this, entity);
	}

	void Manager::RemoveComponent(EntityID entity, ComponentID cid)
	{
		if (!HasComponent(entity, cid))
		{
#ifdef LIC_DEBUG
			std::cout << "No Component " << cid << " found in Entity " << entity << "." << std::endl;
#endif
			return;
		}

		for (auto& cptr : m_components.at(cid))
		{
			if (cptr != nullptr && cptr->entity == entity)
			{
				m_empty_component.at(cid).push_back(&cptr - &m_components.at(cid)[0]);
				m_checklist.at(cptr->entity).set(cid, false);
				cptr.reset();
				break;
			}
		}

#ifdef LIC_DEBUG
		std::cout << "Component " << cid << " removed from Entity " << entity << "." << std::endl;
#endif
	}

	bool Manager::HasComponent(EntityID entity, ComponentID cid) const
	{
		return m_checklist.at(entity).test(cid);
	}

	Entity Component::GetEntity()
	{
		return manager->GetEntity(entity);
	}
	Entity Component::GetEntity() const
	{
		return manager->GetEntity(entity);
	}

	EntityID Component::GetEntityID() const
	{
		return entity;
	}

	Entity::operator EntityID() const
	{
		return id;
	}

	EntityID Entity::GetID() const
	{
		return id;
	}

	void Entity::Destroy()
	{
		manager->DestroyEntity(id);
	}

	void Entity::RemoveComponent(ComponentID cid)
	{
		manager->RemoveComponent(id, cid);
	}

	bool Entity::HasComponent(ComponentID cid) const
	{
		return manager->HasComponent(id, cid);
	}
}