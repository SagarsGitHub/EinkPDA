//    .oooooo.   ooooo        oooooooooooo oooooooooo.    //
//   d8P'  `Y8b  `888'        `888'     `8 `888'   `Y8b   //
//  888      888  888          888          888      888  //
//  888      888  888          888oooo8     888      888  //
//  888      888  888          888    "     888      888  //
//  `88b    d88'  888       o  888       o  888     d88'  //
//   `Y8bood8P'  o888ooooood8 o888ooooood8 o888bood8P'    //     
#include "globals.h"
                                                     
void oledWord(String word) {
  u8g2.clearBuffer();
  
  u8g2.setFont(u8g2_font_ncenB24_tr);
  if (u8g2.getStrWidth(word.c_str()) < u8g2.getDisplayWidth()) {
    u8g2.drawStr((u8g2.getDisplayWidth() - u8g2.getStrWidth(word.c_str()))/2,16+12,word.c_str());
    u8g2.sendBuffer();
    return;
  }

  u8g2.setFont(u8g2_font_ncenB18_tr);
  if (u8g2.getStrWidth(word.c_str()) < u8g2.getDisplayWidth()) {
    u8g2.drawStr((u8g2.getDisplayWidth() - u8g2.getStrWidth(word.c_str()))/2,16+9,word.c_str());
    u8g2.sendBuffer();
    return;
  }

  u8g2.setFont(u8g2_font_ncenB14_tr);
  if (u8g2.getStrWidth(word.c_str()) < u8g2.getDisplayWidth()) {
    u8g2.drawStr((u8g2.getDisplayWidth() - u8g2.getStrWidth(word.c_str()))/2,16+7,word.c_str());
    u8g2.sendBuffer();
    return;
  }

  u8g2.setFont(u8g2_font_ncenB12_tr);
  if (u8g2.getStrWidth(word.c_str()) < u8g2.getDisplayWidth()) {
    u8g2.drawStr((u8g2.getDisplayWidth() - u8g2.getStrWidth(word.c_str()))/2,16+6,word.c_str());
    u8g2.sendBuffer();
    return;
  }

  u8g2.setFont(u8g2_font_ncenB10_tr);
  if (u8g2.getStrWidth(word.c_str()) < u8g2.getDisplayWidth()) {
    u8g2.drawStr((u8g2.getDisplayWidth() - u8g2.getStrWidth(word.c_str()))/2,16+5,word.c_str());
    u8g2.sendBuffer();
    return;
  }

  u8g2.setFont(u8g2_font_ncenB08_tr);
  if (u8g2.getStrWidth(word.c_str()) < u8g2.getDisplayWidth()) {
    u8g2.drawStr((u8g2.getDisplayWidth() - u8g2.getStrWidth(word.c_str()))/2,16+4,word.c_str());
    u8g2.sendBuffer();
    return;
  }
  else {
    u8g2.drawStr(u8g2.getDisplayWidth() - u8g2.getStrWidth(word.c_str()),16+4,word.c_str());
    u8g2.sendBuffer();
    return;
  }
  
}

void oledLine(String line, bool doProgressBar) {
  uint8_t maxLength = maxCharsPerLine;
  u8g2.clearBuffer();
  
  //PROGRESS BAR
  if (doProgressBar && line.length() > 0) {
    //uint8_t progress = map(line.length(), 0, maxLength, 0, 128);

    int16_t x1, y1;
    uint16_t charWidth, charHeight;
    display.getTextBounds(line, 0, 0, &x1, &y1, &charWidth, &charHeight);

    uint8_t progress = map(charWidth, 0, display.width()-5, 0, u8g2.getDisplayWidth());

    u8g2.drawVLine(u8g2.getDisplayWidth(), 0, 2);
    u8g2.drawVLine(0, 0, 2);
    
    u8g2.drawHLine(0,0,progress);
    u8g2.drawHLine(0,1,progress);

    // LINE END WARNING INDICATOR
    if (charWidth > ((display.width()-5) * 0.8)) {   
      if ((millis() / 400) % 2 == 0) {  // ON for 200ms, OFF for 200ms
        u8g2.drawVLine(u8g2.getDisplayWidth()-1, 8, 32-16);
        u8g2.drawLine(u8g2.getDisplayWidth()-1,15,u8g2.getDisplayWidth()-4,12);
        u8g2.drawLine(u8g2.getDisplayWidth()-1,15,u8g2.getDisplayWidth()-4,18);
      }
    }
  }

  // DRAW FN/SHIFT IF NEEDED
  //u8g2.setFont(u8g2_font_u8glib_4_tf);
  u8g2.setFont(u8g2_font_5x7_tf);

  switch (CurrentKBState) {
    case SHIFT:
      u8g2.drawStr((u8g2.getDisplayWidth() - u8g2.getStrWidth("SHIFT")) / 2, u8g2.getDisplayHeight(), "SHIFT");
      break;
    case FUNC:
      u8g2.drawStr((u8g2.getDisplayWidth() - u8g2.getStrWidth("FN")) / 2, u8g2.getDisplayHeight(), "FN");
      break;
  }

  infoBar();

  // DRAW LINE TEXT
  u8g2.setFont(u8g2_font_ncenB18_tr);
  if (u8g2.getStrWidth(line.c_str()) < (u8g2.getDisplayWidth() - 5)) {
    u8g2.drawStr(0,20,line.c_str());
  }
  else {
    u8g2.drawStr(u8g2.getDisplayWidth()-8-u8g2.getStrWidth(line.c_str()),20,line.c_str());
  }

  u8g2.sendBuffer();
}

