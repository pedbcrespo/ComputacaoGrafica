#pragma once


inline float clamp(float v, float a, float b){
	if(v < a)
		return a;
	else if(v > b)
		return b;
	return v;
}

template<class T, int N>
using c_array = T[N];

template<class T, int N>
std::vector<T> to_vector(c_array<T,N>&& ts){
    std::vector<T> r;
    for(T& t: ts)
        r.push_back(std::move(t));
    return r;
}
