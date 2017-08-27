/*
* Part: Linklist algorithm library
*
* Linklist APIs head file
*
* Author: zxc479773533@github.com
* LICENSE: GNU GENERAL PUBLIC LICENSE V3.0
*/

#ifndef __LINKLIST_H_
#define __LINKLIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* struct definations */

// distribution lines
typedef struct {
    // initail
    char number[6];
    char name[20];
    // changeable
    short stations;
    float length; //kilometers
    float total_time; //minutes
    char start_station[10];
    char end_station[10];
    // about principle
    char principal_name[20];
    char principal_tel[8];
    char principal_mobile[12];
    char principal_email[50];
} Line;

// stations in lines
typedef struct {
    // initial
    char line_number[6];
    int No;
    char number[10];
    char name[10];
    float distance; //kilometers
    float distance_to_before; //kilometers
    float time_to_arrive; //minutes
    float time_to_stay; //minutes
} Station;

// goods information
typedef struct {
    float total_capacity; //kilogram
    float unload; //kilogram
    float upload; //kilogram
    float available_capacity; //kilogram
} GoodsInfo;

// cars to distribute goods
typedef struct {
    // initial
    char license_plate[8];
    char line_number[6];
    char station_number[10];
    char driver_name[20];
    char driver_mobile[12];
    // goods information
    GoodsInfo goods_list;
} Car;

/* linklist definations */

typedef struct Linklist {
    char error[50];
    struct FirstNode *head;
} Linklist;

typedef struct FirstNode {
    Line LineInfo;
    struct SecondNode *first_child;
    struct FirstNode *next;
} FirstNode;

typedef struct SecondNode {
    Station StationInfo;
    struct ThirdNode *first_child;
    struct SecondNode *next;
} SecondNode;

typedef struct ThirdNode {
    Car CarInfo;
    struct ThirdNode *next;
} ThirdNode;


/* functions */

extern void ListInitial(Linklist *L);
/*
* Function Name: ListInitial
* Module: Basic
* Parameter: Linklist *L
* Return: None
* Use: initialize a NULL linklist
*/

extern void ListInsert_F(Linklist *L, Line LineInfo);
/*
* Function Name: ListInsert_F
* Module: Basic
* Parameter: Linklist *L, Line LineInfo
* Return: None
* Use: Insert a line in the linklist
*/

extern void ListInsert_S(Linklist *L, Station StationInfo);
/*
* Function Name: ListInsert_S
* Module: Basic
* Parameter: Linklist *L, Station StationInfo
* Return: None
* Use: Insert a station in the linklist
*/

extern void ListInsert_T(Linklist *L, Car CarInfo);
/*
* Function Name: ListInsert_T
* Module: Basic
* Parameter: Linklist *L, Car CarInfo
* Return: None
* Use: Insert a car in the linklist
*/

extern FirstNode* LocateLine(Linklist *L, Line LineInfo);
/*
* Function Name: LocateLine
* Module: Basic
* Parameter: Linklist *L, char *name
* Return: FirstNode*
* Use: Return a node that contain the line with ordered information
*/

extern SecondNode* LocateStation(Linklist *L, Station StationInfo);
/*
* Function Name: LocateStation
* Module: Basic
* Parameter: Linklist *L, Station StationInfo
* Return: SecondNode*
* Use: Return a node that contains the station with ordered information
*/

extern ThirdNode* LocateCar(Linklist *L, Car CarInfo);
/*
* Function Name: LocateCar
* Module: Basic
* Parameter: Linklist *L, Car CarInfo
* Return: ThirdNode*
* Use: Return a node that contains the car with ordered information
*/


extern void ModifyLine(Linklist *L, Line LineInfo);
/*
* Function Name: ModifyLine
* Module: Basic
* Parameter: Linklist *L, Line LineInfo
* Return: None
* Use: Modify a line's information
*/

extern void ModifyStation(Linklist *L, Station StationInfo);
/*
* Function Name: ModifyStation
* Module: Basic
* Parameter: Linklist *L, Station StationInfo
* Return: None
* Use: Modify a station's information
*/

extern void ModifyCar(Linklist *L, Car CarInfo);
/*
* Function Name: ModifyCar
* Module: Basic
* Parameter: Linklist *L, Car CarInfo
* Return: None
* Use: Modify a cat's information
*/

extern void DeleteCar(Linklist *L, Car CarInfo);
/*
* Function Name: DeleteCar
* Module: Basic
* Parameter: Linklist *L, Car CarInfo
* Return: None
* Use: Delete a car
*/

extern void DeleteStation(Linklist *L, Station StationInfo);
/*
* Function Name: DeleteStation
* Module: Basic
* Parameter: Linklist *L, Station StationInfo
* Return: None
* Use: Delete a station
*/

extern void DeleteLine(Linklist *L, Line LineInfo);
/*
* Function Name: DeleteLine
* Module: Basic
* Parameter: Linklist *L, Line LineInfo
* Return: None
* Use: Delete a line
*/

extern void GetLineInfo(Linklist *L, Line LineInfo, char *payload);
/*
* Function Name: GetLineInfo
* Module: Statistics
* Parameter: Linklist *L, Line LineInfo, char *payload
* Return: None
* Use: Get all information of a line
*/

extern void GetStationInfo(Linklist *L, Station StationInfo, char *payload);
/*
* Function Name: GetStationInfo
* Module: Statistics
* Parameter: Linklist *L, Station StationInfo, char *payload
* Return: None
* Use: Get all information of a station
*/

extern void GetCarInfo(Linklist *L, Car CarInfo, char *payload);
/*
* Function Name: GetCarInfo
* Module: Statistics
* Parameter: Linklist *L, Car CarInfo, char *payload
* Return: None
* Use: Get all information of a car
*/

extern void Calculate(Linklist *L, Car CarInfo, char *payload);
/*
* Function Name: Calculate
* Module: Statistics
* Parameter: Linklist L, Car CarInfo, char *payload
* Return: None
* Use: statistical calculation
*/

extern void SearchLines(Linklist *L, Line LineInfo, char *payload);
/*
* Function Name: SearchLines
* Module: Search
* Parameter: Linklist *L, Line LineInfo, char *payload
* Return: None
* Use: search lines
*/

extern void SearchStations(Linklist *L, Station StationInfo, char *payload);
/*
* Function Name: SearchStations
* Module: Search
* Parameter: Linklist *L, Station StationInfo, char *payload
* Return: None
* Use: search stations
*/

extern void SearchCars(Linklist *L, Car CarInfo, char *payload);
/*
* Function Name: SearchCars
* Module: Search
* Parameter: Linklist *L, Car CarInfo, char *payload
* Return: None
* Use: search cars
*/


#endif // !__LINKLIST_H_