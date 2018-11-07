#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent) {
	CubeRobot::CreateCube();
	camera = new Camera();

	currentShader = new Shader(
		SHADERDIR"SceneVertex.glsl",
		SHADERDIR"SceneFragment.glsl"
	);
	if (!currentShader->LinkProgram()) return;

	projMatrix = Matrix4::Perspective(
		1.0f, 10000.0f, (float)width / (float)height, 45.0f
	);

	camera->SetPosition(Vector3(0, 30, 175));

	root = new SceneNode();
	root->AddChild(new CubeRobot());

	glEnable(GL_DEPTH_TEST);
	init = true;
}

Renderer::~Renderer(void) {
	delete root;
	CubeRobot::DeleteCube();
}

void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	root->Update(msec);
}

void Renderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glUseProgram(currentShader->GetProgram());

	UpdateShaderMatrices();
	glUniform1i(
		glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 1
	);

	DrawNode(root);

	glUseProgram(0);
	SwapBuffers();
}

void Renderer::DrawNode(SceneNode* node) {
	if (node->GetMesh()) {
		Matrix4 transform =
			node->GetWorldTransform() *
			Matrix4::Scale(node->GetModelScale());

		glUniformMatrix4fv(
			glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"),
			1, false, (float*)&transform
		);

		glUniform4fv(
			glGetUniformLocation(currentShader->GetProgram(), "nodeColour"),
			1, (float*)&node->GetColour()
		);

		glUniform1i(
			glGetUniformLocation(currentShader->GetProgram(), "useTexture"),
			(int)node->GetMesh()->GetTexture()
		);

		node->Draw(*this);
	}

	for (auto i = node->GetChildIteratorStart();
		i != node->GetChildIteratorEnd();
		++i)
		DrawNode(*i);
}
