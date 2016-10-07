#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <rpm/header.h>
#include <rpm/rpmlib.h>
#include <rpm/rpmdb.h>
#include <rpm/rpmts.h>

#include <harden/check.h>
#include <harden/report.h>
#include <harden/collector/rpm.h>

int package_installed(const char* name) {
  int found = 0;

  /* create RPM transaction */
  rpmts ts = rpmtsCreate();

  /* create an iterator that searches for a name */
  rpmdbMatchIterator mi = rpmtsInitIterator(ts, RPMTAG_NAME, name, 0);

  Header h;

  while((h = rpmdbNextIterator(mi)) != NULL) {
    found = 1;
    break;
  }

  rpmdbFreeIterator(mi);
  rpmtsFree(ts);
    
  return found;
}

void report_add_check_package_absence(struct report* r, const char* collection, const char* id, const char* summary, const char* pkgname) {
  struct check* c = check_new(collection, id, summary, CHECK_PASSED);

  if(package_installed(pkgname)) {
    check_add_findingf(c, "rpm package %s is installed but it should be absent", pkgname);
  }

  report_add_check(r, c);
}

int collector_rpm_evaluate(struct report* report) {

  rpmReadConfigFiles(NULL, NULL);

  report_add_check_package_absence(report, "cis", "1.4.4", "Remove SETroubleshoot", "setroubleshoot");
  report_add_check_package_absence(report, "cis", "1.4.5", "Remove MCS Translation Service (mcstrans)", "mcstrans");
  report_add_check_package_absence(report, "cis", "2.1.1", "Remove telnet-server", "telnet-server");
  report_add_check_package_absence(report, "cis", "2.1.2", "Remove telnet Clients", "telnet");
  report_add_check_package_absence(report, "cis", "2.1.3", "Remove rsh-server", "rsh-server");
  report_add_check_package_absence(report, "cis", "2.1.4", "Remove rsh", "rsh");
  report_add_check_package_absence(report, "cis", "2.1.5", "Remove NIS Client", "ypbind");
  report_add_check_package_absence(report, "cis", "2.1.6", "Remove NIS Server", "ypserv");
  report_add_check_package_absence(report, "cis", "2.1.7", "Remove tftp", "tftp");
  report_add_check_package_absence(report, "cis", "2.1.8", "Remove tftp-server", "tftp-server");
  report_add_check_package_absence(report, "cis", "2.1.9", "Remove talk", "talk");
  report_add_check_package_absence(report, "cis", "2.1.10", "Remove talk-server", "talk-server");
  report_add_check_package_absence(report, "cis", "2.1.11", "Remove xinetd", "xinetd");

  rpmFreeRpmrc();

  return 0;
}
