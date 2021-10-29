#pragma once

#include <algorithm>
#include <any>
#include <array>
#include <atomic>
#include <bitset>
#include <functional>
#include <memory>
#include <ranges>
#include <tuple>
#include <unordered_map>
#include <vector>

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
        // ID of the entity
        EntityID id;

        // Bitfield indicating whether the entity contains a component
        std::bitset<LIC_MAX_COMPONENT> component_field;

        // Hashmap containing component's ID and index in the component vector
        std::unordered_map<ComponentID, size_t> component_indices;

        /**
         * @brief Remove a component from this entity
         * 
         * @param cid ID of the component to be removed
         */
        void RemoveComponent(ComponentID cid) const;

        /**
         * @brief Remove a component from this entity
         * 
         * @tparam TComp type of component to be removed
         */
        template <typename TComp>
        void RemoveComponent() const;

        /**
         * @brief Check if this entity has a component
         * 
         * @param cid ID of the component to be checked
         * @return Boolean indicating the result
         */
        bool HasComponent(ComponentID cid) const;

        /**
         * @brief Check if this entity has a component
         * 
         * @tparam TComp type of the component to be checked
         * @return Boolean indicating the result
         */
        template <typename TComp>
        bool HasComponent() const;

        /**
         * @brief Add a component to this entity
         * 
         * @tparam TComp type of the component to be added
         * @param args arguments to be passed to the component for initialization
         * @return Reference to the component added (with wrapper class)
         */
        template <typename TComp, typename... TArgs>
        Component<TComp>& AddComponent(TArgs&&... args) const;

        /**
         * @brief Get a component from this entity
         * 
         * @tparam TComp type of the component to be returned
         * @return Reference to the component returned (with wrapper class)
         */
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
     * @tparam TComp type of the component of the vector to be retrieved
     * @return Reference to the vector containing all instances of component type TComp
     */
    template <typename TComp>
    static std::vector<Component<TComp>>& GetComponentVec()
    {
        return std::any_cast<std::vector<Component<TComp>>&>(components.at(GetComponentID<TComp>()));
    }

