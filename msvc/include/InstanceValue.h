#ifndef __INSTANCE_VALUE_H
#define __INSTANCE_VALUE_H

class InstanceValueException {};

#ifdef WIN32
	typedef HWND InstanceValueLookup_t;
#else
	typedef unsigned int InstanceValueLookup_t;
#endif

template<typename T>
class InstanceValue_t : public std::map<InstanceValueLookup_t, typename T> 
{
public:
	const T  get   (InstanceValueLookup_t i) const;
	T        get   (InstanceValueLookup_t i);
	void     set   (InstanceValueLookup_t i, T val);
	T        unset (InstanceValueLookup_t i);
};

template<typename T>
inline T InstanceValue_t<T>::get(InstanceValueLookup_t i)	
{
	iterator itr = find(i);

	if (itr == end())
	{
		throw InstanceValueException();
	}

	return itr->second;
}

template<typename T>
inline const T InstanceValue_t<T>::get(InstanceValueLookup_t i)	const
{
	const_iterator itr = find(i);

	if (itr == end())
	{
		throw InstanceValueException();
	}

	return itr->second;
}

template<typename T>
inline void InstanceValue_t<T>::set(InstanceValueLookup_t i, T val)	
{
	iterator itr = find(i);

	if (itr != end())
	{
		throw InstanceValueException();
	}
	
	insert(value_type(i, val));
}

template<typename T>
inline T InstanceValue_t<T>::unset(InstanceValueLookup_t i)
{
	iterator itr = find(i);

	if (itr == end())
	{
		throw InstanceValueException();
	}

	T t = itr->second;

	erase(itr);

	return t;
}

#endif
