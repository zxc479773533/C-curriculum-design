/*
* Part: Linklist algorithm library
* Module: Linklist statistics APIs achieve
* Author: zxc479773533@github.com
* LICENSE: GNU GENERAL PUBLIC LICENSE V3.0
*/

#ifndef __STATISTICS_
#define __STATISTICS_

#include "linklist.h"

#define INFINITE 9999999999

// get max of a, b
float Max(float a, float b) {
    return (a > b) ? a : b;
}

// get min of a, b
float Min(float a, float b) {
    return (a < b) ? a : b;
}

// Get all information of a line
void GetLineInfo(Linklist *L, Line LineInfo, char *payload) {

    // error code
    char error_info1[] = "This line is not exist!";

    FirstNode *Line = LocateLine(L, LineInfo);
    if (Line == NULL)
        strcpy(L->error, error_info1);

    else {

        // build response
        sprintf(payload, \
            "编号：%s\r\n" \
            "名称：%s\r\n" \
            "总站点数：%d\r\n" \
            "总长度：%.2f千米\r\n" \
            "总时间：%.2f分钟\r\n" \
            "起始站：%s\r\n" \
            "终点站：%s\r\n" \
            "负责人姓名：%s\r\n" \
            "负责人办公室电话：%s\r\n" \
            "负责人移动电话：%s\r\n" \
            "负责人电子邮箱：%s\r\n",
            Line->LineInfo.number,
            Line->LineInfo.name,
            Line->LineInfo.stations,
            Line->LineInfo.length,
            Line->LineInfo.total_time,
            Line->LineInfo.start_station,
            Line->LineInfo.end_station,
            Line->LineInfo.principal_name,
            Line->LineInfo.principal_tel,
            Line->LineInfo.principal_mobile,
            Line->LineInfo.principal_email
        );
    }

}

// Get all information of a station
void GetStationInfo(Linklist *L, Station StationInfo, char *payload) {

    // error code
    char error_info1[] = "This station is not exist!";

    SecondNode *Station = LocateStation(L, StationInfo);
    if (Station == NULL)
        strcpy(L->error, error_info1);

    else {

        // build response
        sprintf(payload,
            "所在路线编号：%s\r\n" \
            "在路线上的序号：%d\r\n" \
            "编号：%s\r\n" \
            "名称：%s\r\n" \
            "离起始站的距离：%.2f千米\r\n" \
            "离上个站点的距离：%.2f千米\r\n" \
            "从上个站点到该站点的时间：%.2f分钟\r\n" \
            "在该站点停的时间：%.2f分钟\r\n",
            Station->StationInfo.line_number,
            Station->StationInfo.No,
            Station->StationInfo.number,
            Station->StationInfo.name,
            Station->StationInfo.distance,
            Station->StationInfo.distance_to_before,
            Station->StationInfo.time_to_arrive,
            Station->StationInfo.time_to_stay
        );
    }

}

// Get all information of a car
void GetCarInfo(Linklist *L, Car CarInfo, char *payload) {

    // error code
    char error_info1[] = "This car is not exist!";

    ThirdNode *Car = LocateCar(L, CarInfo);
    if (Car == NULL)
        strcpy(L->error, error_info1);

    else {

        // build response
        sprintf(payload,
            "车牌号：%s\r\n" \
            "所在路线编号：%s\r\n" \
            "所在站点编号：%s\r\n" \
            "司机姓名：%s\r\n" \
            "司机移动电话：%s\r\n" \
            "汽车总容量：%.2f千克\r\n" \
            "在该站的卸货量：%.2f千克\r\n" \
            "在该站的载货量：%.2f千克\r\n" \
            "汽车剩余可载量：%.2f千克\r\n",
            Car->CarInfo.license_plate,
            Car->CarInfo.line_number,
            Car->CarInfo.station_number,
            Car->CarInfo.driver_name,
            Car->CarInfo.driver_mobile,
            Car->CarInfo.goods_list.total_capacity,
            Car->CarInfo.goods_list.unload,
            Car->CarInfo.goods_list.upload,
            Car->CarInfo.goods_list.available_capacity
        );
    }

}

