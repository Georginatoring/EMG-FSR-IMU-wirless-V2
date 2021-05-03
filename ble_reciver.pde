import processing.serial.*;
Serial myPort;
int value= 0 ;
String path = "data/test2.csv";
Table table;
String data;


void setup(){
  //File path
 // selectInput("Select a file to process:", "fileSelected");
  
  //CSV table
  table = new Table();
  
  table.addColumn("count");
  table.addColumn("EMG1");
  table.addColumn("EMG2");
  table.addColumn("EMG3");
  table.addColumn("EMG4");
  table.addColumn("EMG5");
  table.addColumn("EMG6");
  table.addColumn("FMG1");
  table.addColumn("FMG2");
  table.addColumn("FMG3");
  table.addColumn("FMG4");
  table.addColumn("FMG5");
  table.addColumn("FMG6");
  table.addColumn("Game Rotation Vector r");
  table.addColumn("Game Rotation Vector i");
  table.addColumn("Game Rotation Vector j");
  table.addColumn("Game Rotation Vector k");
  table.addColumn("Accelerometer - x");
  table.addColumn("Accelerometer - y");
  table.addColumn("Accelerometer - z");
  table.addColumn("Gyro - x");
  table.addColumn("Gyro - y");
  table.addColumn("Gyro - z");
  table.addColumn("Magnetic Field - x");
  table.addColumn("Magnetic Field - y");
  table.addColumn("Magnetic Field - z");
  
  
  size(600, 600);
 myPort = new Serial(this, "COM5", 115200); // Starts the serial communication
  myPort.bufferUntil('\n'); // Defines up to which character the data from the serial port will be read. The character '\n' or 'New Line'
}

//void serialEvent (Serial myPort){ // Checks for available data in the Serial Port
//  value = myPort.read(); //Reads the data sent from the Arduino (the String "LED: OFF/ON) and it puts into the "ledStatus" variable
//}
void draw(){
  background(237, 240, 241);
  textSize(23);
  text("Bluetooth Receiver Program for EMG & FMG &IMU", 20, 50);  
  
  fill(20, 160, 133); // Green Color
  stroke(33);
  strokeWeight(1);
  rect(50, 130, 100, 50, 10);  // start recording data
  
  fill(255,0,0); // Red Color
  stroke(33);
  strokeWeight(1);
  rect(400, 130, 100, 50, 10); // stop recording data
  
  fill(255);
  
  textSize(32);
  text("Start",60, 165);
  text("Stop", 405, 165);
  textSize(24);
  fill(33);
  

  textSize(16);
  text("Program made by George B,\n  Supervisor: Dr. Naish", 80, 320);
 
 
 // File location 
  textSize(20); 
  text("File Location", 220, 510);
  textSize(10); 
  text(path, 50, 550);
  
  while (myPort.available() > 0) {
    data = myPort.readStringUntil('$');
      if(data != null){
          println(data); // print data
          String[] SplData = split(data, ",");
          TableRow newRow = table.addRow();
          
            newRow.setString("count", SplData[0]);
            newRow.setString("EMG1", SplData[1]);
            newRow.setString("EMG2", SplData[2]);
            newRow.setString("EMG3", SplData[3]);
            newRow.setString("EMG4", SplData[4]);
            newRow.setString("EMG5", SplData[5]);
            newRow.setString("EMG6", SplData[6]);
            newRow.setString("FMG1", SplData[7]);
            newRow.setString("FMG2", SplData[8]);
            newRow.setString("FMG3", SplData[9]);
            newRow.setString("FMG4", SplData[10]);
            newRow.setString("FMG5", SplData[11]);
            newRow.setString("FMG6", SplData[12]);
            newRow.setString("Game Rotation Vector r", SplData[13]);
            newRow.setString("Game Rotation Vector i", SplData[14]);
            newRow.setString("Game Rotation Vector j", SplData[15]);
            newRow.setString("Game Rotation Vector k", SplData[16]);
            newRow.setString("Accelerometer - x", SplData[17]);
            newRow.setString("Accelerometer - y", SplData[18]);
            newRow.setString("Accelerometer - z", SplData[19]);
            newRow.setString("Gyro - x", SplData[20]);
            newRow.setString("Gyro - y", SplData[21]);
            newRow.setString("Gyro - z", SplData[22]);
            newRow.setString("Magnetic Field - x", SplData[23]);
            newRow.setString("Magnetic Field - y", SplData[24]);
            newRow.setString("Magnetic Field - z", SplData[25]);
          
            saveTable(table, path);
      }
  }

  

  
  // If the button "Start" is pressed
  if(mousePressed && mouseX>50 && mouseX<150 && mouseY>130 && mouseY<180){
    myPort.write('1'); // Sends the character '1' to ESP32
    // Highlighs the buttons in red color when pressed
    stroke(255,0,0);
    strokeWeight(2);
    noFill();
    rect(50, 130, 100, 50, 10);
  }
  
  // If the button "Stop" is pressed
  if(mousePressed && mouseX>400 && mouseX<500 && mouseY>130 && mouseY<180){
    myPort.write('0'); // Sends the character '0' to ESP32
    stroke(255,0,0);
    strokeWeight(2);
    noFill();
    rect(400, 130, 100, 50, 10);
  }
}



void fileSelected(File selection) {

  if (selection == null) {
    println("Window was closed or the user hit cancel.");
  } else {
    path = selection.getAbsolutePath();
  }  

}
