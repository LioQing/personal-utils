#pragma once

#include <algorithm>
#include <array>
#include <atomic>
#include <bitset>
#include <cstddef>
#include <unordered_map>
#include <memory>
#include <ranges>
#include <vector>
#include <tuple>
#include <any>

class lic
{
public:
    /**
     * Max number of components
     * Modify this value in header file lic.hpp as needed
     */
    static constexpr size_t LIC_MAX_COMPONENT = 64;

    // Forward declarations
    template <typename TComp>
    struct Component;
    struct Entity;

    // Type for component id
    using ComponentID = uint32_t;

    // Type for entity id
    using EntityID = uint32_t;

    // Entity class
    struct Entity
    {
        EntityID id;
        std::bitset<LIC_MAX_COMPONENT> component_field;
        std::unordered_map<ComponentID, size_t> component_indices;

        operator EntityID() { return id; }
    };

    // Component class
    template <typename TComp>
    struct Component
    {
        EntityID entity_id;
        TComp component;

        operator TComp() { return component; }
        operator TComp&() { return component; }
        operator const TComp&() const { return component; }

        TComp* operator->() { return &component; }
        const TComp* operator->() const { return &component; }
    };

private:
    // Vector of BaseComponentVec (vector of component)
    static std::array<std::any, LIC_MAX_COMPONENT> components;

    // Vector of destroyed components
    static std::array<std::vector<std::size_t>, LIC_MAX_COMPONENT> destroyed_components;

    // Vector of entities
    static std::vector<Entity> entities;

    // Vector of destroyed entity IDs
    static std::vector<EntityID> destroyed_entities;

    // Next component ID to be generated
    static std::atomic<ComponentID> next_component_id;

    // Next entity ID to be generated
    static std::atomic<EntityID> next_entity_id;

    /**
     * @brief Get the vector of component type TComp
     * 
     * @tparam TComp the component type of the vector to be retrieved
     * @return std::vector<Component<TComp>>& the vector containing all instance of component type TComp
     */
    template <typename TComp>
    static std::vector<Component<TComp>>& GetComponentVec()
    {
        return std::any_cast<std::vector<Component<TComp>>&>(components.at(GetComponentID<TComp>()));
    }

public:
    // Prevent instantiation of class
    lic(const lic&) = delete;

    template <typename TComp>
    static ComponentID GetComponentID()
    {
        static ComponentID new_id = [&]()
        {
            components.at(next_component_id) = std::make_any<std::vector<Component<TComp>>>();
            return next_component_id++;
        }();
        return new_id;
    }

    template <typename TComp>
    static ComponentID GetComponentID(const TComp& comp)
    {
        return GetComponentID<TComp>();
    }

    static EntityID AddEntity();

    static void DestroyEntity(EntityID eid);

    static Entity& GetEntity(EntityID eid);

    static void RemoveComponent(EntityID eid, ComponentID cid);

    static bool HasComponent(EntityID eid, ComponentID cid);

    template <typename TComp, typename... TArgs>
    static Component<TComp>& AddComponent(EntityID eid, TArgs&&... args)
    {
        ComponentID cid = GetComponentID<TComp>();

        // Return component if entity already has the component
        if (HasComponent(eid, cid))
        {
            return GetComponent<TComp>(eid);
        }

        auto& component_vec = GetComponentVec<TComp>();
        size_t i = 0;

        // Add component to component_vec
        if (destroyed_components.at(cid).empty())
        {
            i = component_vec.size();
            component_vec.emplace_back(entities.at(eid), TComp(std::forward<TArgs>(args)...));
        }
        else
        {
            i = destroyed_components.at(cid).back();
            component_vec.at(i) = Component<TComp>(entities.at(eid), TComp(std::forward<TArgs>(args)...));
            destroyed_components.at(cid).pop_back();
        }

        // Store component index and field in entity
        entities.at(eid).component_indices.emplace(cid, i);
        entities.at(eid).component_field.set(cid);
        return component_vec.at(i);
    }

    template <typename TComp>
    static Component<TComp>& GetComponent(EntityID eid)
    {
        ComponentID cid = GetComponentID<TComp>();
        auto& component_vec = GetComponentVec<TComp>();
        return component_vec.at(entities.at(eid).component_indices.at(cid));
    }

    struct EntityContainer : public std::vector<EntityID>
    {
        struct Iterator : public std::vector<EntityID>::iterator
        {
            Iterator(const std::vector<EntityID>::iterator& iter) : std::vector<EntityID>::iterator(iter) {}

            inline Entity& operator*() { return entities.at(std::vector<EntityID>::iterator::operator*()); }
            inline const Entity& operator*() const { return entities.at(std::vector<EntityID>::iterator::operator*()); }
        };

        Iterator begin() { return Iterator(std::vector<EntityID>::begin()); }
        Iterator end() { return Iterator(std::vector<EntityID>::end()); }
    };

    template <typename... TComps>
    struct ComponentContainer : public std::vector<EntityID>
    {
        struct Iterator : public std::vector<EntityID>::iterator
        {
            Iterator(const std::vector<EntityID>::iterator& iter) : std::vector<EntityID>::iterator(iter) {}

            std::tuple<Component<TComps>&...> operator*()
            {
                Entity& entity = entities.at(std::vector<EntityID>::iterator::operator*());
                return std::tie(GetComponentVec<TComps>().at(entity.component_indices.at(GetComponentID<TComps>()))...);
            }

            std::tuple<const Component<TComps>&...> operator*() const
            {
                Entity& entity = entities.at(std::vector<EntityID>::iterator::operator*());
                return std::tie(GetComponentVec<TComps>().at(entity.component_indices.at(GetComponentID<TComps>()))...);
            }
        };

        Iterator begin() { return Iterator(std::vector<EntityID>::begin()); }
        Iterator end() { return Iterator(std::vector<EntityID>::end()); }
    };

    template <typename... TComps>
    struct View
    {
        std::vector<EntityID> entities;

        template <typename... TSelectComps>
        View<TComps..., TSelectComps...> Select() const
        {
            View<TComps..., TSelectComps...> view(entities);
            auto cids = { GetComponentID<TSelectComps>()... };
            std::erase_if(view.entities,
                          [&](EntityID eid) -> bool
                          {
                              return std::any_of(begin(cids), end(cids),
                                                 [&](ComponentID cid) -> bool
                                                 { return !HasComponent(eid, cid); });
                          });

            return view;
        }

        EntityContainer Entities()
        {
            return EntityContainer(entities);
        }

        ComponentContainer<TComps...> Components()
        {
            return ComponentContainer<TComps...>(entities);
        }

        template <typename... TOnlyComps> 
        ComponentContainer<TOnlyComps...> OnlyComponents()
        {
            return ComponentContainer<TOnlyComps...>(entities);
        }
    };

    template <typename... TComps>
    static View<TComps...> Select()
    {
        View<TComps...> view;
        auto cids = { GetComponentID<TComps>()... };

        for (auto& entity : entities)
        {
            if (std::all_of(begin(cids), end(cids),
                            [&](ComponentID cid)
                            { return HasComponent(entity, cid); }))
            {
                view.entities.push_back(entity);
            }
        }

        return view;
    }
};