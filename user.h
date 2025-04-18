#ifndef DA6E252B_1596_44C1_B0E2_CD15D533B981
#define DA6E252B_1596_44C1_B0E2_CD15D533B981

#include <pinfo.h>

struct stat;
struct rtcdate;

// system calls
int fork(void);
int exit(void) __attribute__((noreturn));
int wait(void);
int pipe(int *);
int write(int, const void *, int);
int read(int, void *, int);
int close(int);
int kill(int);
int exec(char *, char **);
int open(const char *, int);
int mknod(const char *, short, short);
int unlink(const char *);
int fstat(int fd, struct stat *);
int link(const char *, const char *);
int mkdir(const char *);
int chdir(const char *);
int dup(int);
int getpid(void);
char *sbrk(int);
int sleep(int);
int uptime(void);

int shutdown(void);

// This function returns information about processes know to the kernel.
// information is returned in an array of pinfo. pinfo must point to at
// least enough memory for numberOfEnties pinfos.
int ps(int numberOfEntries, struct pinfo *arrayOfPInfo);

int nice(int pid, int priority);

// ulib.c
int stat(const char *, struct stat *);
char *strcpy(char *, const char *);
void *memmove(void *, const void *, int);
char *strchr(const char *, char c);
int strcmp(const char *, const char *);
int32_t snprintf(char *outbuffer, int32_t n, const char *fmt, ...);
void printf(int, const char *, ...);
char *gets(char *, int max);
uint32_t strlen(const char *);
void *memset(void *, int, uint32_t);
void *malloc(uint32_t);
void free(void *);
int atoi(const char *);

#endif /* DA6E252B_1596_44C1_B0E2_CD15D533B981 */
