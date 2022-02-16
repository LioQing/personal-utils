#pragma once

#include <algorithm>
#include <array>
#include <atomic>
#include <bitset>
#include <functional>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <memory>

class lic
{
public:

    // Max number of components, modify as needed.
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

        /**
         * @brief Get the entity of this component
         * 
         * @return The entity of this component
         */
        Entity GetEntity() const
        {
            return lic::GetEntity(entity_id);
        }

        /**
         * @brief Set a callback function to be called when the component is being removed from or destroyed with entity
         * 
         * @param callback The function to be called when the compeonent is being removed
         */
        void OnRemoval(const std::function<void()>& callback) const
        {
            lic::OnComponentRemoval<TComp>(entity_id, callback);
        };
    };

    // Entity class (implicit convertable to EntityID)
    struct Entity
    {
        // ID of the entity
        EntityID id;

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
         * @return bool Boolean indicating the result
         */
        bool HasComponent(ComponentID cid) const;

        /**
         * @brief Check if this entity has a component
         * 
         * @tparam TComp type of the component to be checked
         * @return boo Boolean indicating the result
         */
        template <typename TComp>
        bool HasComponent() const;

        /**
         * @brief Add a component to this entity, re-construct the old component if the entity already has the component type
         * 
         * @tparam TComp type of the component to be added
         * @param args arguments to be passed to the component for initialization
         * @return Reference to the component added (with wrapper class)
         */
        template <typename TComp, typename... TArgs>
        Component<TComp>& AddComponent(TArgs&&... args) const;

        /**
         * @brief Add a component to this entity, re-construct the old component if the entity already has the component type
         * 
         * @param c the component to be added
         * @return Reference to the component added (with wrapper class) 
         */
        template <typename TComp>
        Component<TComp>& AddComponent(const TComp& c) const;

        /**
         * @brief Get a component from this entity
         * 
         * @tparam TComp type of the component to be returned
         * @return Reference to the component returned (with wrapper class)
         */
        template <typename TComp>
        Component<TComp>& GetComponent() const;

        /**
         * @brief Get the component if this entity has the component, else return a default constructed component
         * 
         * @tparam TComp type of the component to be returned
         * @return The component (without wrapper class)
         */
        template <typename TComp>
        requires std::is_default_constructible<TComp>::value
        TComp GetIfHasComponentElseDefault() const;

        /**
         * @brief Get the component if this entity has the component, else return a new constructed component with specific arguments
         * 
         * @tparam TComp type of the component to be returned
         * @param args arguments to be passed to the new component for construction if this entiy does not have the component
         * @return The component (without wrapper class)
         */
        template <typename TComp, typename... TArgs>
        TComp GetIfHasComponentElse(TArgs&&... args) const;

        /**
         * @brief Set a callback function to be called when the component is being removed from or destroyed with entity
         * 
         * @param cid ID of the component
         * @param callback The function to be called when the compeonent is being removed
         */
        void OnComponentRemoval(ComponentID cid, const std::function<void()>& callback) const;

        /**
         * @brief Set a callback function to be called when the component is being removed from or destroyed with entity
         * 
         * @tparam TComp type of the component
         * @param callback The function to be called when the compeonent is being removed
         */
        template <typename TComp>
        void OnComponentRemoval(const std::function<void()>& callback) const;        
    
        /**
         * @brief Call and remove the on component removal callback function manually
         * 
         * @param cid ID of the component
         */
        void RaiseComponentRemoval(ComponentID cid) const;

        /**
         * @brief Call and remove the on component removal callback function manually
         * 
         * @param eid ID of the entity where the component belongs
         */
        template <typename TComp>
        void RaiseComponentRemoval() const;

        operator EntityID() const { return id; }
    };

