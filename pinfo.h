#ifndef BED954E2_F4DF_4097_8062_D563D1109188
#define BED954E2_F4DF_4097_8062_D563D1109188

#define PINFO_STATE_STR_MAX_LEN (16)
#define PINFO_NAME_MAX_LEN (16)

struct pinfo
{
    int pid;
    int priority;
    char stateStr[PINFO_NAME_MAX_LEN];
    char programNameStr[PINFO_NAME_MAX_LEN];
};


#endif /* BED954E2_F4DF_4097_8062_D563D1109188 */