#include "ll/api/command/CommandHandle.h"
#include "ll/api/command/CommandRegistrar.h"
#include "mc/server/commands/CommandOrigin.h"
#include "mc/server/commands/CommandOutput.h"
#include "mc/server/commands/CommandPermissionLevel.h"

#include "config/ConfigManager.h"
#include "logger.h"
#include "command.h"

namespace cm {
void ReloadCommand(CommandOrigin const& origin, CommandOutput& output) {
    if (ConfigManager::getInstance().reload()) {
        output.success("配置文件已成功重新加载！");
        cm::logger.info("配置文件已成功重新加载！");
    } else {
        output.error("配置文件重新加载失败！请检查日志获取更多信息。");
        cm::logger.error("配置文件重新加载失败！");
    }
}

void RegisterCommand() {
    using ll::command::CommandHandle;
    using ll::command::CommandRegistrar;
    using ll::command::EmptyParam; 

    auto& registrar = CommandRegistrar::getInstance();
    auto& command = registrar.getOrCreateCommand("cm", "重新加载配置", CommandPermissionLevel::GameDirectors);

    command
        .overload<EmptyParam>() 
        .text("reload")
        .execute([](CommandOrigin const& origin, CommandOutput& output) { 
            ReloadCommand(origin, output);
        });
}
}