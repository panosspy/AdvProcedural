////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"


ApplicationClass::ApplicationClass()
{
	m_Input = 0;
	m_Direct3D = 0;
	m_Camera = 0;
	m_Terrain = 0;
	m_Timer = 0;
	m_Position = 0;
	m_Fps = 0;
	m_Cpu = 0;
	m_FontShader = 0;
	m_Text = 0;
	m_TerrainShader = 0;
	m_Light = 0;
	m_SkyDome = 0;
	m_SkyDomeShader = 0;
	m_SkyPlane = 0;
	m_SkyPlaneShader = 0;

	m_HorizontalBlurShader = 0;
	m_VerticalBlurShader = 0;
	m_RenderTexture = 0;
	m_DownSampleTexure = 0;
	m_HorizontalBlurTexture = 0;
	m_VerticalBlurTexture = 0;
	m_UpSampleTexure = 0;
	m_SmallWindow = 0;
	m_FullScreenWindow = 0;


	m_GlowShader = 0;
	m_Bitmap = 0;
	m_TextureShader = 0;

	m_Frustum = 0;
	m_QuadTree = 0;

	m_Model = 0;
	m_LightShader = 0;
	m_Light2 = 0;


}


ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}


ApplicationClass::~ApplicationClass()
{
}


bool ApplicationClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;
	float cameraX, cameraY, cameraZ;
	//removed the baseViewMatrix from thins function making it global for this class to use on functiones we need the starting view matrix
	//D3DXMATRIX baseViewMatrix;
	char videoCard[128];
	int videoMemory;

	int downSampleWidth, downSampleHeight;
	// Set the size to sample down to.
	downSampleWidth = screenWidth / 2;
	downSampleHeight = screenHeight / 2;


	// Create the input object.  The input object will be used to handle reading the keyboard and mouse input from the user.
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	result = m_Input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the Direct3D object.
	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize DirectX 11.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	// using the global view matrix we created
	m_Camera->GetViewMatrix(OriginalViewMatrix);

	// Set the initial position of the camera.
	cameraX = 15.0f;
	cameraY = 2.0f;
	cameraZ = 15.0f;

	m_Camera->SetPosition(cameraX, cameraY, cameraZ);

	// Create the terrain object.
	m_Terrain = new TerrainClass;
	if (!m_Terrain)
	{
		return false;
	}

	// Initialize the terrain object.
	//	result = m_Terrain->Initialize(m_Direct3D->GetDevice(), "../Engine/data/heightmap01.bmp");
	result = m_Terrain->InitializeTerrain(m_Direct3D->GetDevice(), 257, 257, L"../Engine/data/grass.dds", L"../Engine/data/slope.dds",
		L"../Engine/data/rock.dds");   //initialise the flat terrain.
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	// Create the timer object.
	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	// Create the position object.
	m_Position = new PositionClass;
	if (!m_Position)
	{
		return false;
	}

	// Set the initial position of the viewer to the same as the initial camera position.
	m_Position->SetPosition(cameraX, cameraY, cameraZ);

	// Create the fps object.
	m_Fps = new FpsClass;
	if (!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Initialize();

	// Create the cpu object.
	m_Cpu = new CpuClass;
	if (!m_Cpu)
	{
		return false;
	}

	// Initialize the cpu object.
	m_Cpu->Initialize();

	// Create the font shader object.
	m_FontShader = new FontShaderClass;
	if (!m_FontShader)
	{
		return false;
	}

	// Initialize the font shader object.
	result = m_FontShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, OriginalViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Retrieve the video card information.
	m_Direct3D->GetVideoCardInfo(videoCard, videoMemory);

	// Set the video card information in the text object.
	result = m_Text->SetVideoCardInfo(videoCard, videoMemory, m_Direct3D->GetDeviceContext());
	if (!result)
	{
		MessageBox(hwnd, L"Could not set video card info in the text object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_TerrainShader = new TerrainShaderClass;
	if (!m_TerrainShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_TerrainShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.05f, 0.05f, 0.05f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(-0.5f, -1.0f, 0.0f);

	// Create the second light object.
	m_Light2 = new LightClass;
	if (!m_Light2)
	{
		return false;
	}

	// Initialize the light object.
	m_Light2->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light2->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light2->SetDirection(1.0f, 0.0f, 0.0f);

	// Create the sky dome object.
	m_SkyDome = new SkyDomeClass;
	if (!m_SkyDome)
	{
		return false;
	}

	// Initialize the sky dome object.
	result = m_SkyDome->Initialize(m_Direct3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome object.", L"Error", MB_OK);
		return false;
	}

	// Create the sky dome shader object.
	m_SkyDomeShader = new SkyDomeShaderClass;
	if (!m_SkyDomeShader)
	{
		return false;
	}

	// Initialize the sky dome shader object.
	result = m_SkyDomeShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the sky plane object.
	m_SkyPlane = new SkyPlaneClass;
	if (!m_SkyPlane)
	{
		return false;
	}

	// Initialize the sky plane object.
	result = m_SkyPlane->Initialize(m_Direct3D->GetDevice(), L"../Engine/data/cloud001.dds", L"../Engine/data/perturb001.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky plane object.", L"Error", MB_OK);
		return false;
	}

	// Create the sky plane shader object.
	m_SkyPlaneShader = new SkyPlaneShaderClass;
	if (!m_SkyPlaneShader)
	{
		return false;
	}

	// Initialize the sky plane shader object.
	result = m_SkyPlaneShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky plane shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the horizontal blur shader object.
	m_HorizontalBlurShader = new HorizontalBlurShaderClass;
	if (!m_HorizontalBlurShader)
	{
		return false;
	}

	// Initialize the horizontal blur shader object.
	result = m_HorizontalBlurShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the horizontal blur shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the vertical blur shader object.
	m_VerticalBlurShader = new VerticalBlurShaderClass;
	if (!m_VerticalBlurShader)
	{
		return false;
	}

	// Initialize the vertical blur shader object.
	result = m_VerticalBlurShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the vertical blur shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the render to texture object.
	m_RenderTexture = new RenderTextureClass;
	if (!m_RenderTexture)
	{
		return false;
	}

	// Initialize the render to texture object.
	result = m_RenderTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the down sample render to texture object.
	m_DownSampleTexure = new RenderTextureClass;
	if (!m_DownSampleTexure)
	{
		return false;
	}

	// Initialize the down sample render to texture object.
	result = m_DownSampleTexure->Initialize(m_Direct3D->GetDevice(), downSampleWidth, downSampleHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the down sample render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the horizontal blur render to texture object.
	m_HorizontalBlurTexture = new RenderTextureClass;
	if (!m_HorizontalBlurTexture)
	{
		return false;
	}

	// Initialize the horizontal blur render to texture object.
	result = m_HorizontalBlurTexture->Initialize(m_Direct3D->GetDevice(), downSampleWidth, downSampleHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the horizontal blur render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the vertical blur render to texture object.
	m_VerticalBlurTexture = new RenderTextureClass;
	if (!m_VerticalBlurTexture)
	{
		return false;
	}

	// Initialize the vertical blur render to texture object.
	result = m_VerticalBlurTexture->Initialize(m_Direct3D->GetDevice(), downSampleWidth, downSampleHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the vertical blur render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the up sample render to texture object.
	m_UpSampleTexure = new RenderTextureClass;
	if (!m_UpSampleTexure)
	{
		return false;
	}

	// Initialize the up sample render to texture object.
	result = m_UpSampleTexure->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the up sample render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the small ortho window object.
	m_SmallWindow = new OrthoWindowClass;
	if (!m_SmallWindow)
	{
		return false;
	}

	// Initialize the small ortho window object.
	result = m_SmallWindow->Initialize(m_Direct3D->GetDevice(), downSampleWidth, downSampleHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the small ortho window object.", L"Error", MB_OK);
		return false;
	}

	// Create the full screen ortho window object.
	m_FullScreenWindow = new OrthoWindowClass;
	if (!m_FullScreenWindow)
	{
		return false;
	}

	// Initialize the full screen ortho window object.
	result = m_FullScreenWindow->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the full screen ortho window object.", L"Error", MB_OK);
		return false;
	}

	// Create the glow shader object.
	m_GlowShader = new GlowShaderClass;
	if (!m_GlowShader)
	{
		return false;
	}

	// Initialize the glow shader object.
	result = m_GlowShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the glow shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, L"../Engine/data/test.dds", L"../Engine/data/glowmap.dds", 256, 32);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the frustum object.
	m_Frustum = new FrustumClass;
	if (!m_Frustum)
	{
		return false;
	}

	// Create the quad tree object.
	m_QuadTree = new QuadTreeClass;
	if (!m_QuadTree)
	{
		return false;
	}

	// Initialize the quad tree object.
	result = m_QuadTree->Initialize(m_Terrain, m_Direct3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the quad tree object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model->Initialize(m_Direct3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/seafloor.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void ApplicationClass::Shutdown()
{
	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the quad tree object.
	if (m_QuadTree)
	{
		m_QuadTree->Shutdown();
		delete m_QuadTree;
		m_QuadTree = 0;
	}

	// Release the frustum object.
	if (m_Frustum)
	{
		delete m_Frustum;
		m_Frustum = 0;
	}

	// Release the bitmap object.
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the glow shader object.
	if (m_GlowShader)
	{
		m_GlowShader->Shutdown();
		delete m_GlowShader;
		m_GlowShader = 0;
	}

	// Release the full screen ortho window object.
	if (m_FullScreenWindow)
	{
		m_FullScreenWindow->Shutdown();
		delete m_FullScreenWindow;
		m_FullScreenWindow = 0;
	}

	// Release the small ortho window object.
	if (m_SmallWindow)
	{
		m_SmallWindow->Shutdown();
		delete m_SmallWindow;
		m_SmallWindow = 0;
	}

	// Release the up sample render to texture object.
	if (m_UpSampleTexure)
	{
		m_UpSampleTexure->Shutdown();
		delete m_UpSampleTexure;
		m_UpSampleTexure = 0;
	}

	// Release the vertical blur render to texture object.
	if (m_VerticalBlurTexture)
	{
		m_VerticalBlurTexture->Shutdown();
		delete m_VerticalBlurTexture;
		m_VerticalBlurTexture = 0;
	}

	// Release the horizontal blur render to texture object.
	if (m_HorizontalBlurTexture)
	{
		m_HorizontalBlurTexture->Shutdown();
		delete m_HorizontalBlurTexture;
		m_HorizontalBlurTexture = 0;
	}

	// Release the down sample render to texture object.
	if (m_DownSampleTexure)
	{
		m_DownSampleTexure->Shutdown();
		delete m_DownSampleTexure;
		m_DownSampleTexure = 0;
	}

	// Release the render to texture object.
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	// Release the vertical blur shader object.
	if (m_VerticalBlurShader)
	{
		m_VerticalBlurShader->Shutdown();
		delete m_VerticalBlurShader;
		m_VerticalBlurShader = 0;
	}

	// Release the horizontal blur shader object.
	if (m_HorizontalBlurShader)
	{
		m_HorizontalBlurShader->Shutdown();
		delete m_HorizontalBlurShader;
		m_HorizontalBlurShader = 0;
	}

	// Release the sky plane shader object.
	if (m_SkyPlaneShader)
	{
		m_SkyPlaneShader->Shutdown();
		delete m_SkyPlaneShader;
		m_SkyPlaneShader = 0;
	}

	// Release the sky plane object.
	if (m_SkyPlane)
	{
		m_SkyPlane->Shutdown();
		delete m_SkyPlane;
		m_SkyPlane = 0;
	}

	// Release the sky dome shader object.
	if (m_SkyDomeShader)
	{
		m_SkyDomeShader->Shutdown();
		delete m_SkyDomeShader;
		m_SkyDomeShader = 0;
	}

	// Release the sky dome object.
	if (m_SkyDome)
	{
		m_SkyDome->Shutdown();
		delete m_SkyDome;
		m_SkyDome = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light object.
	if (m_Light2)
	{
		delete m_Light2;
		m_Light2 = 0;
	}

	// Release the terrain shader object.
	if (m_TerrainShader)
	{
		m_TerrainShader->Shutdown();
		delete m_TerrainShader;
		m_TerrainShader = 0;
	}

	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the font shader object.
	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the cpu object.
	if (m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// Release the fps object.
	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	// Release the position object.
	if (m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// Release the timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the terrain object.
	if (m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	// Release the input object.
	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	return;
}


bool ApplicationClass::Frame()
{
	bool result, foundHeight;
	D3DXVECTOR3 position;
	float height;

	// Read the user input.
	result = m_Input->Frame();
	if (!result)
	{
		return false;
	}

	// Check if the user pressed escape and wants to exit the application.
	if (m_Input->IsEscapePressed() == true)
	{
		return false;
	}

	// Update the system stats.
	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	// Update the FPS value in the text object.
	result = m_Text->SetFps(m_Fps->GetFps(), m_Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Update the CPU usage value in the text object.
	result = m_Text->SetCpu(m_Cpu->GetCpuPercentage(), m_Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Do the frame input processing.
	result = HandleInput(m_Timer->GetTime());
	if (!result)
	{
		return false;
	}

	// Do the sky plane frame processing.
	m_SkyPlane->Frame();

	// Get the current position of the camera.
	position = m_Camera->GetPosition();

	// Get the height of the triangle that is directly underneath the given camera position.
	foundHeight = m_QuadTree->GetHeightAtPosition(position.x, position.z, height);
	if (foundHeight){
		// If there was a triangle under the camera then position the camera just above it by two units.
		m_Camera->SetPosition(position.x, height + 2.0f, position.z);
		
	}


	if (!finished) {
		// First render the glow maps to a render texture.
		result = RenderGlowMapToTexture();
		if (!result)
		{
			return false;
		}

		// Next down sample the render texture to a smaller sized texture.
		result = DownSampleTexture();
		if (!result)
		{
			return false;
		}

		// Perform a horizontal blur on the down sampled render texture.
		result = RenderHorizontalBlurToTexture();
		if (!result)
		{
			return false;
		}

		// Now perform a vertical blur on the horizontal blur render texture.
		result = RenderVerticalBlurToTexture();
		if (!result)
		{
			return false;
		}

		// Up sample the final blurred render texture to screen size again.
		result = UpSampleTexture();
		if (!result)
		{
			return false;
		}

		// render the texture to the scene
		result = RenderGlowScene();
		if (!result)
		{
			return false;
		}
	}
	else {

		// Render the graphics.
		result = RenderGraphics();
		if (!result)
		{
			return false;
		}
	}

	return result;
}


bool ApplicationClass::HandleInput(float frameTime)
{
	bool keyDown, result;
	float posX, posY, posZ, rotX, rotY, rotZ;


	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(frameTime);

	//Can press onoly after he finishes the maze
	if (!finished) {
		// Handle the input.
		keyDown = m_Input->IsFPressed();
		m_Terrain->GenerateHeightMap(m_Direct3D->GetDevice(), keyDown);
		if (keyDown) {
			m_QuadTree->Initialize(m_Terrain, m_Direct3D->GetDevice());
		}

	}

	keyDown = m_Input->IsLeftPressed();
	m_Position->TurnLeft(keyDown);

	keyDown = m_Input->IsRightPressed();
	m_Position->TurnRight(keyDown);

	keyDown = m_Input->IsUpPressed();
	m_Position->MoveForward(keyDown);

	keyDown = m_Input->IsDownPressed();
	m_Position->MoveBackward(keyDown);

	keyDown = m_Input->IsAPressed();
	m_Position->MoveUpward(keyDown);

	keyDown = m_Input->IsZPressed();
	m_Position->MoveDownward(keyDown);

	keyDown = m_Input->IsPgUpPressed();
	m_Position->LookUpward(keyDown);

	keyDown = m_Input->IsPgDownPressed();
	m_Position->LookDownward(keyDown);

	// Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	// Set the position of the camera.
	m_Camera->SetPosition(posX, posY, posZ);
	m_Camera->SetRotation(rotX, rotY, rotZ);

	if (m_Camera->GetPosition().x >140 && m_Camera->GetPosition().x<144 && m_Camera->GetPosition().z>140 && m_Camera->GetPosition().z<144) {
		finished = false;
	}
	
	
	// Update the position values in the text object.
	result = m_Text->SetCameraPosition(posX, posY, posZ, m_Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Update the rotation values in the text object.
	result = m_Text->SetCameraRotation(rotX, rotY, rotZ, m_Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}


	return true;
}




bool ApplicationClass::RenderGraphics()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	D3DXVECTOR3 cameraPosition;
	bool result;


	// Clear the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();

	// Translate the sky dome to be centered around the camera position.
	D3DXMatrixTranslation(&worldMatrix, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	// Turn off back face culling.
	m_Direct3D->TurnOffCulling();

	// Turn off the Z buffer.
	m_Direct3D->TurnZBufferOff();

	// Render the sky dome using the sky dome shader.
	m_SkyDome->Render(m_Direct3D->GetDeviceContext());
	m_SkyDomeShader->Render(m_Direct3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor());

	// Turn back face culling back on.
	m_Direct3D->TurnOnCulling();


	// Enable additive blending so the clouds blend with the sky dome color.
	m_Direct3D->EnableSecondBlendState();


	// Render the sky plane using the sky plane shader.
	m_SkyPlane->Render(m_Direct3D->GetDeviceContext());
	m_SkyPlaneShader->Render(m_Direct3D->GetDeviceContext(), m_SkyPlane->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_SkyPlane->GetCloudTexture(), m_SkyPlane->GetPerturbTexture(), m_SkyPlane->GetTranslation(), m_SkyPlane->GetScale(),
		m_SkyPlane->GetBrightness());



	// Turn off blending.
	m_Direct3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on.
	m_Direct3D->TurnZBufferOn();

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);





	// Construct the frustum.
	m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);
	// Render the terrain buffers.
	m_Terrain->Render(m_Direct3D->GetDeviceContext());

	// Set the terrain shader parameters that it will use for rendering.
	result = m_TerrainShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(),
		m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Terrain->GetGrassTexture(),
		m_Terrain->GetSlopeTexture(), m_Terrain->GetRockTexture());
	if (!result)
	{
		return false;
	}

	// Render the terrain using the quad tree and terrain shader.
	m_QuadTree->Render(m_Frustum, m_Direct3D->GetDeviceContext(), m_TerrainShader);

	// Set the number of rendered terrain triangles since some were culled.
	result = m_Text->SetRenderCount(m_QuadTree->GetDrawCount(), m_Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	D3DXMatrixTranslation(&worldMatrix, 142, 2, 142);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// Render the model using the light shader.
	m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTexture(), m_Light2->GetDirection(), m_Light2->GetAmbientColor(), m_Light2->GetDiffuseColor());

	

	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	m_Direct3D->TurnOnAlphaBlending();

	// Render the text user interface elements.
	result = m_Text->Render(m_Direct3D->GetDeviceContext(), m_FontShader, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_Direct3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	
	

	
	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}

bool ApplicationClass::RenderGlowScene()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	D3DXVECTOR3 cameraPosition;
	bool result;


	// Clear the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);


	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Put the full screen ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_FullScreenWindow->Render(m_Direct3D->GetDeviceContext());

	// Render the full screen ortho window using the texture shader and the full screen sized blurred render to texture resource.
	m_GlowShader->Render(m_Direct3D->GetDeviceContext(), m_FullScreenWindow->GetIndexCount(), worldMatrix, OriginalViewMatrix, orthoMatrix,
		m_RenderTexture->GetShaderResourceView(), m_UpSampleTexure->GetShaderResourceView(), 2.0f);

	// Setting the new text
	m_Text->SetFinishedText(m_Direct3D->GetDeviceContext());

	// Render the text user interface elements.
	result = m_Text->Render(m_Direct3D->GetDeviceContext(), m_FontShader, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}


	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}

bool ApplicationClass::RenderGlowMapToTexture()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	D3DXVECTOR3 cameraPosition;
	bool result;


	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 0.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	
	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();

	// Translate the sky dome to be centered around the camera position.
	D3DXMatrixTranslation(&worldMatrix, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	// Turn off back face culling.
	m_Direct3D->TurnOffCulling();

	// Turn off the Z buffer.
	m_Direct3D->TurnZBufferOff();

	// Render the sky dome using the sky dome shader.
	m_SkyDome->Render(m_Direct3D->GetDeviceContext());
	m_SkyDomeShader->Render(m_Direct3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor());

	// Turn back face culling back on.
	m_Direct3D->TurnOnCulling();


	// Enable additive blending so the clouds blend with the sky dome color.
	m_Direct3D->EnableSecondBlendState();


	// Render the sky plane using the sky plane shader.
	m_SkyPlane->Render(m_Direct3D->GetDeviceContext());
	m_SkyPlaneShader->Render(m_Direct3D->GetDeviceContext(), m_SkyPlane->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_SkyPlane->GetCloudTexture(), m_SkyPlane->GetPerturbTexture(), m_SkyPlane->GetTranslation(), m_SkyPlane->GetScale(),
		m_SkyPlane->GetBrightness());



	// Turn off blending.
	m_Direct3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on.
	m_Direct3D->TurnZBufferOn();

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);





	// Render the terrain buffers.
	m_Terrain->Render(m_Direct3D->GetDeviceContext());

	// Render the terrain using the terrain shader.
	result = m_TerrainShader->Render(m_Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Terrain->GetGrassTexture(),
		m_Terrain->GetSlopeTexture(), m_Terrain->GetRockTexture());
	if (!result)
	{
		return false;
	}
	
	

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return true;
}


bool ApplicationClass::DownSampleTexture()
{
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
	bool result;


	// Set the render target to be the render to texture.
	m_DownSampleTexure->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	m_DownSampleTexure->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 1.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world and view matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);

	// Get the ortho matrix from the render to texture since texture has different dimensions being that it is smaller.
	m_DownSampleTexure->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_SmallWindow->Render(m_Direct3D->GetDeviceContext());

	// Render the small ortho window using the texture shader and the render to texture of the scene as the texture resource.
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_SmallWindow->GetIndexCount(), worldMatrix, OriginalViewMatrix, orthoMatrix,
		m_RenderTexture->GetShaderResourceView());
	if (!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::RenderHorizontalBlurToTexture()
{
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
	float screenSizeX;
	bool result;


	// Set the render target to be the render to texture.
	m_HorizontalBlurTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	m_HorizontalBlurTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world and view matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Get the ortho matrix from the render to texture since texture has different dimensions.
	m_HorizontalBlurTexture->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Store the screen width in a float that will be used in the horizontal blur shader.
	screenSizeX = (float)m_HorizontalBlurTexture->GetTextureWidth();

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_SmallWindow->Render(m_Direct3D->GetDeviceContext());

	// Render the small ortho window using the horizontal blur shader and the down sampled render to texture resource.
	result = m_HorizontalBlurShader->Render(m_Direct3D->GetDeviceContext(), m_SmallWindow->GetIndexCount(), worldMatrix, OriginalViewMatrix, orthoMatrix,
		m_DownSampleTexure->GetShaderResourceView(), screenSizeX);
	if (!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::RenderVerticalBlurToTexture()
{
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
	float screenSizeY;
	bool result;


	// Set the render target to be the render to texture.
	m_VerticalBlurTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	m_VerticalBlurTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world and view matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Get the ortho matrix from the render to texture since texture has different dimensions.
	m_VerticalBlurTexture->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Store the screen height in a float that will be used in the vertical blur shader.
	screenSizeY = (float)m_VerticalBlurTexture->GetTextureHeight();

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_SmallWindow->Render(m_Direct3D->GetDeviceContext());

	// Render the small ortho window using the vertical blur shader and the horizontal blurred render to texture resource.
	result = m_VerticalBlurShader->Render(m_Direct3D->GetDeviceContext(), m_SmallWindow->GetIndexCount(), worldMatrix, OriginalViewMatrix, orthoMatrix,
		m_HorizontalBlurTexture->GetShaderResourceView(), screenSizeY);
	if (!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::UpSampleTexture()
{
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
	bool result;


	// Set the render target to be the render to texture.
	m_UpSampleTexure->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	m_UpSampleTexure->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world and view matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);

	// Get the ortho matrix from the render to texture since texture has different dimensions.
	m_UpSampleTexure->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Put the full screen ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_FullScreenWindow->Render(m_Direct3D->GetDeviceContext());

	// Render the full screen ortho window using the texture shader and the small sized final blurred render to texture resource.
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_FullScreenWindow->GetIndexCount(), worldMatrix, OriginalViewMatrix, orthoMatrix,
		m_VerticalBlurTexture->GetShaderResourceView());
	if (!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return true;
}
