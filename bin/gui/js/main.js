/*
* Part: Graphical User Interface
* Module: Main page's JS
* Author: zxc479773533@github.com
* LICENSE: GNU GENERAL PUBLIC LICENSE V3.0
*/

// a array to store the form html
var myForm = [];

// push forms in array
myForm.push({
    value: 'ImputLine',
    compulsoryArg: [
        'number',
        'name',
        'principal_name',
        'principal_tel',
        'principal_mobile',
        'principal_email'
    ],
    optionalArg: [],
});

myForm.push({
    value: 'ImputStation',
    compulsoryArg: [
        'line_number',
        'number',
        'name',
        'distance',
        'time_to_arrive',
        'time_to_stay'
    ],
    optionalArg: [],
});

myForm.push({
    value: 'ImputCar',
    compulsoryArg: [
        'license_plate',
        'line_number',
        'station_number',
        'driver_name',
        'driver_mobile',
        'total_capacity',
        'unload',
        'upload'
    ],
    optionalArg: [],
});

myForm.push({
    value: 'ModifyLine',
    compulsoryArg: [
        'number'
    ],
    optionalArg: [
        'name',
        'principal_name',
        'principal_tel',
        'principal_mobile',
        'principal_email'
    ],
});

myForm.push({
    value: 'ModifyStation',
    compulsoryArg: [
        'line_number',
        'number'
    ],
    optionalArg: [
        'name',
        'time_to_arrive',
        'time_to_stay'
    ],
});

myForm.push({
    value: 'ModifyCar',
    compulsoryArg: [
        'license_plate',
        'line_number',
        'station_number'
    ],
    optionalArg: [
        'driver_name',
        'driver_mobile',
        'total_capacity',
        'unload',
        'upload'
    ],
});

myForm.push({
    value: 'DeleteLine',
    compulsoryArg: [
        'number'
    ],
    optionalArg: [],
});

myForm.push({
    value: 'DeleteStation',
    compulsoryArg: [
        'line_number',
        'number'
    ],
    optionalArg: [],
});

myForm.push({
    value: 'DeleteCar',
    compulsoryArg: [
        'line_number',
        'station_number',
        'license_plate'
    ],
    optionalArg: [],
});

myForm.push({
    value: 'Statistics',
    compulsoryArg: [],
    optionalArg: [],
});

myForm.push({
    value: 'Inquire',
    compulsoryArg: [],
    optionalArg: [],
});

// translate C Identifier to Chinese
function translateToChinese (arg) {
    switch (arg) {
        case 'number': return '编号'; break;
        case 'name': return '名称'; break;
        case 'principal_name': return '负责人姓名'; break;
        case 'principal_tel': return '负责人办公室电话'; break;
        case 'principal_mobile': return '负责人移动电话'; break;
        case 'principal_email': return '负责人邮箱'; break;
        case 'line_number': return '所在路线编号'; break;
        case 'distance': return '与起点的距离'; break;
        case 'time_to_arrive': return '与上一个站点交通耗时'; break;
        case 'time_to_stay': return '停留耗时'; break;
        case 'license_plate': return '车牌号'; break;
        case 'station_number': return '所在站点编号'; break;
        case 'driver_name': return '司机姓名'; break;
        case 'driver_mobile': return '司机移动电话'; break;
        case 'total_capacity': return '汽车总容量(单位：kg)'; break;
        case 'unload': return '在该站的卸货量(单位：kg)'; break;
        case 'upload': return '在该站的装载量(单位：kg)'; break;
    }
}

// create arguments
function createArguments(arg, op) {

    var li = new String('<li><label class="argName">');
    li += translateToChinese(arg);

    // creat * for compusory
    if (op) {
        li += '<span class="compusory">*</span>';
    }

    li += '</label><br/>';
    li += '<input type="text" class="inputZone" id="' + arg;
    li += '"/></li>';
    return li;
}

// create button
function createButton(mode) {

    // build button
    var button = new String('<br/><li><input id="submit" type="submit" value="提交" onclick=\'');
    button += 'mainSend("' + mode.value + '"';

    // build compulsory arguments
    mode.compulsoryArg.forEach(function (arg) {
        button += ', document.getElementById("' + arg + '").value';
    });

    // build optional arguments
    mode.optionalArg.forEach(function (arg) {
        button += ', document.getElementById("' + arg + '").value';
    });

    button += ')\'/></li>';
    return button;
}

// create a form
function createForm(mode) {

    var form = new String('<ul class="userForm">');

    // create compulsory arguments
    mode.compulsoryArg.forEach(function (arg) {
        form += createArguments(arg, true);
    });

    // create optional arguments
    mode.optionalArg.forEach(function (arg) {
        form += createArguments(arg, false);
    });

    // create button
    form += createButton(mode);

    // end
    form += '</ul>';
    return form;
}

// main function to make form's inner HTML
function makeFormHTML(val) {

    var HTML = null;

    // find the mode
    myForm.forEach(function (mode) {
        if (mode.value == val) {
            HTML = createForm(mode);
        }
    });

    return HTML;
}

// create main control button
function makeMainButton() {
    var HTML = '<br/><br/><br/>';
    HTML += '<ul><li><h3>点击清空所有数据</h3></li>';
    HTML += '<li><input id="submit" type="submit" value="提交" onclick="ws.send(\'Clear\')"/>';
    HTML += '</li></ul>';
    return HTML;
}

// create statics page
function makeStatisticsPage() {
    
}