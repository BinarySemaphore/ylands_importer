#include "yim.hpp"

#include <unordered_map>

#include "objwavefront.hpp"

YIMMaterial::YIMMaterial() {
	this->color[0] = 0.5f;
	this->color[1] = 0.5f;
	this->color[2] = 0.5f;
}

YIMMaterial::YIMMaterial(const Vector3& color) {
	this->color[0] = color.x;
	this->color[1] = color.y;
	this->color[2] = color.z;
}

YIMFace::YIMFace(const Face& face) {
	this->normal_index = 0;
	for (int i = 0; i < 3; i++) {
		this->vertex_indices[i] = face.vert_index[i] - 1;
	}
	
	// TODO: Compute size

	// TODO: Compute dims
}

bool YIMSurface::isEmpty() {
	return this->faces.size() == 0;
}

void YIMSurface::addFace(const Face& face) {
	this->faces.emplace_back(face);
}

YIMObject::YIMObject(const ObjWavefront& model) {
	int i, j;
	Vector3 *p1, *p2, *p3;
	Vector3 normal;
	YIMSurface* default_surface;
	YIMSurface* current_surface;
	vector<int> surfaces_to_remove;
	unordered_map<Vector3, int> normal_index_map;
	unordered_map<string, YIMSurface*> material_surface_map;

	this->normals.reserve(model.norm_count);
	this->vertices.reserve(model.vert_count);
	// + 1 for default mat and surface
	this->materials.reserve(model.materials.size() + 1);
	this->surfaces.reserve(model.surface_count + 1);

	this->name = model.name;

	// Get vertices
	for (i = 0; i < model.vert_count; i++) {
		this->vertices.push_back(model.verts[i]);
	}

	// Map all similar surface + materials to same surface
	for (const pair<string, Material>& material : model.materials) {
		this->materials.emplace_back(material.second.diffuse);
		this->surfaces.emplace_back();
		// !! WARNING !!
		// Grabbing pointer from vector, make sure it doesn't realloc
		material_surface_map[material.first] = &this->surfaces.back();
		this->surfaces.back().name  = to_string(this->surfaces.size() - 1);
		this->surfaces.back().material_index = this->materials.size() - 1;
	}

	default_surface = nullptr;
	for (i = 0; i < model.surface_count; i++) {
		current_surface = nullptr;
		for (j = 0; j < model.surfaces[i].face_count; j++) {  
			if (model.surfaces[i].material_refs->find(j) != model.surfaces[i].material_refs->end()) {
				current_surface = material_surface_map[(*model.surfaces[i].material_refs)[j]];
			} else if (current_surface == nullptr) {
				// Setup default material and surface (once)
				if (default_surface == nullptr) {
					this->materials.emplace_back();
					this->surfaces.emplace_back();
					this->surfaces.back().name = "default";
					this->surfaces.back().material_index = this->materials.size() - 1;
					default_surface = &this->surfaces.back();
				}
				// Switch to default surface
				current_surface = default_surface;
			}

			if (current_surface == nullptr) continue;
			current_surface->addFace(model.surfaces[i].faces[j]);
		}
	}
	material_surface_map.clear();

	// Remove empty surfaces (caused by unused materials in OBJ)
	for (i = 0; i < this->surfaces.size(); i++) {
		if (this->surfaces[i].isEmpty()) surfaces_to_remove.push_back(i);
	}
	for (i = surfaces_to_remove.size() - 1; i >= 0; i--) {
		this->surfaces.erase(this->surfaces.begin() + surfaces_to_remove[i]);
	}
	surfaces_to_remove.clear();

	// Compute single face normals for each face  (add normal and update face normal index)
	for (YIMSurface& surface : this->surfaces) {
		for (YIMFace& face : surface.faces) {
			p1 = &this->vertices[face.vertex_indices[0]];
			p2 = &this->vertices[face.vertex_indices[1]];
			p3 = &this->vertices[face.vertex_indices[2]];
			normal = (*p2 - *p1).cross(*p3 - *p2);
			normal = normal / sqrtf(normal.dot(normal));

			if (normal_index_map.find(normal) == normal_index_map.end()) {
				normal_index_map[normal] = this->normals.size();
				this->normals.push_back(normal);
			}
			face.normal_index = normal_index_map[normal];
		}
	}
	normal_index_map.clear();
}

void YIMObject::write(const string& filename) {

}