#include "SceneNode.h"

SceneNode::SceneNode(Mesh* m, Vector4 colour) {
  parent = NULL;
  modelScale = Vector3(1, 1, 1);

  this->mesh = m;
  this->colour = colour;
  this->rotation = Vector3(1, 1, 1);

  boundingRadius = 1.0f;
  distanceFromCamera = 0.0f;

  active = true;
}

SceneNode::~SceneNode(void) {
  for(unsigned int i = 0; i < children.size(); ++i)
    delete children[i];
  // if (mesh) delete mesh;
}

void SceneNode::AddChild(SceneNode* child) {
  children.push_back(child);
  child->parent = this;
}

void SceneNode::Draw(const OGLRenderer &r) {
  if (mesh) mesh->Draw();
}

void SceneNode::Update(float msec) {
  if(parent) worldTransform = parent->worldTransform * transform;
  else worldTransform = transform;

  for(auto i = children.begin(); i != children.end(); ++i)
    (*i)->Update(msec);
}
