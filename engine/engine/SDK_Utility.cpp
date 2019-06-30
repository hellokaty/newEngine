#include "StdAfx.h"
/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include "SDK_Utility.h"

#ifdef IOS_REF
	#undef  IOS_REF
	#define IOS_REF (*(pSdkManager->GetIOSettings()))
#endif

struct Point{
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

struct Polygon {
	int size;
	vector<Point> points;
	FbxVector4 normal[3];//法线
	FbxVector4 tangent[3];//切线
	FbxVector2 UV[3][2];//UV坐标,用于UV贴图 
};
int TriangleVertexCount=0;

std::vector<Point> triangleVertexVec;         //控制点列表
std::vector<int> triangleIndexVec;              //索引序列
std::vector<Polygon> polygonVec;    //多边形信息集合


// declare global
FbxManager* gSdkManager = NULL;
FbxScene*      gScene      = NULL;

bool LoadFBXScene(
                  const char *pFbxFilePath  
                  )
{
    // Load the scene.
    if( LoadScene(gSdkManager, gScene, pFbxFilePath ) == false ) return false;

    return true;
}

// Creates an instance of the SDK manager
// and use the SDK manager to create a new scene
void InitializeSdkManagerAndScene()
{
    // Create the FBX SDK memory manager object.
    // The SDK Manager allocates and frees memory
    // for almost all the classes in the SDK.
    gSdkManager = FbxManager::Create();

	// create an IOSettings object
	FbxIOSettings * ios = FbxIOSettings::Create(gSdkManager, IOSROOT );
	gSdkManager->SetIOSettings(ios);

	gScene = FbxScene::Create(gSdkManager,"");
}

// to destroy an instance of the SDK manager
void DestroySdkObjects(
                       FbxManager* pSdkManager,
					   bool pExitStatus
                      )
{
    // Delete the FBX SDK manager. All the objects that have been allocated 
    // using the FBX SDK manager and that haven't been explicitly destroyed 
    // are automatically destroyed at the same time.
    if( pSdkManager ) pSdkManager->Destroy();
	if( pExitStatus ) FBXSDK_printf("Program Success!\n");
}

// to read a file using an FBXSDK reader
bool LoadScene(
               FbxManager       *pSdkManager, 
               FbxScene            *pScene, 
               const char           *pFbxFilePath
               )
{
    bool lStatus;

    // Create an importer.
    FbxImporter* lImporter = FbxImporter::Create(pSdkManager,"");

    // Initialize the importer by providing a filename.
    bool lImportStatus = lImporter->Initialize(pFbxFilePath, -1, pSdkManager->GetIOSettings() );

    if( !lImportStatus )
    {
        // Destroy the importer
        lImporter->Destroy();
        return false;
    }

    if (lImporter->IsFBX())
    {
        // Set the import states. By default, the import states are always set to 
        // true. The code below shows how to change these states.
        IOS_REF.SetBoolProp(IMP_FBX_MATERIAL,        true);
        IOS_REF.SetBoolProp(IMP_FBX_TEXTURE,         true);
        IOS_REF.SetBoolProp(IMP_FBX_LINK,            true);
        IOS_REF.SetBoolProp(IMP_FBX_SHAPE,           true);
        IOS_REF.SetBoolProp(IMP_FBX_GOBO,            true);
        IOS_REF.SetBoolProp(IMP_FBX_ANIMATION,       true);
        IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
    }

    // Import the scene
    lStatus = lImporter->Import(pScene);

    // Destroy the importer
    lImporter->Destroy();

    return lStatus;
}

// to get the root node
FbxNode* GetRootNode()
{
    return gScene->GetRootNode();
}

// to get the root node name
const char * GetRootNodeName()
{
    return GetRootNode()->GetName();
}

// to get a string from the node name and attribute type
FbxString GetNodeNameAndAttributeTypeName(const FbxNode* pNode)
{
    FbxString s = pNode->GetName();

    FbxNodeAttribute::EType lAttributeType;

    if(pNode->GetNodeAttribute() == NULL)
    {
        s += " (No node attribute type)";
    }
    else
    {
        lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());

        switch (lAttributeType)
        {
        case FbxNodeAttribute::eMarker:                s += " (Marker)";               break;
        case FbxNodeAttribute::eSkeleton:              s += " (Skeleton)";             break;
        case FbxNodeAttribute::eMesh:                  s += " (Mesh)";                 break;
        case FbxNodeAttribute::eCamera:                s += " (Camera)";               break;
        case FbxNodeAttribute::eLight:                 s += " (Light)";                break;
        case FbxNodeAttribute::eBoundary:              s += " (Boundary)";             break;
        case FbxNodeAttribute::eOpticalMarker:        s += " (Optical marker)";       break;
        case FbxNodeAttribute::eOpticalReference:     s += " (Optical reference)";    break;
        case FbxNodeAttribute::eCameraSwitcher:       s += " (Camera switcher)";      break;
        case FbxNodeAttribute::eNull:                  s += " (Null)";                 break;
        case FbxNodeAttribute::ePatch:                 s += " (Patch)";                break;
        case FbxNodeAttribute::eNurbs:                  s += " (NURB)";                 break;
        case FbxNodeAttribute::eNurbsSurface:         s += " (Nurbs surface)";        break;
        case FbxNodeAttribute::eNurbsCurve:           s += " (NURBS curve)";          break;
        case FbxNodeAttribute::eTrimNurbsSurface:    s += " (Trim nurbs surface)";   break;
        case FbxNodeAttribute::eUnknown:          s += " (Unidentified)";         break;
        }   
    }

