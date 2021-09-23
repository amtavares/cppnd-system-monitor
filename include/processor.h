#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // DONE: See src/processor.cpp
  long CurrentTotal();
  long CurrentActive();
  long CurrentIdle();
  long PrevTotal();
  long PrevIdle();
  long PrevActive();
  void Update(long idle, long active, long total);

  // DONE: Declare any necessary private members
 private:
    long idle_;
    long active_;
    long total_;
};

#endif