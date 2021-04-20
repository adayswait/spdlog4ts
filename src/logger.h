#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <napi.h>
#include <spdlog/spdlog.h>

enum class ESink
{
    STDOUT,
    ASYNC,
    ROTATING,
    DAILY,
    HOURLY
};

class Logger : public Napi::ObjectWrap<Logger>
{
public:
    explicit Logger(const Napi::CallbackInfo &info);
    static Napi::Object Initialize(Napi::Env env, Napi::Object exports);
    ~Logger();

private:
    static Napi::FunctionReference constructor;

    Napi::Value Critical(const Napi::CallbackInfo &info);
    Napi::Value Error(const Napi::CallbackInfo &info);
    Napi::Value Warn(const Napi::CallbackInfo &info);
    Napi::Value Info(const Napi::CallbackInfo &info);
    Napi::Value Debug(const Napi::CallbackInfo &info);
    Napi::Value Trace(const Napi::CallbackInfo &info);

    static Napi::Value CriticalStatic(const Napi::CallbackInfo &info);
    static Napi::Value ErrorStatic(const Napi::CallbackInfo &info);
    static Napi::Value WarnStatic(const Napi::CallbackInfo &info);
    static Napi::Value InfoStatic(const Napi::CallbackInfo &info);
    static Napi::Value DebugStatic(const Napi::CallbackInfo &info);
    static Napi::Value TraceStatic(const Napi::CallbackInfo &info);
    static Napi::Value GetLevelStatic(const Napi::CallbackInfo &info);
    static void SetLevelStatic(const Napi::CallbackInfo &info,
                               const Napi::Value &value);
    static void SetPatternStatic(const Napi::CallbackInfo &info,
                                 const Napi::Value &value);
    static void FlushEvery(const Napi::CallbackInfo &info);

    Napi::Value GetLevel(const Napi::CallbackInfo &info);
    void SetLevel(const Napi::CallbackInfo &info, const Napi::Value &value);
    void Flush(const Napi::CallbackInfo &info);
    void FlushOn(const Napi::CallbackInfo &info);

    void SetPattern(const Napi::CallbackInfo &info, const Napi::Value &value);

    std::shared_ptr<spdlog::logger> _logger;
};

#endif
