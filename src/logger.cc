#include "logger.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/hourly_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <iostream>
using namespace Napi;
Napi::Object Logger::Initialize(Napi::Env env, Napi::Object exports)
{
    Napi::Object ELevel = Napi::Object::New(env);
    ELevel.Set(Napi::String::New(env, "TRACE"), SPDLOG_LEVEL_TRACE);
    ELevel.Set(Napi::String::New(env, "DEBUG"), SPDLOG_LEVEL_DEBUG);
    ELevel.Set(Napi::String::New(env, "INFO"), SPDLOG_LEVEL_INFO);
    ELevel.Set(Napi::String::New(env, "WARN"), SPDLOG_LEVEL_WARN);
    ELevel.Set(Napi::String::New(env, "ERROR"), SPDLOG_LEVEL_ERROR);
    ELevel.Set(Napi::String::New(env, "CRITICAL"), SPDLOG_LEVEL_CRITICAL);
    ELevel.Set(Napi::String::New(env, "OFF"), SPDLOG_LEVEL_OFF);

    Napi::Object EType = Napi::Object::New(env);
    EType.Set(Napi::String::New(env, "STDOUT"),
              static_cast<int>(ESink::STDOUT));
    EType.Set(Napi::String::New(env, "ASYNC"),
              static_cast<int>(ESink::ASYNC));
    EType.Set(Napi::String::New(env, "ROTATING"),
              static_cast<int>(ESink::ROTATING));
    EType.Set(Napi::String::New(env, "DAILY"),
              static_cast<int>(ESink::DAILY));
    EType.Set(Napi::String::New(env, "HOURLY"),
              static_cast<int>(ESink::HOURLY));
    Napi::Function constructor = DefineClass(env, "Logger", {
        StaticMethod<&Logger::CriticalStatic>("critical"),
        StaticMethod<&Logger::ErrorStatic>("error"),
        StaticMethod<&Logger::WarnStatic>("warn"),
        StaticMethod<&Logger::InfoStatic>("info"),
        StaticMethod<&Logger::DebugStatic>("debug"),
        StaticMethod<&Logger::TraceStatic>("trace"),
        StaticMethod<&Logger::FlushEvery>("flushEvery"),

        StaticAccessor<&Logger::GetLevelStatic,
                       &Logger::SetLevelStatic>("level"),
        StaticAccessor<nullptr, &Logger::SetPatternStatic>("pattern"),

        StaticValue("ELevel", ELevel),
        StaticValue("EType", EType),

        InstanceMethod<&Logger::Critical>("__critical"),
        InstanceMethod<&Logger::Error>("__error"),
        InstanceMethod<&Logger::Warn>("__warn"),
        InstanceMethod<&Logger::Info>("__info"),
        InstanceMethod<&Logger::Debug>("__debug"),
        InstanceMethod<&Logger::Trace>("__trace"),
        InstanceMethod<&Logger::Flush>("flush"),
        InstanceMethod<&Logger::FlushOn>("flushOn"),

        InstanceAccessor<&Logger::GetLevel, &Logger::SetLevel>("level"),
        InstanceAccessor<nullptr, &Logger::SetPattern>("pattern")
    });
    exports.Set("Logger", constructor);

    return exports;
}

Logger::Logger(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Logger>(info)
{
    if (info.Length() < 3 ||
        !info[0].IsString() ||
        !info[1].IsNumber() ||
        !info[2].IsString())
    {
        NAPI_THROW(Napi::TypeError::New(
            info.Env(), "parameter logname, logtype, logpath expected"));
    }

    std::string name = info[0].As<Napi::String>().Utf8Value();
    ESink type = static_cast<ESink>(
        info[1].As<Napi::Number>().Int32Value());

    Napi::String path = info[2].As<Napi::String>();

    switch (type)
    {
    case ESink::ASYNC:
        _logger = spdlog::basic_logger_mt<spdlog::async_factory>(
            name, path.Utf8Value());
        break;
    case ESink::ROTATING:
        _logger = spdlog::rotating_logger_mt(
            name, path.Utf8Value(), 1048576, 100);
        break;
    case ESink::DAILY:
        _logger = spdlog::daily_logger_mt(
            name, path.Utf8Value());
        break;
    case ESink::HOURLY:
        _logger = std::make_shared<spdlog::logger>(
            name, std::make_shared<
                      spdlog::sinks::hourly_file_sink_mt>(path.Utf8Value()));
        spdlog::register_logger(_logger);
        break;
    case ESink::STDOUT:
    default:
        _logger = spdlog::stdout_color_st(name);
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
    return Napi::Number::New(info.Env(), _logger->level());
}

void Logger::SetLevel(const Napi::CallbackInfo &info, const Napi::Value &value)
{
    if (info.Length() <= 0 || !info[0].IsNumber())
    {
        NAPI_THROW(Napi::TypeError::New(info.Env(), "loglevel expected"));
    }
    auto level = info[0].As<Napi::Number>().Int32Value();
    _logger->set_level(static_cast<spdlog::level::level_enum>(level));
}

void Logger::Flush(const Napi::CallbackInfo &info)
{
    _logger->flush();
}

void Logger::FlushOn(const Napi::CallbackInfo &info)
{
    if (info.Length() <= 0 || !info[0].IsNumber())
    {
        NAPI_THROW(Napi::TypeError::New(info.Env(), "loglevel expected"));
    }
    auto level = info[0].As<Napi::Number>().Int32Value();
    _logger->flush_on(static_cast<spdlog::level::level_enum>(level));
}

void Logger::FlushEvery(const Napi::CallbackInfo &info)
{
    if (info.Length() <= 0 || !info[0].IsNumber())
    {
        NAPI_THROW(Napi::TypeError::New(info.Env(), "seconds expected"));
    }
    int seconds = info[0].As<Napi::Number>().Int32Value();
    spdlog::flush_every(std::chrono::seconds(seconds));
}

void Logger::SetPattern(const Napi::CallbackInfo &info, const Napi::Value &value)
{
    if (info.Length() <= 0 || !info[0].IsString())
    {
        NAPI_THROW(Napi::TypeError::New(info.Env(), "pattern string expected"));
    }
    _logger->set_pattern(info[0].As<Napi::String>().Utf8Value());
}

Napi::Value Logger::GetLevelStatic(const Napi::CallbackInfo &info)
{
    return Napi::Number::New(info.Env(), spdlog::default_logger()->level());
}
void Logger::SetLevelStatic(const Napi::CallbackInfo &info,
                            const Napi::Value &value)
{
    if (info.Length() <= 0 || !info[0].IsNumber())
    {
        NAPI_THROW(Napi::TypeError::New(info.Env(), "loglevel expected"));
    }
    auto level = info[0].As<Napi::Number>().Int32Value();
    spdlog::default_logger()->set_level(
        static_cast<spdlog::level::level_enum>(level));
}
void Logger::SetPatternStatic(const Napi::CallbackInfo &info,
                              const Napi::Value &value)
{
    if (info.Length() <= 0 || !info[0].IsString())
    {
        NAPI_THROW(Napi::TypeError::New(info.Env(), "pattern string expected"));
    }
    spdlog::default_logger()->set_pattern(
        info[0].As<Napi::String>().Utf8Value());
}
