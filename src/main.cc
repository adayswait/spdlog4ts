#define NAPI_CPP_EXCEPTIONS
#include <napi.h>
#include "logger.h"

Napi::Object Initialize(Napi::Env env, Napi::Object exports)
{
    Logger::Initialize(env, exports);
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Initialize);
