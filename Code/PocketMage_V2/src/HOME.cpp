//  ooooo   ooooo   .oooooo.   ooo        ooooo oooooooooooo  //
//  `888'   `888'  d8P'  `Y8b  `88.       .888' `888'     `8  //
//   888     888  888      888  888b     d'888   888          //
//   888ooooo888  888      888  8 Y88. .P  888   888oooo8     //
//   888     888  888      888  8  `888'   888   888    "     //
//   888     888  `88b    d88'  8    Y     888   888       o  //
//  o888o   o888o  `Y8bood8P'  o8o        o888o o888ooooood8  //
#include "globals.h"

void commandSelect(String command) {
  command.toLowerCase();

  // OPEN IN FILE WIZARD
  if (command.startsWith("-")) {
    command = removeChar(command, ' ');
    command = removeChar(command, '-');
    keypad.disableInterrupts();
    listDir(SPIFFS, "/");
    keypad.enableInterrupts();

    for (uint8_t i = 0; i < (sizeof(filesList) / sizeof(filesList[0])); i++) {
      String lowerFileName = filesList[i]; 
      lowerFileName.toLowerCase();
      if (command == lowerFileName || (command+".txt") == lowerFileName || ("/"+command+".txt") == lowerFileName) {
        workingFile = filesList[i];
        CurrentAppState = FILEWIZ;
        CurrentFileWizState = WIZ1_;
        CurrentKBState  = FUNC;
        newState = true;
        return;
      }
    }
  }

  // OPEN IN TXT EDITOR
  if (command.startsWith("/")) {
    command = removeChar(command, ' ');
    command = removeChar(command, '/');
    keypad.disableInterrupts();
    listDir(SPIFFS, "/");
    keypad.enableInterrupts();

    for (uint8_t i = 0; i < (sizeof(filesList) / sizeof(filesList[0])); i++) {
      String lowerFileName = filesList[i]; 
      lowerFileName.toLowerCase();
      if (command == lowerFileName || (command+".txt") == lowerFileName || ("/"+command+".txt") == lowerFileName) {
        editingFile = filesList[i];
        loadFile();
        CurrentAppState = TXT;
        CurrentTXTState = TXT_;
        CurrentKBState  = NORMAL;
        newLineAdded = true;
        return;
      }
    }
  }

  if (command.startsWith("timeset")) {
    command = removeChar(command, ' ');
    command = removeChar(command, 't');
    command = removeChar(command, 'i');
    command = removeChar(command, 'm');
    command = removeChar(command, 'e');
    command = removeChar(command, 's');

    setTimeFromString(command);
  }


  else if (command == "home") {
    oledWord("You're home, silly!");
    delay(1000);
  } 
  /////////////////////////////
  else if (command == "note" || command == "text" || command == "write" || command == "notebook" || command == "notepad" || command == "txt" || command == "1") {
    CurrentAppState = TXT;
    CurrentKBState  = NORMAL;
    dynamicScroll = 0;
    newLineAdded = true;
  }
  /////////////////////////////
  else if (command == "file wizard" || command == "wiz" || command == "file wiz" || command == "file" || command == "2") {
    CurrentAppState = FILEWIZ;
    CurrentKBState  = NORMAL;
    forceSlowFullUpdate = true;
    newState = true;
  }
  /////////////////////////////
  else if (command == "back up" || command == "export" || command == "transfer" || command == "usb transfer" || command == "usb" || command == "3") {
    // OPEN USB FILE TRANSFER
  }
  /////////////////////////////
  else if (command == "tasks" || command == "task") {
    CurrentAppState = TASKS;
    CurrentTasksState = TASKS0;
    forceSlowFullUpdate = true;
    newState = true;
  }
  /////////////////////////////
  else if (command == "bluetooth" || command == "bt" || command == "4") {
    // OPEN BLUETOOTH
  }
  /////////////////////////////
  else if (command == "preferences" || command == "setting" || command == "settings" || command == "5") {
    // OPEN SETTINGS
  }
  /////////////////////////////
  else if (command == "calc" || command == "calculator" || command == "math" || command == "10") {
    currentFont = &FreeMonoBold9pt7b;
    setTXTFont(currentFont);
    CurrentAppState = CALC;
    CurrentKBState  = FUNC;
    dynamicScroll = 0;
    forceSlowFullUpdate = true;
    newState = true;
    doFull = true;
    newLineAdded = false;
  }
  /////////////////////////////
  else if (command == "i farted") {
    oledWord("That smells");
    delay(1000);
  } 
  else if (command == "poop") {
    oledWord("Yuck");
    delay(1000);
  } 
  else if (command == "hello") {
    oledWord("Hey, you!");
    delay(1000);
  } 
  else if (command == "hi") {
    oledWord("What's up?");
    delay(1000);
  } 
  else if (command == "i love you") {
    oledWord("luv u 2 <3");
    delay(1000);
  } 
  else if (command == "what can you do") {
    oledWord("idk man");
    delay(1000);
  } 
  else if (command == "alexa") {
    oledWord("...");
    delay(1000);
  } 
  else {
    oledWord("Huh?");
    delay(1000);
  }
}

