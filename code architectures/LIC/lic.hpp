#pragma once

#include <algorithm>
#include <any>
#include <array>
#include <atomic>
#include <bitset>
#include <memory>
#include <ranges>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <functional>

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

    // Component class
    template <typename TComp>
    struct Component : public TComp
    {
        EntityID entity_id;
    };

    // Entity class
    struct Entity
    {
        EntityID id;
        std::bitset<LIC_MAX_COMPONENT> component_field;
        std::unordered_map<ComponentID, size_t> component_indices;

        void RemoveComponent(ComponentID cid) const;

        template <typename TComp>
        void RemoveComponent() const;

        bool HasComponent(ComponentID cid) const;

        template <typename TComp>
        bool HasComponent() const;

        template <typename TComp, typename... TArgs>
        Component<TComp>& AddComponent(TArgs&&... args) const;

        template <typename TComp>
        Component<TComp>& GetComponent() const;

        operator EntityID() const { return id; }
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

    template <typename TComp>
    static void RemoveComponent(EntityID eid)
    {
        RemoveComponent(eid, GetComponentID<TComp>());
    }

    static bool HasComponent(EntityID eid, ComponentID cid);

    template <typename TComp>
    static bool HasComponent(EntityID eid)
    {
        return entities.at(eid).HasComponent(GetComponentID<TComp>());
    }

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
            component_vec.emplace_back(TComp(std::forward<TArgs>(args)...), entities.at(eid));
        }
        else
        {
            i = destroyed_components.at(cid).back();
            component_vec.at(i) = Component<TComp>(TComp(std::forward<TArgs>(args)...), entities.at(eid));
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
        return entities.at(eid).GetComponent<TComp>();
    }

    struct EntityContainer : public std::vector<EntityID>
    {
    private:
        using VecIterator = std::vector<EntityID>::const_iterator;

    public:
        struct Iterator : public VecIterator
        {
            Iterator(const VecIterator& iter) : VecIterator(iter) {}

            const Entity& operator*() const;
        };

        Iterator begin() const;
        Iterator end() const;
        Iterator cbegin() const;
        Iterator cend() const;
    };

    template <typename... TComps>
    struct ComponentContainer : public std::vector<EntityID>
    {
        template <typename TBackingIter>
        requires std::contiguous_iterator<TBackingIter>
        struct BaseIterator : public TBackingIter
        {
            BaseIterator(const TBackingIter& iter) : TBackingIter(iter) {}

            auto operator*()
            {
                if constexpr (std::is_same<TBackingIter, std::vector<EntityID>::const_iterator>::value)
                    return std::tie<const Component<TComps>...>(GetComponent<TComps>(TBackingIter::operator*())...);
                else
                    return std::tie<Component<TComps>...>(GetComponent<TComps>(TBackingIter::operator*())...);
            }
        };

        using Iterator = BaseIterator<std::vector<EntityID>::iterator>;
        using ConstIterator = BaseIterator<std::vector<EntityID>::const_iterator>;

        Iterator begin() { return Iterator(std::vector<EntityID>::begin()); }
        Iterator end() { return Iterator(std::vector<EntityID>::end()); }

        ConstIterator begin() const { return ConstIterator(std::vector<EntityID>::cbegin()); }
        ConstIterator end() const { return ConstIterator(std::vector<EntityID>::cend()); }
    };

    template <typename... TComps>
    struct Range
    {
        std::vector<EntityID> entities;

        template <typename... TSelectComps>
        Range<TComps..., TSelectComps...> Select() const
        {
            Range<TComps..., TSelectComps...> range(entities);
            auto cids = { GetComponentID<TSelectComps>()... };
            std::erase_if(range.entities,
                          [&](EntityID eid) -> bool
                          {
                              return std::any_of(begin(cids), end(cids),
                                                 [&](ComponentID cid) -> bool
                                                 { return !HasComponent(eid, cid); });
                          });

            return range;
        }

        template <typename TFunctor>
        requires std::predicate<TFunctor, TComps...>
        Range<TComps...>& Where(TFunctor functor)
        {
            std::erase_if(entities,
                [&](EntityID eid) -> bool
                {
                    return !functor(GetEntity(eid).GetComponent<TComps>()...);
                });
            return *this;
        }

        EntityContainer Entities()
        {
            return EntityContainer(entities);
        }

        const EntityContainer Entities() const
        {
            return EntityContainer(entities);
        }

        ComponentContainer<TComps...> Components()
        {
            return ComponentContainer<TComps...>(entities);
        }

        const ComponentContainer<TComps...> Components() const
        {
            return ComponentContainer<TComps...>(entities);
        }

        template <typename... TOnlyComps>
        ComponentContainer<TOnlyComps...> OnlyComponents()
        {
            return ComponentContainer<TOnlyComps...>(entities);
        }

        template <typename... TOnlyComps>
        const ComponentContainer<TOnlyComps...> OnlyComponents() const
        {
            return ComponentContainer<TOnlyComps...>(entities);
        }
    };

    template <typename... TComps>
    static Range<TComps...> Select()
    {
        Range<TComps...> range;
        auto cids = { GetComponentID<TComps>()... };

        for (auto& entity : entities)
        {
            if (std::all_of(begin(cids), end(cids),
                            [&](ComponentID cid)
                            { return HasComponent(entity, cid); }))
            {
                range.entities.push_back(entity);
            }
        }

        return range;
    }
};

template <typename TComp>
void lic::Entity::RemoveComponent() const
{
    RemoveComponent(this->id, GetComponentID<TComp>());
}

template <typename TComp>
bool lic::Entity::HasComponent() const
{
    return HasComponent(GetComponentID<TComp>());
}

template <typename TComp, typename... TArgs>
lic::Component<TComp>& lic::Entity::AddComponent(TArgs&&... args) const
{
    AddComponent<TComp>(this->id, std::forward<TArgs>(args)...);
}

template <typename TComp>
lic::Component<TComp>& lic::Entity::GetComponent() const
{
    return GetComponentVec<TComp>().at(component_indices.at(GetComponentID<TComp>()));
}