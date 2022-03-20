import 'package:flutter/material.dart';
import 'package:external_texture/external_texture.dart';
import 'dart:async';
import 'package:flutter/services.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  static const platform = MethodChannel('external_texture');
  /*
  Future<String> platformVersion() async {
    String response;
    try {
      final String result = await platform.invokeMethod('getPlatformVersion');
      final Int64 texture_id = await platform.invokeMethod('getPlatformVersion');

      print("----------------&&&&&& result: " + result);
      //response = '$result';
      //print(response);
      return "";
    } on PlatformException catch (e) {
      print(e);
      return "error";
    }
  }
  */
  static Future<int?> getTexture() async {
    //return null;
    return await ExternalTexture.registerTexture();
  }

  Future<void> doSomething() async {
    print("platform version: " +
        await ExternalTexture.platformVersion.toString());
    int? texture_id = await ExternalTexture.registerTexture();
    if (texture_id != null) {
      print("texture_id: " + texture_id.toString());
    }
  }

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    //doSomething();
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: MyHomePage(title: 'Flutter Demo Home Page'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  MyHomePage({Key? key, required this.title}) : super(key: key);

  final String title;

  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  int _counter = 0;

  void _incrementCounter() {
    setState(() {
      _counter++;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(widget.title),
      ),
      body: Center(
          child: FutureBuilder<int?>(
              future: MyApp.getTexture(),
              builder: (BuildContext context, AsyncSnapshot<int?> snapshot) {
                //return Container();
                if (snapshot.hasData && snapshot.data != null) {
                  return Texture(textureId: snapshot.data!);
                } else {
                  return Container();
                }
              })),
      floatingActionButton: FloatingActionButton(
        onPressed: _incrementCounter,
        tooltip: 'Increment',
        child: Icon(Icons.add),
      ), // This trailing comma makes auto-formatting nicer for build methods.
    );
  }
}
