#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;


bool g_verbose = false;

// A helper function to allow cout << vector
template<class T>
ostream& operator<< (ostream& os, const vector<T>& v)
{
  copy (v.begin(), v.end(), ostream_iterator<T> (cout, " "));
  return os;
}


bool process_stream (
  istream& is,
  string const filename,
  unsigned int line_length_limit)
{
  string line;
  unsigned int line_no = 1;
  bool found_error=false;

  while (is) {
    getline (is, line);
    if (line.length() > line_length_limit) {
      if (g_verbose)
        cout << filename << ": " << line << endl;
      cout << filename << ":" << line_no
           << ": line exceeds length limit." << endl;
      found_error=true;
    }
    line_no++;
  }

  return found_error;
}

int main (int argc, char* argv[])
{
  unsigned int line_length_limit;
  vector<string> input_files;

  // These are the options that will be displayed in help
  po::options_description visible_options (
    "Usage: check-line-length [OPTION]... [FILE]...\n"
    "\n"
    "Check FILE(s), or standard input, for line lengths exceeding the limit\n"
    "\n"
    "Allowed options");
  visible_options.add_options()
  ("help", "produce help message")
  ("line-length",
   po::value<unsigned int> (&line_length_limit)->default_value (80),
   "set line length limit")
  ("verbose,v",
   po::value<bool> (&g_verbose)->zero_tokens()->implicit_value (true),
   "increase verbosity level")
  ;

  // This options_description contains the positional options (input files)
  po::options_description hidden_options ("Hidden options");
  hidden_options.add_options()
  ("input-file", po::value< vector<string> > (&input_files), "input file")
  ;
  po::positional_options_description pd;
  pd.add ("input-file", -1);   // make positional args count as input-file

  // Combine the two above options_description and use this for parsing
  po::options_description cmdline_options ("Cmdline");
  cmdline_options.add (visible_options).add (hidden_options);

  
  // parse command line options, exit on error
  po::variables_map vm;
  try {
    po::store (po::command_line_parser (argc, argv).
               options (cmdline_options).positional (pd).run(), vm);

    po::notify (vm);
  }
  catch (exception& e) {
    cout << e.what() << "\n";
    return 2;
  }

  // display help, exit
  if (vm.count ("help")) {
    cout << visible_options << "\n";
    return 1;
  }

  bool found_error=false;
  // check line length in specified files or from standard input
  if (vm.count ("input-file")) {
    if (g_verbose)
      cout << "Input files are: "
           << vm["input-file"].as< vector<string> >() << "\n";

    for (vector<string>::const_iterator i = input_files.begin();
         i != input_files.end();
         i++) {
      ifstream in (i->c_str());

      if (!in)
        cerr << "could not open " << *i << "." << endl;
      else
        found_error |= process_stream (in, *i, line_length_limit);
    }
  }
  else {
    found_error = process_stream (cin, "stdin", line_length_limit);
  }

  if (found_error)
    return 2;

  return 0;
}
