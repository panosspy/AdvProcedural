////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "inputclass.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "terrainclass.h"
#include "timerclass.h"
#include "positionclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "fontshaderclass.h"
#include "textclass.h"
#include "terrainshaderclass.h"
#include "lightclass.h"

#include "skydomeclass.h"
#include "skydomeshaderclass.h"

#include "skyplaneclass.h"
#include "skyplaneshaderclass.h"

#include "horizontalblurshaderclass.h"
#include "verticalblurshaderclass.h"
#include "rendertextureclass.h"
#include "orthowindowclass.h"

#include "glowshaderclass.h"
#include "bitmapclass.h"
#include "textureshaderclass.h"

#include "frustumclass.h"
#include "quadtreeclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationClass
////////////////////////////////////////////////////////////////////////////////
class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

private:
	bool HandleInput(float);
	bool RenderGraphics();


	bool DownSampleTexture();
	bool RenderHorizontalBlurToTexture();
	bool RenderVerticalBlurToTexture();
	bool UpSampleTexture();


	bool RenderGlowMapToTexture();
	
	bool RenderGlowScene();

private:
	InputClass* m_Input;
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	TerrainClass* m_Terrain;
	TimerClass* m_Timer;
	PositionClass* m_Position;
	FpsClass* m_Fps;
	CpuClass* m_Cpu;
	FontShaderClass* m_FontShader;
	TextClass* m_Text;
	TerrainShaderClass* m_TerrainShader;
	LightClass* m_Light;
	SkyDomeClass* m_SkyDome;
	SkyDomeShaderClass* m_SkyDomeShader;
	SkyPlaneClass *m_SkyPlane;
	SkyPlaneShaderClass* m_SkyPlaneShader;

	HorizontalBlurShaderClass* m_HorizontalBlurShader;
	VerticalBlurShaderClass* m_VerticalBlurShader;

	RenderTextureClass *m_RenderTexture, *m_DownSampleTexure, *m_HorizontalBlurTexture, *m_VerticalBlurTexture, *m_UpSampleTexure;
	OrthoWindowClass *m_SmallWindow, *m_FullScreenWindow;
	GlowShaderClass* m_GlowShader;
	BitmapClass* m_Bitmap;
	TextureShaderClass* m_TextureShader;

	D3DXMATRIX OriginalViewMatrix;

	FrustumClass* m_Frustum;
	QuadTreeClass* m_QuadTree;

	ModelClass* m_Model;
	LightShaderClass* m_LightShader;
	LightClass* m_Light2;

	bool finished;

};

#endif