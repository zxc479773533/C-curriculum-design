/*
* Module: Linklist APIs head file
* Author: zxc479773533@github.com
* LICENSE: GNU GENERAL PUBLIC LICENSE V3.0
*/

#ifndef __LINKLIST_H_
#define __LINKLIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* defines */



/* informatino struct definations */

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
    char principal_name[8];
    char principal_tel[8];
    char principal_mobile[11];
    char principal_email[50];
} Line;

// stations in lines
typedef struct {
    // initial
    char line_number[6];
    int No;
    char number[10];
    char name;
    float distance; //kilometers
    float distance_to_before; //kilometers
    float time_to_arrive; //kilometers
    float time_to_stay; //minutes
    // a linklist to hode the lines through self
    // REMEMBER TO ADD!
} Station;

// goods information
typedef struct {
    char  goods_name[8];
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
    char driver_name[8];
    char driber_mobile[11];
    // goods information
    GoodsInfo goods_list;
} Car;

/* linklist definations */

typedef struct Linklist {
    char error[100];
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
* Parameter: Linklist *L
* Return: None
* Use: initialize a NULL linklist
*/

extern void ListInsert_F(Linklist *L, Line LineInfo);
/*
* Function Name: ListInsert_F
* Parameter: Linklist *L, Line LineInfo
* Return: None
* Use: Insert a line in the linklist
*/

extern void ListInsert_S(Linklist *L, Station StationInfo);
/*
* Function Name: ListInsert_S
* Parameter: Linklist *L, Station StationInfo
* Return: None
* Use: Insert a station in the linklist
*/

extern void ListInsert_T(Linklist *L, Car CarInfo);
/*
* Function Name: ListInsert_T
* Parameter: Linklist *L, Car CarInfo
* Return: None
* Use: Insert a car in the linklist
*/

extern FirstNode* LocateLine(Linklist *L, Line LineInfo);
/*
* Function Name: LocateLine
* Parameter: Linklist *L, char *name
* Return: FirstNode*
* Use: Return a node that contain the line with ordered information
*/

extern SecondNode* LocateStation(Linklist *L, Station StationInfo);
/*
* Function Name: LocateStation
* Parameter: Linklist *L, Station StationInfo
* Return: SecondNode*
* Use: Return a node that contains the station with ordered information
*/

extern ThirdNode* LocateCar(Linklist *L, Car CarInfo);
/*
* Function Name: LocateCar
* Parameter: Linklist *L, Car CarInfo
* Return: ThirdNode*
* Use: Return a node that contains the car with ordered information
*/






#endif // !__LINKLIST_H_