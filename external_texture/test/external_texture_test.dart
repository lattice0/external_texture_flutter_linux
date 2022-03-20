import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:external_texture/external_texture.dart';

void main() {
  const MethodChannel channel = MethodChannel('external_texture');

  TestWidgetsFlutterBinding.ensureInitialized();

  setUp(() {
    channel.setMockMethodCallHandler((MethodCall methodCall) async {
      return '42';
    });
  });

  tearDown(() {
    channel.setMockMethodCallHandler(null);
  });

  test('getPlatformVersion', () async {
    expect(await ExternalTexture.platformVersion, '42');
  });
}
