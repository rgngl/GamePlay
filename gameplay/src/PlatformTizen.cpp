#ifdef __TIZEN__

#include "Base.h"
#include "Platform.h"
#include "FileSystem.h"
#include "Game.h"
#include "Form.h"
#include "ScriptController.h"

#include <FApp.h>
#include <FBase.h>
#include <FText.h>
#include <FSystem.h>
#include <FUi.h>
#include <FUiIme.h>
#include <FGraphics.h>
#include <FGraphicsOpengl2.h>
#include <FGrpGlPlayer.h>
#include <FGrpIGlRenderer.h>

PFNGLBINDVERTEXARRAYOESPROC glBindVertexArray = NULL;
PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArrays = NULL;
PFNGLGENVERTEXARRAYSOESPROC glGenVertexArrays = NULL;
PFNGLISVERTEXARRAYOESPROC glIsVertexArray = NULL;

class GamePlay3DGLRenderer :
    public Tizen::Graphics::Opengl::IGlRenderer
{
    friend class gameplay::Platform;

public:
    GamePlay3DGLRenderer(void) : __controlWidth(0), __controlHeight(0) {}
    ~GamePlay3DGLRenderer(void) {}

    virtual bool InitializeGl(void)
    {
        return true;
    }

    virtual bool TerminateGl(void)
    {
        return true;
    }

    void startGame()
    {
        if (game == NULL)
        {
            game = gameplay::Game::getInstance();
            long long ticks;
            Tizen::System::SystemTime::GetTicks(ticks);
            __timeStart = (double) ticks;
            game->run();
        }
    }

    virtual bool Draw(void)
    {
        if (game == NULL)
        {
            startGame();

            // HACK: Skip the first display update after creating buffers and initializing the game.
            // If we don't do this, the first frame (which includes any drawing during initialization)
            // does not make it to the display for some reason.
            return true;
        }

        game->frame();

        return true;
    }

    virtual bool Pause(void)
    {
        return true;
    }

    virtual bool Resume(void)
    {
        return true;
    }

    virtual int GetTargetControlWidth(void)
    {
        return __controlWidth;
    }

    virtual int GetTargetControlHeight(void)
    {
        return __controlHeight;
    }

    virtual void SetTargetControlWidth(int width)
    {
        __controlWidth = width;
    }

    virtual void SetTargetControlHeight(int height)
    {
        __controlHeight = height;
    }

private:
    double __timeStart;
    double __timeAbsolute;

    int __controlWidth;
    int __controlHeight;

    gameplay::Game *game;
};

class GamePlay3DFrame : public Tizen::Ui::Controls::Frame
{
public:
    GamePlay3DFrame(void) {}

    virtual ~GamePlay3DFrame(void) {}

    virtual result OnInitializing(void)
    {
        SetOrientation(Tizen::Ui::ORIENTATION_LANDSCAPE);
        return E_SUCCESS;
    }

    virtual result OnTerminating(void)
    {
        return E_SUCCESS;
    }
  };

class GamePlay3DApp : public Tizen::App::UiApp,
    public Tizen::System::IScreenEventListener,
    public Tizen::Ui::IKeyEventListener
{
    friend class gameplay::Platform;

public:
    static Tizen::App::UiApp* CreateInstance(void)
    {
        return new (std::nothrow) GamePlay3DApp();
    }

    GamePlay3DApp(void) {};
    virtual~GamePlay3DApp(void) {};

    virtual bool OnAppInitializing(Tizen::App::AppRegistry& appRegistry)
    {
        Tizen::System::PowerManager::SetScreenEventListener(*this);
        return true;
    }

    virtual bool OnAppInitialized(void)
    {
        GamePlay3DFrame* pFrame = new GamePlay3DFrame();
        pFrame->Construct();
        pFrame->SetShowMode(Tizen::Ui::Controls::FRAME_SHOW_MODE_FULL_SCREEN);
        AddFrame(*pFrame);

        pFrame->AddKeyEventListener(*this);

        __player = new Tizen::Graphics::Opengl::GlPlayer;
        __player->Construct(Tizen::Graphics::Opengl::EGL_CONTEXT_CLIENT_VERSION_2_X, pFrame);

        __player->SetFps(60);
        __player->SetEglAttributePreset(Tizen::Graphics::Opengl::EGL_ATTRIBUTES_PRESET_RGB565);

        __renderer = new GamePlay3DGLRenderer();
        __player->SetIGlRenderer(__renderer);

        __player->Start();

        return true;
    }

    virtual bool OnAppWillTerminate(void)
    {
        return true;
    }

    virtual bool OnAppTerminating(Tizen::App::AppRegistry& appRegistry, bool forcedTermination = false)
    {
        __player->Stop();

        if(__renderer != null)
        {
            delete __renderer;
        }
        delete __player;

        return true;
    }

    virtual void OnForeground(void)
    {

    }

    virtual void OnBackground(void)
    {

    }

    virtual void OnLowMemory(void)
    {

    }

    virtual void OnBatteryLevelChanged(Tizen::System::BatteryLevel batteryLevel)
    {

    }

    virtual void OnScreenOn(void)
    {

    }

    virtual void OnScreenOff(void)
    {

    }

    virtual void OnKeyPressed(const Tizen::Ui::Control& source, Tizen::Ui::KeyCode keyCode)
    {

    }

    virtual void OnKeyReleased(const Tizen::Ui::Control& source, Tizen::Ui::KeyCode keyCode)
    {

    }

    virtual void OnKeyLongPressed(const Tizen::Ui::Control& source, Tizen::Ui::KeyCode keyCode)
    {

    }

  private:
    Tizen::Graphics::Opengl::GlPlayer* __player;
    Tizen::Graphics::Opengl::IGlRenderer* __renderer;
};