private:

    // Information of Entites for internal use only
    struct EntityInfo
    {
        // ID of the entity
        EntityID id;

        // Bitfield indicating whether the entity contains a component
        std::bitset<LIC_MAX_COMPONENT> component_field;

        // Hashmap containing component's ID and index in the component vector
        std::unordered_map<ComponentID, size_t> component_indices;

        operator EntityID() const { return id; }
    };

    // Vector of vectors of components (std::vector<Component<TComp>>)
    static std::array<void*, LIC_MAX_COMPONENT> components;

    // Vector of destroyed components
    static std::array<std::vector<std::size_t>, LIC_MAX_COMPONENT> destroyed_components;

    // Vector of on component destroy callback functions
    static std::array<std::vector<std::vector<std::function<void()>>>, LIC_MAX_COMPONENT> on_component_removals;

    // Vector of entities infos
    static std::vector<EntityInfo> entities;

    // Vector of destroyed entity IDs
    static std::vector<EntityID> destroyed_entities;

    // Next component ID to be generated
    static ComponentID next_component_id;

    // Next entity ID to be generated
    static EntityID next_entity_id;

    /**
     * @brief Get the vector of component type TComp
     * 
     * @tparam TComp type of the component of the vector to be retrieved
     * @return Reference to the vector containing all instances of component type TComp
     */
    template <typename TComp>
    static std::vector<std::unique_ptr<Component<TComp>>>& GetComponentVec()
    {
        return *static_cast<std::vector<std::unique_ptr<Component<TComp>>>*>(components.at(GetComponentID<TComp>()));
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
    requires (!requires (TComp x) {
        { Component{x} } -> std::same_as<TComp>;
    })
    static ComponentID GetComponentID()
    {
        static ComponentID new_id = [&]()
        {
            components.at(next_component_id) = new std::vector<std::unique_ptr<Component<TComp>>>();
            destroyed_components.at(next_component_id) = std::vector<std::size_t>();
            on_component_removals.at(next_component_id) = std::vector<std::vector<std::function<void()>>>();
            return next_component_id++;
        }();
        return new_id;
    }

    /**
     * @brief Get the ID of a component type by extracting it from a Componen<T> class
     * If this is the first time this function is called with the type, a new ID will be generated
     * 
     * @param comp Component which the ID will be returned
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
     * @param comp Component which the ID will be returned
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
     * @return The entity added
     */
    static Entity AddEntity();

    /**
     * @brief Destroy an entity (including its components)
     * 
     * @param eid ID of the entity to be destroyed
     */
    static void DestroyEntity(EntityID eid);

    /**
     * @brief Check whether an entity is alive (added and not destroyed)
     * 
     * @param eid ID of the entity
     * @return bool Boolean indicating whether the entity is alive
     */
    static bool HasEntity(EntityID eid);

    /**
     * @brief Get an entity
     * 
     * @param eid ID of the entity to be returned
     * @return The entity
     */
    static Entity GetEntity(EntityID eid);

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
     * @return bool Boolean indicating the result
     */
    static bool HasComponent(EntityID eid, ComponentID cid);

    /**
     * @brief Check if an entity has a component
     * 
     * @tparam TComp type of the component to be checked
     * @param eid ID of the entity where the component will be checked
     * @return bool Boolean indicating the result
     */
    template <typename TComp>
    static bool HasComponent(EntityID eid)
    {
        return HasComponent(eid, GetComponentID<TComp>());
    }

    /**
     * @brief Add a component to an entity, re-construct the old component if the entity already has the component type
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
        std::vector<std::unique_ptr<Component<TComp>>>& component_vec = GetComponentVec<TComp>();
        size_t i = 0;

        // Re-construct component if entity already has the component
        if (HasComponent(eid, cid))
        {
            component_vec.at(entities.at(eid).component_indices.at(cid)) = std::make_unique<Component<TComp>>(TComp(std::forward<TArgs>(args)...), eid);
            return GetComponent<TComp>(eid);
        }

        // Add component to component_vec
        if (destroyed_components.at(cid).empty())
        {
            i = component_vec.size();
            component_vec.emplace_back(new Component<TComp>(TComp(std::forward<TArgs>(args)...), eid));
            on_component_removals.at(cid).emplace_back(std::vector<std::function<void()>>());
        }
        else
        {
            auto& available_slots = destroyed_components.at(cid);
            i = available_slots.back();
            component_vec.at(i) = std::make_unique<Component<TComp>>(TComp(std::forward<TArgs>(args)...), eid);
            available_slots.pop_back();
        }

        // Store component index and field in entity
        entities.at(eid).component_indices.emplace(cid, i);
        entities.at(eid).component_field.set(cid);
        return *component_vec.at(i);
    }

    /**
     * @brief Add a component to an entity, re-construct the old component if the entity already has the component type
     * 
     * @param eid ID of the entity where the component will be added
     * @param c the component to be added
     * @return Reference to the component added (with wrapper class) 
     */
    template <typename TComp>
    static Component<TComp>& AddComponent(EntityID eid, const TComp& c)
    {
        return AddComponent<TComp, decltype(c)>(eid, c);
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
        if (!HasComponent<TComp>(eid))
        {
            throw std::out_of_range(std::string("Component ") + typeid(TComp).name() + " not found in Entity " + std::to_string(eid));
        }

        return *GetComponentVec<TComp>().at(entities.at(eid).component_indices.at(GetComponentID<TComp>()));
    }

    /**
     * @brief Set a callback function to be called when the component is being removed from or destroyed with entity
     * 
     * @param eid ID of the entity where the component belongs
     * @param cid ID of the component
     * @param callback The function to be called when the compeonent is being removed
     */
    static void OnComponentRemoval(EntityID eid, ComponentID cid, const std::function<void()>& callback);

    /**
     * @brief Set a callback function to be called when the component is being removed from or destroyed with entity
     * 
     * @tparam TComp type of the component
     * @param eid ID of the entity where the component belongs
     * @param callback The function to be called when the compeonent is being removed
     */
    template <typename TComp>
    static void OnComponentRemoval(EntityID eid, const std::function<void()>& callback)
    {
        OnComponentRemoval(eid, GetComponentID<TComp>(), callback);
    }

    /**
     * @brief Call and remove the on component removal callback function manually
     * 
     * @param eid ID of the entity where the component belongs
     * @param cid ID of the component
     */
    static void RaiseComponentRemoval(EntityID eid, ComponentID cid);

    /**
     * @brief Call and remove the on component removal callback function manually
     * 
     * @tparam TComp type of the component
     * @param eid ID of the entity where the component belongs
     */
    template <typename TComp>
    static void RaiseComponentRemoval(EntityID eid)
    {
        RaiseComponentRemoval(eid, GetComponentID<TComp>());
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

            Entity operator*() const;
        };

        Iterator begin() const;
        Iterator end() const;
        Iterator cbegin() const;
        Iterator cend() const;
    };

    /**
     * @brief A container class for storing selected entities and iterating through their components
     * 
     * @tparam IncludeEntities boolean indicating whether entities will be returned with the components
     * @tparam TComps types of selected entities' components
     */
    template <bool IncludeEntities, typename... TComps>
    struct ComponentContainer : public std::vector<EntityID>
    {
        template <std::contiguous_iterator TBackingIter>
        struct BaseIterator : public TBackingIter
        {
            BaseIterator(const TBackingIter& iter) : TBackingIter(iter) {}

            auto operator*()
            {
                if constexpr (IncludeEntities == true)
                {
                    if constexpr (std::is_same<TBackingIter, std::vector<EntityID>::const_iterator>::value)
                        return std::tuple<Entity, const Component<TComps>&...>(GetEntity(TBackingIter::operator*()), GetComponent<TComps>(TBackingIter::operator*())...);
                    else
                        return std::tuple<Entity, Component<TComps>&...>(GetEntity(TBackingIter::operator*()), GetComponent<TComps>(TBackingIter::operator*())...);
                }
                else
                {
                    if constexpr (std::is_same<TBackingIter, std::vector<EntityID>::const_iterator>::value)
                        return std::tie<const Component<TComps>...>(GetComponent<TComps>(TBackingIter::operator*())...);
                    else
                        return std::tie<Component<TComps>...>(GetComponent<TComps>(TBackingIter::operator*())...);
                }
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
         * @param predicate A function that takes all selected entity and components as parameter and returns a boolean value indicating whether the entity will be further selected
         * @return Range of entities with selected components
         */
        template <std::predicate<Entity, TComps...> TPred>
        Range<TComps...> Where(TPred predicate) const
        {
            auto temp_entities = entities;
            std::erase_if(temp_entities,
                [&](EntityID eid) -> bool
                {
                    auto entity = GetEntity(eid);
                    return !predicate(entity, entity.GetComponent<TComps>()...);
                });
            return Range<TComps...>(temp_entities);
        }

        /**
         * @brief Gets a container that allows iterating through every selected entities
         * 
         * @return An EntityContainer object that contains all selected entities
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
        ComponentContainer<false, TComps...> Components()
        {
            return ComponentContainer<false, TComps...>(entities);
        }

        /**
         * @brief Gets a container that allows iterating through every selected entities' components bundled in a tuple
         * 
         * @return an ComponentContainer object that contains all selected entities
         */
        const ComponentContainer<false, TComps...> Components() const
        {
            return ComponentContainer<false, TComps...>(entities);
        }

        /**
         * @brief Gets a container that allows iterating through every selected entities' components of types TOnlyComps bundled in a tuple
         * 
         * @return an ComponentContainer object that contains all selected entities
         */
        template <typename... TOnlyComps>
        ComponentContainer<false, TOnlyComps...> OnlyComponents()
        {
            return ComponentContainer<false, TOnlyComps...>(entities);
        }

        /**
         * @brief Gets a container that allows iterating through every selected entities' components of types TOnlyComps bundled in a tuple
         * 
         * @return an ComponentContainer object that contains all selected entities
         */
        template <typename... TOnlyComps>
        const ComponentContainer<false, TOnlyComps...> OnlyComponents() const
        {
            return ComponentContainer<false, TOnlyComps...>(entities);
        }

        /**
         * @brief Gets a container that allows iterating through every selected entities and their components bundled in a tuple
         * 
         * @return an ComponentContainer object that contains all selected entities
         */
        ComponentContainer<true, TComps...> EntitiesAndComponents()
        {
            return ComponentContainer<true, TComps...>(entities);
        }

        /**
         * @brief Gets a container that allows iterating through every selected entities and their components bundled in a tuple
         * 
         * @return an ComponentContainer object that contains all selected entities
         */
        const ComponentContainer<true, TComps...> EntitiesAndComponents() const
        {
            return ComponentContainer<true, TComps...>(entities);
        }

        /**
         * @brief Gets a container that allows iterating through every selected entities and their components of types TOnlyComps bundled in a tuple
         * 
         * @return an ComponentContainer object that contains all selected entities
         */
        template <typename... TOnlyComps>
        ComponentContainer<true, TOnlyComps...> EntitiesAndOnlyComponents()
        {
            return ComponentContainer<true, TOnlyComps...>(entities);
        }

        /**
         * @brief Gets a container that allows iterating through every selected entities and their components of types TOnlyComps bundled in a tuple
         * 
         * @return an ComponentContainer object that contains all selected entities
         */
        template <typename... TOnlyComps>
        const ComponentContainer<true, TOnlyComps...> EntitiesAndOnlyComponents() const
        {
            return ComponentContainer<true, TOnlyComps...>(entities);
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
    lic::RemoveComponent(this->id, GetComponentID<TComp>());
}

template <typename TComp>
bool lic::Entity::HasComponent() const
{
    return lic::HasComponent(this->id, GetComponentID<TComp>());
}

template <typename TComp, typename... TArgs>
lic::Component<TComp>& lic::Entity::AddComponent(TArgs&&... args) const
{
    return lic::AddComponent<TComp>(this->id, std::forward<TArgs>(args)...);
}

template <typename TComp>
lic::Component<TComp>& lic::Entity::AddComponent(const TComp& c) const
{
    return lic::AddComponent<TComp>(this->id, c);
}

template <typename TComp>
lic::Component<TComp>& lic::Entity::GetComponent() const
{
    return lic::GetComponent<TComp>(this->id);
}

template <typename TComp>
requires std::is_default_constructible<TComp>::value
TComp lic::Entity::GetIfHasComponentElseDefault() const
{
    if (HasComponent<TComp>())
    {
        return GetComponent<TComp>();
    }
    return TComp();
}

template <typename TComp, typename... TArgs>
TComp lic::Entity::GetIfHasComponentElse(TArgs&&... args) const
{
    if (HasComponent<TComp>())
    {
        return GetComponent<TComp>();
    }
    return TComp(std::forward<TArgs>(args)...);
}

template <typename TComp>
void lic::Entity::OnComponentRemoval(const std::function<void()>& callback) const
{
    lic::OnComponentRemoval(id, GetComponentID<TComp>(), callback);
}

template <typename TComp>
void lic::Entity::RaiseComponentRemoval() const
{
    RaiseComponentRemoval(id, GetComponentID<TComp>());
}