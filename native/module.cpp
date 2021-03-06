#include <napi.h>

#include <chrono>
#include <fstream>
#include <sstream>
#include <map>
#include <thread>

std::ofstream logFile("native.log");
std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
std::string logTime()
{
   std::stringstream ss;
   ss << std::this_thread::get_id();
  return ss.str() + " : " + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count()) + ": ";
}

void javascript_callback(Napi::Env env, Napi::Function callback, Napi::Reference<Napi::Value>* context, int* data)
{
  logFile << logTime()  << "javascript_callback env: " << env << " callback: " << callback << std::endl;
  if (nullptr != env && nullptr != callback)
  {
    logFile << logTime() << "javascript_callback start callback.Call(" << env << ", " << *data << ")"<< std::endl;
    callback.Call(context->Value(), { Napi::Number::New(env, *data) });
    logFile << logTime() << "javascript_callback end callback.Call" << std::endl;
  }
}

std::map<uint32_t, Napi::TypedThreadSafeFunction<Napi::Reference<Napi::Value>, int, javascript_callback> > gCallbacks;
uint32_t gNextCallback = 1;
uint32_t gResult = 0;
bool sendCallback = false;

void callbackThreadFunc()
{
  while (true)
  {
    if (sendCallback)
    {
      logFile << logTime() << "Sending callback" << std::endl;
      int* value = new int(gResult++);
      for(auto it = gCallbacks.begin(); it != gCallbacks.end(); ++it)
      {
        logFile << logTime() << "Start NonBlockingCall" << std::endl;
        it->second.NonBlockingCall(value);
        logFile << logTime() << "End NonBlockingCall" << std::endl;
      }
    }
    else
    {
      logFile << logTime() << "Not sending callback" << std::endl;
    }
    logFile << logTime() << "Start sleep 1 second" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    logFile << logTime() << "End sleep" << std::endl;
  }
}

Napi::Value add_callback(const Napi::CallbackInfo& info)
{
   logFile << logTime() << "add_callback start" << std::endl;
   Napi::Env env = info.Env();

   if (info.Length() < 1)
   {
      Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
      return env.Null();
   }

   if (!info[0].IsFunction())
   {
      Napi::TypeError::New(env, "Expected arguement to be a function").ThrowAsJavaScriptException();
      return env.Null();
   }

   Napi::Reference<Napi::Value>* context = new Napi::Reference<Napi::Value>(Persistent(info.This()));

   uint32_t callbackId = gNextCallback++;
   gCallbacks[callbackId] = Napi::TypedThreadSafeFunction<Napi::Reference<Napi::Value>, int, javascript_callback>::New(
      env,                            // napi_env env
      info[0].As<Napi::Function>(),   // const Function& callback
      "CPCAPI2 Callback",             // ResourceString resourceName
      0,                              // size_t maxQueueSize
      1,                              // size_t initialThreadCount
      context,
      [callbackId](Napi::Env, void*, Napi::Reference<Napi::Value>* ctx) {     // Finalizer finalizeCallback
         gCallbacks.erase(callbackId);
         delete ctx;
      }
   );
   logFile << logTime() << "add_callback end" << std::endl;
   return Napi::Number::New(env, callbackId);
}

Napi::Value remove_callback(const Napi::CallbackInfo& info)
{
   logFile << logTime() << "remove_callback start" << std::endl;
   Napi::Env env = info.Env();

   if (info.Length() < 1)
   {
      Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
      return env.Null();
   }

   if (!info[0].IsNumber())
   {
      Napi::TypeError::New(env, "Expected arguement to be a number").ThrowAsJavaScriptException();
      return env.Null();
   }

   logFile << logTime() << "remove_callback pass checks" << std::endl;

   uint32_t handle = info[0].As<Napi::Number>();
   {
      if (gCallbacks.end() != gCallbacks.find(handle))
      {
         logFile << logTime() << "remove_callback release it" << std::endl;
         gCallbacks[handle].Release();
         gCallbacks.erase(handle);
      }
   }

   return Napi::Number::New(env, -2);
}

Napi::Value trigger_callback(const Napi::CallbackInfo& info)
{
   Napi::Env env = info.Env();

   sendCallback = true;

   return Napi::Number::New(env, -3);
}

Napi::Value query(const Napi::CallbackInfo& info)
{
   Napi::Env env = info.Env();
   return Napi::Number::New(env, gResult++);
}

Napi::Object load_API(Napi::Env env, Napi::Object exports)
{
   logFile << logTime() << "load_API start" << std::endl;

   std::thread callbackThread(callbackThreadFunc);
   callbackThread.detach();
   exports.Set(Napi::String::New(env, "query"), Napi::Function::New(env, query));
   exports.Set(Napi::String::New(env, "add_callback"), Napi::Function::New(env, add_callback));
   exports.Set(Napi::String::New(env, "remove_callback"), Napi::Function::New(env, remove_callback));
   exports.Set(Napi::String::New(env, "trigger_callback"), Napi::Function::New(env, trigger_callback));
   
   logFile << logTime() << "load_API end" << std::endl;
   return exports;
   
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, load_API)