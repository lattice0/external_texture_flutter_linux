#import "ExternalTexturePlugin.h"
#if __has_include(<external_texture/external_texture-Swift.h>)
#import <external_texture/external_texture-Swift.h>
#else
// Support project import fallback if the generated compatibility header
// is not copied when this plugin is created as a library.
// https://forums.swift.org/t/swift-static-libraries-dont-copy-generated-objective-c-header/19816
#import "external_texture-Swift.h"
#endif

@implementation ExternalTexturePlugin
+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar {
  [SwiftExternalTexturePlugin registerWithRegistrar:registrar];
}
@end
