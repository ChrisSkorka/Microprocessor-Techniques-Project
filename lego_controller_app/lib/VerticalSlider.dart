import 'package:flutter/material.dart';
import 'dart:math';
import 'dart:core';

class VerticalSlider extends StatefulWidget{
  VerticalSlider({Key key, this.onUpdate}) : super(key: key);

  final VoidCallback onUpdate;

  @override
  VerticalSliderState createState() => new VerticalSliderState();

}

class VerticalSliderState extends State<VerticalSlider>{

  double value = 0.0;
  double position = 0.0;

  double width = 100.0;
  double height = 300.0;

  void onVerticalDrag(DragUpdateDetails details){
    print(details.delta.toString());
  }

  @override
  Widget build(BuildContext context){

    return new SizedBox(
      width: width,
      height: height,
      child: new Stack(
        children: <Widget>[
          new CustomPaint(
            painter: new Background(value: value),
          ),
          new Positioned(
            
            child: new Row(
              children: <Widget>[
                new CustomPaint(
                  painter: new Slider(value: value),
                )
              ],
              )
          )
        ],
      )
    );


    return new GestureDetector(
      onVerticalDragUpdate: onVerticalDrag,
      // child: new CustomPaint(),
      child: new SizedBox(
        width: 100.0,
        height: 100.0,
        child: new Text("My tetx box", style: new TextStyle(color: Colors.white))
      ),
    );
  }
}

class Background extends CustomPainter{

  Background({this.value});

  double value = 0.0;
  double top = 20.0;
  double left = 20.0;
  double right = 60.0;
  double bottom = 300.0;

  @override
  void paint(Canvas canvas, Size size){
    
    // bottom = size.height - top;

    double center = top + (bottom - top) / 2;
    double indicatorPosition = center - value * (bottom - top) / 2;

      // track
    canvas.drawRRect(
      new RRect.fromLTRBR(left, top, right, bottom, new Radius.circular(5.0)), 
      new Paint()..color = new Color(0x22FFFFFF),
    );

      // shaded area
    canvas.drawRect(
      new Rect.fromLTRB(left, center, right, indicatorPosition), 
      new Paint()..color = new Color(0x55B31B1B),
    );

  }

  @override
  bool shouldRepaint(Background oldDelegate) => true;
}

class Slider extends CustomPainter{
  Slider({this.value});
  double value = 0.0;
  
  @override
  void paint(Canvas canvas, Size size){
      // slider
    canvas.drawRRect(
      new RRect.fromLTRBR(10.0, 10.0, 70.0, 30.0, new Radius.circular(5.0)), 
      new Paint()..color = new Color(0xAAB31B1B),
    );
  }

  @override
  bool shouldRepaint(Slider oldDelegate) => false;
}