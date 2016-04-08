/*

    Credit for this goes to https://github.com/jeboo/bypasslkm

*/

#include <jni.h>
#include <sys/syscall.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <android/log.h>


#define MODULE_NAME  "Bypass LKM"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, MODULE_NAME, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,MODULE_NAME, __VA_ARGS__)

// ATT SGH-I337 MK2
//unsigned long address = 0xC00B9C28;
// TMO SGH-M919 MK2
unsigned long address = 0xC00B9A8C;
// VZW SGH-I545 MJ7/MK2
//unsigned long address = 0xC00C9D58;
unsigned long value = 0x0;
unsigned long origvalue = 0x1a000002;

bool bChiled;

void ptrace_write_value_at_address(unsigned long int address, void *value) {
    pid_t pid;
    long ret;
    int status;

    bChiled = false;
    pid = fork();
    if (pid < 0) {
        return;
    }
    if (pid == 0) {
        ret = ptrace(PTRACE_TRACEME, 0, 0, 0);
        if (ret < 0) {
            LOGE("PTRACE_TRACEME failed\n");
        }
        bChiled = true;
        signal(SIGSTOP, SIG_IGN);
        kill(getpid(), SIGSTOP);
        exit(EXIT_SUCCESS);
    }

    do {
        ret = syscall(__NR_ptrace, PTRACE_PEEKDATA, pid, &bChiled, &bChiled);
    } while (!bChiled);

    ret = syscall(__NR_ptrace, PTRACE_PEEKDATA, pid, &value, (void *)address);
    if (ret < 0) {
        LOGE("PTRACE_PEEKDATA failed: %s\n", strerror(errno));
    }

    kill(pid, SIGKILL);
    waitpid(pid, &status, WNOHANG);
}

jlong Java_bypasslkm_bypasslkm_MainActivity_bypasslkm(JNIEnv* env, jobject thiz, jboolean restore) {
    unsigned long int patch;

    LOGI("\nPatching kernel @ 0x%X: ", address);
    if (restore) {
        LOGI("restoring original value.\n");
        patch = origvalue;
    } else {
        LOGI("unsigned modules can now be inserted.\n");
        patch = value;
    }

    ptrace_write_value_at_address(address, (void *)patch);

    LOGI("Done.\n\n");

    return 1;
}
