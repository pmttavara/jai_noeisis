
#include "noesis_jai.hpp"

#include "NoesisLicense.h"
#include "NoesisPCH.h"

#include <NsApp/LocalFontProvider.h>
#include <NsApp/LocalTextureProvider.h>
#include <NsApp/ApplicationLauncher.h>
#include <NsApp/Application.h>
#include <NsApp/Window.h>
#include <NsApp/ThemeProviders.h>

using namespace Noesis;
using namespace NoesisApp;

extern "C" void noesis_load_xaml(char filename[512]);

class MyXamlProvider : public Noesis::XamlProvider
{
public:
    MyXamlProvider(const char* rootPath) : Noesis::XamlProvider{} {
        StrCopy(mRootPath, sizeof(mRootPath), rootPath);

        noesis_load_xaml("bruh.xaml");
    }

private:
    /// From XamlProvider
    //@{
    Noesis::Ptr<Noesis::Stream> LoadXaml(const Noesis::Uri& uri) override {
        FixedString<512> path;
        uri.GetPath(path);

        char filename[512];

        if (StrIsNullOrEmpty(mRootPath))
        {
            StrCopy(filename, sizeof(filename), path.Str());
        }
        else
        {
            StrCopy(filename, sizeof(filename), mRootPath);
            StrAppend(filename, sizeof(filename), "/");
            StrAppend(filename, sizeof(filename), path.Str());
        }

        noesis_load_xaml(filename);

        return OpenFileStream(filename);
    }
    //@}

private:
    char mRootPath[512];
};

void Noesis_Jai::HelloWorld() {
    Noesis::SetLogHandler([](const char*, uint32_t, uint32_t level, const char*, const char* msg)
    {
        // [TRACE] [DEBUG] [INFO] [WARNING] [ERROR]
        const char* prefixes[] = { "T", "D", "I", "W", "E" };
        printf("[NOESIS/%s] %s\n", prefixes[level], msg);
    });

    // Sets the active license
    Noesis::GUI::SetLicense(NS_LICENSE_NAME, NS_LICENSE_KEY);

    // Noesis initialization. This must be the first step before using any NoesisGUI functionality
    Noesis::GUI::Init();

    Noesis::GUI::SetXamlProvider(MakePtr<MyXamlProvider>("."));
    /*
    Noesis::GUI::SetFontProvider(MakePtr<LocalFontProvider>("."));
    Noesis::GUI::SetTextureProvider(MakePtr<LocalTextureProvider>("."));


    const char* fonts[] = { "Fonts/#PT Root UI", "Arial", "Segoe UI Emoji" };
    GUI::SetFontFallbacks(fonts, 3);
    GUI::SetFontDefaultProperties(15.0f, FontWeight_Normal, FontStretch_Normal, FontStyle_Normal);

    Ptr<LocalXamlProvider> myXamlProvider = MakePtr<LocalXamlProvider>(".");
    Ptr<LocalFontProvider> myFontProvider = MakePtr<LocalFontProvider>(".");
    Ptr<LocalTextureProvider> myTextureProvider = MakePtr<LocalTextureProvider>(".");

    NoesisApp::SetThemeProviders(myXamlProvider, myFontProvider, myTextureProvider);

    Noesis::GUI::LoadApplicationResources("Theme/NoesisTheme.DarkBlue.xaml");

    Ptr<FrameworkElement> xaml = Noesis::GUI::LoadXaml<FrameworkElement>("Reflections.xaml");
    Ptr<IView> view = Noesis::GUI::CreateView(xaml);
    view->SetFlags(Noesis::RenderFlags_PPAA | Noesis::RenderFlags_LCD);
    view->SetSize(1024, 768);

    Ptr<RenderDevice> device = NoesisApp::GLFactory::CreateDevice();
    view->GetRenderer()->Init(device);
    */

}
