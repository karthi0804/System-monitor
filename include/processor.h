#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  double Utilization();  // DONE: See src/processor.cpp

  // DONE: Declare any necessary private members
 private:
 float prev_total_{0};
 float prev_idle_{0};
};

#endif