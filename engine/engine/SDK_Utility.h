/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

// use the fbxsdk.h
#include <fbxsdk.h>
#include <vector>
#include<gl/glut.h>
using std::vector;
// to build a scene from an FBX file
bool LoadFBXScene(
                    const char *pFbxFilePath 
                 );

// to read a file using an FBX SDK reader
bool LoadScene(
                FbxManager       *pSdkManager, 
                FbxScene            *pScene, 
                const char           *pFbxFilePath
              );


// to create a SDK manager and a new scene
void InitializeSdkManagerAndScene();



// to destroy an instance of the SDK manager
void DestroySdkObjects(
                        FbxManager* pSdkManager,
						bool pExitStatus
                      );

// to get the root node
FbxNode* GetRootNode();

// to get the root node name
const char * GetRootNodeName();

// to get a string from the node name and attribute type
FbxString GetNodeNameAndAttributeTypeName(
                                         const FbxNode* pNode
                                       );

// to get a string from the node default translation values
FbxString GetDefaultTranslationInfo(
								   const FbxNode* pNode
								 );

// to get a string from the node visibility value
FbxString GetNodeVisibility(
							const FbxNode* pNode
						 );
void ProcessNode(FbxNode* pNode);

void ProcessMesh(FbxMesh* mesh);
void ProcessSkeleton(FbxNode* inRootNode);

void ReadNormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, FbxVector4& outNormal);
void ReadTangent(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, FbxVector4& outTangent);
bool ReadUV(FbxMesh* pMesh , int ctrlPointIndex , int textureUVIndex , int uvLayer , FbxVector2& pUV);

void ProcessSkeletonHierarchyRecursively(FbxNode* inNode, int inDepth, int myIndex, int inParentIndex);

void ProcessJointsAndAnimations(FbxNode* inNode);
FbxAMatrix GetGeometryTransformation(FbxNode* inNode);

class SDK_Utility
{
public:
	SDK_Utility(void);
	~SDK_Utility(void);
};
