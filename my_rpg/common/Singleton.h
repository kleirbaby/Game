#pragma once

template<typename T>
class Singleton
{
public:
	static T* getInstance()
	{
		if (nullptr == value_){
			value_ = new T();
		}
		return value_;
	}

	static void clean()
	{
		if(value_){
			delete value_;
			value_ = nullptr;
		}
	}

	Singleton() = default;
	~Singleton()= default;

	Singleton(const Singleton& s) = delete;
	Singleton& operator=(const Singleton& s) = delete;
private:
	static T* value_;
};

template<typename T>
T* Singleton<T>::value_ = nullptr;