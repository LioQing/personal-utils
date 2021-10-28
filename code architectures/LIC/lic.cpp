#include "lic.hpp"

std::array<std::any, lic::LIC_MAX_COMPONENT> lic::components;
std::array<std::vector<std::size_t>, lic::LIC_MAX_COMPONENT> lic::destroyed_components;
std::vector<lic::Entity> lic::entities;
std::vector<lic::EntityID> lic::destroyed_entities;
std::atomic<lic::ComponentID> lic::next_component_id;
std::atomic<lic::EntityID> lic::next_entity_id;

lic::EntityID lic::AddEntity()
{
    if (destroyed_entities.empty())
    {
        EntityID eid = next_entity_id++;
        entities.emplace_back(eid, false);
        return eid;
    }
    else
    {
        EntityID eid = destroyed_entities.back();
        entities.at(eid) = Entity(eid, false);
        destroyed_entities.pop_back();
        return eid;
    }
}

void lic::DestroyEntity(EntityID eid)
{
    for (ComponentID cid = 0u; cid < LIC_MAX_COMPONENT; ++cid)
    {
        if (HasComponent(eid, cid))
            RemoveComponent(eid, cid);
    }
    
    entities.at(eid).component_field.reset();
    destroyed_entities.push_back(eid);
}

lic::Entity& lic::GetEntity(EntityID eid)
{
    return entities.at(eid);
}

void lic::RemoveComponent(EntityID eid, ComponentID cid)
{
    if (!HasComponent(eid, cid))
    {
        return;
    }

    auto& entity = entities.at(eid);
    auto cind = entities.at(eid).component_indices.at(cid);

    destroyed_components.at(cid).push_back(cind);
    entities.at(eid).component_field.set(cid, false);
}

bool lic::HasComponent(EntityID eid, ComponentID cid)
{
    return entities.at(eid).component_field.test(cid);
}