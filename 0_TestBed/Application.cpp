#include "ApplicationClass.h"
#include "BoundingBox.h"


void ApplicationClass::InitAppVariables()
{
	float fSteve = 0.0f;
	float fMiddle = 1.5f;
	float fCreeper = 3.0f;

	m_pMeshMngr = MeshManagerSingleton::GetInstance();
	m_pMeshMngr->LoadModel("Minecraft\\MC_Steve.obj", "Steve");
	m_pMeshMngr->LoadModel("Minecraft\\MC_Creeper.obj", "Creeper");
	m_pMeshMngr->LoadModel("Minecraft\\MC_Cow.obj", "Cow");
	m_pMeshMngr->LoadModel("Minecraft\\MC_Pig.obj", "Pig");
	m_pMeshMngr->LoadModel("Minecraft\\MC_Zombie.obj", "Zombie");
	//m_pMeshMngr->m_pModelMngr->LoadModelUnthreaded("Minecraft\\MC_Steve.obj", "A");
	pBoundingSphere1 = new BoundingSphereClass();
	pBoundingSphere2 = new BoundingSphereClass();
	pBoundingBox1 = new BoundingBox();
	pBoundingBox2 = new BoundingBox();

	//m_pBSMngr = BoundingSphereManagerSingleton::GetInstance();
	boxmngr = BoundingBoxManagerSingleton::GetInstance();

	m_m4Creeper = glm::translate(vector3(3.0f,0.0f,0.0f));
	m_pCamera->SetPosition(vector3(fSteve,1.0f,11.0f));
}

void ApplicationClass::GenerateBoundingSphere(matrix4 a_mModelToWorld, String a_sInstanceName)
{
	if(m_pMeshMngr->IsInstanceCreated(a_sInstanceName))
	{
		static bool bInitialized = false;
		static vector3 vCenterPoint;
		static float fDistance;
		if(!bInitialized)
		{
			std::vector<vector3> lVertices = m_pMeshMngr->m_pModelMngr->GetVertices(a_sInstanceName);
			unsigned int nVertices = lVertices.size();
			vCenterPoint = lVertices[0];
			vector3 v3Max(lVertices[0]);
			vector3 v3Min(lVertices[0]);
			for(unsigned int nVertex = 1; nVertex < nVertices; nVertex++)
			{
				//m_v3Centroid += lVertices[nVertex];
				if(v3Min.x > lVertices[nVertex].x)
					v3Min.x = lVertices[nVertex].x;
				else if(v3Max.x < lVertices[nVertex].x)
					v3Max.x = lVertices[nVertex].x;
			
				if(v3Min.y > lVertices[nVertex].y)
					v3Min.y = lVertices[nVertex].y;
				else if(v3Max.y < lVertices[nVertex].y)
					v3Max.y = lVertices[nVertex].y;

				if(v3Min.z > lVertices[nVertex].z)
					v3Min.z = lVertices[nVertex].z;
				else if(v3Max.z < lVertices[nVertex].z)
					v3Max.z = lVertices[nVertex].z;
			}
			vCenterPoint = (v3Min + v3Max) / 2.0f;

			m_pMeshMngr->AddAxisToQueue(a_mModelToWorld * glm::translate(vCenterPoint));

			fDistance = glm::distance(vCenterPoint, lVertices[0]);
			for(unsigned int nVertex = 1; nVertex < nVertices; nVertex++)
			{
				float fDistanceNew = glm::distance(vCenterPoint, lVertices[nVertex]);
				if(fDistance < fDistanceNew)
					fDistance = fDistanceNew;
			}

			bInitialized = true;
		}
		//m_pMeshMngr->AddSphereToQueue(a_mModelToWorld * glm::translate(vCenterPoint) * glm::scale(vector3(fDistance * 2.0f)), MERED, MERENDER::WIRE);
		m_pMeshMngr->AddCubeToQueue(a_mModelToWorld * glm::translate(vCenterPoint) * glm::scale(vector3(fDistance * 2.0f)), MERED, MERENDER::WIRE);
	}
}

