/* cfifo �ӿڶ��� 
 * qq: 25088970 maohw
 */

#ifndef _LINUX_CFIFO_H
#define _LINUX_CFIFO_H

#ifdef __cplusplus
extern "C" {
#endif

#define CFIFO_MIN(x,y) ({ \
        typeof(x) _x = (x);     \
        typeof(y) _y = (y);     \
        (void) (&_x == &_y);    \
        _x < _y ? _x : _y; })

typedef unsigned int(*cfifo_recsize_t)(unsigned char *p1, unsigned int n1, unsigned char *p2);
typedef unsigned int(*cfifo_rectag_t)(unsigned char *p1, unsigned int n1, unsigned char *p2);
typedef unsigned int(*cfifo_recput_t)(unsigned char *p1, unsigned int n1, unsigned char *p2, void *u);
typedef unsigned int(*cfifo_recgut_t)(unsigned char *p1, unsigned int n1, unsigned char *p2, void *u);
typedef unsigned int(*cfifo_recrel_t)(unsigned char *p1, unsigned int n1, unsigned char *p2);

struct cfifo_reader {
    unsigned int out;
};

// cfifo op
struct cfifo_ex;

// create 
struct cfifo_ex *cfifo_alloc(unsigned int size
                        , cfifo_recsize_t rec_size    /* parse recsize */
                        , cfifo_rectag_t rec_tag      /* parse rectag  */
                        , cfifo_recrel_t rec_rel      /* release rec   */
                        , int *shmid);
// shmat
struct cfifo_ex *cfifo_shmat(cfifo_recsize_t rec_size
                        , cfifo_rectag_t rec_tag
                        , int shmid);
// free                
void cfifo_free(struct cfifo_ex *fifo_ex);

// newest
void cfifo_newest(struct cfifo_ex *fifo_ex
                , struct cfifo_reader *r
                , unsigned int tag);
// oldest
void cfifo_oldest(struct cfifo_ex *fifo_ex
                , struct cfifo_reader *r
                , unsigned int tag);
// put
signed int cfifo_put(struct cfifo_ex *fifo_ex
                    , unsigned int len
                    , cfifo_recput_t put_cb, void *u);
// get
signed int cfifo_get(struct cfifo_ex *fifo_ex
                    , struct cfifo_reader *r
                    , cfifo_recgut_t get_cb, void *u);

// epoll

enum {
  CFIFO_EP_ADD = 0,
  CFIFO_EP_DEL = 1,
};

// create
signed int cfifo_ep_alloc(int size);

// free
void cfifo_ep_free(int epoll_fd);

// add & del
signed int cfifo_ep_ctl(int epoll_fd, int op, struct cfifo_ex *fifo_ex);

// wait
signed int cfifo_ep_wait(int epoll_fd, int to, struct cfifo_ex **result, int n);


// info
void cfifo_info(struct cfifo_ex *fifo_ex
                , unsigned int out);

#ifdef __cplusplus
}
#endif

#endif
