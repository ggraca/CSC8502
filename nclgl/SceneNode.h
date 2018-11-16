#pragma once

#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Mesh.h"
#include <vector>

class SceneNode {
public:
	SceneNode(Mesh* mesh = NULL, Vector4 colour = Vector4(1, 1, 1, 1));
	~SceneNode(void);

	// GameObject
	void AddChild(SceneNode* child);

	virtual void Update(float msec);
	virtual void Draw(const OGLRenderer &r);

	vector<SceneNode*>::const_iterator GetChildIteratorStart() { return children.begin(); }
	vector<SceneNode*>::const_iterator GetChildIteratorEnd() { return children.end(); }

	// Transform
	void SetTransform(const Matrix4 &matrix) { transform = matrix; }
	const Matrix4& GetTransform() const { return transform; }
	Matrix4 GetWorldTransform() const { return worldTransform; }

	Vector3 GetModelScale() const { return modelScale; }
	void SetModelScale(Vector3 ms) { modelScale = ms; }

	Vector3 GetRotation() const { return rotation; }
	void SetRotation(Vector3 r) { rotation = r; }

	// Visual
	Mesh* GetMesh() const { return mesh; }
	void SetMesh(Mesh* m) { mesh = m; }

	Vector4 GetColour() const { return colour; }
	void SetColour(Vector4 c) { colour = c; }

	// Rendering
	float GetBoundingRadius() const { return boundingRadius; }
	void SetBoundingRadius(float br) { boundingRadius = br; }

	float GetCameraDistance() const { return distanceFromCamera; }
	void SetCameraDistance(float cd) { distanceFromCamera = cd; }

	static bool CompareByCameraDistance(SceneNode* a, SceneNode* b) {
		return a->GetCameraDistance() < b->GetCameraDistance();
	}

protected:
	SceneNode* parent;
	vector<SceneNode*> children;

	Matrix4 transform, worldTransform;
	Vector3 modelScale;
	Vector3 rotation;

	Mesh* mesh;
	Vector4 colour;

	float distanceFromCamera;
	float boundingRadius;
};
