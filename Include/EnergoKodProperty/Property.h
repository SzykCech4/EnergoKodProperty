#ifndef PROPERTY_H
#define PROPERTY_H

template<typename tParent, typename tType>
class Property
{
public:
    Property(tParent* aParent, tType& (tParent::*aGetter)() , void (tParent::*aSetter)(const tType&) = nullptr, void (tParent::*aNotify)(tType&, tType&)  = nullptr);
    Property(const Property& aOther);
    inline operator tType&();
    inline void operator=(tType& aValue);
    inline void operator=(const tType& aValue);
    inline bool operator==(tType& aValue);
    inline bool operator!=(tType& aValue);

    inline bool isReadable();
    inline bool isWritable();
    inline bool isNotifable();
protected:
    tParent* mParent;
    tType& (tParent::*mGetter)() = nullptr;
    void (tParent::*mSetter)(const tType&) = nullptr;
    void (tParent::*mNotify)(tType&, tType&) = nullptr;
};

template<typename tParent, typename tType>
Property<tParent, tType>::Property(tParent* aParent, tType& (tParent::*aGetter)() , void (tParent::*aSetter)(const tType&), void (tParent::*aNotify)(tType&, tType&))
    : mParent(aParent), mGetter(aGetter), mSetter(aSetter), mNotify(aNotify)
{
}

template<typename tParent, typename tType>
Property<tParent, tType>::Property(const Property &aOther)
{
    mParent = aOther.mParent;
    mGetter = aOther.mGetter;
    mSetter = aOther.mSetter;
    mNotify = aOther.mNotify;
}

template<typename tParent, typename tType>
Property<tParent, tType>::operator tType&()
{
    return (mParent->*mGetter)();
}

template<typename tParent, typename tType>
void Property<tParent, tType>::operator=(tType& aValue)
{
    tType aOldValue((mParent->*mGetter)());
    if(aOldValue == aValue)
        return;
    (mParent->*mSetter)(aValue);
    tType aNewValue((mParent->*mGetter)());
    (mParent->*mNotify)(aOldValue, aNewValue);
}

template<typename tParent, typename tType>
void Property<tParent, tType>::operator=(const tType& aValue)
{
    tType aOldValue((mParent->*mGetter)());
    if(aOldValue == aValue)
        return;
    (mParent->*mSetter)(aValue);
    tType aNewValue((mParent->*mGetter)());
    (mParent->*mNotify)(aOldValue, aNewValue);
}

template<typename tParent, typename tType>
bool Property<tParent, tType>::operator==(tType& aValue)
{
    return (mParent->*mGetter)() == aValue;
}

template<typename tParent, typename tType>
bool Property<tParent, tType>::operator!=(tType& aValue)
{
    return !operator==(aValue);
}

template<typename tParent, typename tType>
bool Property<tParent, tType>::isReadable()
{
    return mGetter;
}

template<typename tParent, typename tType>
bool Property<tParent, tType>::isWritable()
{
    return mSetter;
}

template<typename tParent, typename tType>
bool Property<tParent, tType>::isNotifable()
{
    return mNotify;
}

#endif  // PROPERTY_H
