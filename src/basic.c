/*
* Part: Linklist algorithm library
* Module: Linklist basic APIs achieve
* Author: zxc479773533@github.com
* LICENSE: GNU GENERAL PUBLIC LICENSE V3.0
*/

#ifndef __BASIC_
#define __BASIC_

#include "linklist.h"

// initialize a NULL linklist 
void ListInitial(Linklist *L) {
    strcpy(L->error, "");
    L->head = NULL;
}

// set No. and total time of a line
void set_No_and_time(FirstNode *line) {

    // definations
    int count = 1;
    float total_time = 0;
    SecondNode *station = line->first_child, *pre = line->first_child;

    // settle first station
    strcpy(line->LineInfo.start_station, station->StationInfo.number);
    station->StationInfo.No = count;
    total_time += station->StationInfo.time_to_arrive;
    total_time += station->StationInfo.time_to_stay;
    station = station->next;
    count++;

    // traversal stations
    while (station != NULL) {
        station->StationInfo.No = count;
        total_time += station->StationInfo.time_to_arrive;
        total_time += station->StationInfo.time_to_stay;
        station->StationInfo.distance_to_before = station->StationInfo.distance - pre->StationInfo.distance;
        station = station->next;
        pre = pre->next;
        count++;
    }

    strcpy(line->LineInfo.end_station, pre->StationInfo.number);
    line->LineInfo.stations = count - 1;
    line->LineInfo.total_time = total_time;
}

// set available capacity of a car in a line
void set_car_capacity(FirstNode *line, Car CarInfo) {

    //definations
    float available_caoacity = CarInfo.goods_list.total_capacity;
    SecondNode *station = line->first_child;

    //traversal stations
    while (station != NULL) {

        // traversal cars
        ThirdNode *car = station->first_child;
        while (car != NULL && strcmp(car->CarInfo.license_plate, CarInfo.license_plate) != 0)
            car = car->next;

        // calculate
        if (car != NULL) {
            available_caoacity += car->CarInfo.goods_list.unload;
            available_caoacity -= car->CarInfo.goods_list.upload;
            car->CarInfo.goods_list.available_capacity = available_caoacity;
        }
        station = station->next;
    }

}

// Insert a line in the linklist
void ListInsert_F(Linklist *L, Line LineInfo) {

    // new linklist
    if (L->head == NULL) {
        L->head = (FirstNode*)malloc(sizeof(FirstNode));
        L->head->LineInfo = LineInfo;
        L->head->next = NULL;
    }

    // move to the end
    else {
        FirstNode *tail_L = L->head;
        while (tail_L->next != NULL)
            tail_L = tail_L->next;
        tail_L->next = (FirstNode*)malloc(sizeof(FirstNode));
        tail_L->next->LineInfo = LineInfo;
        tail_L->next->next = NULL;
    }
    strcpy(L->error, "");
}

// Insert a station in the linklist
void ListInsert_S(Linklist *L, Station StationInfo) {

    // error code
    char error_info1[] = "You should enter lines before!";
    char error_info2[] = "The line this station lie in is not exist!";

    // if line do not exist
    if (L->head == NULL) {
        strcpy(L->error, error_info1);
    }

    // move to the line
    else {
        FirstNode *tail_L = L->head;
        while (strcmp(tail_L->LineInfo.number, StationInfo.line_number) != 0 && tail_L != NULL)
            tail_L = tail_L->next;
        if (tail_L == NULL)
            strcpy(L->error, error_info2);

        // in the line
        else {

            // new line
            if (tail_L->first_child == NULL) {
                tail_L->first_child = (SecondNode*)malloc(sizeof(SecondNode));
                tail_L->first_child->StationInfo = StationInfo;
                tail_L->first_child->next = NULL;
            }

            // insert
            else {
                
                // if insert at first
                if (tail_L->first_child->StationInfo.distance > StationInfo.distance) {
                    SecondNode *new_node = (SecondNode *)malloc(sizeof(SecondNode));
                    new_node->StationInfo = StationInfo;
                    new_node->next = tail_L->first_child;
                    tail_L->first_child = new_node;
                }

                // find suitable position
                else {
                    SecondNode *tail_S = tail_L->first_child;
                    while (tail_S->next != NULL && tail_S->next->StationInfo.distance < StationInfo.distance)
                        tail_S = tail_S->next;

                    // if insert at end
                    if (tail_L->next == NULL) {
                        SecondNode *new_node = (SecondNode *)malloc(sizeof(SecondNode));
                        new_node->StationInfo = StationInfo;
                        new_node->next = NULL;
                        tail_S->next = new_node;
                    }

                    // in insert in intermediate
                    else {
                        SecondNode *new_node = (SecondNode *)malloc(sizeof(SecondNode));
                        new_node->StationInfo = StationInfo;
                        new_node->next = tail_S->next;
                        tail_S->next = new_node;
                    }

                }

            }
            set_No_and_time(tail_L);
            strcpy(L->error, "");
        }

    }

}