void infoBar() {
  // CLOCK
  if (SYSTEM_CLOCK) {
  }

  // Battery Indicator
  u8g2.drawXBMP(0,u8g2.getDisplayHeight()-6,10,6,batt_allArray[battState]);
}

void oledScroll() {
  // CLEAR DISPLAY
  u8g2.clearBuffer();

  // DRAW BACKGROUND
  u8g2.drawXBMP(0, 0, 128, 32, scrolloled0);

  // DRAW LINES PREVIEW
  long int count = allLines.size();
  long int startIndex = max((long int)(count - dynamicScroll), 0L);
  long int endIndex = max((long int)(count - dynamicScroll - 9), 0L);

  for (long int i = startIndex; i > endIndex && i >= 0; i--) {
    if (i >= count) continue;  // Ensure i is within bounds

    int16_t x1, y1;
    uint16_t charWidth, charHeight;

    // CHECK IF LINE STARTS WITH A TAB
    if (allLines[i].startsWith("    ")) {
      display.getTextBounds(allLines[i].substring(4), 0, 0, &x1, &y1, &charWidth, &charHeight);
      int lineWidth = map(charWidth, 0, 320, 0, 49);

      lineWidth = constrain(lineWidth, 0, 49);

      u8g2.drawBox(68, 28 - (4 * (startIndex - i)), lineWidth, 2);
    }
    else {
      display.getTextBounds(allLines[i], 0, 0, &x1, &y1, &charWidth, &charHeight);
      int lineWidth = map(charWidth, 0, 320, 0, 56);

      lineWidth = constrain(lineWidth, 0, 56);

      u8g2.drawBox(61, 28 - (4 * (startIndex - i)), lineWidth, 2);
    }

  }

  // PRINT CURRENT LINE
  u8g2.setFont(u8g2_font_ncenB08_tr);
  String lineNumStr = String(startIndex) + "/" + String(count);
  u8g2.drawStr(0,12,"Line:");
  u8g2.drawStr(0,24,lineNumStr.c_str());

  // PRINT LINE PREVIEW
  if (startIndex >= 0 && startIndex < allLines.size()) {
    String line = allLines[startIndex];
    if (line.length() > 0) {
      u8g2.setFont(u8g2_font_ncenB18_tr);
      u8g2.drawStr(140, 24, line.c_str());
    }
  }

  // SEND BUFFER 
  u8g2.sendBuffer();
}

///////////////////////////// CALC FUNCTIONS
// calc specific oled
void oledScrollCalc() {
  // CLEAR DISPLAY
  u8g2.clearBuffer();

  // DRAW BACKGROUND
  u8g2.drawXBMP(0, 0, 128, 32, scrolloled0);

  // DRAW LINES PREVIEW
  long int count = allLinesCalc.size();
  long int startIndex = max((long int)(count - dynamicScroll), 0L);
  long int endIndex = max((long int)(count - dynamicScroll - 9), 0L);

  for (long int i = startIndex; i > endIndex && i >= 0; i--) {
    if (i >= count) continue;  // Ensure i is within bounds

    int16_t x1, y1;
    uint16_t charWidth, charHeight;

    // CHECK IF LINE STARTS WITH A TAB
    if (allLinesCalc[i].startsWith("    ")) {
      display.getTextBounds(allLinesCalc[i].substring(4), 0, 0, &x1, &y1, &charWidth, &charHeight);
      int lineWidth = map(charWidth, 0, 320, 0, 49);

      lineWidth = constrain(lineWidth, 0, 49);

      // REMOVED TAB BOX DRAWING
      //u8g2.drawBox(68, 28 - (4 * (startIndex - i)), lineWidth, 2);
    }
    else {
      display.getTextBounds(allLinesCalc[i], 0, 0, &x1, &y1, &charWidth, &charHeight);
      int lineWidth = map(charWidth, 0, 320, 0, 56);

      lineWidth = constrain(lineWidth, 0, 56);

      u8g2.drawBox(61, 28 - (4 * (startIndex - i)), lineWidth, 2);
    }

  }

  // PRINT CURRENT LINE
  u8g2.setFont(u8g2_font_ncenB08_tr);
  String lineNumStr = String(startIndex) + "/" + String(count);
  u8g2.drawStr(0,12,"Line:");
  u8g2.drawStr(0,24,lineNumStr.c_str());

  // PRINT LINE PREVIEW
  if (startIndex >= 0 && startIndex < allLinesCalc.size()) {
    String line = allLinesCalc[startIndex];
    if (line.length() > 0) {
      u8g2.setFont(u8g2_font_ncenB18_tr);
      u8g2.drawStr(140, 24, line.c_str());
    }
  }

  // SEND BUFFER 
  u8g2.sendBuffer();
}
