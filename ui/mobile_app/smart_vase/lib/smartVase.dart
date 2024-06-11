import 'package:flutter/material.dart';

class SmartVase extends StatefulWidget {
  const SmartVase({super.key});

  @override
  State<SmartVase> createState() => _SmartvaseState();
}

class _SmartvaseState extends State<SmartVase> {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      home: Scaffold(
        body: Container(
        ),
      ),
    );
  }
}