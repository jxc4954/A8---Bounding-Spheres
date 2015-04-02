#pragma once
#ifndef __BOUNDINGBOX_H_
#define __BOUNDINGBOX_H_

#include "MyEngine.h"

//System Class
class BoundingBox
{
	bool m_bInitialized; //Has the elements of this object been initialized before?
	//float m_fRadius;//Radius of the collision sphere
	vector3 m_v3Min;//Collection of minimal components of the model
	vector3 m_v3Max;//Collection of maximum componets of the model
	vector3 m_v3Size;//Size of the Box
	vector3 m_v3Centroid;//Center point of the model, the origin in GSpace in which the sphere is going to be located
	String m_sName;//Name of the BoundingSphere (to relate to the instance)
	vector3 scaleVec;
public:
	/* Constructor */
	BoundingBox(void);
	
	/* Copy Constructor */
	BoundingBox(BoundingBox const& other);
	
	/* Copy Assignment Operator*/
	BoundingBox& operator=(BoundingBox const& other);
	
	/* Destructor */
	~BoundingBox(void);


/// FUNCTIONS ///
	/* Swaps the contents of the object with another object's content */
	void Swap(BoundingBox& other);

	// IsInitialized - Returns information about whether or not the object has been initialized
	bool IsInitialized(void);

	/*
	GetMinBBox
		Returns the minimum vector
	*/
	vector3 GetMinBBox(void);

	/*
	GetMaxBBox
		Returns the maximum vector
	*/
	vector3 GetMaxBBox(void);

	// GetRadius - Returns the radius of the sphere
	//float GetRadius(void);

	// GetCentroid - Returns the origin of the Cartesian Coordinate System in which the sphere is drawn in GSpace
	vector3 GetCentroid(void);

	// GetName - Returns the name of the sphere to associate it with an instance
	String GetName(void);

	// ConstructSphere - Creates the sphere in local space of the model specified in the instance name
	void GenerateBoundingBox(String a_sInstanceName);

	void GenerateAxisAlignedBoundingBox(matrix4 a_m4ModeltoWorld);

	// AddSphereToRenderList - Adds the sphere to the render list specifying also if rendering the centroid is necessary
	void AddBoxToRenderList(matrix4 a_mModelToWorld, vector3 a_vColor, bool a_bRenderCentroid = false);

	
private:
	/* Releases the object from memory */
	void Release(void);

	/* Initialize the object's fields */
	void Init(void);
};


#endif //__BOUNDINGBOXLASS_H__

