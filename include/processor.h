#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // DONE: See src/processor.cpp


  // DONE: Declare any necessary private members
 private:
    float prevIdle_ = 0.0;
    float prevIoWait_ = 0.0;
    float prevUser_ = 0.0;
    float prevNice_ = 0.0; 
    float prevSystem_ = 0.0; 
    float prevIrq_ = 0.0;
    float prevSoftIrq_ = 0.0;
    float prevSteal_ = 0.0;
};

#endif