void ApplicationClass::Update (void)
{
	m_pSystem->UpdateTime(); //Update the system

	m_pMeshMngr->Update(); //Updates the mesh information

	float fTimeSpan = m_pSystem->StopClock(); //Check the time difference between this method calls
	static float fRunTime = 0.0f;
	if(m_pMeshMngr->IsInstanceCreated("Steve"))
	{
		fRunTime += fTimeSpan; //update the run time count
		matrix4 m4Steve = glm::rotate(matrix4(IDENTITY), fRunTime * 15, vector3( 0.0f,-1.0f, 0.0f));

		matrix4 m4Zombie = glm::translate(vector3(-6.0f, 0.0f, 0.0f));
		matrix4 m4Cow = glm::translate(vector3(-3.0f, 0.0f, 0.0f));
		matrix4 m4Pig = glm::translate(vector3(6.0f, 0.0f, 0.0f));
	
		m_pMeshMngr->SetModelMatrix(m4Steve, "Steve");
		m_pMeshMngr->SetModelMatrix(m_m4Creeper, "Creeper");
		m_pMeshMngr->SetModelMatrix(m4Pig, "Pig");
		m_pMeshMngr->SetModelMatrix(m4Zombie, "Zombie");
		m_pMeshMngr->SetModelMatrix(m4Cow, "Cow");

		std::vector<vector3> lvertex = m_pMeshMngr->GetVertices("Steve");

		int numVertices = static_cast<int>(lvertex.size());
		vector3 vCentroid = lvertex[0]; 

		for(int vertex = 1; vertex < numVertices; vertex++)
		{
			vCentroid += lvertex[vertex];
		}

		vCentroid /= numVertices; 
		m_pMeshMngr->AddAxisToQueue(m4Steve * glm::translate(vCentroid)* glm::scale(vector3(2.0f)));
		
		m_pMeshMngr->AddAxisToQueue(m4Pig * glm::translate(vCentroid)* glm::scale(vector3(2.0f)));
		m_pMeshMngr->AddAxisToQueue(m4Cow * glm::translate(vCentroid)* glm::scale(vector3(2.0f)));
		m_pMeshMngr->AddAxisToQueue(m4Zombie * glm::translate(vCentroid)* glm::scale(vector3(2.0f)));
		float fDistance = 0.0f;

		for(int vertex = 0; vertex < numVertices; vertex++)
		{
			float fNewDistance = glm::distance(vCentroid, lvertex[vertex]);
			if(fDistance < fNewDistance)
			{
				fDistance = fNewDistance;
			}
		}
		m_pMeshMngr->AddCubeToQueue(m4Steve * glm::translate(vCentroid)* glm::scale(vector3(2.0f)), MEBLUE, MERENDER::WIRE);
		
		m_pMeshMngr->AddCubeToQueue(m4Pig * glm::translate(vCentroid)* glm::scale(vector3(2.0f)), MEBLUE, MERENDER::WIRE);
		m_pMeshMngr->AddCubeToQueue(m4Cow * glm::translate(vCentroid)* glm::scale(vector3(2.0f)), MEBLUE, MERENDER::WIRE);
		m_pMeshMngr->AddCubeToQueue(m4Zombie * glm::translate(vCentroid)* glm::scale(vector3(2.0f)), MEBLUE, MERENDER::WIRE);
	}
	if(m_pMeshMngr->IsInstanceCreated("Creeper"))
	{
		std::vector<vector3> lvertex = m_pMeshMngr->GetVertices("Creeper");

		int numVertices = static_cast<int>(lvertex.size());
		vector3 vCentroid = lvertex[0]; 

		for(int vertex = 1; vertex < numVertices; vertex++)
		{
			vCentroid += lvertex[vertex];
		}

		vCentroid /= numVertices; 

		m_pMeshMngr->AddAxisToQueue(m_m4Creeper * glm::translate(vCentroid)* glm::scale(vector3(2.0f)));

		float fDistance = 0.0f;

		for(int vertex = 0; vertex < numVertices; vertex++)
		{
			float fNewDistance = glm::distance(vCentroid, lvertex[vertex]);
			if(fDistance < fNewDistance)
			{
				fDistance = fNewDistance;
			}
		}
		m_pMeshMngr->AddCubeToQueue(m_m4Creeper * glm::translate(vCentroid)* glm::scale(vector3(2.0f)), MEBLUE, MERENDER::WIRE);
	}
	else if(m_pMeshMngr->IsInstanceCreated("Pig"))
	{

	}
	else if(m_pMeshMngr->IsInstanceCreated("Cow"))
	{

	}
	else if(m_pMeshMngr->IsInstanceCreated("Zombie"))
	{

	}

#pragma region Sphere Method
	//GenerateBoundingSphere(mA,"A");
#pragma endregion

#pragma region Bounding Sphere Class
	//pBoundingSphere1->GenerateBoundingSphere("Steve");
	//pBoundingSphere2->GenerateBoundingSphere("Creeper");
	
	//pBoundingSphere1->AddSphereToRenderList(m4Steve, MEYELLOW, true);
	//pBoundingSphere2->AddSphereToRenderList(m_m4Creeper, MEYELLOW, true);
#pragma endregion

#pragma region Bounding Sphere Manager
	
	/*m_pBSMngr->GenerateBoundingSphere("Steve");
	m_pBSMngr->GenerateBoundingSphere("Creeper");
	m_pBSMngr->GenerateBoundingSphere("Cow");
	m_pBSMngr->GenerateBoundingSphere("Zombie");
	m_pBSMngr->GenerateBoundingSphere("Pig");

	m_pBSMngr->SetBoundingSphereSpace(m4Steve, "Steve");
	m_pBSMngr->SetBoundingSphereSpace(m_m4Creeper, "Creeper");
	m_pBSMngr->SetBoundingSphereSpace(m4Cow, "Cow");
	m_pBSMngr->SetBoundingSphereSpace(m4Pig, "Pig");
	m_pBSMngr->SetBoundingSphereSpace(m4Zombie, "Zombie");

	m_pBSMngr->CalculateCollision();

	m_pBSMngr->AddSphereToRenderList("ALL");
	//m_pBSMngr->AddBoxToRenderList("ALL");*/
	
#pragma endregion

#pragma region BOX Method
	//GenerateBoundingBox(mA,"A");
#pragma endregion

#pragma region Bounding BOX Class
	pBoundingBox1->GenerateBoundingBox("Steve");
	pBoundingBox2->GenerateBoundingBox("Creeper");
	
	pBoundingBox1->AddBoxToRenderList(m4Steve, MEYELLOW, true);
	pBoundingBox2->AddBoxToRenderList(m_m4Creeper, MEYELLOW, true);
#pragma endregion

#pragma region Bounding BOX Manager
	
	boxmngr->GenerateBoundingBox("Steve");
	boxmngr->GenerateBoundingBox("Creeper");
	boxmngr->GenerateBoundingBox("Cow");
	boxmngr->GenerateBoundingBox("Zombie");
	boxmngr->GenerateBoundingBox("Pig");

	boxmngr->SetBoundingBoxSpace(m4Steve, "Steve");
	boxmngr->SetBoundingBoxSpace(m_m4Creeper, "Creeper");
	boxmngr->SetBoundingBoxSpace(m4Cow, "Cow");
	boxmngr->SetBoundingBoxSpace(m4Pig, "Pig");
	boxmngr->SetBoundingBoxSpace(m4Zombie, "Zombie");

	boxmngr->CalculateCollision();

	//m_pBSMngr->AddCubeToRenderList("ALL");
	boxmngr->AddCubeToRenderList("ALL");
	
#pragma endregion


	m_pMeshMngr->AddInstanceToRenderList();
	
	//First person camera movement
	if(m_bFPC == true)
		CameraRotation();

	m_pCamera->PrintInfo();

	printf("FPS: %d\r", m_pSystem->FPS);//print the Frames per Second
} // End update

void ApplicationClass::Display (void) //for OpenGL 3.X Applications
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // clear the window

	//m_pGrid->Render(100.0f); //renders the grid with a 100 scale

	m_pMeshMngr->Render();

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}