// Insert a car in the linklist
void ListInsert_T(Linklist *L, Car CarInfo) {

    // error code
    char error_info1[] = "You should enter lines before!";
    char error_info2[] = "You should enter stations before!";
    char error_info3[] = "The line this car stay in is not exist!";
    char error_info4[] = "The station this car stay in is not exist!";

    // if line do not exist
    if (L->head  == NULL) {
        strcpy(L->error, error_info1);
    }

    // move to the line
    else {
        FirstNode *tail_L = L->head;
        while (strcmp(tail_L->LineInfo.number, CarInfo.line_number) != 0 && tail_L != NULL)
            tail_L = tail_L->next;
        if (tail_L == NULL)
            strcpy(L->error, error_info3);
        
        // in the line
        else {

            // new line
            if (tail_L->first_child == NULL) {
                strcpy(L->error, error_info2);
            }                

            // move to the station
            else {
                SecondNode *tail_S = tail_L->first_child;
                while (strcmp(tail_S->StationInfo.number, CarInfo.station_number) != 0 && tail_S != NULL)
                    tail_S = tail_S->next;
                if (tail_S == NULL)
                    strcpy(L->error, error_info4);
                
                // in the station
                else {

                    // new station
                    if (tail_S->first_child == NULL) {
                        tail_S->first_child = (ThirdNode*)malloc(sizeof(ThirdNode));
                        tail_S->first_child->CarInfo = CarInfo;
                        tail_S->first_child->next = NULL;
                    }

                    // insert
                    else {
                        ThirdNode *tail_C = tail_S->first_child;
                        while (tail_C->next != NULL)
                            tail_C = tail_C->next;
                        tail_C->next = (ThirdNode*)malloc(sizeof(ThirdNode));
                        tail_C->next->CarInfo = CarInfo;
                        tail_C->next->next = NULL;
                    }
                    set_car_capacity(tail_L, CarInfo);
                    strcpy(L->error, "");
                }

            }

        }

    }

}

// Return a node that contains the line with ordered information
FirstNode* LocateLine(Linklist *L, Line LineInfo) {

    FirstNode *tail_L = L->head;
    
    while (strcmp(tail_L->LineInfo.number, LineInfo.number) != 0 && tail_L != NULL)
        tail_L = tail_L->next;

    return tail_L;
}

// Return a node that contains the station with ordered information
SecondNode* LocateStation(Linklist *L, Station StationInfo) {

    FirstNode *tail_L = L->head;

    while (strcmp(tail_L->LineInfo.number, StationInfo.line_number) != 0 && tail_L != NULL)
        tail_L = tail_L->next;
    
    if (tail_L == NULL)
        return NULL;
    
    SecondNode *tail_S = tail_L->first_child;

    while (strcmp(tail_S->StationInfo.number, StationInfo.number) != 0 && tail_S != NULL)
        tail_S = tail_S->next;

    return tail_S;
}

