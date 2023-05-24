#include <argparse/argparse.hpp>
#include <csignal>
#include <cstring>
#include <iostream>
#include <thread>

#include "CommandLine.hpp"
#include "Server.hpp"
#include "logging/Logger.hpp"
#include "options.hpp"
#include "protocol/ServerPackets.hpp"
#include "world_storage/Persistence.hpp"

#if GUI_UNAVAILABLE == 0
#include "interface/InterfaceContainer.hpp"
#endif

auto initArgs(int argc, const char *const argv[])
{
    auto program = configuration::ConfigHandler(PROGRAM_NAME, PROGRAM_VERSION);

    // clang-format off
    program.add("nogui")
        .help("prevents the GUI from displaying")
        .valueFromArgument("--nogui")
        .valueFromEnvironmentVariable("CBSRV_NOGUI")
        .possibleValues(true, false)
        .implicit();

    program.add("ip")
        .help("sets the ip on which the server will listen")
        .valueFromConfig("network", "ip")
        .valueFromEnvironmentVariable("CBSRV_IP")
        .defaultValue("0.0.0.0")
        .required();

    program.add("port")
        .help("sets the port on which the server will listen")
        .valueFromConfig("network", "port")
        .valueFromEnvironmentVariable("CBSRV_PORT")
        .defaultValue(25565)
        .required();

    program.add("max-players")
        .help("sets the maximum number of players")
        .valueFromConfig("general", "max_players")
        .valueFromEnvironmentVariable("CBSRV_MAX_PLAYERS")
        .defaultValue(20)
        .required();

    program.add("motd")
        .help("sets the message of the day")
        .valueFromConfig("general", "motd")
        .valueFromEnvironmentVariable("CBSRV_MOTD")
        .defaultValue("A Cubic Server")
        .required();

    // program.add("world")
    //     .help("sets the world to load")
    //     .valueFromConfig("general", "world")
    //     .valueFromEnvironmentVariable("CBSRV_WORLD")
    //     .defaultValue("world");

    program.add("whitelist-enabled")
        .help("enables the whitelist")
        .valueFromConfig("general", "whitelist-enabled")
        .valueFromEnvironmentVariable("CBSRV_WHITELIST_ENABLED")
        .possibleValues(true, false)
        .defaultValue(false)
        .required();

    program.add("enforce-whitelist")
        .help("enforces the whitelist")
        .valueFromConfig("general", "enforce-whitelist")
        .valueFromEnvironmentVariable("CBSRV_ENFORCE_WHITELIST")
        .possibleValues(true, false)
        .defaultValue(false)
        .required();

    program.add("enable-generation")
        .help("Enables chunk generation")
        .valueFromConfig("general", "enable-generation")
        .valueFromEnvironmentVariable("CBSRV_ENABLE_GENERATION")
        .valueFromArgument("--enable-generation")
        .possibleValues(true, false)
        .defaultValue(true);

    program.add("level-name")
        .help("Name of the world")
        .valueFromConfig("general", "level-name")
        .valueFromEnvironmentVariable("CBSRV_LEVEL_NAME")
        .valueFromArgument("--level-name")
        .defaultValue("world");

    program.add("num-gen-thread")
        .help("Number of threads allocated to chunk generation")
        .valueFromConfig("general", "num-gen-thread")
        .valueFromEnvironmentVariable("CBSRV_NUM_GEN_THREAD")
        .valueFromArgument("--num-gen-thread")
        .defaultValue(4);

    program.add("level-type")
        .help("World type to generate")
        .valueFromConfig("general", "level-type")
        .valueFromEnvironmentVariable("CBSRV_LEVEL_TYPE")
        .valueFromArgument("--level-type")
        .possibleValues("flat", "default", "void")
        .defaultValue("default");
    // clang-format on

    try {
        program.load("./config.yml");
    } catch (const configuration::BadFile &) {
        if (std::filesystem::exists("./config.yml")) {
            LERROR("Failed to open config file, check permissions");
            std::exit(1);
        }
        LWARN("No config file found, creating one");
        program.save("./config.yml");
    }

    try {
        program.parse(argc, argv);
    } catch (const std::runtime_error &err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    return program;
}

void signalHandler(int sig)
{
    if (sig != SIGINT && sig != SIGTERM)
        return;
    Server::getInstance()->stop();
}

int main(int argc, char *argv[])
{
    auto program = initArgs(argc, argv);

    auto srv = Server::getInstance();

#if GUI_UNAVAILABLE == 0
    InterfaceContainer interfaceContainer;
#endif

    CommandLine cmd;

    auto logger = logging::Logger::getInstance();
    logger->unsetDisplaySpecificationLevelInConsole(logging::LogLevel::DEBUG);

    std::signal(SIGTERM, signalHandler);
    std::signal(SIGINT, signalHandler);
    std::signal(SIGPIPE, SIG_IGN);

#if GUI_UNAVAILABLE == 0
    if (program["nogui"] == false)
        interfaceContainer.launch(argc, argv);
#endif

    // This should be inside the server
    cmd.launch();

    srv->launch(program);

    cmd.stop();

#if GUI_UNAVAILABLE == 0
    interfaceContainer.stop();
#endif
    return 0;
}
