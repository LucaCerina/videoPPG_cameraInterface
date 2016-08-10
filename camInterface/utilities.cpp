#include "utilities.h"

uint calculateAge(QDate bDay, QDate currentDay)
{
    uint currentAge = currentDay.year() - bDay.year();
    if(bDay.month() > currentDay.month()
            || (bDay.month() == currentDay.month() && bDay.day() > currentDay.day()))
        currentAge--;
    return currentAge;
}