public:
    // Prevent instantiation of class
    lic(const lic&) = delete;

    /**
     * @brief Get the ID of a component type
     * If this is the first time this function is called with the type, a new ID will be generated
     * 
     * @tparam TComp type of the component which the ID will be returned
     * @return ID of the component type TComp
     */
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

    /**
     * @brief Get the ID of a component type
     * If this is the first time this function is called with the type, a new ID will be generated
     * 
     * @param comp component which the ID will be returned (with wrapper class)
     * @return ID of the component comp
     */
    template <typename TComp>
    static ComponentID GetComponentID(const Component<TComp>& comp)
    {
        return GetComponentID<TComp>();
    }

    /**
     * @brief Get the ID of a component type
     * If this is the first time this function is called with the type, a new ID will be generated
     * 
     * @param comp component which the ID will be returned
     * @return ID of the component comp
     */
    template <typename TComp>
    static ComponentID GetComponentID(const TComp& comp)
    {
        return GetComponentID<TComp>();
    }

    /**
     * @brief Add a new entity
     * 
     * @return ID of the entity added
     */
    static EntityID AddEntity();

    /**
     * @brief Destroy a entity (including its components)
     * 
     * @param eid ID of the entity to be destroyed
     */
    static void DestroyEntity(EntityID eid);

    /**
     * @brief Get a entity
     * 
     * @param eid ID of the entity to be returned
     * @return Reference to the entity
     */
    static Entity& GetEntity(EntityID eid);

    /**
     * @brief Remove a component from an entity
     * 
     * @param eid ID of the entity where the component will be removed
     * @param cid ID of the component to be removed
     */
    static void RemoveComponent(EntityID eid, ComponentID cid);

    /**
     * @brief Remove a component from an entity
     * 
     * @tparam TComp type of component to be removed
     * @param eid ID of the entity where the component will be removed
     */
    template <typename TComp>
    static void RemoveComponent(EntityID eid)
    {
        RemoveComponent(eid, GetComponentID<TComp>());
    }

    /**
     * @brief Check if an entity has a component
     * 
     * @param eid ID of the entity where the component will be checked
     * @param cid ID of the component to be checked
     * @return Boolean indicating the result
     */
    static bool HasComponent(EntityID eid, ComponentID cid);

    /**
     * @brief Check if an entity has a component
     * 
     * @tparam TComp type of the component to be checked
     * @param eid ID of the entity where the component will be checked
     * @return Boolean indicating the result
     */
    template <typename TComp>
    static bool HasComponent(EntityID eid)
    {
        return entities.at(eid).HasComponent(GetComponentID<TComp>());
    }

    /**
     * @brief Add a component to an entity
     * 
     * @tparam TComp type of the component to be added
     * @param eid ID of the entity where the component will be added
     * @param args arguments to be passed to the component for initialization
     * @return Reference to the component added (with wrapper class)
     */
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
            auto available_slots = destroyed_components.at(cid);
            i = available_slots.back();
            component_vec.at(i) = Component<TComp>(TComp(std::forward<TArgs>(args)...), entities.at(eid));
            available_slots.pop_back();
        }

        // Store component index and field in entity
        entities.at(eid).component_indices.emplace(cid, i);
        entities.at(eid).component_field.set(cid);
        return component_vec.at(i);
    }

    /**
     * @brief Get a component from an entity
     * 
     * @tparam TComp type of the component to be returned
     * @param eid ID of the entity where the component will be return
     * @return Reference to the component returned (with wrapper class)
     */
    template <typename TComp>
    static Component<TComp>& GetComponent(EntityID eid)
    {
        return entities.at(eid).GetComponent<TComp>();
    }

    /**
     * @brief A container class for storing and iterating through selected entities
     * 
     */
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

    /**
     * @brief A container class for storing selected entities and iterating through their components
     * 
     * @tparam TComps types of selected entities' components
     */
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

    /**
     * @brief A class for selecting entities by their components
     * 
     * @tparam TComps types of selected entities' components
     */
    template <typename... TComps>
    struct Range
    {
        std::vector<EntityID> entities;

        /**
         * @brief Further select entities with certain components
         * 
         * @tparam TSelectComps types of the components to be selected
         * @return Range of entities with selected components
         */
        template <typename... TSelectComps>
        Range<TComps..., TSelectComps...> Select() const
        {
            auto selected_entities = entities;
            auto cids = { GetComponentID<TSelectComps>()... };
            std::erase_if(selected_entities,
                [&](EntityID eid)
                {
                    return std::any_of(begin(cids), end(cids),
                        [&](ComponentID cid)
                        { return !HasComponent(eid, cid); });
                });

            return Range<TComps..., TSelectComps...>(selected_entities);
        }

        /**
         * @brief Apply a function to selected entities' components, to which only entities with components that fufill the condition in the function are further selected
         * 
         * @param functor a function that takes all selected components as parameter and returns a boolean value indicating whether the entity will be further selected
         * @return Range of entities with selected components
         */
        template <typename TFunctor>
        requires std::predicate<TFunctor, TComps...>
        Range<TComps...> &Where(TFunctor functor)
        {
            std::erase_if(entities,
                [&](EntityID eid) -> bool
                {
                    return !functor(GetEntity(eid).GetComponent<TComps>()...);
                });
            return *this;
        }

        /**
         * @brief Gets a container that allows iterating through every selected entities
         * 
         * @return an EntityContainer object that contains all selected entities
         */
        EntityContainer Entities() const
        {
            return EntityContainer(entities);
        }

        /**
         * @brief Gets a container that allows iterating through every selected entities' components bundled in a tuple
         * 
         * @return an ComponentContainer object that contains all selected entities
         */
        ComponentContainer<TComps...> Components()
        {
            return ComponentContainer<TComps...>(entities);
        }

        /**
         * @brief Gets a container that allows iterating through every selected entities' components bundled in a tuple
         * 
         * @return an ComponentContainer object that contains all selected entities
         */
        const ComponentContainer<TComps...> Components() const
        {
            return ComponentContainer<TComps...>(entities);
        }

        /**
         * @brief Gets a container that allows iterating through every selected entities' components of types TOnlyComps bundled in a tuple
         * 
         * @return an ComponentContainer object that contains all selected entities
         */
        template <typename... TOnlyComps>
        ComponentContainer<TOnlyComps...> OnlyComponents()
        {
            return ComponentContainer<TOnlyComps...>(entities);
        }

        /**
         * @brief Gets a container that allows iterating through every selected entities' components of types TOnlyComps bundled in a tuple
         * 
         * @return an ComponentContainer object that contains all selected entities
         */
        template <typename... TOnlyComps>
        const ComponentContainer<TOnlyComps...> OnlyComponents() const
        {
            return ComponentContainer<TOnlyComps...>(entities);
        }
    };

    /**
     * @brief Select entities with certain components
     * 
     * @tparam TSelectComps types of the components to be selected
     * @return Range of entities with selected components
     */
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