    return s;
}

// to get a string from the node default translation values
FbxString GetDefaultTranslationInfo(
                                  const FbxNode* pNode
                                  )
{
    FbxVector4 v4;
    v4 = ((FbxNode*)pNode)->LclTranslation.Get();

    return FbxString("Translation (X,Y,Z): ") + FbxString(v4[0]) + ", " + FbxString(v4[1]) + ", " + FbxString(v4[2]);
}

// to get a string from the node visibility value
FbxString GetNodeVisibility(
                          const FbxNode* pNode
                          )
{
    return FbxString("Visibility: ") + (pNode->GetVisibility() ? "Yes":"No");
}

void ProcessNode(FbxNode* pNode)  
{   
	FbxNodeAttribute::EType lAttributeType;
	
    if(pNode->GetNodeAttribute() != NULL)
    {
		lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());
		switch (lAttributeType) {
			case FbxNodeAttribute::eMarker:
				//ProcessMaker(pNode);
				break;
			case FbxNodeAttribute::eSkeleton:
				//ProcessSkeleton(pNode);
				//return;//processSkeleton里面有循环
				break;
			case FbxNodeAttribute::eMesh:
				ProcessMesh(pNode->GetMesh());
				break;
			case FbxNodeAttribute::eCamera:
				//ProcessCamera(pNode);
				break;
			case FbxNodeAttribute::eLight:
				//ProcessLight(pNode);
				break;
			case FbxNodeAttribute::eBoundary:
				//ProcessBoundary(pNode);
				break;
		}
	} 
  
    for(int i = 0 ; i < pNode->GetChildCount() ; ++i)  
    {  
        ProcessNode(pNode->GetChild(i));  
    }  
}  

void ProcessMesh(FbxMesh *mesh)
{
    int ctrlcount=mesh->GetControlPointsCount();
    FbxVector4 *ctrlpoints=mesh->GetControlPoints();
    for(int i=0;i<ctrlcount;i++)
    {
        Point p;
        p.x=ctrlpoints[i].mData[0];
        p.y=ctrlpoints[i].mData[1];
        p.z=ctrlpoints[i].mData[2];
        triangleVertexVec.push_back(p);
    }

    int pcount=mesh->GetPolygonCount();
	int vertexCounter = 0;//当前正在处理的Polygon Vertex的索引 
    for(int i=0;i<pcount;i++)
    {
		int psize=mesh->GetPolygonSize(i);
		Polygon polygon;
		polygon.size = psize;
		
        for(int j=0;j<psize;j++){
			int index = mesh->GetPolygonVertex(i,j);
			polygon.points.push_back(triangleVertexVec[index]);
			ReadNormal(mesh, index, vertexCounter, polygon.normal[j]);
			ReadTangent(mesh, index, vertexCounter, polygon.tangent[j]);
			// ???
            for (int k = 0; k < 2; ++k)
            {
                ReadUV(mesh, index, mesh->GetTextureUVIndex(i, j), k, polygon.UV[j][k]);  //读取UV贴图信息
            }
           // triangleIndexVec.push_back(mesh->GetPolygonVertex(i,j));//为什么要加上TriangleIndexCount
		   ++vertexCounter;
		}
		polygonVec.push_back(polygon);
    }
    //TriangleVertexCount+=ctrlcount;          //加上上一个node的控制点个数

}

void ReadNormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, FbxVector4& outNormal){
	if(inMesh->GetElementNormalCount() < 1)
    {
		return;
    }

    FbxGeometryElementNormal* vertexNormal = inMesh->GetElementNormal(0);   //读取mesh的第一层Layer中保存的法向量信息
    switch(vertexNormal->GetMappingMode())  
    {
    case FbxGeometryElement::eByControlPoint:
        switch(vertexNormal->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        {
			float x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
            float y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
            float z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[2]);
			outNormal.Set(x, y, z);
        }
        break;

        case FbxGeometryElement::eIndexToDirect:
        {
            int index = vertexNormal->GetIndexArray().GetAt(inCtrlPointIndex);
            float x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			float y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			float z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
			outNormal.Set(x, y, z);
        }
        break;

        default:
			{
				throw std::exception("Invalid Reference");
				return;
			}
        }
        break;

    case FbxGeometryElement::eByPolygonVertex:
        switch(vertexNormal->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        {
            float x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[0]);
            float y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[1]);
            float z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[2]);
			outNormal.Set(x, y, z);
        }
        break;

        case FbxGeometryElement::eIndexToDirect:
        {
            int index = vertexNormal->GetIndexArray().GetAt(inVertexCounter);
            float x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
            float y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
            float z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
			outNormal.Set(x, y, z);
        }
        break;

        default:
            throw std::exception("Invalid Reference");
			return;
        }
        break;
    }
}

void ReadTangent(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, FbxVector4& outTangent){
	int k = inMesh->GetElementTangentCount();
	if(inMesh->GetElementTangentCount() < 1)
    {
		return;
    }

	FbxGeometryElementTangent* vertexTangent = inMesh->GetElementTangent(0);   //读取mesh的第一层Layer中保存的切向量信息
    switch(vertexTangent->GetMappingMode())  
    {
    case FbxGeometryElement::eByControlPoint:
        switch(vertexTangent->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        {
			float x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
            float y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
            float z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inCtrlPointIndex).mData[2]);
			outTangent.Set(x, y, z);
        }
        break;

        case FbxGeometryElement::eIndexToDirect:
        {
            int index = vertexTangent->GetIndexArray().GetAt(inCtrlPointIndex);
            float x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
			float y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
			float z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
			outTangent.Set(x, y, z);
        }
        break;

        default:
            throw std::exception("Invalid Reference");
			return;
        }
        break;

    case FbxGeometryElement::eByPolygonVertex:
        switch(vertexTangent->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        {
            float x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inVertexCounter).mData[0]);
            float y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inVertexCounter).mData[1]);
            float z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inVertexCounter).mData[2]);
			outTangent.Set(x, y, z);
        }
        break;

        case FbxGeometryElement::eIndexToDirect:
        {
            int index = vertexTangent->GetIndexArray().GetAt(inVertexCounter);
            float x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
            float y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
            float z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
			outTangent.Set(x, y, z);
        }
        break;

        default:
            throw std::exception("Invalid Reference");
			return;
        }
        break;
    }
}

bool ReadUV(FbxMesh* pMesh , int ctrlPointIndex , int textureUVIndex , int uvLayer , FbxVector2& pUV)  
{  
    if(uvLayer >= 2 || pMesh->GetElementUVCount() <= uvLayer)  
    {  
        return false;  
    }  
  
    FbxGeometryElementUV* pVertexUV = pMesh->GetElementUV(uvLayer);  
  
    switch(pVertexUV->GetMappingMode())  
    {  
    case FbxGeometryElement::eByControlPoint:
        {  
            switch(pVertexUV->GetReferenceMode())  
            {  
            case FbxGeometryElement::eDirect:  
                {  
                    int x = pVertexUV->GetDirectArray().GetAt(ctrlPointIndex).mData[0];  
                    int y = pVertexUV->GetDirectArray().GetAt(ctrlPointIndex).mData[1];  
					pUV.Set(x, y);
                }  
                break;  
  
            case FbxGeometryElement::eIndexToDirect:  
                {  
                    int id = pVertexUV->GetIndexArray().GetAt(ctrlPointIndex);  
                    int x = pVertexUV->GetDirectArray().GetAt(id).mData[0];  
                    int y = pVertexUV->GetDirectArray().GetAt(id).mData[1];  
					pUV.Set(x, y);
                }  
                break;  
  
            default:  
                break;  
            }  
        }  
        break;  
  
    case FbxGeometryElement::eByPolygonVertex:  
        {  
            switch (pVertexUV->GetReferenceMode())  
            {  
            case FbxGeometryElement::eDirect:  
            case FbxGeometryElement::eIndexToDirect:  
                {  
                    int x = pVertexUV->GetDirectArray().GetAt(textureUVIndex).mData[0];  
                   int y = pVertexUV->GetDirectArray().GetAt(textureUVIndex).mData[1];  
				   pUV.Set(x, y);
                }  
                break;  
  
            default:  
                break;  
            }  
        }  
        break;  
    }  
	return true;
}  
