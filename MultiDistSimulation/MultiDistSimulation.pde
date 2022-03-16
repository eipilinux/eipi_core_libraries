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
    // fill(0,128,128);
    // textSize(10);
    // String name = layer + id;
    // text(name, xLocation + 10, yLocation - 10);
    // fill(255,255,255);
  }
}

perceptron[] inputNeurons;
perceptron[] hiddenNeurons;
perceptron[] outputNeurons;

float[] inputNeuronsInputWeights;
float[][] hiddenNeuronsInputWeights;
float[][] outputNeuronsInputWeights;

void setup(){
  size(1000, 1000);
  inputNeurons = new perceptron[10];
  for(int i = 0; i < inputNeurons.length; i++){
    inputNeurons[i] = new perceptron("Input Layer ", i, 100, i*80 + 50);
  }

  hiddenNeurons = new perceptron[10];
  for(int i = 0; i < hiddenNeurons.length; i++){
    hiddenNeurons[i] = new perceptron("Hidden Layer ", i, 300, i*80 + 50);
  }

  outputNeurons = new perceptron[3];
  for(int i = 0; i < outputNeurons.length; i++){
    outputNeurons[i] = new perceptron("Output Layer ", i, 500, i*80 + 225);
  }

  inputNeuronsInputWeights = new float[inputNeurons.length];
  for(int i = 0; i < inputNeurons.length; i++){
    inputNeuronsInputWeights[i] = random(-1.0, 1.0);
  }

  hiddenNeuronsInputWeights = new float[hiddenNeurons.length][inputNeurons.length];
  for(int i = 0; i < hiddenNeurons.length; i++){
    for(int j = 0; j < inputNeurons.length; j++){
      hiddenNeuronsInputWeights[i][j] = random(-1.0, 1.0);
    }
  }

  outputNeuronsInputWeights = new float[outputNeurons.length][hiddenNeurons.length];
  for(int i = 0; i < outputNeurons.length; i++){
    for(int j = 0; j < hiddenNeurons.length; j++){
      outputNeuronsInputWeights[i][j] = random(-1.0, 1.0);
    }
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

  strokeWeight(2);

  for(int i = 0; i < inputNeurons.length; i++){
    // if(inputNeuronsInputWeights[i] < 0){
    //   stroke(255,0,0);
    // }
    // strokeWeight(abs(inputNeuronsInputWeights[i])*5.0);
    stroke(128, round((inputNeuronsInputWeights[i]+1)*128), 128);
    line(50, i*80 + 50, 100 - 14, i*80 + 50);
    // fill(0,128,128);
    // textSize(10);
    // String label = "val: " + nf(inputNeuronsInputWeights[i], 0, 3);
    // text(label, 45, i*50 + 45);
    // fill(255,255,255);
    stroke(0,0,0);
  }

  for(int i = 0; i < hiddenNeurons.length; i++){
    for(int j = 0; j < inputNeurons.length; j++){
      // stroke(0,0,0);
      // if(hiddenNeuronsInputWeights[i][j] < 0){
      //   stroke(255,0,0);
      // }
      // strokeWeight(abs(hiddenNeuronsInputWeights[i][j])*5.0);
      stroke(128, round((hiddenNeuronsInputWeights[i][j]+1)*128), 128);
      line(113, j*80 + 50, 300 - 14, i*80 + 50);
      stroke(0,0,0);
    }
  }

  for(int i = 0; i < outputNeurons.length; i++){
    for(int j = 0; j < hiddenNeurons.length; j++){
      // stroke(0,0,0);
      // if(outputNeuronsInputWeights[i][j] < 0){
      //   stroke(255,0,0);
      // }
      // strokeWeight(abs(outputNeuronsInputWeights[i][j])*5.0);
      stroke(128, round((outputNeuronsInputWeights[i][j]+1)*128), 128);
      line(487, i*80 + 225, 313, j*80 + 50);
      stroke(0,0,0);
    }
  }

}
