/*
 * statistic_histogram.h
 *
 */

#ifndef STATISTIC_HISTOGRAM_H_
#define STATISTIC_HISTOGRAM_H_

#include <stdio.h>
#include <vector>

class Statistic_histogram
{
public:
    Statistic_histogram(std::string h_name, const ssize_t domain_start, const ssize_t domain_end, const size_t range_size);

    void inc_val(std::uint64_t val);
    size_t report(char * report_buffer, size_t report_buffer_size);

private:
    std::string  name;
    ssize_t dstart;
    ssize_t dend;
    size_t  rsize;

    typedef std::vector<std::uint64_t>::iterator hv_iterator;
    std::vector<std::uint64_t> hval;
};


#endif /* STATISTIC_HISTOGRAM_H_ */