void processKB_HOME() {
  int currentMillis = millis();

  switch (CurrentHOMEState) {
    case HOME_HOME:
      if (currentMillis - KBBounceMillis >= KB_COOLDOWN) {  
        char inchar = updateKeypress();
        // HANDLE INPUTS
        //No char recieved
        if (inchar == 0);   
        //CR Recieved
        else if (inchar == 13) {                          
          commandSelect(currentLine);
          currentLine = "";
        }                                      
        //SHIFT Recieved
        else if (inchar == 17) {                                  
          if (CurrentKBState == SHIFT) CurrentKBState = NORMAL;
          else CurrentKBState = SHIFT;
        }
        //FN Recieved
        else if (inchar == 18) {                                  
          if (CurrentKBState == FUNC) CurrentKBState = NORMAL;
          else CurrentKBState = FUNC;
        }
        //Space Recieved
        else if (inchar == 32) {                                  
          currentLine += " ";
        }
        //ESC / CLEAR Recieved
        else if (inchar == 20) {                                  
          currentLine = "";
        }
        //BKSP Recieved
        else if (inchar == 8) {                  
          if (currentLine.length() > 0) {
            currentLine.remove(currentLine.length() - 1);
          }
        }
        else {
          currentLine += inchar;
          if (inchar >= 48 && inchar <= 57) {}  //Only leave FN on if typing numbers
          else if (CurrentKBState != NORMAL) {
            CurrentKBState = NORMAL;
          }
        }

        currentMillis = millis();
        //Make sure oled only updates at 60fps
        if (currentMillis - OLEDFPSMillis >= 16) {
          OLEDFPSMillis = currentMillis;
          oledLine(currentLine, false);
        }
      }
      /*disableTimeout = false;
      if (OLEDPowerSave) {
        u8g2.setPowerSave(0);
        OLEDPowerSave = false;
      }
      CurrentKBState = FUNC;
      //Make sure oled only updates at 60fps
      if (currentMillis - KBBounceMillis >= KB_COOLDOWN) {  
        char inchar = updateKeypress();
        //No char recieved
        if (inchar == 0);
        //BKSP Recieved
        else if (inchar == 127 || inchar == 8) {}
        else {
          int fileIndex = (inchar == '0') ? 10 : (inchar - '0');
          //Edit a new file
          switch (fileIndex) {
            case 1: //TXT
              CurrentAppState = TXT;
              CurrentKBState  = NORMAL;
              einkRefresh = FULL_REFRESH_AFTER + 1;
              //newState = true;
              newLineAdded = true;
              break;
            case 2: //FILE WIZARD
              CurrentAppState = FILEWIZ;
              CurrentKBState  = FUNC;
              einkRefresh = FULL_REFRESH_AFTER + 1;
              newState = true;
              break;
            case 3: //USB TRANSFER
              CurrentAppState = USB;
              break;
            case 4: //BLUETOOTH TRANSFER
              CurrentAppState = BT;
              break;
            case 5: //SETTINGS
              CurrentAppState = SETTINGS;
              break;
          }
        }

        currentMillis = millis();
        //Make sure oled only updates at 60fps
        if (currentMillis - OLEDFPSMillis >= 16) {
          OLEDFPSMillis = currentMillis;
          oledWord(currentWord);
        }
        KBBounceMillis = currentMillis;
      }*/
      break;

    case NOWLATER:
      newState = false;
      break;
  }
}

void einkHandler_HOME() {
  switch (CurrentHOMEState) {
    case HOME_HOME:
      if (newState) {
        newState = false;
        drawHome();
        refresh();
      }
      break;

    case NOWLATER:
      
      break;
  }
}

void drawHome() {
  display.setRotation(3);
  display.fillScreen(GxEPD_WHITE);
  
  int16_t x1, y1;
  uint16_t charWidth, charHeight;
  uint8_t appsPerRow = 5; // Number of apps per row
  uint8_t spacingX = 60;  // Horizontal spacing
  uint8_t spacingY = 60;  // Vertical spacing
  uint8_t iconSize = 40;  // Icon width and height
  uint8_t startX = 20;    // Initial X position
  uint8_t startY = 20;    // Initial Y position

  display.setFont(&FreeSerif9pt7b);
  for (int i = 0; i < sizeof(appIcons) / sizeof(appIcons[0]); i++) {
    int row = i / appsPerRow;
    int col = i % appsPerRow;
    
    int xPos = startX + (spacingX * col);
    int yPos = startY + (spacingY * row);

    display.drawBitmap(xPos, yPos, appIcons[i], iconSize, iconSize, GxEPD_BLACK);
    display.getTextBounds(appStateNames[i], 0, 0, &x1, &y1, &charWidth, &charHeight);
    display.setCursor(xPos + (iconSize / 2) - (charWidth / 2), yPos + iconSize + 13);
    display.print(appStateNames[i]);
  }
  display.setFont(&FreeMonoBold9pt7b);

  drawStatusBar(" Type What You Want To Do");
}