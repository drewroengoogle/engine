// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SHELL_PLATFORM_IOS_FRAMEWORK_SOURCE_FLUTTERPLATFORMPLUGIN_H_
#define SHELL_PLATFORM_IOS_FRAMEWORK_SOURCE_FLUTTERPLATFORMPLUGIN_H_

#include "flutter/fml/platform/darwin/weak_nsobject.h"
#import "flutter/shell/platform/darwin/common/framework/Headers/FlutterChannels.h"
#import "flutter/shell/platform/darwin/ios/framework/Headers/FlutterEngine.h"

@interface FlutterPlatformPlugin : NSObject
- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)initWithEngine:(fml::WeakNSObject<FlutterEngine>)engine NS_DESIGNATED_INITIALIZER;
- (void)handleMethodCall:(FlutterMethodCall*)call result:(FlutterResult)result;

@end

namespace flutter {

extern const char* const kOrientationUpdateNotificationName;
extern const char* const kOrientationUpdateNotificationKey;
extern const char* const kOverlayStyleUpdateNotificationName;
extern const char* const kOverlayStyleUpdateNotificationKey;

}  // namespace flutter

#endif  // SHELL_PLATFORM_IOS_FRAMEWORK_SOURCE_FLUTTERPLATFORMPLUGIN_H_
