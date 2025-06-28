#ifndef YIM_H
#define YIM_H

#include <string>
#include <vector>

#include "space.hpp"

using namespace std;

class Face;
class Surface;
class ObjWavefront;

class YIMMaterial {
public:
	float color[3];

	YIMMaterial();
	YIMMaterial(const Vector3& color);
};

class YIMFace {
private:
	int size;
	int dims[3];
	Vector3 center;
public:
	int normal_index;
	int vertex_indices[3];
	YIMFace(const Face& face);
};

class YIMRaster {
private:
	int face_index;
	vector<Vector3> positions;
public:
};

class YIMVolume {
private:
	int level;
	int dims[3];
	Vector3 position;
public:
};

class YIMSurface {
public:
	int material_index;
	string name;
	vector<YIMFace> faces;

	bool isEmpty();
	void addFace(const Face& face);
};

class YIMObject {
private:
	vector<Vector3> normals;
	vector<Vector3> vertices;
	vector<YIMMaterial> materials;
	vector<YIMSurface> surfaces;
public:
	string name;

	YIMObject(const ObjWavefront& model);

	void write(const string& filename);
};

#endif // YIM_H