//
//  Generated file. Do not edit.
//

// clang-format off

#include "generated_plugin_registrant.h"

#include <external_texture/external_texture_plugin.h>

void fl_register_plugins(FlPluginRegistry* registry) {
  g_autoptr(FlPluginRegistrar) external_texture_registrar =
      fl_plugin_registry_get_registrar_for_plugin(registry, "ExternalTexturePlugin");
  external_texture_plugin_register_with_registrar(external_texture_registrar);
}
