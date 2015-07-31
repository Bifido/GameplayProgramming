/*
*	Standard defines and functions
*/

#pragma once
#include <wtypes.h>

// Typedef base types
typedef MSG OS_MSG;

typedef __int8				int8;
typedef __int16				int16;
typedef __int32				int32;
typedef __int64				int64;
typedef unsigned __int8		uint8;
typedef unsigned __int16	uint16;
typedef unsigned __int32	uint32;
typedef unsigned __int64	uint64;


#define uint8MAX  0xffui8
#define uint16MAX 0xffffui16
#define uint32MAX 0xffffffffui32
#define uint64MAX 0xffffffffffffffffui64


//See bullet physics engine
#if defined(USE_DOUBLE_PRECISION)
typedef double real;
#define BT_LARGE_FLOAT 1e30
#else
typedef float real;
#define BT_LARGE_FLOAT 1e18f
#endif

#include <vector>
#include <map>
#include <hash_map>

//Redefinition of standard STL vector class
template<class T>
class MGDVector: public std::vector<T, std::allocator<T>>
{

};

//redefinition of standard STL Map class
template<class T, class V>
class MGDMap: public std::map<T, V, std::less<T>, std::allocator<std::pair<const T, V>> >
{

};

//Redefinition of standard STL HashMap class
template<class T, class V>
class MGDHashMap: public stdext::hash_map<T, V, stdext::hash_compare<T, stdext::less<T>>, std::allocator<stdext::pair<const T, V>> >
{

};

#define ID_DECLARATION			static const ObjectId ID; virtual const ObjectId& GetID() const { return ID; };
#define ID_IMPLEMENTATION(c)	const ObjectId c::ID(#c);


//Id used to idetifier an intance object
class ObjectId
{
public:
	ObjectId() : m_uiId(0) {}
	//ObjectId(uint32 i_uiId) : m_uiId(i_uiId) {}
	explicit ObjectId(const char* i_pId) : m_uiId(CalculateFNV(i_pId)), m_szId(i_pId) {}

	~ObjectId() {}
	size_t GetId() const { return m_uiId; }

//#ifdef _DEBUG
	const std::string& GetDebugName() const
	{
		return m_szId;
	}
//#endif // _DEBUG
	
	bool operator==(const ObjectId& i_rObj) const;
	bool operator!=(const ObjectId& i_rObj) const;
	bool operator< (const ObjectId& i_rObj) const;
	bool operator> (const ObjectId& i_rObj) const;
	bool operator<=(const ObjectId& i_rObj) const;
	bool operator>=(const ObjectId& i_rObj) const;

private:
	size_t m_uiId;

//#ifdef _DEBUG
	std::string m_szId;
//#endif // _DEBUG

	size_t CalculateFNV(const char* str)
	{
		const size_t length = strlen(str) + 1;
		size_t hash = 2166136261u;
		for (size_t i=0; i<length; ++i)
		{
			hash ^= *str++;
			hash *= 16777619u;
		}
		return hash;
	}
};

inline bool ObjectId::operator==(const ObjectId& i_rObj) const
{
	bool res = (m_uiId == i_rObj.m_uiId);
	return res;
}

inline bool ObjectId::operator!=(const ObjectId& i_rObj) const
{
	bool res = (m_uiId != i_rObj.m_uiId);
	return res;
}

inline bool ObjectId::operator<(const ObjectId& i_rObj) const
{
	bool res = (m_uiId < i_rObj.m_uiId);
	return res;
}

inline bool ObjectId::operator>(const ObjectId& i_rObj) const
{
	bool res = (m_uiId > i_rObj.m_uiId);
	return res;
}

inline bool ObjectId::operator<=(const ObjectId& i_rObj) const
{
	bool res = (m_uiId <= i_rObj.m_uiId);
	return res;
}

inline bool ObjectId::operator>=(const ObjectId& i_rObj) const
{
	bool res = (m_uiId >= i_rObj.m_uiId);
	return res;
}


static const ObjectId INVALID_HASH = ObjectId();

/*
*	TYPEDEF
*/

typedef ObjectId			StateID;
typedef ObjectId			TransitionID;
typedef ObjectId			FSMID;
typedef ObjectId			AudioID;