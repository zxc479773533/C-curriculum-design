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
    bzero(L->error, 50);
    L->head = NULL;
}

// set No. and total time of a line
void set_No_and_time(FirstNode *line) {

    // definations
    int count = 1;
    float total_time = 0;
    SecondNode *station = line->first_child, *pre = line->first_child;

    // settle first station
    strcpy(line->LineInfo.start_station, station->StationInfo.name);
    station->StationInfo.No = count;
    station->StationInfo.distance_to_before = 0;
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
        pre = pre->next;
        station = pre->next;
        count++;
    }

    strcpy(line->LineInfo.end_station, pre->StationInfo.name);
    line->LineInfo.stations = count - 1;
    line->LineInfo.total_time = total_time;
    line->LineInfo.length = pre->StationInfo.distance;
}

// set available capacity of a car in a line
void set_car_capacity(FirstNode *line, Car CarInfo) {

    //definations
    float available_capacity = CarInfo.goods_list.total_capacity;
    SecondNode *station = line->first_child;

    //traversal stations
    while (station != NULL) {

        // traversal cars
        ThirdNode *car = station->first_child;
        while (car != NULL && strcmp(car->CarInfo.license_plate, CarInfo.license_plate) != 0)
            car = car->next;

        // calculate
        if (car != NULL) {
            available_capacity += car->CarInfo.goods_list.unload;
            available_capacity -= car->CarInfo.goods_list.upload;
            car->CarInfo.goods_list.available_capacity = available_capacity;
        }
        station = station->next;
    }

}

// Insert a line in the linklist
void ListInsert_F(Linklist *L, Line LineInfo) {

    // initial line
    LineInfo.stations = 0;
    LineInfo.length = 0;
    LineInfo.total_time = 0;
    bzero(LineInfo.start_station, 10);
    bzero(LineInfo.end_station, 10);

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
    //bzero(L->error, 50);
}

// Insert a station in the linklist
void ListInsert_S(Linklist *L, Station StationInfo) {

    // error code
    char error_info1[] = "You should enter lines before!";
    char error_info2[] = "The line this station lie in is not exist!";

    // initial station
    StationInfo.No = 0;
    StationInfo.distance_to_before = 0;

    // if line do not exist
    if (L->head == NULL) {
        strcpy(L->error, error_info1);
    }

    // move to the line
    else {
        FirstNode *tail_L = L->head;
        while (tail_L != NULL && strcmp(tail_L->LineInfo.number, StationInfo.line_number) != 0)
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
                    if (tail_S->next == NULL) {
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
            //bzero(L->error, 50);
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

    // initial car
    CarInfo.goods_list.available_capacity = 0;

    // if line do not exist
    if (L->head  == NULL) {
        strcpy(L->error, error_info1);
    }

    // move to the line
    else {
        FirstNode *tail_L = L->head;
        while (tail_L != NULL && strcmp(tail_L->LineInfo.number, CarInfo.line_number) != 0)
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
                while (tail_S != NULL && strcmp(tail_S->StationInfo.number, CarInfo.station_number) != 0)
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
                    //bzero(L->error, 50);
                }

            }

        }

    }

}

// Return a node that contains the line with ordered information
FirstNode* LocateLine(Linklist *L, Line LineInfo) {

    FirstNode *tail_L = L->head;
    
    while (tail_L != NULL && strcmp(tail_L->LineInfo.number, LineInfo.number) != 0)
        tail_L = tail_L->next;

    return tail_L;
}

// Return a node that contains the station with ordered information
SecondNode* LocateStation(Linklist *L, Station StationInfo) {

    FirstNode *tail_L = L->head;

    while (tail_L != NULL && strcmp(tail_L->LineInfo.number, StationInfo.line_number) != 0)
        tail_L = tail_L->next;
    
    if (tail_L == NULL)
        return NULL;
    
    SecondNode *tail_S = tail_L->first_child;

    while (tail_S != NULL && strcmp(tail_S->StationInfo.number, StationInfo.number) != 0)
        tail_S = tail_S->next;

    return tail_S;
}

