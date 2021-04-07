#include "logger.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <iostream>
using namespace Napi;
Napi::Object Logger::Initialize(Napi::Env env, Napi::Object exports)
{
    Napi::Object LOGLEVEL = Napi::Object::New(env);
    LOGLEVEL.Set(Napi::String::New(env, "TRACE"), SPDLOG_LEVEL_TRACE);
    LOGLEVEL.Set(Napi::String::New(env, "DEBUG"), SPDLOG_LEVEL_DEBUG);
    LOGLEVEL.Set(Napi::String::New(env, "INFO"), SPDLOG_LEVEL_INFO);
    LOGLEVEL.Set(Napi::String::New(env, "WARN"), SPDLOG_LEVEL_WARN);
    LOGLEVEL.Set(Napi::String::New(env, "ERROR"), SPDLOG_LEVEL_ERROR);
    LOGLEVEL.Set(Napi::String::New(env, "CRITICAL"), SPDLOG_LEVEL_CRITICAL);
    LOGLEVEL.Set(Napi::String::New(env, "OFF"), SPDLOG_LEVEL_OFF);

    Napi::Object LOGTYPE = Napi::Object::New(env);
    LOGTYPE.Set(Napi::String::New(env, "STDOUT"),
                static_cast<int>(SINKTYPE::STDOUT));
    LOGTYPE.Set(Napi::String::New(env, "ASYNC"),
                static_cast<int>(SINKTYPE::ASYNC));
    LOGTYPE.Set(Napi::String::New(env, "ROTATING"),
                static_cast<int>(SINKTYPE::ROTATING));
    LOGTYPE.Set(Napi::String::New(env, "DAILY"),
                static_cast<int>(SINKTYPE::DAILY));
    LOGTYPE.Set(Napi::String::New(env, "HOURLY"),
                static_cast<int>(SINKTYPE::HOURLY));
    Napi::Function constructor = DefineClass(env, "Logger", {
        InstanceMethod<&Logger::Critical>("critical"),
        InstanceMethod<&Logger::Error>("error"),
        InstanceMethod<&Logger::Warn>("warn"),
        InstanceMethod<&Logger::Info>("info"),
        InstanceMethod<&Logger::Debug>("debug"),
        InstanceMethod<&Logger::Trace>("trace"),
        InstanceMethod<&Logger::Flush>("flush"),
        InstanceMethod<&Logger::Drop>("drop"),
        InstanceMethod<&Logger::ClearFormatters>("clearFormatters"),
    
        StaticValue("LEVEL", LOGLEVEL),
        StaticValue("TYPE", LOGTYPE),
        StaticMethod<&Logger::CriticalStatic>("critical"),
        StaticMethod<&Logger::ErrorStatic>("error"),
        StaticMethod<&Logger::WarnStatic>("warn"),
        StaticMethod<&Logger::InfoStatic>("info"),
        StaticMethod<&Logger::DebugStatic>("debug"),
        StaticMethod<&Logger::TraceStatic>("trace"),

        InstanceAccessor<&Logger::GetLevel, &Logger::SetLevel>("level"),
        InstanceAccessor<nullptr, &Logger::SetPattern>("pattern")
    });
    exports.Set("Logger", constructor);
    return exports;
}

Logger::Logger(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Logger>(info)
{
    SINKTYPE type = static_cast<SINKTYPE>(
        info[0].As<Napi::Number>().Int32Value());
    Napi::String path = info[1].As<Napi::String>();
    switch (type)
    {
    case SINKTYPE::STDOUT:
        _logger = spdlog::stdout_color_st("SINKTYPE::STDOUT");
        break;
    case SINKTYPE::ASYNC:
        _logger = spdlog::basic_logger_mt<spdlog::async_factory>(
            "SINKTYPE::ASYNC", path.Utf8Value());
        break;
    case SINKTYPE::ROTATING:
        // _logger = spdlog::rotating_logger_mt(
        //     "SINKTYPE::ROTATING", path.Utf8Value(), maxFileSize, maxFiles);
        break;
    case SINKTYPE::DAILY:
        _logger = spdlog::daily_logger_mt("SINKTYPE::DAILY", path.Utf8Value());
        break;
    case SINKTYPE::HOURLY:
        break;
    default:
        break;
    }
}

Logger::~Logger()
{
    if (_logger == nullptr)
    {
        return;
    }

    try
    {
        spdlog::drop(_logger->name());
    }
    catch (...)
    {
        // noop
    }

    _logger = nullptr;
}

Napi::Value Logger::CriticalStatic(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() <= 0 || !info[0].IsString())
    {
        NAPI_THROW(Napi::TypeError::New(env, "string expected"));
    }
    spdlog::critical(info[0].As<Napi::String>().Utf8Value());
    return Napi::Number::New(env, 0);
}