gameplay::Platform::Platform(Game *game)
    : _game(game)
{
}

gameplay::Platform::~Platform()
{
}

gameplay::Platform* gameplay::Platform::create(Game* game)
{
    GP_ASSERT(game);

    Tizen::App::UiApp* app = Tizen::App::UiApp::GetInstance();
    Tizen::Base::String resPath = app->GetAppResourcePath().GetPointer();

    Tizen::Text::AsciiEncoding ascii;
    Tizen::Base::ByteBuffer* buffer = ascii.GetBytesN(resPath);
    FileSystem::setResourcePath((const char*)buffer->GetPointer());
    print("Setting resource path: %s", buffer->GetPointer());
    delete buffer;

    Platform* platform = new Platform(game);
    return platform;
}

unsigned int gameplay::Platform::getDisplayWidth()
{
    return Tizen::App::UiApp::GetInstance()->GetFrame(0)->GetWidth();
}

unsigned int gameplay::Platform::getDisplayHeight()
{
    return Tizen::App::UiApp::GetInstance()->GetFrame(0)->GetHeight();
}

double gameplay::Platform::getAbsoluteTime()
{
    long long ticks;
    Tizen::System::SystemTime::GetTicks(ticks);

    return (double)ticks;
}

void gameplay::Platform::setAbsoluteTime(double time)
{
    GamePlay3DApp *app = (GamePlay3DApp*) Tizen::App::UiApp::GetInstance();

    ((GamePlay3DGLRenderer*)app->__renderer)->__timeAbsolute = time;
}

int gameplay::Platform::enterMessagePump()
{
    Tizen::Base::Collection::ArrayList args(Tizen::Base::Collection::SingleObjectDeleter);
    args.Construct();

    result r = Tizen::App::UiApp::Execute(GamePlay3DApp::CreateInstance, &args);
    TryLog(r == E_SUCCESS, "[%s] Application execution failed", GetErrorMessage(r));
    AppLog("Application finished.");

    return static_cast<int>(r);
}

void gameplay::Platform::signalShutdown()
{
    Tizen::App::UiApp::GetInstance()->Terminate();
}

bool gameplay::Platform::isVsync()
{
    return false;
}

void gameplay::Platform::setVsync(bool enable)
{
}

void gameplay::Platform::swapBuffers()
{
}

void gameplay::Platform::sleep(long ms)
{
    usleep(ms * 1000);
}

bool gameplay::Platform::hasAccelerometer()
{
    return false;
}

bool gameplay::Platform::hasMouse()
{
    return false;
}

bool gameplay::Platform::isCursorVisible()
{
    return false;
}

void gameplay::Platform::setMouseCaptured(bool captured)
{
    // not supported
}

bool gameplay::Platform::isMouseCaptured()
{
    // not supported
    return false;
}

void gameplay::Platform::setCursorVisible(bool visible)
{
    // not supported
}

bool gameplay::Platform::isMultiSampling()
{
    return false;
}

void gameplay::Platform::setMultiSampling(bool enabled)
{
    //TODO
}

bool gameplay::Platform::isMultiTouch()
{
    return true;
}

void gameplay::Platform::setMultiTouch(bool enable)
{
    Tizen::App::UiApp::GetInstance()->GetFrame(0)->SetMultipointTouchEnabled(enable);
}

bool gameplay::Platform::isGestureSupported(Gesture::GestureEvent evt)
{
    return false;
}

void gameplay::Platform::registerGesture(Gesture::GestureEvent evt)
{
    //TODO:not supported
}

void gameplay::Platform::unregisterGesture(Gesture::GestureEvent evt)
{
    //TODO:not supported
}

bool gameplay::Platform::isGestureRegistered(Gesture::GestureEvent evt)
{
    //TODO:not supported
    return false;
}

void gameplay::Platform::displayKeyboard(bool display)
{
    //TODO
}

void gameplay::Platform::pollGamepadState(Gamepad* gamepad)
{
    //TODO:not supported
}

void gameplay::Platform::getAccelerometerValues(float* pitch, float* roll)
{
    //TODO
}

void gameplay::Platform::getSensorValues(float* accelX, float* accelY, float* accelZ, float* gyroX, float* gyroY, float* gyroZ)
{
    //TODO
}

bool gameplay::Platform::canExit()
{
    return false;
}

bool gameplay::Platform::launchURL(char const* url)
{
    //TODO
    return true;
}

void gameplay::Platform::getArguments(int* argc, char*** argv)
{
    //TODO: NOT SUPPORTED YET
    *argc = 0;
    *argv = NULL;
}

namespace gameplay {
extern void print(const char* format, ...)
{
    GP_ASSERT(format);
    va_list argptr;
    va_start(argptr, format);
    char printText[1024];
    vsnprintf(printText, 1023, format, argptr);
    AppLogDebug(printText);
    va_end(argptr);
}
}

#endif //__TIZEN__
