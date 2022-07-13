#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    CvCapture* capture = cvCreateCameraCapture(0);
    if (!capture)
    {
        printf("Error in Video data\n");
        return 0;
    }
    int i = 0;
    struct timespec start, end, st0, st1, st2, st3, st4, st5, temp;
    printf("Name: time (sec), part-time with \"wait\", (part-time without \"wait\")\n");
    printf("======================================================\n");
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    while (1)
    {
        temp = st0;
        clock_gettime(CLOCK_MONOTONIC_RAW, &st0);
        printf("Full itaration: %lf\n\n", (st0.tv_sec - temp.tv_sec + 0.000000001 * (st0.tv_nsec - temp.tv_nsec)));
        i++;
        printf("Iteration %d: \n", i);
        clock_gettime(CLOCK_MONOTONIC_RAW, &st1);
        IplImage* frame = cvQueryFrame(capture);
        if (!frame)
        {
            printf("Error in Photo data\n");
            break;
        }
        clock_gettime(CLOCK_MONOTONIC_RAW, &st2);
        for (int y = 0; y < frame->height; y++)
        {
            uchar* ptr = (uchar*)(frame->imageData + y * frame->widthStep);
            for (int x = 0; x < frame->width; x++)
            {
                ptr[3 * x] = 255 - ptr[3 * x]; // Blue
                ptr[3 * x + 1] = 255 - ptr[3 * x + 1]; // Green
                ptr[3 * x + 2] = 255 - ptr[3 * x + 2]; // Red
            }
        }
        clock_gettime(CLOCK_MONOTONIC_RAW, &st3);
        cvShowImage("test", frame);
        clock_gettime(CLOCK_MONOTONIC_RAW, &st4);
        char c = cvWaitKey(1);
        clock_gettime(CLOCK_MONOTONIC_RAW, &st5);
        if (c == 27) break;
        double part_time = st5.tv_sec - st1.tv_sec + 0.000000001 * (st5.tv_nsec - st1.tv_nsec);
        double part_fps = 1.0 / part_time;
        double part_time_without_wait = st4.tv_sec - st1.tv_sec + 0.000000001 * (st4.tv_nsec - st1.tv_nsec);
        double time1 = st2.tv_sec - st1.tv_sec + 0.000000001 * (st2.tv_nsec - st1.tv_nsec);
        double time2 = st3.tv_sec - st2.tv_sec + 0.000000001 * (st3.tv_nsec - st2.tv_nsec);
        double time3 = st4.tv_sec - st3.tv_sec + 0.000000001 * (st4.tv_nsec - st3.tv_nsec);
        double time4 = st5.tv_sec - st4.tv_sec + 0.000000001 * (st5.tv_nsec - st4.tv_nsec);
        double sum_time1 = time1 + time2 + time3;
        double sum_time2 = sum_time1 + time4;
        printf("Get image:  %lf sec;  %lf, (%lf)\n", time1, (time1 / sum_time2), (time1 / sum_time1));
        printf("Edit image: %lf sec;  %lf, (%lf)\n", time2, (time2 / sum_time2), (time2 / sum_time1));
        printf("Show image: %lf sec;  %lf, (%lf)\n", time3, (time3 / sum_time2), (time3 / sum_time1));
        printf("Wait key:   %lf sec;  %lf\n", time4, (time4 / sum_time2));
        printf("Moment FPS: %lf, (%lf)\n", 1.0 / sum_time2, 1.0 / sum_time1);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    printf("======================================================\n");
    printf("Iterations: %d\n", i);
    double time = end.tv_sec - start.tv_sec + 0.000000001 * (end.tv_nsec - start.tv_nsec);
    printf("All time: %lf\n", time);
    double fps = (double)i / time;
    printf("FPS: %lf\n", fps);
    printf("Name: time (sec), part-time with \"wait\", (part-time without \"wait\")\n");
    cvReleaseCapture(&capture);
    cvDestroyWindow("test");
    return 0;
}