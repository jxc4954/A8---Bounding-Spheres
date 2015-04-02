
/// INCLUDE + NAMESPACES
#include "BoundingBox.h"


/// BOUNDING BOX ///
void BoundingBox::Init(void)
{
	m_bInitialized = false;
	m_v3Centroid = vector3(0.0f);
	m_sName = "NULL";
	vector3 scaleVec;
}

void BoundingBox::Swap(BoundingBox& other)
{
	std::swap(m_bInitialized, other.m_bInitialized);
	std::swap(m_v3Centroid, other.m_v3Centroid);
	std::swap(m_sName, other.m_sName);
	

}

void BoundingBox::Release(void)
{
	//No pointers to release
}

/// THE BIG THREE ///
BoundingBox::BoundingBox() { Init(); }

BoundingBox::BoundingBox(BoundingBox const& other)
{
	m_bInitialized = other.m_bInitialized;
	m_v3Centroid = other.m_v3Centroid;
	m_sName = other.m_sName;
}

 //OVERLOADOPERATOR=
BoundingBox& BoundingBox::operator=(BoundingBox const& other)
{
	if(this != &other)
	{
		Release();
		Init();
		BoundingBox temp(other);
		Swap(temp);
	}
	return *this;
}


BoundingBox::~BoundingBox() { Release(); };

/// ACCESSORS ///
bool BoundingBox::IsInitialized(void){ return m_bInitialized; }
vector3 BoundingBox::GetCentroid(void){ return m_v3Centroid; }
String BoundingBox::GetName(void){return m_sName;}

/// METHODS ///
void BoundingBox::GenerateBoundingBox(String a_sInstanceName)
{
	//If this has already been initialized there is nothing to do here
	if(m_bInitialized)
		return;
	MeshManagerSingleton* pMeshMngr = MeshManagerSingleton::GetInstance();
	if(pMeshMngr->IsInstanceCreated(a_sInstanceName))
	{
		m_sName = a_sInstanceName;
		
		std::vector<vector3> lVertices = pMeshMngr->GetVertices(m_sName);
		unsigned int nVertices = lVertices.size();
		m_v3Centroid = lVertices[0];
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
		m_v3Centroid = (v3Min + v3Max) / 2.0f;

		scaleVec = vector3((v3Max.x - v3Min.x), (v3Max.y - v3Min.y), (v3Max.z - v3Min.z)); 

		m_bInitialized = true;
	}
}

// 
void BoundingBox::AddBoxToRenderList(matrix4 a_mModelToWorld, vector3 a_vColor, bool a_bRenderCentroid)
{
	if(!m_bInitialized) { return; }
	MeshManagerSingleton* pMeshMngr = MeshManagerSingleton::GetInstance();

	if(a_bRenderCentroid)
	pMeshMngr->AddAxisToQueue(a_mModelToWorld * glm::translate(m_v3Centroid));
	//pMeshMngr->AddSphereToQueue(a_mModelToWorld * glm::translate(m_v3Centroid) * glm::scale(vector3(m_fRadius * 2.0f)), a_vColor, MERENDER::WIRE);
	pMeshMngr->AddCubeToQueue(a_mModelToWorld * glm::translate(m_v3Centroid) * glm::scale(vector3(scaleVec * 2.0f)), a_vColor, MERENDER::WIRE);
}
