#pragma once

#include <map>
#include <string>
#include <vector>


struct Config {
    int                                version             = 1;
    bool                               enablePlugin        = true;
    std::string chatMessageFormat = "<{author}> {message}"; // 新增的聊天消息格式配置，包含占位符
};
