/*
* dirtyJOE - Java Overall Editor
* Copyright 2008 ReWolf
* Contact:
* rewolf@dirty-joe.com
* http://dirty-joe.com
*
* --
*
* see license.txt for more info
*
*/
#pragma once
#include <memory>

#define GETTER(_type, _name) \
	public: \
	virtual _type get_##_name() const { return _name; } \
	private: \
	_type _name;

#define GETSET(_type, _name) \
	public: \
	virtual _type get_##_name() const { return _name; } \
	virtual void set_##_name(_type x) { _name = x; } \
	private: \
	_type _name;

#define GETTER_PROT(_type, _name) \
	public: \
	virtual _type get_##_name() const { return _name; } \
	protected: \
	_type _name;

#define GETSET_PROT(_type, _name) \
	public: \
	virtual _type get_##_name() const { return _name; } \
	virtual void set_##_name(_type x) { _name = x; } \
	protected: \
	_type _name;

#define GETTER_PTR(_type, _name) \
	public: \
	virtual _type& get_##_name() { return _name; } \
	private: \
	_type _name;

#define BIT(val, bitn) \
	((val) & (1 << bitn))

struct free_delete { void operator()(void* x) { std::free(x); } };
template <typename T>
using malloc_ptr = std::unique_ptr<T, free_delete>;
template <typename T>
malloc_ptr<T> malloc(size_t size) { return malloc_ptr<T>((T*)std::malloc(size * sizeof(T))); }