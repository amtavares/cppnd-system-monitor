#include "processor.h"
#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() { 
    long totalOld, totalNew, activeNew, idleOld, idleNew;
    totalNew = CurrentTotal();
    activeNew = CurrentActive();
    idleNew = CurrentIdle();
    totalOld = PrevTotal();
    idleOld = PrevIdle();

    Update(idleNew, activeNew, totalNew);

    float totalDif = float(totalNew) - float(totalOld);
    float idleDif = float(idleNew) - float(idleOld);

    float utilization = (totalDif - idleDif)/totalDif;

    return utilization;

 }

 long Processor::CurrentTotal() {return LinuxParser:: Jiffies(); }
 long Processor::CurrentActive() {return LinuxParser::ActiveJiffies();}
 long Processor::CurrentIdle() {return LinuxParser::IdleJiffies();}
 long Processor::PrevTotal() {return total_; }
 long Processor::PrevActive() {return active_;}
 long Processor::PrevIdle() {return idle_; }
 void Processor::Update(long idle, long active, long total) {
     idle_ = idle;
     active_ = active;
     total_ = total;
 }