// Return a node that contains the car with ordered information
ThirdNode* LocateCar(Linklist *L, Car CarInfo) {

    FirstNode *tail_L = L->head;
    
    while (strcmp(tail_L->LineInfo.number, CarInfo.line_number) != 0 && tail_L != NULL)
        tail_L = tail_L->next;

    if (tail_L == NULL)
        return NULL;

    SecondNode *tail_S = tail_L->first_child;

    while (strcmp(tail_S->StationInfo.number, CarInfo.station_number) != 0 && tail_S != NULL)
        tail_S = tail_S->next;

    if (tail_S == NULL)
        return NULL;

    ThirdNode *tail_C = tail_S->first_child;

    while (strcmp(tail_C->CarInfo.license_plate, CarInfo.license_plate) != 0 && tail_C != NULL)
        tail_C = tail_C->next;

    return tail_C;
}

// Modify a line's information
void ModifyLine(Linklist *L, Line LineInfo) {

    // error code
    char error_info1[] = "This line is not exist!";

    FirstNode *Line =  LocateLine(L, LineInfo);
    if (Line == NULL)
        strcpy(L->error, error_info1);

    // find the line
    else {
        strcpy(Line->LineInfo.name,LineInfo.name);
        strcpy(Line->LineInfo.principal_name,LineInfo.principal_name);
        strcpy(Line->LineInfo.principal_tel,LineInfo.principal_tel);
        strcpy(Line->LineInfo.principal_mobile,LineInfo.principal_mobile);
        strcpy(Line->LineInfo.principal_email,LineInfo.principal_email);
        strcpy(L->error, "");
    }

}

// Modify a station's information
void ModifyStation(Linklist *L, Station StationInfo) {

    //error code
    char error_info1[] = "This station is not exist!";

    FirstNode *tail_L = L->head;
    while (strcmp(tail_L->LineInfo.number, StationInfo.line_number) != 0 && tail_L != NULL)
        tail_L = tail_L->next;

    if (tail_L == NULL)
        strcpy(L->error, error_info1);

    // find the line
    else {

        SecondNode *tail_S = tail_L->first_child;
        while (strcmp(tail_S->StationInfo.number, StationInfo.number) != 0 && tail_S != NULL)
            tail_S = tail_S->next;

        if (tail_S == NULL)
            strcpy(L->error, error_info1);

        // find the station
        else  {
            strcpy(tail_S->StationInfo.name, StationInfo.name);
            tail_S->StationInfo.time_to_arrive = StationInfo.time_to_arrive;
            tail_S->StationInfo.time_to_stay = StationInfo.time_to_stay;
            set_No_and_time(tail_L);
            strcpy(L->error, "");
        }

    }

}

// Modify a cat's information
void ModifyCar(Linklist *L, Car CarInfo) {

    // error code
    char error_info1[] = "This car is not exist!";

    FirstNode *tail_L = L->head;
    while (strcmp(tail_L->LineInfo.number, CarInfo.line_number) != 0 && tail_L != NULL)
        tail_L = tail_L->next;

    if (tail_L == NULL)
        strcpy(L->error, error_info1);

    // find the line
    else {

        SecondNode *tail_S = tail_L->first_child;
        while (strcmp(tail_S->StationInfo.number, CarInfo.station_number) != 0 && tail_S != NULL)
            tail_S = tail_S->next;

        if (tail_S == NULL)
            strcpy(L->error, error_info1);

        // find the station
        else {

            ThirdNode *tail_C = tail_S->first_child;
            while (strcmp(tail_C->CarInfo.license_plate, CarInfo.license_plate) != 0 && tail_C != NULL)
                tail_C = tail_C->next;

            if (tail_C == NULL)
                strcpy(L->error, error_info1);

            // find the car
            else {
                strcpy(tail_C->CarInfo.driver_name, CarInfo.driver_name);
                strcpy(tail_C->CarInfo.driver_mobile, CarInfo.driver_mobile);
                tail_C->CarInfo.goods_list.unload = CarInfo.goods_list.unload;
                tail_C->CarInfo.goods_list.upload = CarInfo.goods_list.upload;
                set_car_capacity(tail_L, CarInfo);
                strcpy(L->error, "");
            }

        }

    }

}

