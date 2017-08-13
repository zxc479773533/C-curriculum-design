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
    float time; //minutes
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

} Station;

// goods information
typedef struct {
    float total_capacity; //gallon
    float unload; //gallon
    float upload; //gallon
    float available_capacity; //gallon
} GoodsInfo;

// cars to distribute goods
typedef struct {
    // initial
    char license_plate[8];
    char line_number[6];
    char driver_name[8];
    char driber_mobile[11];
    // goods information
    GoodsInfo goods_list;
} Car;

/* linklist definations */

typedef struct Linklist {
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


#endif // !__LINKLIST_H_