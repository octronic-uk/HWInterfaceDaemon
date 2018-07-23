#include <string>
#include <memory>
#include "Daemon.h"
#include <signal.h>
#include <unistd.h>

#include "cxxopts.hpp"
#include "spdlog/spdlog.h"

using namespace HWID;
using namespace std;
using namespace cxxopts;

static auto _log = spdlog::stdout_color_mt("Main");
static shared_ptr<Daemon> _daemon;

// Handle termination signal politely
void terminate(int signum)
{
   _log->info("Oh Shit... One Time!! (SIGTERM)");
   if (_daemon != nullptr)
   {
       _daemon->stop();
   }
}


int main(int argc, const char** argv)
{
    // Handle termination signal politely
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = terminate;
    sigaction(SIGTERM, &action, nullptr);

    // Args
    _log->info("Starting up...");
    Options options("HWInterfaceDaemon", "rPi Hardware/Button Interface Daemon");
    options.add_options()
          ("c,config", "Configuration File", cxxopts::value<string>());

    // Do the thing
    int retval = 1;
    try
    {
        ParseResult result = options.parse(argc, argv);
        OptionValue configResult = result["config"];
        if (configResult.count() > 0)
        {
            string configFilePath = configResult.as<string>();
            _daemon = make_shared<Daemon>(configFilePath);
            shared_ptr<LogListener> logListener = make_shared<LogListener>();
            _daemon->registerListener(logListener.get());
            retval = _daemon->run();
        }
        else
        {
            cout << options.help() << endl;
            retval = 0;
        }

    }
    catch (OptionException ex)
    {
        cout << options.help() << endl;
        _log->error(ex.what());
        retval = 1;
    }

    _log->debug("Exiting Main");
    return retval;
}
