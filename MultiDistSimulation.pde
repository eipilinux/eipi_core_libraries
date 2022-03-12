float sigmoid(float input){
  return exp(input)/(exp(input) + 1);
}

class perceptron{
  int id;
  
  perceptron(int uniqueID){
    id = uniqueID;
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
  
  float cycle(float[] inputs){
    return activation(sumInputs(inputs)); 
  }
  
  void display(int x, int y){
    circle(x, y, 25);
  }
}

perceptron[] neurons;

void setup(){
  size(1000, 1000);
  neurons = new perceptron[10];
  for(int i = 0; i < neurons.length; i++){
    neurons[i] = new perceptron(i);
  }
  noLoop();
}

void draw(){
   background(255,255,255);
   strokeWeight(3);
   stroke(0,0,0);
   //fill(0,0,0);
   for(int i = 0; i < neurons.length; i++){
     neurons[i].display(100, i*50 + 50);
   }
}
