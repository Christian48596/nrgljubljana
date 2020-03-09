// portabil.h - Portability code. Low-level stuff belongs here.
// Copyright (C) 2005-2020 Rok Zitko

#ifndef _portabil_h_
#define _portabil_h_

using namespace std;

// Square raises x to the power of two
#ifndef HAVE_SQR
#define sqr(x) ((x) * (x))
#endif

#define BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED
#include <boost/stacktrace.hpp>

void print_trace() {
  cout << "Backtrace:" << endl << boost::stacktrace::stacktrace() << endl;
}

#define assert_isfinite(x) finite_test_fnc(x, __FILE__, __LINE__)

bool my_isfinite(double x) { return std::isfinite(x); }

using cmpl = complex<double>;

bool my_isfinite(cmpl z) { return std::isfinite(z.real()) && std::isfinite(z.imag()); }

inline int isfinite(cmpl z) { return (std::isfinite(z.real()) && std::isfinite(z.imag()) ? 1 : 0); }

template <typename T> T finite_test_fnc(T x, const char *file, int line) {
  if (!my_isfinite(x)) {
    cout << "#### EXITING DUE TO FAILED ASSERTION." << endl;
    cout << "File " << file << ", line " << line << "." << endl;
    cout << "Finiteness assertion" << endl;
    print_trace();
    exit(1);
  }
  return x;
}

#define my_assert(x)                                                                                                                                 \
  do {                                                                                                                                               \
    if (!(x)) {                                                                                                                                      \
      cout << "#### EXITING DUE TO FAILED ASSERTION." << endl;                                                                                       \
      cout << "File " << __FILE__ << ", line " << __LINE__ << "." << endl;                                                                           \
      cout << #x << endl;                                                                                                                       \
      print_trace();                                                                                                                                 \
      exit(1);                                                                                                                                       \
    }                                                                                                                                                \
  } while (0)

// Assert a==b
#define my_assert_equal(a, b)                                                                                                                        \
  do {                                                                                                                                               \
    if (!(a == b)) {                                                                                                                                 \
      cout << "#### EXITING DUE TO FAILED ASSERTION." << endl;                                                                                       \
      cout << "File " << __FILE__ << ", line " << __LINE__ << "." << endl;                                                                           \
      cout << #a << "=" << a << endl;                                                                                                         \
      cout << #b << "=" << b << endl;                                                                                                         \
      cout << "Exiting." << endl;                                                                                                                    \
      print_trace();                                                                                                                                 \
      exit(1);                                                                                                                                       \
    }                                                                                                                                                \
  } while (0)

#define my_warning(...)                                                                                                                              \
  do {                                                                                                                                               \
    cout << "Warning." << endl;                                                                                                                      \
    cout << "File " << __FILE__ << ", line " << __LINE__ << "." << endl;                                                                             \
    char buf[256];                                                                                                                                   \
    snprintf(buf, 256, __VA_ARGS__);                                                                                                                       \
    cout << buf << endl;                                                                                                                             \
    cout << "Continuing." << endl;                                                                                                                   \
  } while (0)

#define my_error(...)                                                                                                                                \
  do {                                                                                                                                               \
    cout << "#### EXITING DUE TO ERROR." << endl;                                                                                                    \
    cout << "File " << __FILE__ << ", line " << __LINE__ << "." << endl;                                                                             \
    char buf[256];                                                                                                                                   \
    snprintf(buf, 256, __VA_ARGS__);                                                                                                                       \
    cout << buf << endl;                                                                                                                             \
    print_trace();                                                                                                                                   \
    exit(1);                                                                                                                                         \
  } while (0)

#define my_assert_not_reached()                                                                                                                      \
  do {                                                                                                                                               \
    cout << "#### EXITING DUE TO FAILED ASSERTION." << endl;                                                                                         \
    cout << "File " << __FILE__ << ", line " << __LINE__ << "." << endl;                                                                             \
    cout << "Should never be reached." << endl;                                                                                                      \
    print_trace();                                                                                                                                   \
    exit(1);                                                                                                                                         \
  } while (0)

// Workaround
#ifndef __TIMESTAMP__
#define __TIMESTAMP__ ""
#endif

#ifndef ARRAYLENGTH
#define ARRAYLENGTH(x) (sizeof(x) / sizeof(*(x)))
#endif

// TO DO: std::clamp in C++17
#ifndef CLIP
#define CLIP(x, xmin, xmax) min(max(x, xmin), xmax)
#endif

// *** Memory usage

#if defined(__APPLE__) && defined(__MACH__)
#include <mach/mach_init.h>
#include <mach/task.h>

// Source: http://blog.kuriositaet.de/?p=257
int getmem(unsigned int *rss, unsigned int *vs) {
  struct task_basic_info t_info{};
  mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;

  if (KERN_SUCCESS != task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&t_info, &t_info_count)) { return -1; }
  *rss = t_info.resident_size;
  *vs  = t_info.virtual_size;
  return 0;
}

int memoryused() {
  unsigned int rss;
  unsigned int vs;

  getmem(&rss, &vs);

  return rss / 1024; // result in kB block of memory!
}
#define HAS_MEMORY_USAGE
#endif

#if defined(__linux)
#include <string.h>
#include <stdio.h>
// Returns the number of kB blocks of memory used by this process (self)
int memoryused() {
  int used = 0;

  // See /usr/src/linux/Documentation/filesystems/proc.txt
  if (FILE *info = fopen("/proc/self/status", "r")) {
    char buf[512];
    int val;

    while (fgets(buf, sizeof(buf), info)) {
      if (!strncmp(buf, "VmSize:", 7) && sscanf(buf + 7, "%d", &val)) used = val;
    }
    fclose(info);
  }

  return used;
}
#define HAS_MEMORY_USAGE
#endif

// Fall-back
#if !defined(HAS_MEMORY_USAGE)
int memoryused() { return 0; }
#endif

// More considerate way of opening output files:
// - check if we really want to open the file (non-empty filename)
// - check for open errors
ofstream safeopen(string filename, bool binary = false) {
  my_assert(filename != "");
  ios::openmode flag = ios::out;
  if (binary) flag |= ios::binary;
  ofstream F(filename.c_str(), flag);
  if (!F) my_error("safeopen: Can't open %s", filename.c_str());
  return F;
}

#if defined(__clang__) || defined (__GNUC__)
# define ATTRIBUTE_NO_SANITIZE_ADDRESS __attribute__((no_sanitize_address))
#else
# define ATTRIBUTE_NO_SANITIZE_ADDRESS
#endif

#if defined(__clang__) || defined (__GNUC__)
# define ATTRIBUTE_NO_SANITIZE_DIV_BY_ZERO __attribute__((no_sanitize("float-divide-by-zero")))
#else
# define ATTRIBUTE_NO_SANITIZE_DIV_BY_ZERO
#endif

#endif
