#pragma once
#include "ISystem.h"
#include "engine/core/debug.h"
#include "engine/core/memory/memory.h"

// for using RTTI
#include <type_traits>
#include <typeindex>

#include <memory>
#include <unordered_map>

namespace deadrop
{
    namespace systems
    {
        // NOTE: this class makes use of RTTI (Run-Time Type Information)
        // NOTE: this requirement could be removed by changing the way systems identify themselves,
        // for example using a guid system
        class CoreSystem
        {
        public:
            // delete the copy assignment operator
            CoreSystem operator=(const CoreSystem&) = delete;

            // register a system
            template<class T>
            T* Register();

            // check if a system is registered
            template<class T>
            bool Has();

            // get a reference to the registered system
            template<class T>
            T* Get();

            // unregister a system
            template<class T>
            bool UnRegister();

        private:
            // helpers
            typedef std::type_index TypeIndex;

            // NOTE: this function makes use of RTTI
            template<class T>
            static TypeIndex getTypeIndex()
            {
                return std::type_index(typeid(T));
            }

            // storage for all the systems with their type_index being the key
            std::unordered_map<TypeIndex, uptr<ISystem>> m_systems;
        };

        // definition of the template functions
        template<class T>
        T* CoreSystem::Register()
        {
            // enforce that all systems must inherit from the ISystem interface
            static_assert(std::is_base_of<ISystem, T>::value, "All systems must inherit from ISystem!");

            // check if a system exists before registering it,
            // if it does, return it instead of creating a new instance
            auto type_index = getTypeIndex<T>();
            if (Has<T>())
            {
                // it is guaranteed that m_systems contains that system 
                // because of the previous call to Has<T>()
                auto result = m_systems.find(type_index);
                return static_cast<T*>(result->second.get());
            }

            auto system_ptr = std::make_unique<T>();
            // store the pointer of the system
            m_systems.insert(std::make_pair(type_index, std::move(system_ptr)));
            // return the stored pointer so it can be used by the caller
            return static_cast<T*>(m_systems[type_index].get());
        }

        template<class T>
        bool CoreSystem::Has()
        {
            // enforce that all systems must inherit from the ISystem interface
            static_assert(std::is_base_of<ISystem, T>::value, "All systems must inherit from ISystem!");

            auto type_index = getTypeIndex<T>();
            auto result = m_systems.find(type_index);
            if (result != m_systems.end())
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        template<class T>
        T* CoreSystem::Get()
        {
            // enforce that all systems must inherit from the ISystem interface
            static_assert(std::is_base_of<ISystem, T>::value, "All systems must inherit from ISystem!");

            // we are not using Has<T>() because we need access to the system pointer
            auto type_index = getTypeIndex<T>();
            auto result = m_systems.find(type_index);
            if (result != m_systems.end())
            {
                // the system was found, return a pointer to it
                return static_cast<T*>(result->second.get());
            }
            else
            {
                //error, requested system pointer does not exist
                DebugBreak();
                return nullptr;
            }
        }

        template<class T>
        bool CoreSystem::UnRegister()
        {
            // enforce that all systems must inherit from the ISystem interface
            static_assert(std::is_base_of<ISystem, T>::value, "All systems must inherit from ISystem!");

            auto type_index = getTypeIndex<T>();
            auto result = m_systems.erase(type_index);
            // the call to erase() will return the number of elements removed,
            // if it is equal to one, then it was successful
            if(result == 1)
            {
                // the successfully system has been removed
                return false;
            }
            else
            {
                // the system was not removed or does not exist
                return false;
            }
        }

        namespace detail
        {
            // keep a global reference to an instance of the CoreSystem
            // so it can be used directly without creating an instance
            CoreSystem& instance();
        }

        // forward calls to these function to the 
        // global instance we created previously
        template<class T>
        T* Register()
        {
            return detail::instance().Register<T>();
        }

        template<class T>
        bool Has()
        {
            return detail::instance().Has<T>();
        }

        template<class T>
        T* Get()
        {
            return detail::instance().Get<T>();
        }

        template<class T>
        bool UnRegister()
        {
            return detail::instance().UnRegister<T>();
        }
    }
}