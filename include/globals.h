#include "main.h"
#define setMinAbs(num, bound) ((num > -(bound) && num < bound) ? ((num >= 0) ? bound : -(bound)) : num)

// Comp Specifications
extern bool skills;
extern bool elim;
extern bool qual;
extern bool redAlliance;
extern bool blueAlliance;
extern bool match;
extern int autoSelection;