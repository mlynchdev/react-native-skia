#pragma once

#include <ReactCommon/CallInvokerHolder.h>
#include <fbjni/fbjni.h>
#include <jsi/jsi.h>
#include <map>
#include <memory>
#include <string>

#include <JniPlatformContext.h>
#include <JniPlatformContextWrapper.h>
#include <RNSkManager.h>

namespace RNSkia {

using namespace facebook;

using JSCallInvokerHolder =
    jni::alias_ref<facebook::react::CallInvokerHolder::javaobject>;

using JavaPlatformContext = jni::alias_ref<JniPlatformContext::javaobject>;

class JniSkiaDrawView;

class JniSkiaManager : public jni::HybridClass<JniSkiaManager> {
   public:
    static auto constexpr kJavaDescriptor = "Lcom/shopify/reactnative/skia/SkiaManager;";
    static auto constexpr TAG = "ReactNativeSkia";

    static jni::local_ref<jni::HybridClass<JniSkiaManager>::jhybriddata> initHybrid(
        jni::alias_ref<jhybridobject> jThis,
        jlong jsContext,
        JSCallInvokerHolder jsCallInvokerHolder,
        JavaPlatformContext platformContext);

    static void registerNatives();

    JniSkiaManager() {}
    ~JniSkiaManager() {
        RNSkLogger::logToConsole("JniSkiaManager dtor");
    }

    explicit JniSkiaManager(
        jni::alias_ref<JniSkiaManager::jhybridobject> jThis,
        jsi::Runtime *runtime,
        std::shared_ptr<facebook::react::CallInvoker> jsCallInvoker,
        JniPlatformContext* platformContext)
        : _javaPart(jni::make_global(jThis)),
          _jsRuntime(runtime),
          _jsCallInvoker(jsCallInvoker),
          _context(std::make_shared<JniPlatformContextWrapper>(platformContext, runtime, jsCallInvoker)) {

    }

    void registerSkiaView(int viewTag, JniSkiaDrawView *skiaView);
    void unregisterSkiaView(int viewTag);

    std::shared_ptr<JniPlatformContextWrapper> getPlatformContext() { return _context; }

    void invalidate() {
        _context->stopDrawLoop();
        _skManager = nullptr;
        _context = nullptr;
    }

   private:
    friend HybridBase;

    std::shared_ptr<RNSkManager> _skManager;

    jni::global_ref<JniSkiaManager::javaobject> _javaPart;

    jsi::Runtime *_jsRuntime;
    std::shared_ptr<facebook::react::CallInvoker> _jsCallInvoker;
    std::shared_ptr<JniPlatformContextWrapper> _context;

    void initializeRuntime();
    void installJSIBindings();
};

} // namespace RNSkia
