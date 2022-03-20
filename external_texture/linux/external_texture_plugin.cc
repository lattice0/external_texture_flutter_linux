#include "include/external_texture/external_texture_plugin.h"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <sys/utsname.h>

#include <cstring>
#include <iostream>
#include <memory>

#include "include/external_texture/fl_my_texture_gl.h"
#include "include/external_texture/opengl_renderer.h"
std::unique_ptr<OpenGLRenderer> openglRenderer;

#define EXTERNAL_TEXTURE_PLUGIN(obj)                                     \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), external_texture_plugin_get_type(), \
                              ExternalTexturePlugin))

struct _ExternalTexturePlugin {
  GObject parent_instance;
  FlTextureRegistrar* texture_registrar;
  FlView* fl_view;
};

G_DEFINE_TYPE(ExternalTexturePlugin,
              external_texture_plugin,
              g_object_get_type())

// Called when a method call is received from Flutter.
static void external_texture_plugin_handle_method_call(
    ExternalTexturePlugin* self,
    FlMethodCall* method_call) {
  g_autoptr(FlMethodResponse) response = nullptr;

  const gchar* method = fl_method_call_get_name(method_call);

  if (strcmp(method, "getPlatformVersion") == 0) {
    struct utsname uname_data = {};
    uname(&uname_data);
    g_autofree gchar* version = g_strdup_printf("Linux %s", uname_data.version);
    g_autoptr(FlValue) result = fl_value_new_string(version);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(result));
  } else if (strcmp(method, "registerTexture") == 0) {
    GdkWindow* window = gtk_widget_get_parent_window(GTK_WIDGET(self->fl_view));
    GError* error = NULL;
    GdkGLContext* context = gdk_window_create_gl_context(window, &error);
    gdk_gl_context_make_current(context);
    openglRenderer = std::make_unique<OpenGLRenderer>(context);
    int width = 1280;
    int height = 720;
    int texture_name = openglRenderer->genTexture(width, height);
    FlMyTextureGL* t =
        fl_my_texture_gl_new(GL_TEXTURE_2D, texture_name, width, height);
    g_autoptr(FlTexture) texture = FL_TEXTURE(t);
    FlTextureRegistrar* texture_registrar = self->texture_registrar;
    fl_texture_registrar_register_texture(texture_registrar, texture);
    fl_texture_registrar_mark_texture_frame_available(texture_registrar,
                                                      texture);
    g_autoptr(FlValue) result =
        fl_value_new_int(reinterpret_cast<int64_t>(texture));
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(result));
  } else {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  }

  fl_method_call_respond(method_call, response, nullptr);
}

static void external_texture_plugin_dispose(GObject* object) {
  G_OBJECT_CLASS(external_texture_plugin_parent_class)->dispose(object);
}

static void external_texture_plugin_class_init(
    ExternalTexturePluginClass* klass) {
  G_OBJECT_CLASS(klass)->dispose = external_texture_plugin_dispose;
}

static void external_texture_plugin_init(ExternalTexturePlugin* self) {}

static void method_call_cb(FlMethodChannel* channel,
                           FlMethodCall* method_call,
                           gpointer user_data) {
  ExternalTexturePlugin* plugin = EXTERNAL_TEXTURE_PLUGIN(user_data);
  external_texture_plugin_handle_method_call(plugin, method_call);
}

void external_texture_plugin_register_with_registrar(
    FlPluginRegistrar* registrar) {
  ExternalTexturePlugin* plugin = EXTERNAL_TEXTURE_PLUGIN(
      g_object_new(external_texture_plugin_get_type(), nullptr));
  FlView* fl_view = fl_plugin_registrar_get_view(registrar);
  plugin->fl_view = fl_view;
  plugin->texture_registrar =
      fl_plugin_registrar_get_texture_registrar(registrar);
  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  g_autoptr(FlMethodChannel) channel =
      fl_method_channel_new(fl_plugin_registrar_get_messenger(registrar),
                            "external_texture", FL_METHOD_CODEC(codec));
  fl_method_channel_set_method_call_handler(
      channel, method_call_cb, g_object_ref(plugin), g_object_unref);

  g_object_unref(plugin);
}
