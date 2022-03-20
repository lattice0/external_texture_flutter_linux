import 'dart:async';
import 'dart:ffi';

import 'package:flutter/services.dart';

class ExternalTexture {
  static const MethodChannel _channel = const MethodChannel('external_texture');

  static Future<String?> get platformVersion async {
    final String? version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }

  static Future<int?> registerTexture() async {
    final int? texture_id = await _channel.invokeMethod('registerTexture');
    return texture_id;
  }
}
