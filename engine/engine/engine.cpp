// engine.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "SDK_Utility.h"

/* Tab character ("\t") counter */
    int numTabs = 0;

    /**
     * Print the required number of tabs.
     */
    void PrintTabs() {
        for(int i = 0; i < numTabs; i++)
            printf("\t");
    }
	 /**
     * Return a string-based representation based on the attribute type.
     */
    FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) {
        switch(type) {
            case FbxNodeAttribute::eUnknown: return "unidentified";
            case FbxNodeAttribute::eNull: return "null";
            case FbxNodeAttribute::eMarker: return "marker";
            case FbxNodeAttribute::eSkeleton: return "skeleton";
            case FbxNodeAttribute::eMesh: return "mesh";
            case FbxNodeAttribute::eNurbs: return "nurbs";
            case FbxNodeAttribute::ePatch: return "patch";
            case FbxNodeAttribute::eCamera: return "camera";
            case FbxNodeAttribute::eCameraStereo: return "stereo";
            case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
            case FbxNodeAttribute::eLight: return "light";
            case FbxNodeAttribute::eOpticalReference: return "optical reference";
            case FbxNodeAttribute::eOpticalMarker: return "marker";
            case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
            case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
            case FbxNodeAttribute::eBoundary: return "boundary";
            case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
            case FbxNodeAttribute::eShape: return "shape";
            case FbxNodeAttribute::eLODGroup: return "lodgroup";
            case FbxNodeAttribute::eSubDiv: return "subdiv";
            default: return "unknown";
        }
    }

int _tmain(int argc, _TCHAR* argv[]){
	InitializeSdkManagerAndScene();
	const char* lFilename = "clowngishtest.fbx";
	LoadFBXScene(lFilename);
	FbxNode* rootNode = GetRootNode();
	ProcessNode(rootNode);
	//ProcessSkeleton(rootNode);
	return 0;
}

