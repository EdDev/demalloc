/*
 * statistic_histogram.cpp
 *
 */

#include "statistic_histogram.h"

#include <stdio.h>
#include <vector>


Statistic_histogram::Statistic_histogram(std::string h_name, const ssize_t domain_start, const ssize_t domain_end, const size_t range_size) :
    name(h_name),
    dstart(domain_start),
    dend(domain_end),
    rsize(range_size)
{
    for(ssize_t i = dstart; i < dend; i += rsize)
        hval.push_back(0);
}

void Statistic_histogram::inc_val(uint64_t val)
{
    int range = (val - dstart) / rsize;

    if(range < 0)
        range = 0;
    else if((unsigned)range >= hval.size())
        range = hval.size() - 1;

    ++hval[range];
}

size_t Statistic_histogram::report(char * report_buffer, size_t report_buffer_size)
{
    char * buf = report_buffer;
    int  len;

    len = snprintf(buf, report_buffer_size, "%s Report: (range = %lu)", name.c_str(), rsize);
    buf += len;
    report_buffer_size -= len;

    int entry_id = 0;
    for(hv_iterator range_itr = hval.begin(); range_itr != hval.end(); ++range_itr, ++entry_id)
    {
        if((0 == entry_id) || ( 0x0 == (entry_id & 0x07)))
        {
            len = snprintf(buf, report_buffer_size, "\n""%02u: ", entry_id);
            buf += len;
            report_buffer_size -= len;
        }

        len = snprintf(buf, report_buffer_size, "%8lu  ", *range_itr);
        buf += len;
        report_buffer_size -= len;
    }

    len = snprintf(buf, report_buffer_size, "\n");
    buf += len;
    report_buffer_size -= len;

    return (buf - report_buffer);
}


