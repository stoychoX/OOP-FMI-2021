//
//simple class pair written by 82275 on 5.15.2021
//
#pragma once
template<class T, class U>
class pair {
public:
	T first;
	U second;

	pair();
	pair(const T& _first, const U& _second);

	//helpers:
	pair make_pair(const T& _valFirst, const U& _valSecond) const;

	const T& c_first() const;
	const U& c_second() const;
};

template<class T, class U>
pair<T, U>::pair() : first(T()), second(U()){}

template<class T, class U>
pair<T, U>::pair(const T& _first, const U& _second) : first(_first), second(_second) {}

template<class T, class U>
pair<T, U> pair<T, U>::make_pair(const T& _valFirst, const U& _valSecond) const{
	return pair(_valFirst, _valSecond);
}

template<class T, class U>
const T& pair<T, U>::c_first() const{
	return this->first;
}

template<class T, class U>
const U& pair<T, U>::c_second() const {
	return this->second;
}
