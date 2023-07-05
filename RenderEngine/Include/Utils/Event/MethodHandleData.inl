namespace RenderEngine::Utils
{
    template <typename C, typename... Args>
    MethodHandleData<C, Args...>::MethodHandleData(C* _caller, void(C::* _func)(Args...)) :
        caller{ _caller },
        func{ _func }
    {
    }

    template <typename C, typename... Args>
    bool MethodHandleData<C, Args...>::Compare(const MethodHandleDataBase* _other) const 
    {
        const MethodHandleData* const otherData = reinterpret_cast<const MethodHandleData*>(_other);
        if(otherData != nullptr)
            return caller == otherData->caller && func == otherData->func;
        return false;
    }

    template <typename C, typename... Args>
    void MethodHandleData<C, Args...>::InterfaceCall(void* _data, Args... _args)
    {
        MethodHandleData* const fData = reinterpret_cast<MethodHandleData*>(_data);
        if (fData != nullptr)
            (fData->caller->*fData->func)(std::forward<Args>(_args)...);
    }
}