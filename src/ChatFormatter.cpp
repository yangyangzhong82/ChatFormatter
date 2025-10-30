#include "ll/api/memory/Hook.h"
#include "ll/api/service/Bedrock.h"
#include "mc/network/packet/TextPacket.h"
#include "mc/world/level/Level.h"
#include "Config/ConfigManager.h" 
#include <string> 

#include "PA/PlaceholderAPI.h"


// 辅助函数：替换字符串中的所有占位符
std::string
replacePlaceholders(std::string format, const std::string& author, const std::string& message, Player* player) {
    // 替换 {author}
    size_t pos = format.find("{author}");
    while (pos != std::string::npos) {
        format.replace(pos, 8, author);
        pos = format.find("{author}", pos + author.length());
    }

    // 替换 {message}
    pos = format.find("{message}");
    while (pos != std::string::npos) {
        format.replace(pos, 9, message);
        pos = format.find("{message}", pos + message.length());
    }

    // 使用 PlaceholderAPI 替换其他占位符
    if (player) {
        PA::PlayerContext ctx;
        ctx.player = player;
        return PA::PA_GetPlaceholderService()->replace(format, &ctx);
    } else {
        return PA::PA_GetPlaceholderService()->replaceServer(format);
    }
}
LL_AUTO_TYPE_INSTANCE_HOOK(
    Hook2,
    HookPriority::Normal,
    TextPacket,
    &TextPacket::$write,
    void,
    ::BinaryStream& stream
) {
    if (this->mType == TextPacketType::Chat) {
        auto& config = ConfigManager::getInstance().get(); 

        if (!config.enablePlugin) {
            origin(stream);
            return;
        }
        auto        player              = ll::service::getLevel()->getPlayer(this->mAuthor);
        std::string originalAuthor = this->mAuthor;
        std::string originalMessage = this->mMessage;

        this->mMessage = replacePlaceholders(config.chatMessageFormat, originalAuthor, originalMessage, player);
        this->mAuthor.clear(); 

        origin(stream);
        return;
    }
    origin(stream);
    }