// Delete a car
void DeleteCar(Linklist *L, Car CarInfo) {

    // error code
    char error_info1[] = "This car is not exist!";

    ThirdNode *Car = LocateCar(L, CarInfo);
    if (Car == NULL)
        strcpy(L->error, error_info1);

    else {

        FirstNode *tail_L = L->head;
        while (strcmp(tail_L->LineInfo.number, CarInfo.line_number) != 0)
            tail_L = tail_L->next;

        SecondNode *tail_S = tail_L->first_child;
        while (strcmp(tail_S->StationInfo.number, CarInfo.station_number) != 0)
            tail_S = tail_S->next;

        ThirdNode *tail_C = tail_S->first_child;

        // if the car to be delete is the only car
        if (tail_C->next == NULL) {
            tail_S->first_child = NULL;
            free(Car);
        }

        else {
            while (strcmp(tail_C->next->CarInfo.license_plate, CarInfo.license_plate) != 0)
                tail_C = tail_C->next;
            tail_C->next = tail_C->next->next;
            free(Car);
        }
        strcpy(L->error, "");
    }

}

// Delete a Station
void DeleteStation(Linklist *L, Station StationInfo) {

    // error code
    char error_info1[] = "This station is not exist!";

    SecondNode *Station = LocateStation(L, StationInfo);
    if (Station == NULL)
        strcpy(L->error, error_info1);

    else {

        FirstNode *tail_L = L->head;
        while (strcmp(tail_L->LineInfo.number, StationInfo.line_number) != 0)
            tail_L = tail_L->next;

        // delete cars in the station
        ThirdNode *tail_C = Station->first_child;
        while (tail_C != NULL) {
            ThirdNode *tmp = tail_C;
            tail_C = tail_C->next;
            free(tmp);
        }

        SecondNode *tail_S = tail_L->first_child;

        // if the station to be delete is the only station
        if (tail_S->next == NULL) {
            tail_L->first_child = NULL;
            free(Station);
        }

        else {
            while (strcmp(tail_S->next->StationInfo.number, StationInfo.number) != 0)
                tail_S = tail_S->next;
            tail_S->next->next->StationInfo.time_to_arrive += Station->StationInfo.time_to_arrive;
            tail_S->next = tail_S->next->next;
            free(Station);
        }
        set_No_and_time(tail_L);
        strcpy(L->error, "");
    }

}

// Delete a line
void DeleteLine(Linklist *L, Line LineInfo) {

    // error code
    char error_info1[] = "This line is not exist!";

    FirstNode *Line = LocateLine(L, LineInfo);
    if (Line == NULL)
        strcpy(L->error, error_info1);

    else {

        // delete cars
        SecondNode *tail_S = Line->first_child;
        while (tail_S != NULL) {

            ThirdNode *tail_C = tail_S->first_child;
            while (tail_C != NULL) {
                ThirdNode *tmp =  tail_C;
                tail_C = tail_C->next;
                free(tmp);
            }
            tail_S = tail_S->next;
        }

        // delete stations
        tail_S = Line->first_child;
        while (tail_S != NULL) {
            SecondNode *tmp = tail_S;
            tail_S = tail_S->next;
            free(tmp);
        }

        FirstNode *tail_L = L->head;

        // if the line to be delete is the only line
        if (tail_L->next == NULL) {
            L->head = NULL;
            free(Line);
        }

        else {
            while(strcmp(tail_L->next->LineInfo.number, LineInfo.number) != 0)
                tail_L = tail_L->next;
            tail_L->next = tail_L->next->next;
            free(Line);
        }
        strcpy(L->error, "");
    }

}

#endif // !__BASIC_