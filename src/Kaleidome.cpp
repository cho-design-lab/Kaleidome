#include "Kaleidome.h"
using namespace kaleidome;

Kaleidome::Kaleidome() :
	radius(1.0f),iterations(1),
	of3dPrimitive()
{
	subdivision(iterations);
}

Kaleidome::Kaleidome(float _radius, int _iterations):
	radius(_radius),iterations(_iterations),
	of3dPrimitive()
{
	subdivision(iterations);
}


Kaleidome::~Kaleidome()
{
}

void Kaleidome::subdivision(int iterations)
{
	VertexPool vpool;
	EdgePool epool;
	std::vector<Triangle> triangles;
#ifndef SINGLE_TRIANGLE
	//// initialize
	// 頂点生成
	vpool.addVertex(Vertex(0, 0, 1, LABEL::A));

	float step = 360 / 6.0;
	for (int i = 0; i < 6; i++) {
		vpool.addVertex(
			Vertex(
				cos((step * i) * PI / 180.0),
				sin((step * i) * PI / 180.0),
				0,
				i % 2 == 0 ? LABEL::B : LABEL::C
			)
		);
	}

	// 辺生成
	for (int i = 1; i < 7; i++) {
		epool.addEdge(Edge(0, i));
	}
	for (int i = 0; i < 6; i++) {
		int v1 = i + 1; int v2 = (i + 2) % 7;
		if (v2 == 0) v2 = 1;
		epool.addEdge(Edge(v1, v2));
	}
	
	// 三角形生成
	triangles.push_back(Triangle(0, 6, 1));
	triangles.push_back(Triangle(1, 7, 2));
	triangles.push_back(Triangle(2, 8, 3));
	triangles.push_back(Triangle(3, 9, 4));
	triangles.push_back(Triangle(4, 10, 5));
	triangles.push_back(Triangle(5, 11, 0));
#else
	vpool.addVertex(Vertex(0, 0, 0, A));
	vpool.addVertex(Vertex(-1, -1, 0, B));
	vpool.addVertex(Vertex(1, -1, 0, C));
	epool.addEdge(Edge(0, 1));
	epool.addEdge(Edge(0, 2));
	epool.addEdge(Edge(1, 2));
	triangles.push_back(Triangle(0, 2, 1));
#endif

	//// subdivide
	for (int i = 0; i < iterations; i++) {
		//          *                inner triangle
		//   e1_1  / \ e3_1             ne3
		//	      *---*              *------*
		// e1_2  / \ / \ e3_2         \    /
		//      *---*---*          ne1 \  /  ne2
		//	    e2_1  e2_2               *

		// begin subdivide
		EdgePool nepool;
		std::map<EdgeIndex, std::pair<EdgeIndex, EdgeIndex>> edgeMap;

		// 辺の分割
		for (auto ite = epool.pool.begin(); ite != epool.pool.end(); ite++) {
			Edge edge = *ite;
			Vertex mv = medianPoint(
				vpool.pool[edge.vtcs.first],
				vpool.pool[edge.vtcs.second]);
			VertexIndex nidx = vpool.addVertex(mv);
			Edge ne1(edge.vtcs.first, nidx); // second element => new vertex index (share)
			Edge ne2(edge.vtcs.second, nidx); // second element => new vertex index (share)
			EdgeIndex ne1idx = nepool.addEdge(ne1);
			EdgeIndex ne2idx = nepool.addEdge(ne2);

			edgeMap[(EdgeIndex)std::distance(epool.pool.begin(), ite)] = std::pair<EdgeIndex, EdgeIndex>(ne1idx, ne2idx);
		}


		// 三角形分割
		std::vector<Triangle> nTriangles;
		for (auto tri : triangles) {
			std::vector<EdgeIndex> eiList;
			std::vector<EdgeIndex> innerEdges;
			for (int i = 0; i < 3; i++) {
				eiList.push_back(edgeMap[tri.edges[i]].first);
				eiList.push_back(edgeMap[tri.edges[i]].second);
			}
			VertexIndex v0_1 = nepool.pool[edgeMap[tri.edges[0]].second].vtcs.second;
			VertexIndex v1_2 = nepool.pool[edgeMap[tri.edges[1]].second].vtcs.second;
			VertexIndex v2_0 = nepool.pool[edgeMap[tri.edges[2]].second].vtcs.second;


			Edge ne1 = Edge(v0_1, v1_2);
			Edge ne2 = Edge(v1_2, v2_0);
			Edge ne3 = Edge(v2_0, v0_1);

			innerEdges.push_back(nepool.addEdge(ne1));
			innerEdges.push_back(nepool.addEdge(ne2));
			innerEdges.push_back(nepool.addEdge(ne3));
			// 内部三角形
			nTriangles.push_back(Triangle(innerEdges[0], innerEdges[1], innerEdges[2]));
		
			// 三角形に含まれる頂点
			std::vector<VertexIndex> vilist = getVertexIndexList(tri, epool);
			for (auto vidx : vilist) {
				std::vector<EdgeIndex> shared;
				for (auto eidx : eiList) {
					if (nepool.isSharedVertex(vidx, eidx)) {
						shared.push_back(eidx);
					} 
				}

				std::vector<VertexIndex> medianPointList;
				for (auto eidx : shared) {
					VertexIndex indivisual = -1;
					if (nepool.pool[eidx].vtcs.first != vidx)
						indivisual = nepool.pool[eidx].vtcs.first;
					if (nepool.pool[eidx].vtcs.second != vidx)
						indivisual = nepool.pool[eidx].vtcs.second;
					medianPointList.push_back(indivisual);
				}

				EdgeIndex innerIdx = -1;
				for (auto eidx : innerEdges) {
					Edge e = nepool.pool[eidx];
					VertexIndex p1 = medianPointList[0], p2 = medianPointList[1];
					if (
						e.vtcs.first == medianPointList[0] && e.vtcs.second == medianPointList[1] ||
						e.vtcs.first == medianPointList[1] && e.vtcs.second == medianPointList[0]
						)
					{
						innerIdx = eidx;
						break;
					}
				}
				nTriangles.push_back(Triangle(shared[0], shared[1], innerIdx));
			}
			

		}

		// swap edges and triangles
		epool = nepool;
		triangles = nTriangles;

		// adjust
#ifndef SINGLE_TRIANGLE
		for (auto ite = vpool.pool.begin(); ite != vpool.pool.end(); ite++) {
			ite->normalize();
		}
#endif
	}
	//// 分割周長

	//// ofMeshへ変換
	ofMesh& mesh = getMesh();
	for (auto vertex : vpool.pool) {
		mesh.addVertex(ofVec3f(vertex.x, vertex.y, vertex.z));
		//mesh.addColor(getLabelColor(vertex.label)); // 頂点カラー
		mesh.addTexCoord(getLabelUV(vertex.label));
	}
	for (auto tri : triangles) {
		Edge e1 = epool.pool[tri.edges[0]];
		Edge e2 = epool.pool[tri.edges[1]];
		Edge e3 = epool.pool[tri.edges[2]];
		std::vector<int> vidxs;
		vidxs.push_back(e1.vtcs.first), vidxs.push_back(e1.vtcs.second);
		vidxs.push_back(e2.vtcs.first), vidxs.push_back(e2.vtcs.second);
		vidxs.push_back(e3.vtcs.first), vidxs.push_back(e3.vtcs.second);
		
		std::sort(vidxs.begin(), vidxs.end());
		vidxs.erase(std::unique(vidxs.begin(), vidxs.end()), vidxs.end());

		if (vidxs.size() != 3) continue;
		mesh.addTriangle(vidxs[0], vidxs[1], vidxs[2]);
	}
}

