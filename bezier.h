#ifndef BEZIER_H
#define BEZIER_H

#include "vec.h"

template<int N>
Vec<N> cubic_bezier(const Vec<N> P[4], float t){
	Vec<N> Q[3] = {
		lerp(t, P[0], P[1]),
		lerp(t, P[1], P[2]),
		lerp(t, P[2], P[3]),
	};

	Vec<N> R[2] = {
		lerp(t, Q[0], Q[1]),
		lerp(t, Q[1], Q[2]),
	};

	return lerp(t, R[0], R[1]);
}

template<int N>
std::vector<Vec<N>> sample_cubic_bezier(const Vec<N> P[4], int n){
	std::vector<Vec<N>> C(n);
	for(int i = 0; i < n; i++){
		float t = i/(n-1.0);
		C[i] = cubic_bezier(P, t);
	}
	return C;
}

template<int N>
std::vector<Vec<N>> sample_cubic_bezier_spline(const std::vector<Vec<N>>& P, int n_samples){
	std::vector<Vec<N>> C;
	int n = P.size();
	for(int i = 0; i+3 < n; i+=3){
		std::vector<vec2> Q = sample_cubic_bezier(P.data() + i, n_samples);
		C.insert(C.end(), Q.begin(), Q.end());
	}
	return C;
}

template<int N>
Vec<N> bezier(const std::vector<Vec<N>>& P, float t){
	int n = P.size();
	if(n == 1)
		return P[0];

	std::vector<Vec<N>> Q(n-1);
	for(int i = 0; i < n-1; i++)
		Q[i] = lerp(t, P[i], P[i+1]);

	return bezier(Q, t);
}

template<int N>
std::vector<Vec<N>> sample_bezier(const std::vector<Vec<N>>& P, int n_samples){
	std::vector<Vec<N>> C(n_samples);
	for(int i = 0; i < n_samples; i++){
		float t = i/(n_samples-1.0);
		C[i] = bezier(P, t);
	}
	return C;
}

#endif
