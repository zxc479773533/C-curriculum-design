/*
* Part: Linklist algorithm library
* Module: Linklist APIs test
* Author: zxc479773533@github.com
* LICENSE: GNU GENERAL PUBLIC LICENSE V3.0
*/

# include "linklist.h"

int main(void) {
    Linklist List;
    ListInitial(&List);
    Line L1, L2, L3;

    strcpy(L1.number, "1");
    strcpy(L1.name, "panyue");
    L1.stations = 0;
    L1.length = 0;
    L1.total_time = 0;
    strcpy(L1.start_station, "first");
    strcpy(L1.end_station, "end");
    strcpy(L1.principal_name, "baobao");
    strcpy(L1.principal_tel, "8232778");
    strcpy(L1.principal_mobile, "13995718033");
    strcpy(L1.principal_email, "479773533@qq.com");

    strcpy(L2.number, "2");
    strcpy(L2.name, "baobao");
    L2.stations = 0;
    L2.length = 0;
    L2.total_time = 0;
    strcpy(L2.start_station, "second");
    strcpy(L2.end_station, "end");
    strcpy(L2.principal_name, "adgsfab");
    strcpy(L2.principal_tel, "98754");
    strcpy(L2.principal_mobile, "13995718033");
    strcpy(L2.principal_email, "479773533@qq.com");

    strcpy(L3.number, "3");
    strcpy(L3.name, "dongdong");
    L3.stations = 0;
    L3.length = 0;
    L3.total_time = 0;
    strcpy(L3.start_station, "third");
    strcpy(L3.end_station, "end");
    strcpy(L3.principal_name, "xvcxc");
    strcpy(L3.principal_tel, "984564");
    strcpy(L3.principal_mobile, "13995718033");
    strcpy(L3.principal_email, "479773533@qq.com");

    ListInsert_F(&List, L1);
    ListInsert_F(&List, L3);
    ListInsert_F(&List, L2);

    Station S1, S2, S3;

    strcpy(S1.line_number, "3");
    strcpy(S1.number, "001");
    strcpy(S1.name, "zxc");
    S1.distance = 1000;
    S1.time_to_arrive = 1;
    S1.time_to_stay = 2;

    strcpy(S2.line_number, "2");
    strcpy(S2.number, "002");
    strcpy(S2.name, "asd");
    S2.distance = 3000;
    S2.time_to_arrive = 1;
    S2.time_to_stay = 2;

    strcpy(S3.line_number, "3");
    strcpy(S3.number, "003");
    strcpy(S3.name, "qwe");
    S3.distance = 500;
    S3.time_to_arrive = 1;
    S3.time_to_stay = 2;

    ListInsert_S(&List, S1);
    ListInsert_S(&List, S2);
    ListInsert_S(&List, S3);
    
    Car C1, C2, C3;

    strcpy(C1.line_number, "3");
    strcpy(C1.license_plate, "456789");
    strcpy(C1.station_number,"003");
    C1.goods_list.total_capacity = 1000;
    C1.goods_list.upload = 800;
    C1.goods_list.unload = 0;

    strcpy(C2.line_number, "3");
    strcpy(C2.license_plate, "456789");
    strcpy(C2.station_number,"001");
    C2.goods_list.total_capacity = 1000;
    C2.goods_list.upload = 0;
    C2.goods_list.unload = 300;

    ListInsert_T(&List, C1);
    ListInsert_T(&List, C2);
    DeleteCar(&List, C2);
    return 0;
}