void Kaleidome::calcTexCoord()
{

}

void Kaleidome::adjustVertexColor()
{

}

void Kaleidome::fixEdge()
{

}

Vertex kaleidome::medianPoint(const Vertex& p1, const Vertex& p2)
{
	Vertex mp(0,0,0,LABEL::A);
	LABEL l = kaleidome::getExeptLabel(p1.label,p2.label);
	mp.set(
		(p1.x + p2.x) / 2.0,
		(p1.y + p2.y) / 2.0,
		(p1.z + p2.z) / 2.0,
		l
	);
	return mp;
}


LABEL kaleidome::getExeptLabel(const LABEL l1, const LABEL l2)
{
	LABEL ret = LABEL::A;
	if ((l1 == A && l2 == B) || (l1 == B && l2 == A)) {
		ret = C;
	}
	else if ((l1 == A && l2 == C) || (l1 == C && l2 == A)) {
		ret = B;
	}
	return ret;
}


std::vector<VertexIndex> kaleidome::getVertexIndexList(Triangle triangle, EdgePool pool) {
	std::vector<VertexIndex> list;
	for (auto eidx : triangle.edges) {
		list.push_back(pool.pool[eidx].vtcs.first);
		list.push_back(pool.pool[eidx].vtcs.second);
	}
	std::sort(list.begin(), list.end());
	list.erase(std::unique(list.begin(), list.end()), list.end());
	return list;
}