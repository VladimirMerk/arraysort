#include <napi.h>

void swap (unsigned int *a, unsigned int *b) {
  unsigned int temp;
  temp = *a;
  *a = *b;
  *b = temp;
}

void quickSort(unsigned int *array, unsigned int length) {
  unsigned int partition;
  unsigned int i, j;
  unsigned int rightLength, leftLength;
  unsigned int *rightArray, *leftArray;

  if (length < 2) {
    return;
  } 
  partition = *(array);
  i = 1;

  for (j = 1; j <= length; j++){
    if (*(array + j) < partition) {
      swap(array + i, array + j);
      i++;
    }
  }
  swap(array, array + i - 1);

  leftLength = i - 1;
  leftArray = array;
  rightArray = array + i;
  rightLength = length - i;

  quickSort(rightArray, rightLength);
  quickSort(leftArray, leftLength);
}

napi_value Sort(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Wrong number of arguments, expected 1").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsArray()) {
    Napi::TypeError::New(env, "Wrong first argument, should be an array").ThrowAsJavaScriptException();
    return env.Null();
  }
  const Napi::Array inputArray = info[0].As<Napi::Array>();
  unsigned int length = inputArray.Length();  
  std::vector<unsigned int> array;
  unsigned int i;

  for (i = 0; i < length; i++) {
    array.push_back(inputArray[i].As<Napi::Number>().Uint32Value());
  }
  
  unsigned int *arrayPointer = &array[0];
  quickSort(arrayPointer, length);
  Napi::Array outputArray = Napi::Array::New(env, length);
  for (i = 0; i < length; i++) {
    outputArray[i] = Napi::Number::New(env, double(array[i]));
  }
  
  return outputArray;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "sort"), Napi::Function::New(env, Sort));
  return exports;
}

NODE_API_MODULE(module, Init)
