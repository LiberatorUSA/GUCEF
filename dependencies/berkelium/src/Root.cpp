/*  Berkelium Implementation
 *  Root.cpp
 *
 *  Copyright (c) 2009, Patrick Reiter Horn
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name of Sirikata nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// Berkelium headers
#include "berkelium/Berkelium.hpp"
#include "Root.hpp"
#include "MemoryRenderViewHost.hpp"

// Chromium headers
#include "base/message_loop.h"
#include "base/at_exit.h"
#include "base/path_service.h"
#include "base/threading/thread.h"
#include "base/command_line.h"
#include "base/file_util.h"
#include "base/scoped_temp_dir.h"
#include "base/i18n/icu_util.h"
#include "base/string_number_conversions.h"
#include "net/base/cookie_monster.h"
#include "chrome/common/chrome_constants.h"
#include "chrome/common/chrome_paths.h"
#include "chrome/common/chrome_switches.h"
#include "chrome/common/url_constants.h"
#include "chrome/browser/metrics/histogram_synchronizer.h"
#include "chrome/browser/prefs/browser_prefs.h"
#include "chrome/browser/browser_process_impl.h"
#include "chrome/browser/process_singleton.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "content/browser/plugin_service.h"
#include "content/browser/renderer_host/resource_dispatcher_host.h"
#include "chrome/browser/renderer_host/browser_render_process_host.h"
#include "content/browser/browser_thread.h"
#include "chrome/browser/browser_url_handler.h"
#include "chrome/browser/net/predictor_api.h"
#include "chrome/common/hi_res_timer_manager.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/ui_base_paths.h"
#include "ui/base/system_monitor/system_monitor.h"
#include "chrome/browser/prefs/pref_service.h"
#include "chrome/common/pref_names.h"
#include "chrome/common/logging_chrome.h"
#include "base/logging.h"
#include <signal.h>
#include <sys/stat.h>
#if defined(OS_MACOSX)
#include "base/mac/mac_util.h"
#include "chrome/common/chrome_paths_internal.h"
#include "chrome/app/breakpad_mac.h"
#include "third_party/WebKit/Source/WebKit/mac/WebCoreSupport/WebSystemInterface.h"
#endif
#include "chrome/common/sandbox_init_wrapper.h"
#if defined(OS_WIN)
#include <direct.h>
#include "base/win/win_util.h"
#include "tools/memory_watcher/memory_watcher.h"
#include "sandbox/src/broker_services.h"
#include "chrome/common/sandbox_policy.h"
#endif
#if defined(OS_LINUX)
#include <gdk/gdk.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "content/browser/renderer_host/render_sandbox_host_linux.h"
#include "content/browser/zygote_host_linux.h"
#endif
#if defined(USE_NSS)
#include "base/nss_util.h"
#endif
#include <iostream>

#include "chrome/browser/ui/webui/chrome_url_data_manager_backend.h"
#include "chrome/common/net/url_request_context_getter.h"	
#include "base/utf_string_conversions.h"

#if !defined(OS_WIN)
extern "C"
void handleINT(int sig) {
    FilePath homedirpath;
    PathService::Get(chrome::DIR_USER_DATA,&homedirpath);
    FilePath child = homedirpath.AppendASCII("SingletonLock");
    unlink(child.value().c_str());
    exit(-sig);
}
#else
namespace sandbox {
    // Not exported.
    HANDLE g_shared_section = NULL;
    bool s_is_broker = true;
}
#endif

BERKELIUM_SINGLETON_INSTANCE(Berkelium::Root);
namespace Berkelium {

#if defined(OS_POSIX) && !defined(OS_MACOSX)
static void GLibLogHandler(const gchar* log_domain,
                           GLogLevelFlags log_level,
                           const gchar* message,
                           gpointer userdata) {
  if (!log_domain)
    log_domain = "<unknown>";
  if (!message)
    message = "<no message>";

  if (strstr(message, "Loading IM context type") ||
      strstr(message, "wrong ELF class: ELFCLASS64")) {
    // http://crbug.com/9643
    // Until we have a real 64-bit build or all of these 32-bit package issues
    // are sorted out, don't fatal on ELF 32/64-bit mismatch warnings and don't
    // spam the user with more than one of them.
    static bool alerted = false;
    if (!alerted) {
      LOG(ERROR) << "Bug 9643: " << log_domain << ": " << message;
      alerted = true;
    }
  } else if (strstr(message, "gtk_widget_size_allocate(): attempt to "
                             "allocate widget with width") &&
             !GTK_CHECK_VERSION(2, 16, 1)) {
    // This warning only occurs in obsolete versions of GTK and is harmless.
    // http://crbug.com/11133
  } else if (strstr(message, "Theme file for default has no") ||
             strstr(message, "Theme directory") ||
             strstr(message, "theme pixmap")) {
    LOG(ERROR) << "GTK theme error: " << message;
  } else if (strstr(message, "gtk_drag_dest_leave: assertion")) {
    LOG(ERROR) << "Drag destination deleted: http://crbug.com/18557";
  } else {
#ifdef NDEBUG
    LOG(ERROR) << log_domain << ": " << message;
#else
    LOG(FATAL) << log_domain << ": " << message;
#endif
  }
}

void Root::SetUpGLibLogHandler() {
  // Register GLib-handled assertions to go through our logging system.
  const char* kLogDomains[] = { NULL, "Gtk", "Gdk", "GLib", "GLib-GObject" };
  for (size_t i = 0; i < arraysize(kLogDomains); i++) {
    g_log_set_handler(kLogDomains[i],
                      static_cast<GLogLevelFlags>(G_LOG_FLAG_RECURSION |
                                                  G_LOG_FLAG_FATAL |
                                                  G_LOG_LEVEL_ERROR |
                                                  G_LOG_LEVEL_CRITICAL |
                                                  G_LOG_LEVEL_WARNING),
                      &GLibLogHandler,
                      NULL);
  }
}
#else  // defined(OS_POSIX) && !defined(OS_MACOSX)
void Root::SetUpGLibLogHandler() {
}
#endif  // defined(OS_POSIX) && !defined(OS_MACOSX)


Root::Root () {
}

bool Root::init(FileString homeDirectory, FileString subprocessDirectory, unsigned int extra_argc, const char* extra_argv[] ) {

    new base::AtExitManager();

    FilePath subprocess;

    // convert extra arguments in a more useful form
    std::vector< std::string > extra_args;
    if( extra_argc > 0 )
    {
        for( unsigned int arg_idx = 0; arg_idx < extra_argc; ++arg_idx )
        {
            const char* raw_arg = extra_argv[ arg_idx ];
            assert( raw_arg );
            extra_args.push_back( raw_arg );
        }
    }


    {
// From <base/command_line.h>:
  // Initialize the current process CommandLine singleton.  On Windows,
  // ignores its arguments (we instead parse GetCommandLineW()
  // directly) because we don't trust the CRT's parsing of the command
  // line, but it still must be called to set up the command line.
// This means that on windows, we have to call ::Init with whatever we feel
// like (since this is the only way to create the static CommandLine instance),
// and then we have manually call ParseFromString.
// (InitFromArgv does not exist on OS_WIN!)
#if defined(OS_WIN)
    FilePath module_dir;
    if (subprocessDirectory.size()) {
        module_dir = FilePath(subprocessDirectory.get<FilePath::StringType>());
    } else {
        PathService::Get(base::DIR_MODULE, &module_dir);
    }
#ifdef _DEBUG
    subprocess = module_dir.Append(L"berkelium_d.exe");
#else
    subprocess = module_dir.Append(L"berkelium.exe");
#endif

	std::wstring subprocess_str = L"berkelium --enable-webgl --browser-subprocess-path=";
	subprocess_str += L"\"";
	subprocess_str += subprocess.value();
	subprocess_str += L"\"";

    // add extra arguments if any
    if( !extra_args.empty() )
    {
        for( unsigned int arg_idx = 0, arg_count = extra_args.size(); arg_idx < arg_count; ++arg_idx )
        {
            const std::string& str_arg = extra_args[ arg_idx ];
            std::wstring wstr_arg( str_arg.begin(), str_arg.end() );

            subprocess_str += L" " + wstr_arg;
        }
    }

    //std::wcout << "Berkelium subprocess_str : " << subprocess_str;

    CommandLine::Init(0, NULL);
    CommandLine::ForCurrentProcess()->ParseFromString(subprocess_str);
#elif defined(OS_MACOSX)
    FilePath app_contents;
    PathService::Get(base::FILE_EXE, &app_contents);
    FilePath module_dir;
    if (subprocessDirectory.size()) {
        module_dir = FilePath(subprocessDirectory.get<FilePath::StringType>());
    } else {
        module_dir = app_contents.DirName();
    }
    subprocess = module_dir.Append("berkelium");
    std::string subprocess_str = "--browser-subprocess-path=";
    subprocess_str += subprocess.value();

    std::vector<const char*> argv;
    argv.push_back( "berkelium" );
    argv.push_back( subprocess_str.c_str() );
    argv.push_back( "--enable-webgl" );

    for( std::vector<std::string>::iterator it = extra_args.begin(); it != extra_args.end(); ++it )
    {
        argv.push_back( it->c_str() );
    }

    //for( std::vector<const char*>::iterator it = argv.begin(); it != argv.end(); ++it )
    //    std::cout << "Berkelium arg : " << *it;

    CommandLine::Init( argv.size(), &argv[0] );
#elif defined(OS_POSIX)
    FilePath module_file;
    PathService::Get(base::FILE_EXE, &module_file);
    FilePath module_dir;
    if (subprocessDirectory.size()) {
        module_dir = FilePath(subprocessDirectory.get<FilePath::StringType>());
    } else {
        module_dir = module_file.DirName();
    }
    subprocess = module_dir.Append("berkelium");
    std::string subprocess_str = "--browser-subprocess-path=";
    subprocess_str += subprocess.value();
    std::vector<const char*> argv;
    argv.push_back( "berkelium" );
    argv.push_back( subprocess_str.c_str() );
    argv.push_back( "--enable-webgl" );

    for( std::vector<std::string>::iterator it = extra_args.begin(); it != extra_args.end(); ++it )
    {
        argv.push_back( it->c_str() );
    }
    //for( std::vector<const char*>::iterator it = argv.begin(); it != argv.end(); ++it )
    //    std::cout << "Berkelium arg : " << *it;

    CommandLine::Init( argv.size(), &argv[0]) ;
#endif
    }

    PathService::Override(base::FILE_EXE, subprocess);

#if !defined(OS_WIN)
/// Temporary SingletonLock fix:
// Do not do this for child processes--they should only be initialized.
// Children should never delete the lock.
    if (signal(SIGINT, handleINT) == SIG_IGN) {
        signal(SIGINT, SIG_IGN);
    }
    if (signal(SIGHUP, handleINT) == SIG_IGN) {
        signal(SIGHUP, SIG_IGN);
    }
    if (signal(SIGTERM, handleINT) == SIG_IGN) {
        signal(SIGTERM, SIG_IGN);
    }
#endif

    chrome::RegisterPathProvider();
    ui::RegisterPathProvider();
    FilePath homedirpath;

    if (homeDirectory.data() && homeDirectory.length()) {
        FilePath homeDirectoryPath(homeDirectory.get<FilePath::StringType>());
        PathService::Override(chrome::DIR_USER_DATA, homeDirectoryPath);
        PathService::Override(chrome::DIR_LOGS, homeDirectoryPath);
#if defined(OS_POSIX)
        PathService::Override(base::DIR_CACHE, homeDirectoryPath);
#endif
    } else {
        mTempProfileDir.reset(new ScopedTempDir());
        if (mTempProfileDir->CreateUniqueTempDir()) {
            PathService::Override(chrome::DIR_USER_DATA, mTempProfileDir->path());
            PathService::Override(chrome::DIR_LOGS, mTempProfileDir->path());
#if defined(OS_POSIX)
            PathService::Override(base::DIR_CACHE, mTempProfileDir->path());
#endif
        }
    }
    PathService::Get(chrome::DIR_USER_DATA,&homedirpath);
    bool SINGLE_PROCESS=false;
#if defined(OS_MACOSX)
    base::mac::SetOverrideAppBundlePath(chrome::GetFrameworkBundlePath());
    if (SINGLE_PROCESS) {
        InitWebCoreSystemInterface();
        //CGColorSpaceCreateDeviceRGB();
    }
#endif  // OS_MACOSX
    if (SINGLE_PROCESS) {
        RenderProcessHost::set_run_renderer_in_process(true);
    }
    mMessageLoop.reset(new MessageLoop(MessageLoop::TYPE_UI));
    mSysMon.reset(new ui::SystemMonitor);
    mTimerMgr.reset(new HighResolutionTimerManager);
    mUIThread.reset(new BrowserThread(BrowserThread::UI, mMessageLoop.get()));
    mErrorHandler = 0;

#if defined(OS_LINUX)
    if (!g_thread_supported ()) // the client application might have already setup glib
	{
		g_thread_init(NULL);
		// Glib type system initialization. Needed at least for gconf,
		// used in net/proxy/proxy_config_service_linux.cc. Most likely
		// this is superfluous as gtk_init() ought to do this. It's
		// definitely harmless, so retained as a reminder of this
		// requirement for gconf.
		g_type_init();
		// gtk_init() can change |argc| and |argv|.
		char argv0data[] = "[Berkelium]";
		char *argv0 = argv0data;
		char **argv = &argv0;
		int argc = 1;
		gtk_init(&argc, &argv);
    }
    SetUpGLibLogHandler();
#endif

    mProcessSingleton.reset(new ProcessSingleton(homedirpath));
    BrowserProcessImpl *browser_process;
    browser_process=new BrowserProcessImpl(*CommandLine::ForCurrentProcess());
    browser_process->local_state()->RegisterStringPref(prefs::kApplicationLocale, "");
    browser_process->local_state()->RegisterBooleanPref(prefs::kMetricsReportingEnabled, false);
    assert(g_browser_process == browser_process);

#ifdef OS_WIN
    logging::InitLogging(
        L"chrome.log",
        logging::LOG_NONE,
        logging::DONT_LOCK_LOG_FILE,
        logging::DELETE_OLD_LOG_FILE,
        logging::DISABLE_DCHECK_FOR_NON_OFFICIAL_RELEASE_BUILDS
        );
#else
    logging::InitLogging(
        "chrome.log",
        logging::LOG_ONLY_TO_SYSTEM_DEBUG_LOG,
        logging::DONT_LOCK_LOG_FILE,
        logging::DELETE_OLD_LOG_FILE,
        logging::DISABLE_DCHECK_FOR_NON_OFFICIAL_RELEASE_BUILDS
        );
#endif
    logging::InitChromeLogging(
        *CommandLine::ForCurrentProcess(),
        logging::DELETE_OLD_LOG_FILE);
    //APPEND_TO_OLD_LOG_FILE

  chrome::RegisterChromeSchemes(); // Required for "chrome-extension://" in InitExtensions
#if defined(OS_LINUX)
    const char* sandbox_binary = NULL;
    struct stat st;

    // In Chromium branded builds, developers can set an environment variable to
    // use the development sandbox. See
    // http://code.google.com/p/chromium/wiki/LinuxSUIDSandboxDevelopment
    if (stat("/proc/self/exe", &st) == 0 && st.st_uid == getuid())
      sandbox_binary = getenv("CHROME_DEVEL_SANDBOX");

    std::string sandbox_cmd;
    if (sandbox_binary && !CommandLine::ForCurrentProcess()->HasSwitch(switches::kNoSandbox))
      sandbox_cmd = sandbox_binary;

    // Tickle the sandbox host and zygote host so they fork now.
    RenderSandboxHostLinux* shost = RenderSandboxHostLinux::GetInstance();
    shost->Init(sandbox_cmd);
    ZygoteHost* zhost = ZygoteHost::GetInstance();
    zhost->Init(sandbox_cmd);

    // We want to be sure to init NSPR on the main thread.
    base::EnsureNSPRInit();
#endif  // defined(OS_LINUX)

  SandboxInitWrapper sandbox_wrapper;
#if defined(OS_WIN)
  // Get the interface pointer to the BrokerServices or TargetServices,
  // depending who we are.
  sandbox::SandboxInterfaceInfo sandbox_info = {0};
  sandbox_info.broker_services = sandbox::BrokerServicesBase::GetInstance();
  sandbox::InitBrokerServices(sandbox_info.broker_services);
  if (!CommandLine::ForCurrentProcess()->HasSwitch(switches::kNoSandbox)) {
    bool use_winsta = !CommandLine::ForCurrentProcess()->HasSwitch(
        switches::kDisableAltWinstation);
    // Precreate the desktop and window station used by the renderers.
    sandbox::TargetPolicy* policy = sandbox_info.broker_services->CreatePolicy();
    sandbox::ResultCode result = policy->CreateAlternateDesktop(use_winsta);
    CHECK(sandbox::SBOX_ERROR_FAILED_TO_SWITCH_BACK_WINSTATION != result);
    policy->Release();
  }
  sandbox_wrapper.SetServices(&sandbox_info);
#endif
  sandbox_wrapper.InitializeSandbox(*CommandLine::ForCurrentProcess(), "");

  bool icu_result = icu_util::Initialize();
  CHECK(icu_result);

    mRenderViewHostFactory.reset(new MemoryRenderViewHostFactory);

//    mNotificationService=new NotificationService();
//    ChildProcess* coreProcess=new ChildProcess;
//    coreProcess->set_main_thread(new ChildThread);
    g_browser_process->SetApplicationLocale("en-US");
    ResourceBundle::InitSharedInstance("en-US");// FIXME: lookup locale
    // We only load the theme dll in the browser process.
    net::CookieMonster::EnableFileScheme();

    browser_process->profile_manager();
    browser_process->db_thread();
    browser_process->file_thread();
    browser_process->process_launcher_thread();
    browser_process->cache_thread();
    browser_process->io_thread();

    // Initialize histogram synchronizer system. This is a singleton and is used
    // for posting tasks via NewRunnableMethod. Its deleted when it goes out of
    // scope. Even though NewRunnableMethod does AddRef and Release, the object
    // will not be deleted after the Task is executed.
    mHistogramSynchronizer= (new HistogramSynchronizer());

    browser::RegisterLocalState(g_browser_process->local_state());

    ProfileManager* profile_manager = browser_process->profile_manager();
    homedirpath = homedirpath.Append(profile_manager->GetCurrentProfileDir());
    {
        //std::cout << "Profile path: " << homedirpath.value() << std::endl;
        FilePath prefs_path (ProfileManager::GetProfilePrefsPath(homedirpath));
        FILE *fp = file_util::OpenFile(prefs_path, "a");
        file_util::CloseFile(fp);
        FilePath history_path (homedirpath);
        history_path = history_path.Append(chrome::kHistoryFilename);
        //std::cout << "  Profile exists: " << ProfileManager::IsProfile(homedirpath) << std::endl;
    }
    mProf = profile_manager->GetProfile(homedirpath, false);
    if (!mProf) {
        mProcessSingleton.reset();
        return false;
    }
    mProf->GetPrefs()->SetBoolean(prefs::kSafeBrowsingEnabled, false);
    mProf->GetPrefs()->RegisterStringPref(prefs::kSafeBrowsingClientKey, "");
    mProf->GetPrefs()->RegisterStringPref(prefs::kSafeBrowsingWrappedKey, "");
    mProf->InitExtensions();

    PrefService* user_prefs = mProf->GetPrefs();
    DCHECK(user_prefs);

//    browser_process->local_state()->SetString(prefs::kApplicationLocale,std::wstring());
    mProcessSingleton->Create();

    mDNSPrefetch.reset(new chrome_browser_net::PredictorInit(
      user_prefs,
      browser_process->local_state(),
      CommandLine::ForCurrentProcess()->HasSwitch(switches::kEnablePreconnect)));

    BrowserURLHandler::InitURLHandlers();

    // From chrome/browser/browser_main.cc
  // Register our global network handler for chrome:// and
  // chrome-extension:// URLs.
  ChromeURLDataManagerBackend::Register();
/*
  RegisterExtensionProtocols();
  RegisterMetadataURLRequestHandler();
  RegisterBlobURLRequestJobFactory();
  RegisterFileSystemURLRequestJobFactory();
*/


    {
        FilePath plugindata = homedirpath.AppendASCII("plugin_");
        if (!file_util::CreateDirectory(plugindata)) {
            return false;
        }
        PluginService::GetInstance()->SetChromePluginDataDir(plugindata);
    }
    PluginService::GetInstance()->LoadChromePlugins(
        g_browser_process->resource_dispatcher_host());

    mDefaultRequestContext=mProf->GetRequestContext();

    if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kRemoteDebuggingPort)) {
        std::string debugging_port_str =
            CommandLine::ForCurrentProcess()->GetSwitchValueASCII(switches::kRemoteDebuggingPort);
        int64 debugging_port = -1;
        bool has_debugging_port = base::StringToInt64(debugging_port_str, &debugging_port);
        if (has_debugging_port && debugging_port > 0 && debugging_port < 65535) {
            devtools_http_handler_ =
                Berkelium::DevToolsHttpProtocolHandler::Start(
                    "127.0.0.1",
                    static_cast<int>(debugging_port),
                    ""
                );
        }
    }

    return true;
}

