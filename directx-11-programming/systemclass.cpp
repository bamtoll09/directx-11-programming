#include "systemclass.h"

class SystemClass
{
public:
	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hInstance;
	HWND m_hwnd;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;

public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();
};

// FUNCTION PROTOTYPES
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// GLOBALS
static SystemClass* ApplicationHandle = 0;

// 클래스 생성자에서 객체 포인터들을 nullptr로 초기화
//  * 애플리케이션 내의 모든 포인터와 변수들을 null로 초기화 시키는건 좋은 습관입니다.
SystemClass::SystemClass() : m_Input(nullptr), m_Graphics(nullptr)
{
}

// 복사 생성자
SystemClass::SystemClass(const SystemClass& other)
{
}

// 소멸자
//  * 컴파일러가 생성하는데, 빈 상태로 만들어주지 않을 경우를 대비해서 직접 정의
//  * Shutdown 함수에서 소멸 작업. 소멸자가 불린다는 확신이 없기 때문 ( 예를들면 ExitThread() )
SystemClass::~SystemClass()
{
}

// Make window screen and create & initialize input & graphics system
bool SystemClass::Initialize()
{
	// Initialize the width and height of the screen to zero before sending the variables into the function.
	int screenWidth(0);
	int screenHeight(0);
	bool result(false);

	// Initialize the windows API.
	InitializeWindows(screenWidth, screenHeight);

	// Create the input object. This object will be used to handle reading the keyboard input from the user.
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	m_Input->Initialize();

	// Create the graphics object. This object will handle rendering all the graphics for this application.
	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	// Initialize the graphics object.
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void SystemClass::Run()
{
	MSG msg;
	bool done(false);
	bool result(false);

	// Initialize the message structure
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}

		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}

	return;
}