// get max and min time, length information of lines
void MaxAndMin(Linklist *L, char *payload) {

    // error code
    char error_info1[] = "No lines exist!";

    if (L->head == NULL)
        strcpy(L->error, error_info1);

    else {

        float max_time = 0, max_length = 0;
        float min_time = INFINITE, min_length = INFINITE;

        // find the max and min
        FirstNode *tail_L = L->head;
        while (tail_L != NULL) {
            max_length = Max(max_length, tail_L->LineInfo.length);
            max_time = Max(max_time, tail_L->LineInfo.total_time);
            min_length = Min(min_length, tail_L->LineInfo.length);
            min_time = Min(min_time, tail_L->LineInfo.total_time);
            tail_L = tail_L->next;
        }

        // build the response
        tail_L = L->head;
        char maxlen[] = "最长路线长度：";
        sprintf(payload, "最长路线长度：%.2f\r\n\r\n", max_length);
        payload += sizeof(maxlen) + sizeof(max_length) + 4;
        while (tail_L != NULL) {
            if (tail_L->LineInfo.length == max_length) {
                sprintf(payload, "%s ", tail_L->LineInfo.name);
                payload += sizeof(tail_L->LineInfo.name) + 1;
            }
            tail_L = tail_L->next;
        }
        sprintf(payload, "\r\n\r\n");
        payload += 4;

        tail_L = L->head;
        char minlen[] = "最短路线长度：";
        sprintf(payload, "最短路线长度：%.2f\r\n\r\n", min_length);
        payload += sizeof(minlen) + sizeof(min_length) + 4;
        while (tail_L != NULL) {
            if (tail_L->LineInfo.length == min_length) {
                sprintf(payload, "%s ", tail_L->LineInfo.name);
                payload += sizeof(tail_L->LineInfo.name) + 1;
            }
            tail_L = tail_L->next;
        }
        sprintf(payload, "\r\n\r\n");
        payload += 4;

        tail_L = L->head;
        char maxt[] = "最长路线时间：";
        sprintf(payload, "最长路线时间：%.2f\r\n\r\n", max_time);
        payload += sizeof(maxt) + sizeof(max_time) + 4;
        while (tail_L != NULL) {
            if (tail_L->LineInfo.total_time == max_time) {
                sprintf(payload, "%s ", tail_L->LineInfo.name);
                payload += sizeof(tail_L->LineInfo.name) + 1;
            }
            tail_L = tail_L->next;
        }
        sprintf(payload, "\r\n\r\n");
        payload += 4;

        tail_L = L->head;
        char mint[] = "最短路线时间：";
        sprintf(payload, "最短路线时间：%.2f\r\n\r\n", min_time);
        payload += sizeof(mint) + sizeof(min_time) + 4;
        while (tail_L != NULL) {
            if (tail_L->LineInfo.total_time == min_time) {
                sprintf(payload, "%s ", tail_L->LineInfo.name);
                payload += sizeof(tail_L->LineInfo.name) + 1;
            }
            tail_L = tail_L->next;
        }
        sprintf(payload, "\r\n\r\n");
        payload += 4;
    }

}

// judge if the car is the lase in the line
int IsLast(SecondNode *tail_S, Car CarInfo) {
    while (tail_S != NULL) {
        ThirdNode *tail_C = tail_S->first_child;
        while (tail_C != NULL) {
            if (strcmp(tail_C->CarInfo.license_plate, CarInfo.license_plate) == 0)
                return 1;
            else
                tail_C = tail_C->next;
        }
        tail_S = tail_S->next;
    }
    return 0;
}

// statistical calculation
void Calculate(Linklist *L, Car CarInfo, char *payload) {

    // counts
    int count = 0;
    float max_upload = 0;
    float max_unload = 0;
    float total_unload = 0;
    float total_distance = 0;
    float total_time = 0;

    // initinal
    char stations[] = "经停站点：";
    char station[] = "站点：";
    char upload[] = "载货：";
    char unload[] = "卸货：";
    char maxupload[] = "最大载货：";
    char maxunload[] = "最大卸货：";
    char averageunload[] = "平均卸货：";
    char totallength[] = "总路程：";
    char totaltime[] = "总耗时：";
    sprintf(payload, "经停站点：\r\n");
    payload += sizeof(stations) + 2;

    FirstNode *tail_L = L->head;
    while (tail_L != NULL) {

        // a flag to find the start
        int flag = 0;

        SecondNode *tail_S = tail_L->first_child;
        while (tail_S != NULL) {

            ThirdNode *tail_C = tail_S->first_child;
            while (tail_C != NULL) {

                if (flag) {
                    total_distance += tail_S->StationInfo.distance_to_before;
                    total_time += tail_S->StationInfo.time_to_arrive;
                    total_time += tail_S->StationInfo.time_to_stay;
                }

                if (strcmp(tail_C->CarInfo.license_plate, CarInfo.license_plate) == 0) {

                    count += 1;
                    flag = 1;
                    total_unload += tail_C->CarInfo.goods_list.unload;
                    max_upload = Max(max_upload, tail_C->CarInfo.goods_list.upload);
                    max_unload = Max(max_unload, tail_C->CarInfo.goods_list.unload);

                    sprintf(payload, "站点：");
                    payload += sizeof(station);
                    sprintf(payload, "%s\r\n", tail_S->StationInfo.name);
                    payload += sizeof(tail_S->StationInfo.name) + 2;
                    sprintf(payload, "载货：%.2f, 卸货%.2f\r\n\r\n", tail_C->CarInfo.goods_list.upload, tail_C->CarInfo.goods_list.unload);
                    payload += sizeof(upload) + sizeof(tail_C->CarInfo.goods_list.upload) + 2;
                    payload += sizeof(unload) + sizeof(tail_C->CarInfo.goods_list.unload) + 4;
                }

                if (flag) {
                    flag = IsLast(tail_S->next, CarInfo);
                }

                tail_C = tail_C->next;
            }
            tail_S = tail_S->next;
        }
        tail_L = tail_L->next;
    }

    if (count != 0) {
        sprintf(payload, "最大载货：%.2f, 最大卸货：%.2f\r\n\r\n", max_upload, max_unload);
        payload += sizeof(maxupload) + sizeof(max_upload) + sizeof(maxunload) + sizeof(max_unload) + 6;
        sprintf(payload, "平均卸货：%.2f\r\n", total_unload * 1.0 / count);
        payload += sizeof(averageunload) + sizeof(total_unload * 1.0 / count) + 2;
        sprintf(payload, "总路程：%.2f, 总耗时：%.2f\r\n\r\n", total_distance, total_time);
    }

}

#endif // !__STATISTICS_
#define __STATISTICS_