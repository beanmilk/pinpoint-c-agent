//
// Created by Bluse on 12/8/2019.
//

#include "SharedObj.h"
#include <string.h>
#include <assert.h>


#ifdef __linux__
#define SHARE_OBJ_NAME "/dev/shm/pinpoint-php.shm"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>

typedef struct shared_object_s{
    int      sha_fd;
    void*  region;
    int     length;
    const char *address;
}SharedObject_T;

static SharedObject_T object;

// also this is not thread_safe version, but works fine
bool pre_init_shared_object()
{
    object.address = SHARE_OBJ_NAME;
    mode_t mode = S_IRUSR |S_IWUSR|S_IRGRP|S_IWGRP;
    int fd = open(object.address,O_WRONLY | O_CREAT,mode);
    if( fd == -1)
    {
        pp_trace("%s: open address with:%s \n",__FUNCTION__,strerror(errno));
        return false;
    }
    
    struct stat _stat;
    size_t length;
    fstat(fd,&_stat);
    length = _stat.st_size;
    if(length == 0)
    {
        int64_t pagesize = sysconf(_SC_PAGESIZE);
        ftruncate(fd,pagesize);
    }
    close(fd);

    return true;
}

bool init_shared_obj()
{
    if(object.region != NULL || object.address == NULL )
    {
        return true;
    }

    int fd = open(object.address,O_RDWR);
    if(fd == -1)
    {
        pp_trace("attach file:[%s] with:[%s]",object.address,strerror(errno));
        return false;
    }

    struct stat _stat;
    size_t length;
    fstat(fd,&_stat);
    length = _stat.st_size;

    void* addr = mmap(NULL,length,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(addr == (void*)-1)
    {
        close(fd);
        pp_trace("mmap file:[%s] with:[%s]",object.address,strerror(errno));
        return false;
    }

    object.region = addr;
    object.length = length;
    close(fd);
    return true;

}

void detach_shared_obj()
{
    munmap(object.region,object.length);
}

#elif _WIN32
#include <windows.h>

typedef struct shared_object_s{
    HANDLE hMap;
    LPVOID region;
    int length;
    const char *address;
}SharedObject_T;

static SharedObject_T object;

#define SHARE_OBJ_NAME "pinpoint-php.shm"


bool pre_init_shared_object()
{
    memset(&object,0, sizeof(SharedObject_T));
    object.address = SHARE_OBJ_NAME;
    return true;
}

bool init_shared_obj()
{
    assert(object.address);

    LPVOID pBuffer = NULL;
    HANDLE hMap = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, object.address);
    if(hMap == NULL)
    {
        hMap = ::CreateFileMapping(INVALID_HANDLE_VALUE,
                                   NULL,PAGE_READWRITE, 0,4096,object.address);
        if(hMap == NULL)
        {
            goto FAILED;
        }
        pBuffer = ::MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        if(pBuffer == NULL){
            goto FAILED;
        }
        object.region = pBuffer;
    }else{
        object.hMap =hMap;
        pBuffer = ::MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        if(pBuffer == NULL){
            goto FAILED;
        }
        object.region = pBuffer;
        object.length = 4096;
    };

    return true;
FAILED:
    if(hMap){
        ::CloseHandle(hMap);
    }

    //::UnmapViewOfFile(pBuffer);

    pp_trace("init_shared_obj failed with [%ld]",GetLastError());
    return false;
}


void detach_shared_obj()
{

    ::UnmapViewOfFile(object.region);
    ::CloseHandle(object.hMap);
}


#else

#error "not support platform"
#endif


void* fetch_shared_obj_addr()
{
    return object.region;
}


int  fetch_shared_obj_length()
{
    return object.length;
}


bool checking_and_init()
{
    if(object.region == NULL){
        if(pre_init_shared_object() && init_shared_obj()){
            return true;
        }else{
            return false;
        }
    }
    return true;
}



