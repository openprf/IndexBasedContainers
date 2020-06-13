#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <zconf.h>
#include <sys/mman.h>

#include "buffer.h"

/*
 * Basic Using of Buffer
 */
void test1() {
    printf("\nBasic usege:\n");
    void *buf = ibb_create_buffer(4, sizeof(int));

    if (!buf) {
        printf("Cannot create Buffer");
    }


    printf("allocated %u  bytes (size %d)\n", ibb_buffer_size(buf), ibb_size(buf));

    int *a = ibb_alloc(buf);
    int *b = ibb_alloc(buf);
    int *sum = ibb_alloc(buf);

    if (a && b && sum) {
        *a = 4;
        *b = 3;
        *sum = *a + *b;
        printf("%d + %d = %d\n", *a, *b, *sum);

        ibb_free(buf, sum);

        int *mul = ibb_alloc(buf);

        if (mul) {
            *mul = *a * *b;
            printf("%d * %d = %d\n", *a, *b, *mul);
        }
    }

    printf("elements: %u\n", ibb_size(buf));

    free(buf);

}

/*
 * Using list of allocated elements
 */
void test2() {
    printf("\nUsing list of allocated elements:\n");
    void *buf = ibb_create_buffer(15, sizeof(int));

    if (!buf) {
        printf("Cannot create Buffer");
    }

    for (int i = 10; i < 100; i += 10) {
        int *a = ibb_alloc(buf);
        if (a) *a = i;
    }

    size_t sz = ibb_size(buf);
    if (sz) {
        int *elems[sz];
        sz = ibb_get_allocated_ptrs(buf, (void **) elems);
        for (size_t j = 0; j < sz; j++) {
            printf("%d \n", *elems[j]);
        }
    }

}

struct user_data_t {
    char name[16];
    char surname[16];
    int phone;
};

/*
 * Storing Buffer in file
 */
void test3() {
    printf("\nStoring Buffer in file:\n");
    FILE *f = fopen("phone_book.bin", "w");
    if (!f) {
        printf("cannot open file");
        return;
    }

    void *buf = ibb_create_buffer(4, sizeof(struct user_data_t));
    if (!buf) {
        printf("Cannot create Buffer");
    }


    struct user_data_t *data = (struct user_data_t *) ibb_alloc(buf);
    if (data) {
        strcpy(data->name, "Piter");
        strcpy(data->surname, "Parker");
        data->phone = 12345;
    }

    data = (struct user_data_t *) ibb_alloc(buf);
    if (data) {
        strcpy(data->name, "Otto");
        strcpy(data->surname, "Octavius");
        data->phone = 54321;
    }

    fwrite(buf, ibb_buffer_size(buf), 1, f);
    fclose(f);
    free(buf);


    f = fopen("phone_book.bin", "r");
    if (!f) {
        printf("cannot reopen file");
        return;
    }

    size_t buf_size = ibb_get_buffer_memory_size(4, sizeof(struct user_data_t));
    void *ph_book = malloc(buf_size);
    size_t rd = fread(ph_book, buf_size, 1, f);
    fclose(f);

    if (rd) {
        struct user_data_t *data = (struct user_data_t *) ibb_alloc(ph_book);
        if (data) {
            strcpy(data->name, "Green");
            strcpy(data->surname, "Goblin");
            data->phone = 66666;
        }

        size_t sz = ibb_size(ph_book);
        if (sz) {
            struct user_data_t *elems[sz];
            sz = ibb_get_allocated_ptrs(ph_book, (void **) elems);
            for (size_t j = 0; j < sz; j++) {
                printf("%s | %s | %d\n", elems[j]->name, elems[j]->surname, elems[j]->phone);
            }
        }
    } else {
        printf("read fail! %u", rd);
    }

    free(ph_book);
}

/*
 * Mapping Buffer file in memory
 */
void test4() {
    printf("\nMapping Buffer file:\n");
    int fd = open("phone_book.bin", O_RDWR, 0);

    if (fd < 0) {
        printf("cannot reopen file (fd %d)\n", fd);
        return;
    }

    size_t buf_size = ibb_get_buffer_memory_size(4, sizeof(struct user_data_t));
    void *buf = mmap(0, buf_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (buf == MAP_FAILED) {
        printf("fail to mmap file (fd %d)", fd);
        return;
    }

    size_t sz = ibb_size(buf);
    if (sz) {
        struct user_data_t *elems[sz];
        sz = ibb_get_allocated_ptrs(buf, (void **) elems);
        for (size_t j = 0; j < sz; j++) {
            printf("%s | %s | %d\n", elems[j]->name, elems[j]->surname, elems[j]->phone);
        }
    }

    close(fd);
}

int main() {
    printf("Buffer tests:\n");

    test1();
    test2();
    test3();
    test4();

    return 0;
}
