#pragma once
#include "of3dPrimitives.h"


//#define SINGLE_TRIANGLE

namespace kaleidome {
	enum LABEL {
		A = 0,
		B,
		C
	};

	using EdgeIndex = int;
	using VertexIndex = int;

	struct Vertex {
		float x, y, z;
		LABEL label;
		Vertex(float _x, float _y, float _z, LABEL _label) {
			set(_x, _y, _z, _label);
		}
		void set(float _x, float _y, float _z, LABEL _label) {
			x = _x, y = _y, z = _z, label = _label;
		}
		float length() {
			return sqrt(x*x + y*y + z*z);
		}
		void normalize() {
			float mag = 1.0 / length();
			x *= mag;
			y *= mag;
			z *= mag;
		}
	};

	struct Triangle {
		std::vector<EdgeIndex> edges{-1,-1,-1};
		Triangle(EdgeIndex e1, EdgeIndex e2, EdgeIndex e3) {
			setEdgeIndex( e1, e2, e3);
		}
		void setEdgeIndex(EdgeIndex e1, EdgeIndex e2, EdgeIndex e3) {
			edges[0] = e1;
			edges[1] = e2;
			edges[2] = e3;
		}
		
	};

	struct Edge {
		std::pair<VertexIndex, VertexIndex> vtcs{-1,-1};
		Edge(VertexIndex v1, VertexIndex v2) {
			setVertexIndex(v1, v2);
		}
		void setVertexIndex(VertexIndex v1, VertexIndex v2) {
			vtcs.first = v1;
			vtcs.second = v2;
		}
	};

	struct VertexPool {
		std::vector<Vertex> pool;
		VertexIndex addVertex(Vertex v) {
			pool.push_back(v);
			return pool.size() - 1;
		}
	};

	struct EdgePool {
		std::vector<Edge> pool;
		EdgeIndex addEdge(Edge e) {
			pool.push_back(e);
			return pool.size() - 1;
		}
		EdgeIndex sharedEdge(VertexIndex v1, VertexIndex v2) {
			EdgeIndex idx = -1;
			for (auto ite = pool.begin(); ite != pool.end(); ite++) {
				Edge e = *ite;
				if (
					(e.vtcs.first == v1 && e.vtcs.second == v2) ||
					(e.vtcs.first == v2 && e.vtcs.second == v1)) {
					idx = std::distance(pool.begin(), ite);
					break;
				}
			}
			return idx;
		}
		bool isSharedVertex(VertexIndex vidx, EdgeIndex eidx) {
			bool result = false;
			std::pair<VertexIndex, VertexIndex> p = pool[eidx].vtcs;
			if (pool[eidx].vtcs.first == vidx || pool[eidx].vtcs.second == vidx) {
				result = true;
			}
			return result;
		}
	};

	static Vertex medianPoint(const Vertex& p1, const Vertex& p2);
	static LABEL getExeptLabel( const LABEL l1, const LABEL l2);
	static std::vector<VertexIndex> getVertexIndexList(Triangle triangle, EdgePool pool);

	static ofColor getLabelColor(LABEL label) {
		static std::map<LABEL, ofColor> labelColor =
		{
			{ LABEL::A, ofColor::red },
			{ LABEL::B, ofColor::blue },
			{ LABEL::C, ofColor::green }
		};
		return labelColor[label];
	}

	static ofVec2f getLabelUV(LABEL label) {
		static std::map<LABEL, ofVec2f> labelUV =
		{
			{ LABEL::A, ofVec2f(cos(90 * PI / 180.0f)* 0.5 + 0.5,sin(90 * PI / 180.0f)* 0.5 + 0.5) },
			{ LABEL::B, ofVec2f(cos(210 * PI / 180.0f)* 0.5 + 0.5,sin(210 * PI / 180.0f)* 0.5 + 0.5) },
			{ LABEL::C, ofVec2f(cos(330 * PI / 180.0f)* 0.5 + 0.5,sin(330 * PI / 180.0f)* 0.5 + 0.5) }
		};
		return labelUV[label];
	}




	class Kaleidome : public of3dPrimitive
	{

	public:
		Kaleidome();
		Kaleidome(float radius, int iterations);
		~Kaleidome();

	private:
		void subdivision(int iterations);
		void calcTexCoord();
		void adjustVertexColor();
		void fixEdge();

		VertexPool vpool;
		float radius;
		int iterations;
	};
};