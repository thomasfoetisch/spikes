
#include <sstream>
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <syslog.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

bool daemonized = false;

bool daemonize() {
  pid_t pid;

  pid = fork();
  if (pid < 0) exit(1);
  if (pid > 0) exit(0);

  if (setsid() < 0) exit(1);

  pid = fork();
  if (pid < 0) exit(1);
  if (pid > 0) exit(0);

  umask(0);
  chdir("~");

  int fd(open("/dev/null", O_RDWR));
  dup2(fd, 0), dup2(fd, 1); dup2(fd, 2);
  
  openlog("simulation daemon", LOG_PID, LOG_USER);
  syslog(LOG_NOTICE, "Simulation sent to the background.");

  daemonized = true;
  
  return true;
}


void handle_signal(int signal) {
  if (signal == SIGINT) {
    std::cout << std::endl
	      << "Working silently in the background."
	      << std::endl;
    daemonize();
  }
}

int main(int argc, char *argv[]) {
  struct sigaction sa;
  sa.sa_handler = &handle_signal;
  sa.sa_flags = SA_RESTART;

  sigfillset(&sa.sa_mask);
  if (sigaction(SIGINT, &sa, NULL) == -1)
    throw std::string("Cannot handle SIGINT.");

  int i(0);
  while (i < 100) {
    usleep(1000000);
    
    if (daemonized) {
      std::ostringstream oss; oss << "Simulation daemon running, it = " << i;
      syslog(LOG_WARNING, oss.str().c_str());
    } else {
      std::cout << "Simulation running, it = " << i << std::endl;
    }

    i += 1;
  }

  if (daemonized) {
    syslog(LOG_NOTICE, "Simulation daemon terminated.");
    closelog();
  } else {
    std::cout << "Simulation terminated." << std::endl;
  }    
  
  return 0;
}
