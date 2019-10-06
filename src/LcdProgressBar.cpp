/*
  LcdProgressBar.cpp - LcdProgressBar library v1.0.1 - 2017-02-02

  LcdProgressBar is an Arduino library for displaying a progress bar in LCD display,
  which is previously initialized. This library uses LiquidCrystal library for displaying.

  Copyright (c) Wilfried Loche.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.

  http://url/of/online/tutorial.cc
*/

/**

  Sketch title

  Describe what it does in layman's terms.  Refer to the components
  attached to the various pins.

  The circuit:
  * list the components attached to each input
  * list the components attached to each output

  Created day month year
  By author's name
  Modified day month year
  By author's name

 */

#include "Arduino.h"
#include "LcdProgressBar.h"

#ifdef LiquidCrystal_I2C_h
#include <LiquidCrystal_I2C.h>

LcdProgressBar::LcdProgressBar(LiquidCrystal_I2C *lcd, int row, int numCols)
{
  _lcd = lcd;
  _numCols = numCols;
  _computedNumCols = numCols - 2;
  _row = row;
}
#endif

#ifdef LiquidCrystal_h
#include <LiquidCrystal.h>

LcdProgressBar::LcdProgressBar(LiquidCrystal *lcd, int row, int numCols)
{
  _lcd = lcd;
  _numCols = numCols;
  _computedNumCols = numCols - 2;
  _row = row;
}
#endif

void LcdProgressBar::setMinValue(unsigned long minValue)
{
  _minValue = minValue;
}

void LcdProgressBar::setMaxValue(unsigned long maxValue)
{
  if (maxValue <= _minValue)
  {
    Serial.println("LcdProgressBar::setMaxValue() error: maxValue <= _minValue");
    exit(100);
  }
  _maxValue = maxValue;
  _computedMaxValue = maxValue - _minValue;
}

void LcdProgressBar::drawValue(unsigned long value)
{
  draw(value);
}

void LcdProgressBar::reDraw(unsigned long value)
{
  draw(value, true);
}

void LcdProgressBar::draw(unsigned long value, bool re)
{

#ifdef LCDPROGRESSBAR_DEBUG
  Serial.print("value=");
  Serial.print(value);
  Serial.print(", _minValue=");
  Serial.print(_minValue);
  Serial.print(", _maxValue=");
  Serial.println(_maxValue);
#endif

  //byte progressPos = map(value, minValue, maxValue, 2, 14);
  byte progressPos = 0;
  const char bar_char[] = "[= ]";
  if (value > _minValue)
  {
    if (value >= _maxValue)
    {
      progressPos = _computedNumCols;
    }
    else
    {
      progressPos = round((float)(value - _minValue) / (float)(_computedMaxValue)*_computedNumCols);
    }
  }

  if ((_previousProgressPos != progressPos) || re)
  {
#ifdef LCDPROGRESSBAR_DEBUG
    Serial.print("> progressPos=");
    Serial.print(progressPos);
#endif
    _previousProgressPos = progressPos;
    char progressBarString[_numCols + 1];
    memset(progressBarString, bar_char[2], sizeof(progressBarString) - 1);
    progressBarString[0] = bar_char[0];
    progressBarString[_numCols - 1] = bar_char[3];
    progressBarString[_numCols] = 0;            //last char must be NULL, othervise lcd.print fails

    for (int i = 1; i <= progressPos; ++i)
    {
      progressBarString[i] = bar_char[1];
    }

    _lcd->setCursor(0, _row);
    _lcd->print(progressBarString);
#ifdef LCDPROGRESSBAR_DEBUG
    Serial.print("> progressstring=");
    Serial.print(progressBarString);
    Serial.println("END");
#endif
  }
}
