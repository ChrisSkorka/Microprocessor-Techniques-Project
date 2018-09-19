import 'package:flutter/material.dart';
import 'dart:math';
import 'dart:core';

class Indiocator extends StatefulWidget{
  Indiocator({Key key, this.text, this.direction, this.color}) : super(key: key);

  final String text;
  final IndicatorDirection direction;
  final Color color;

  @override
  IndiocatorState createState() => new IndiocatorState();

}

class IndiocatorState extends State<Indiocator>{

  double height = 40.0;

  @override
  Widget build(BuildContext context){


  }

}

enum IndicatorDirection{
  top, right, bottom, left
}