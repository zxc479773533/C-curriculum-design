/*
* Part: Linklist algorithm library
* Module: Linklist statistics APIs achieve
* Author: zxc479773533@github.com
* LICENSE: GNU GENERAL PUBLIC LICENSE V3.0
*/

#ifndef __STATISTICS_
#define __STATISTICS_

#include "linklist.h"

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
            "起始站编号：%s\r\n" \
            "终点站编号：%s\r\n" \
            "负责人姓名：%s" \
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


#endif // !__STATISTICS_
#define __STATISTICS_