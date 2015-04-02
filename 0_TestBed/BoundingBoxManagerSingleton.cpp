#include "BoundingBoxManagerSingleton.h"

//  BoundingBoxManagerSingleton
BoundingBoxManagerSingleton* BoundingBoxManagerSingleton::m_pInstance = nullptr;
void BoundingBoxManagerSingleton::Init(void)
{
	m_nBoxes = 0;
}
void BoundingBoxManagerSingleton::Release(void)
{
	//Clean the list of Boxes
	for(int n = 0; n < m_nBoxes; n++)
	{
		//Make sure to release the memory of the pointers
		if(m_lBox[n] != nullptr)
		{
			delete m_lBox[n];
			m_lBox[n] = nullptr;
		}
	}
	m_lBox.clear();
	m_lMatrix.clear();
	m_lColor.clear();
	m_nBoxes = 0;
}
BoundingBoxManagerSingleton* BoundingBoxManagerSingleton::GetInstance()
{
	if(m_pInstance == nullptr)
	{
		m_pInstance = new BoundingBoxManagerSingleton();
	}
	return m_pInstance;
}
void BoundingBoxManagerSingleton::ReleaseInstance()
{
	if(m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
//The big 3
BoundingBoxManagerSingleton::BoundingBoxManagerSingleton(){Init();}
BoundingBoxManagerSingleton::BoundingBoxManagerSingleton(BoundingBoxManagerSingleton const& other){ }
BoundingBoxManagerSingleton& BoundingBoxManagerSingleton::operator=(BoundingBoxManagerSingleton const& other) { return *this; }
BoundingBoxManagerSingleton::~BoundingBoxManagerSingleton(){Release();};
//Accessors
int BoundingBoxManagerSingleton::GetBoxTotal(void){ return m_nBoxes; }

//--- Non Standard Singleton Methods
void BoundingBoxManagerSingleton::GenerateBoundingBox(matrix4 a_mModelToWorld, String a_sInstanceName)
{
	MeshManagerSingleton* pMeshMngr = MeshManagerSingleton::GetInstance();
	//Verify the instance is loaded
	if(pMeshMngr->IsInstanceCreated(a_sInstanceName))
	{//if it is check if the Box has already been created
		int nBox = IdentifyBox(a_sInstanceName);
		if(IdentifyBox(a_sInstanceName) == -1)
		{
			//Create a new bounding Box
			BoundingBox* pBS = new BoundingBox();
			//construct its information out of the instance name
			pBS->GenerateBoundingBox(a_sInstanceName);
			//Push the Box back into the list
			m_lBox.push_back(pBS);
			//Push a new matrix into the list
			m_lMatrix.push_back(matrix4(IDENTITY));
			//Specify the color the Box is going to have
			m_lColor.push_back(vector3(1.0f));
			//Increase the number of Boxes
			m_nBoxes++;
		}
		else //If the box has already been created you will need to check its global orientation
		{
			m_lBox[nBox]->GenerateAxisAlignedBoundingBox(a_mModelToWorld);
		}
		nBox = IdentifyBox(a_sInstanceName);
		m_lMatrix[nBox] = a_mModelToWorld;
	}
}

void BoundingBoxManagerSingleton::SetBoundingBoxSpace(matrix4 a_mModelToWorld, String a_sInstanceName)
{
	int nBox = IdentifyBox(a_sInstanceName);
	//If the Box was found
	if(nBox != -1)
	{
		//Set up the new matrix in the appropriate index
		m_lMatrix[nBox] = a_mModelToWorld;
	}
}

int BoundingBoxManagerSingleton::IdentifyBox(String a_sInstanceName)
{
	//Go one by one for all the Boxes in the list
	for(int nBox = 0; nBox < m_nBoxes; nBox++)
	{
		//If the current Box is the one we are looking for we return the index
		if(a_sInstanceName == m_lBox[nBox]->GetName())
			return nBox;
	}
	return -1;//couldnt find it return with no index
}

void BoundingBoxManagerSingleton::AddCubeToRenderList(String a_sInstanceName)
{
	//If I need to render all
	if(a_sInstanceName == "ALL")
	{
		for(int nBox = 0; nBox < m_nBoxes; nBox++)
		{
			m_lBox[nBox]->AddBoxToRenderList(m_lMatrix[nBox], m_lColor[nBox], true);
		}
	}
	else
	{
		int nBox = IdentifyBox(a_sInstanceName);
		if(nBox != -1)
		{
			m_lBox[nBox]->AddBoxToRenderList(m_lMatrix[nBox], m_lColor[nBox], true);
		}
	}
}

void BoundingBoxManagerSingleton::CalculateCollision(void)
{
	//Create a placeholder for all center points
	std::vector<vector3> lCentroid;
	//for all Boxes...
	for(int nBox = 0; nBox < m_nBoxes; nBox++)
	{
		//Make all the Boxes white
		m_lColor[nBox] = vector3(1.0f);
		//Place all the centroids of Boxes in global space
		lCentroid.push_back(static_cast<vector3>(m_lMatrix[nBox] * vector4(m_lBox[nBox]->GetCentroid(), 1.0f)));
	}

	//now the actual check for all Boxes among all Boxes (so... one by one), this is not the most optimal way, there is a more clever one
	for(int i = 0; i < m_nBoxes - 1; i++)
	{
		for(int j = i + 1; j < m_nBoxes; j++)
		{
			//If the distance between the center of both Boxes is less than the radial sum there is a collision
			//Assume they will be colliding unless they are not in the same space in X, Y or Z
			//so we place them in global positions
			vector3 v1Min = static_cast<vector3>(m_lMatrix[i] * vector4(m_lBox[i]->GetMinBBox(),1));
			vector3 v1Max = static_cast<vector3>(m_lMatrix[i] * vector4(m_lBox[i]->GetMaxBBox(),1));

			vector3 v2Min = static_cast<vector3>(m_lMatrix[j] * vector4(m_lBox[j]->GetMinBBox(),1));
			vector3 v2Max = static_cast<vector3>(m_lMatrix[j] * vector4(m_lBox[j]->GetMaxBBox(),1));

			bool bColliding = true;
			if(v1Max.x < v2Min.x || v1Min.x > v2Max.x)
				bColliding = false;
			else if(v1Max.y < v2Min.y || v1Min.y > v2Max.y)
				bColliding = false;
			else if(v1Max.z < v2Min.z || v1Min.z > v2Max.z)
				bColliding = false;

			if(bColliding)
				m_lColor[i] = m_lColor[j] = MERED; //Color the boxes red
		}
	}
}
	

	////This way is more optimal, just half the checks are needed
	//for(int i = 0; i < m_nBoxes - 1; i++)
	//{
	//	for(int j = i + 1; j < m_nBoxes; j++)
	//	{
	//		//If the distance between the center of both Boxes is less than the sum of their radius there is a collision
	//		if(glm::distance(lCentroid[i], lCentroid[j]) < (m_lBox[i]->GetRadius() + m_lBox[j]->GetRadius()))
	//			m_lColor[i] = m_lColor[j] = MERED; //We make the Boxes red
	//	}
	//}
}