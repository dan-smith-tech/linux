#include <stdlib.h>

static const Block blocks[] = {
    /*Icon*/	     /*Command*/                                               /*Update Interval*/   /*Update Signal*/

    {"  ",  	     "top -bn1 -n1 | grep '%Cpu(s)' | awk '{print $2+$4+$6\"%  \"}'",      		         60,                 0},
    {"",	     "sensors | grep 'Tctl' | awk '{print substr($2, 2)}'",                      		 60,                 0}, // PC
    {"",	     "sensors | grep 'Package id 0' | awk '{print substr($4, 2)}'",                      	 60,                 0}, // Laptop
    {"      ",      "nvidia-smi --query-gpu=utilization.gpu --format=csv,noheader | sed 's/ //g'",              60,                 0}, // PC
    {"  ",	     "nvidia-smi --query-gpu=temperature.gpu --format=csv,noheader | awk '{print $1 \"°C\"}'", 	 60,                 0}, // PC
    {"      ",      "top -bn1 -n1 | grep 'MiB Mem' | awk '{printf \"%.1fGB\\n\", \$8 / 1024\"GB\"}'",	         60,                 0},
    {"      ",      "echo $(cat /sys/class/power_supply/BAT0/capacity)%",                    		         60,                 0}, // Laptop
    {"    󰥔  ",	     "date '+%H:%M %-d %B %Y'",                                          	                 60,                 0},
};

// amixer get Master | tail -n1 | sed -r 's/.*\\[(.*)%\\].*/\\1/'

static char delim[] = "\0";
static unsigned int delimLen = 0;
