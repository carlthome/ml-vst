
#pragma once

#include <tensorflow/lite/interpreter.h>

class TfLiteModel {
public:
  TfLiteModel(std::string name);
  ~TfLiteModel();
  void run(float *input, float *output, int bufferSize);

private:
  std::unique_ptr<tflite::Interpreter> interpreter;
};