float sigmoid(float input){
  return exp(input)/(exp(input) + 1);
}

class perceptron{
  int id;
  int xLocation;
  int yLocation;
  String layer;
  
  perceptron(String layerName, int uniqueID, int x, int y){
    layer = layerName;
    id = uniqueID;
    xLocation = x;
    yLocation = y;
  }
  
  float sumInputs(float[] inputs){
    float rollingTotal = 0.0;
    for(int i = 0; i < inputs.length; i++){
       rollingTotal += inputs[i];
     }
     return rollingTotal;
  }
  
  float activation(float input){
     return sigmoid(input);
  }
  
  float run(float[] inputs){
    return activation(sumInputs(inputs)); 
  }
  
  void display(){
    circle(xLocation, yLocation, 25);
    fill(0,128,128);
    textSize(10);
    String name = layer + id;
    text(name, xLocation + 10, yLocation - 10);
    fill(255,255,255);
  }
}

perceptron[] inputNeurons;
perceptron[] hiddenNeurons;
perceptron[] outputNeurons;

void setup(){
  size(1000, 1000);
  inputNeurons = new perceptron[10];
  for(int i = 0; i < inputNeurons.length; i++){
    inputNeurons[i] = new perceptron("Input Layer ", i, 100, i*50 + 50);
  }

  hiddenNeurons = new perceptron[10];
  for(int i = 0; i < hiddenNeurons.length; i++){
    hiddenNeurons[i] = new perceptron("Hidden Layer ", i, 300, i*50 + 50);
  }

  outputNeurons = new perceptron[3];
  for(int i = 0; i < outputNeurons.length; i++){
    outputNeurons[i] = new perceptron("Output Layer ", i, 500, i*50 + 225);
  }

  noLoop();
}

void draw(){
  background(255,255,255);
  strokeWeight(3);
  stroke(0,0,0);
  //fill(0,0,0);
  for(int i = 0; i < inputNeurons.length; i++){
    inputNeurons[i].display();
  }

  for(int i = 0; i < hiddenNeurons.length; i++){
    hiddenNeurons[i].display();
  }

  for(int i = 0; i < outputNeurons.length; i++){
    outputNeurons[i].display();
  }
}
