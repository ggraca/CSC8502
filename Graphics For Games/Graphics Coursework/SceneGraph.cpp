#include "Renderer.h"

void Renderer::BuildNodeLists(SceneNode* from) {
  if (frameFrustum.InsideFrustum(*from)) {
    Vector3 dir =
      from->GetWorldTransform().GetPositionVector() -
      camera->GetPosition();

    from->SetCameraDistance(Vector3::Dot(dir, dir));

    if(from->GetColour().w < 1.0f)
      transparentNodeList.push_back(from);
    else
      nodeList.push_back(from);
  }

  for(auto i = from->GetChildIteratorStart();
      i != from->GetChildIteratorEnd(); i++) {
    if ((*i)->GetActive()) BuildNodeLists(*i);
  }
}

void Renderer::SortNodeLists() {
  sort(
    transparentNodeList.begin(),
    transparentNodeList.end(),
    SceneNode::CompareByCameraDistance
  );

  sort(
    nodeList.begin(),
    nodeList.end(),
    SceneNode::CompareByCameraDistance
  );
}

void Renderer::DrawNodes(bool transparents) {
  for(auto n : nodeList) DrawNode(n);
  if(transparents) {
    for(auto i = transparentNodeList.rbegin();
        i != transparentNodeList.rend(); ++ i ) {
      DrawNode(*i);
    }
  }
}

void Renderer::DrawNode(SceneNode* node) {
  if (node->GetMesh()) {
    Matrix4 transform =
      node->GetWorldTransform() *
      Matrix4::Scale(node->GetModelScale()) *
      Matrix4::Rotation(node->GetRotation().x, Vector3(1, 0, 0)) *
      Matrix4::Rotation(node->GetRotation().y, Vector3(0, 1, 0)) *
      Matrix4::Rotation(node->GetRotation().z, Vector3(0, 0, 1));

    glUniformMatrix4fv(
      glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"),
      1, false,
      (float*)&transform
    );

    node->Draw(*this);
  }
}

void Renderer::ClearNodeLists() {
	transparentNodeList.clear();
	nodeList.clear();
}
