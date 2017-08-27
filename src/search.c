/*
* Part: Linklist algorithm library
* Module: Linklist search APIs achieve
* Author: zxc479773533@github.com
* LICENSE: GNU GENERAL PUBLIC LICENSE V3.0
*/

#ifndef __INQUIRE_
#define __INQUIRE_

#include "linklist.h"

// compare string
int my_strcmp(char *str1, char *str2) {
    if (strcmp(str2, "#") == 0)
        return 0;
    else
        return strcmp(str1, str2);
}

// compare float
int floatcmp(float f1, float f2) {
    if (f1 - f2 > -1e-3 && f1 - f2 < 1e-3)
        return 0;
    else
        return 1;
}

// compare number
int my_numcmp(float num1, float num2) {
    if (floatcmp(num2, -1) == 0)
        return 0;
    else
        return floatcmp(num1, num2);
}

// compare line A and B
int LineCmp(Line LineA, Line LineB) {

    if (my_strcmp(LineA.number, LineB.number) != 0)
        return -1;
    if (my_strcmp(LineA.name, LineB.name) != 0)
        return -1;
    if (my_strcmp(LineA.principal_name, LineB.principal_name) != 0)
        return -1;
    if (my_strcmp(LineA.principal_tel, LineB.principal_tel) != 0)
        return -1;
    if (my_strcmp(LineA.principal_mobile, LineB.principal_mobile) != 0)
        return -1;
    if (my_strcmp(LineA.principal_email, LineB.principal_email) != 0)
        return -1;

    return 0;
}

// compare station A and B
int StationCmp(Station StationA, Station StationB) {

    if (my_strcmp(StationA.line_number, StationB.line_number) != 0)
        return -1;
    if (my_strcmp(StationA.number, StationB.number) != 0)
        return -1;
    if (my_strcmp(StationA.name, StationB.name) != 0)
        return -1;
    if (my_numcmp(StationA.distance, StationB.distance) != 0)
        return -1;
    if (my_numcmp(StationA.time_to_arrive, StationB.time_to_arrive) != 0)
        return -1;
    if (my_numcmp(StationA.time_to_stay, StationB.time_to_stay) != 0)
        return -1;

    return 0;
}

// compare car A and B
int CarCmp(Car CarA, Car CarB) {

    if (my_strcmp(CarA.license_plate, CarB.license_plate) != 0)
        return -1;
    if (my_strcmp(CarA.line_number, CarB.line_number) != 0)
        return -1;
    if (my_strcmp(CarA.station_number, CarB.station_number) != 0)
        return -1;
    if (my_strcmp(CarA.driver_name, CarB.driver_name) != 0)
        return -1;
    if (my_strcmp(CarA.driver_mobile, CarB.driver_mobile) != 0)
        return -1;
    if (my_numcmp(CarA.goods_list.total_capacity, CarB.goods_list.total_capacity) != 0)
        return -1;
    if (my_numcmp(CarA.goods_list.available_capacity, CarB.goods_list.available_capacity) != 0)
        return -1;

    return 0;
}

// search lines
void SearchLines(Linklist *L, Line LineInfo, char *payload) {

    char number[] = "编号：";
    FirstNode *tail_L = L->head;

    while (tail_L != NULL) {

        if (LineCmp(tail_L->LineInfo, LineInfo) == 0) {
            sprintf(payload, "编号：");
            payload += sizeof(number);
            sprintf(payload, "%s\r\n", tail_L->LineInfo.number);
            payload += sizeof(tail_L->LineInfo.number) + 2;
        }
        tail_L = tail_L->next;
    }

}

// search stations
void SearchStations(Linklist *L, Station StationInfo, char *payload) {

    char line[] = "路线：";
    char number[] = "编号：";
    FirstNode *tail_L = L->head;

    while (tail_L != NULL) {
        SecondNode *tail_S = tail_L->first_child;
        while (tail_S != NULL) {

            if (StationCmp(tail_S->StationInfo, StationInfo) == 0) {
                sprintf(payload, "路线：");
                payload += sizeof(line);
                sprintf(payload, "%s\r\n", tail_L->LineInfo.number);
                payload += sizeof(tail_L->LineInfo.number) + 2;
                sprintf(payload, "编号：");
                payload += sizeof(number);
                sprintf(payload, "%s\r\n\r\n", tail_S->StationInfo.number);
                payload += sizeof(tail_S->StationInfo.number) + 4;
            }
            tail_S = tail_S->next;
        }
        tail_L = tail_L->next;
    }
}

// search cars
void SearchCars(Linklist *L, Car CarInfo, char *payload) {

    char line[] = "路线：";
    char station[] = "站点：";
    char license[] = "车牌号";
    FirstNode *tail_L = L->head;

    while (tail_L != NULL) {
        SecondNode *tail_S = tail_L->first_child;
        while (tail_S != NULL) {
            ThirdNode *tail_C = tail_S->first_child;
            while (tail_C != NULL)  {

                if (CarCmp(tail_C->CarInfo, CarInfo) == 0) {
                    sprintf(payload, "路线：");
                    payload += sizeof(line);
                    sprintf(payload, "%s\r\n", tail_L->LineInfo.number);
                    payload += sizeof(tail_L->LineInfo.number) + 2;
                    sprintf(payload, "站点：");
                    payload += sizeof(station);
                    sprintf(payload, "%s\r\n", tail_S->StationInfo.number);
                    payload += sizeof(tail_S->StationInfo.number) + 2;
                    sprintf(payload, "车牌号");
                    payload += sizeof(license);
                    sprintf(payload, "%s\r\n\r\n", tail_C->CarInfo.license_plate);
                    payload += sizeof(tail_C->CarInfo.license_plate) + 4;
                }
                tail_C = tail_C->next;
            }
            tail_S = tail_S->next;
        }
        tail_L = tail_L->next;
    }
}

#endif // !__INQUIRE_