#pragma once

namespace CGL::Core::Utils
{
    template <typename T>
    class Singleton final
    {
    public:
        static T& Get() noexcept { return *GetPtr(); }

        static void Destroy() noexcept
        {
            T* ptr = GetPtr();
            if (ptr)
            {
                delete ptr;
                ptr = nullptr;
            }
        }

        static bool Exists() noexcept { return s_instanceCreated; }

        Singleton(Singleton&&)            = delete;
        Singleton(const Singleton&)       = delete;
        Singleton& operator=(Singleton&&) = delete;
        Singleton& operator=(Singleton&)  = delete;

    private:
        static T* GetPtr() noexcept
        {
            static Singleton<T> s_instance;
            if (!s_instanceCreated)
            {
                s_instanceCreated = true;
            }
            return s_instance.m_obj;
        }

    private:
        Singleton() = default;

        T* m_obj                             = new T();
        static inline bool s_instanceCreated = false;
    };
}  // namespace CGL::Core::Utils
