#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";
	vector3 v3Position(0.0f, 0.0f, 10.0f);
	vector3 v3Target = ZERO_V3;
	vector3 v3Upward = AXIS_Y;
	m_pCameraMngr->SetPositionTargetAndUpward(v3Position, v3Target, v3Upward);

	MyMesh* pMesh = new MyMesh();
	pMesh->GenerateTorus(0.5f, 0.25f, uSub, uSub, C_RED);
	m_meshList.push_back(pMesh);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Update Entity Manager
	m_pEntityMngr->Update();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	// draw a skybox
	m_pModelMngr->AddSkyboxToRenderList();

	//make a new clock and time it
	static uint uClock = m_pSystem->GenClock();
	static float fTime = 0.0f;
	float fDelta = m_pSystem->GetDeltaTime(uClock);
	fTime += fDelta;
	
	//make something happen every one second
	static int iSeconds = 0.0f;
	if ((fTime - iSeconds) > 0.0f)
	{
		iSeconds++;
		MyMesh* pMesh = new MyMesh();
		pMesh->GenerateTorus(0.5f, 0.25f, uSub, uSub, C_RED);
		m_meshList.push_back(pMesh);
	}
	
	// get the matrices
	matrix4 m4Model = glm::rotate(IDENTITY_M4, glm::radians(fTime * 60.0f), AXIS_Z) * glm::translate(vector3(3.0f, 0.0f, 0.0f));
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();

	// draw the list
	for (uint i = 0; i < m_meshList.size(); i++)
		m_meshList[i]->Render(m4Projection, m4View, m4Model * glm::translate(vector3(i, 0.0f, 0.0f)));

	//render list call
	m_uRenderCallCount = m_pModelMngr->Render();

	//clear the render list
	m_pModelMngr->ClearRenderList();

	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	for (uint i = 0; i < m_meshList.size(); i++)
		SafeDelete(m_meshList[i]);

	//release GUI
	ShutdownGUI();
}