// Return a node that contains the car with ordered information
ThirdNode* LocateCar(Linklist *L, Car CarInfo) {

    FirstNode *tail_L = L->head;
    
    while (tail_L != NULL && strcmp(tail_L->LineInfo.number, CarInfo.line_number) != 0)
        tail_L = tail_L->next;

    if (tail_L == NULL)
        return NULL;

    SecondNode *tail_S = tail_L->first_child;

    while (tail_S != NULL && strcmp(tail_S->StationInfo.number, CarInfo.station_number) != 0)
        tail_S = tail_S->next;

    if (tail_S == NULL)
        return NULL;

    ThirdNode *tail_C = tail_S->first_child;

    while (tail_C != NULL && strcmp(tail_C->CarInfo.license_plate, CarInfo.license_plate) != 0)
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
        if (strcmp(LineInfo.name, "#") != 0)
            strcpy(Line->LineInfo.name, LineInfo.name);
        if (strcmp(LineInfo.principal_name, "#") != 0)
            strcpy(Line->LineInfo.principal_name, LineInfo.principal_name);
        if (strcmp(LineInfo.principal_tel, "#") != 0)
            strcpy(Line->LineInfo.principal_tel, LineInfo.principal_tel);
        if (strcmp(LineInfo.principal_mobile, "#") != 0)
            strcpy(Line->LineInfo.principal_mobile, LineInfo.principal_mobile);
        if (strcmp(LineInfo.principal_email, "#") != 0)
            strcpy(Line->LineInfo.principal_email, LineInfo.principal_email);
        //bzero(L->error, 50);
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
            if (strcmp(StationInfo.name, "#") != 0)
                strcpy(tail_S->StationInfo.name, StationInfo.name);
            if (StationInfo.time_to_arrive != -1)
                tail_S->StationInfo.time_to_arrive = StationInfo.time_to_arrive;
            if (StationInfo.time_to_stay != -1)
            tail_S->StationInfo.time_to_stay = StationInfo.time_to_stay;
            set_No_and_time(tail_L);
            //bzero(L->error, 50);
        }

    }

}

// Modify a cat's information
void ModifyCar(Linklist *L, Car CarInfo) {

    // error code
    char error_info1[] = "This car is not exist!";

    FirstNode *tail_L = L->head;
    while ( tail_L != NULL && strcmp(tail_L->LineInfo.number, CarInfo.line_number) != 0)
        tail_L = tail_L->next;

    if (tail_L == NULL)
        strcpy(L->error, error_info1);

    // find the line
    else {

        SecondNode *tail_S = tail_L->first_child;
        while (tail_S != NULL && strcmp(tail_S->StationInfo.number, CarInfo.station_number) != 0)
            tail_S = tail_S->next;

        if (tail_S == NULL)
            strcpy(L->error, error_info1);

        // find the station
        else {

            ThirdNode *tail_C = tail_S->first_child;
            while (tail_C != NULL && strcmp(tail_C->CarInfo.license_plate, CarInfo.license_plate) != 0)
                tail_C = tail_C->next;

            if (tail_C == NULL)
                strcpy(L->error, error_info1);

            // find the car
            else {
                if (strcmp(CarInfo.driver_name, "#") != 0)
                    strcpy(tail_C->CarInfo.driver_name, CarInfo.driver_name);
                if (strcmp(CarInfo.driver_mobile, "#") != 0)
                    strcpy(tail_C->CarInfo.driver_mobile, CarInfo.driver_mobile);
                if (CarInfo.goods_list.total_capacity != -1)
                    tail_C->CarInfo.goods_list.total_capacity = CarInfo.goods_list.total_capacity;
                if (CarInfo.goods_list.unload != -1)
                    tail_C->CarInfo.goods_list.unload = CarInfo.goods_list.unload;
                if (CarInfo.goods_list.upload != -1)
                    tail_C->CarInfo.goods_list.upload = CarInfo.goods_list.upload;
                set_car_capacity(tail_L, CarInfo);
                //bzero(L->error, 50);
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

        // if the car to be delete is the first car
        if (strcmp(tail_C->CarInfo.license_plate, CarInfo.license_plate) == 0) {
            tail_S->first_child = tail_C->next;
            free(Car);
        }

        else {
            while (strcmp(tail_C->next->CarInfo.license_plate, CarInfo.license_plate) != 0)
                tail_C = tail_C->next;
            tail_C->next = tail_C->next->next;
            free(Car);
        }
        //bzero(L->error, 50);
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

        // if the station to be delete is the first station
        if (strcmp(tail_S->StationInfo.number, StationInfo.number) == 0) {
            tail_L->first_child = tail_S->next;
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
        //bzero(L->error, 50);
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

        // if the line to be delete is the first line
        if (strcmp(tail_L->LineInfo.number, LineInfo.number) == 0) {
            L->head = tail_L->next;
            free(Line);
        }

        else {
            while(strcmp(tail_L->next->LineInfo.number, LineInfo.number) != 0)
                tail_L = tail_L->next;
            tail_L->next = tail_L->next->next;
            free(Line);
        }
        //bzero(L->error, 50);
    }

}

#endif // !__BASIC_