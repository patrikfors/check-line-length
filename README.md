#check-line-length
##Usage
    Usage: check-line-length [OPTION]... [FILE]...

    Check FILE(s), or standard input, for line lengths exceeding the limit

    Allowed options:
      --help                  produce help message
      --line-length arg (=80) set line length limit
      -v [ --verbose ]        increase verbosity level


### Return value
Returns 0 if file contains no lines that exceed the limit.


## Building
### Requirements
- libboost-program-options-dev
