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
        'Number',
        'Name',
        'Principal\'s name',
        'Principal\'s tel',
        'Principal\'s mobile',
        'Principal\'s email'
    ],
    optionalArg: [],
});

myForm.push({
    value: 'ImputStation',
    compulsoryArg: [
        'Line number',
        'Number',
        'Name',
        'Distance',
        'Time to arrive',
        'Time to stay'
    ],
    optionalArg: [],
});

myForm.push({
    value: 'ImputCar',
    compulsoryArg: [
        'License plate',
        'Line number',
        'Station number',
        'Driver\'s name',
        'Driver\'s modile',
        'Car\'s total capacity',
        'Goods unload',
        'Goods upload'
    ],
    optionalArg: [],
});

myForm.push({
    value: 'ModifyLine',
    compulsoryArg: [
        'Number'
    ],
    optionalArg: [
        'Name',
        'Principal\'s name',
        'Principal\'s tel',
        'Principal\'s mobile',
        'Principal\'s email'
    ],
});

myForm.push({
    value: 'ModifyStation',
    compulsoryArg: [
        'Line number',
        'Number'
    ],
    optionalArg: [
        'Name',
        'Distance',
        'Time to arrive',
        'Time to stay'
    ],
});

myForm.push({
    value: 'ModifyCar',
    compulsoryArg: [
        'License plate',
        'Line number',
        'Station number'
    ],
    optionalArg: [
        'Driver\'s name',
        'Driver\'s modile',
        'Car\'s total capacity',
        'Goods unload',
        'Goods upload'
    ],
});

myForm.push({
    value: 'DeleteLine',
    compulsoryArg: [
        'Number'
    ],
    optionalArg: [],
});

myForm.push({
    value: 'DeleteStation',
    compulsoryArg: [
        'Line number',
        'Number'
    ],
    optionalArg: [],
});

myForm.push({
    value: 'DeleteCar',
    compulsoryArg: [
        'Line number',
        'Station number',
        'License plate'
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

// create arguments
function createArguments(arg, op) {

    var li = new String('<li><label class="argName">');
    li += arg;

    // creat * for compusory
    if (op) {
        li += '<span class="compusory">*</span>';
    }

    li += '</label><br/>';
    li += '<input type="text" class="inputZone" id="arg_' + arg;
    li += '"/></li>';
    return li;
}

// create button
function createButton(mode) {

    var button = new String('<br/><li><input id="submit" type="submit" value="提交" onclick=\'');
    button += '\'/></li>';
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