void Root::runUntilStopped() {
    MessageLoopForUI::current()->MessageLoop::Run();
}

void Root::stopRunning() {
    MessageLoopForUI::current()->Quit();
}

void Root::update() {
    MessageLoopForUI::current()->RunAllPending();
}

void Root::addWindow(WindowImpl* w) {
    mWindows.push_back(w);
}

void Root::removeWindow(WindowImpl* w) {
    for(WindowList::iterator it = mWindows.begin(); it != mWindows.end(); it++) {
        if (*it == w) {
            mWindows.erase(it);
            break;
        }
    }
}

Root::WindowList Root::getWindows() {
    return mWindows;
}

Root::~Root(){
    // Debugger must be cleaned up before IO thread and NotificationService.
    if (devtools_http_handler_.get()) {
        devtools_http_handler_->Stop();
        devtools_http_handler_ = NULL;
    }

    // FIXME: RemoveProfile gone--do we leak profiles?
    //g_browser_process->profile_manager()->RemoveProfile(mProf);

    g_browser_process->EndSession();
    mRenderViewHostFactory.reset();
    mTimerMgr.reset();
    mSysMon.reset();
    mHistogramSynchronizer = NULL;
    mDNSPrefetch.reset();
    mNotificationService.reset();
    delete g_browser_process;
    mUIThread.reset();
    mMessageLoop.reset();

    if (mProcessSingleton.get()) {
        mProcessSingleton->Cleanup();
    }
    mTempProfileDir.reset(); // Delete the profile if necessary.
}

void Root::setCookie (URLString url, WideString cookieString) {
     if (getDefaultRequestContext() )
         if (net::CookieStore * cs = getDefaultRequestContext () -> GetCookieStore ())
             cs -> SetCookie (GURL(url.get<std::string>()), WideToUTF8(cookieString.get<std::wstring>()));
}

}