Napi::Value Logger::ErrorStatic(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() <= 0 || !info[0].IsString())
    {
        NAPI_THROW(Napi::TypeError::New(env, "string expected"));
    }
    spdlog::error(info[0].As<Napi::String>().Utf8Value());
    return Napi::Number::New(env, 0);
}

Napi::Value Logger::WarnStatic(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() <= 0 || !info[0].IsString())
    {
        NAPI_THROW(Napi::TypeError::New(env, "string expected"));
    }
    spdlog::warn(info[0].As<Napi::String>().Utf8Value());
    return Napi::Number::New(env, 0);
}

Napi::Value Logger::InfoStatic(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() <= 0 || !info[0].IsString())
    {
        NAPI_THROW(Napi::TypeError::New(env, "string expected"));
    }
    spdlog::info(info[0].As<Napi::String>().Utf8Value());
    return Napi::Number::New(env, 0);
}

Napi::Value Logger::DebugStatic(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() <= 0 || !info[0].IsString())
    {
        NAPI_THROW(Napi::TypeError::New(env, "string expected"));
    }
    spdlog::debug(info[0].As<Napi::String>().Utf8Value());
    return Napi::Number::New(env, 0);
}

Napi::Value Logger::TraceStatic(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() <= 0 || !info[0].IsString())
    {
        NAPI_THROW(Napi::TypeError::New(env, "string expected"));
    }
    spdlog::trace(info[0].As<Napi::String>().Utf8Value());
    return Napi::Number::New(env, 0);
}

Napi::Value Logger::Critical(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() <= 0 || !info[0].IsString())
    {
        NAPI_THROW(Napi::TypeError::New(env, "string expected"));
    }
    _logger->critical(info[0].As<Napi::String>().Utf8Value());
    return Napi::Number::New(env, 0);
}
Napi::Value Logger::Error(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() <= 0 || !info[0].IsString())
    {
        NAPI_THROW(Napi::TypeError::New(env, "string expected"));
    }
    _logger->error(info[0].As<Napi::String>().Utf8Value());
    return Napi::Number::New(env, 0);
}

Napi::Value Logger::Warn(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() <= 0 || !info[0].IsString())
    {
        NAPI_THROW(Napi::TypeError::New(env, "string expected"));
    }
    _logger->warn(info[0].As<Napi::String>().Utf8Value());
    return Napi::Number::New(env, 0);
}

Napi::Value Logger::Info(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() <= 0 || !info[0].IsString())
    {
        NAPI_THROW(Napi::TypeError::New(env, "string expected"));
    }
    _logger->info(info[0].As<Napi::String>().Utf8Value());
    return Napi::Number::New(env, 0);
}

Napi::Value Logger::Debug(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() <= 0 || !info[0].IsString())
    {
        NAPI_THROW(Napi::TypeError::New(env, "string expected"));
    }
    _logger->debug(info[0].As<Napi::String>().Utf8Value());
    return Napi::Number::New(env, 0);
}

Napi::Value Logger::Trace(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() <= 0 || !info[0].IsString())
    {
        NAPI_THROW(Napi::TypeError::New(env, "string expected"));
    }
    _logger->trace(info[0].As<Napi::String>().Utf8Value());
    return Napi::Number::New(env, 0);
}

Napi::Value Logger::GetLevel(const Napi::CallbackInfo &info)
{
    Napi::String v = info[0].As<Napi::String>();
    std::cout << v << std::endl;
    Napi::Env env = info.Env();
    return Napi::Number::New(env, 0);
}

void Logger::SetLevel(const Napi::CallbackInfo &info, const Napi::Value &value)
{
}

Napi::Value Logger::Flush(const Napi::CallbackInfo &info)
{
    Napi::String v = info[0].As<Napi::String>();
    std::cout << v << std::endl;
    Napi::Env env = info.Env();
    return Napi::Number::New(env, 0);
}

Napi::Value Logger::Drop(const Napi::CallbackInfo &info)
{
    Napi::String v = info[0].As<Napi::String>();
    std::cout << v << std::endl;
    Napi::Env env = info.Env();
    return Napi::Number::New(env, 0);
}

void Logger::SetPattern(const Napi::CallbackInfo &info, const Napi::Value &value)
{
}

Napi::Value Logger::ClearFormatters(const Napi::CallbackInfo &info)
{
    Napi::String v = info[0].As<Napi::String>();
    std::cout << v << std::endl;
    Napi::Env env = info.Env();
    return Napi::Number::New(env, 0);
}
