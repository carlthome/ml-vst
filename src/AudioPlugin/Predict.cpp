#include "Predict.h"

#include "Models.h"

#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/kernels/register.h>
#include <tensorflow/lite/model.h>
#include <tensorflow/lite/optional_debug_tools.h>

#define ASSERT(x)                                                              \
  if (!(x)) {                                                                  \
    fprintf(stderr, "Error at %s:%d\n", __FILE__, __LINE__);                   \
    exit(1);                                                                   \
  }

TfLiteModel::TfLiteModel(std::string name) {

  // Load model.
  int size = 0;
  const auto buffer = BinaryData::getNamedResource(name.c_str(), size);
  const auto model = tflite::FlatBufferModel::VerifyAndBuildFromBuffer(
      buffer, static_cast<size_t>(size));

  // Build TensorFlow Lite interpreter.
  tflite::ops::builtin::BuiltinOpResolver resolver;
  tflite::InterpreterBuilder builder(*model, resolver);
  builder(&this->interpreter);
  ASSERT(interpreter != nullptr);

  // Allocate tensor buffers.
  ASSERT(interpreter->AllocateTensors() == kTfLiteOk);
  tflite::PrintInterpreterState(interpreter.get());
}

TfLiteModel::~TfLiteModel() {
  // TODO Destroy interpreter (if needed?).
  tflite::PrintInterpreterState(interpreter.get());
}

void TfLiteModel::run(float *input, float *output, int bufferSize) {

  // Fill input buffers.
  float *inputTensor = interpreter->typed_input_tensor<float>(0);
  for (int i = 0; i < bufferSize; i++) {
    inputTensor[i] = input[i];
  }

  // Run inference.
  ASSERT(interpreter->Invoke() == kTfLiteOk);

  // Read output buffer.
  float *outputTensor = interpreter->typed_output_tensor<float>(1);
  for (int i = 0; i < bufferSize; i++) {
    output[i] = outputTensor[i];
  }
}
