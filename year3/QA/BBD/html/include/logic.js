/********************************************************************
 *
 * $Id: CarConfigurator, v 1.1 2012/03/20 16:01:28
 *
 * Copyright (C) 2012 Quality First Software
 * All rights reserved
 *
 * Author: QFS (qfs@qfs.de), Yann Spoeri
 *******************************************************************/
  
  /*
   * Logic Calculations for Prices etc.
   */
  
  // {{{ vars for the selectable data as vehicle, accessory etc.
  
 // standard Vehicl, Special and Accessory cossts
  var vehicleNames = Array(getLanguageStringbyIdStr("VehicleRolo"), getLanguageStringbyIdStr("VehicleGolf"), getLanguageStringbyIdStr("VehicleRassant"), getLanguageStringbyIdStr("VehicleRassantFamily"), getLanguageStringbyIdStr("VehicleI5"));
  var vehicleID = Array("M1", "M2", "M3", "M4", "M5");
  var vehicle_price = Array(12300, 15000, 17000, 18500, 29000);
  var vehicle_specialmodel_name = Array(getLanguageStringbyIdStr("ModelNoModel"), getLanguageStringbyIdStr("ModelGomera"), getLanguageStringbyIdStr("ModelJazz"), getLanguageStringbyIdStr("ModelLuxus"));
  var vehicle_specialmodel_desc = Array(getLanguageStringbyIdStr("ModelNoModelDesc"), getLanguageStringbyIdStr("ModelGomeraDesc"), getLanguageStringbyIdStr("ModelJazzDesc"), getLanguageStringbyIdStr("ModelLuxusDesc"));
  var vehicle_specialmodel_extra = Array("", "xS3xxS2xxS5x", "xS6xxS7xxS3x", "xS6xxS5xxS4xxS2x");
  var vehicle_specialmodel_price = Array(0, 1413, 1049, 2499.99);
  var accessory_name = Array(getLanguageStringbyIdStr("AccessoTableWheel"), getLanguageStringbyIdStr("AccessoTableMirror"), getLanguageStringbyIdStr("AccessoTableDoors"), getLanguageStringbyIdStr("AccessoTableRims"), getLanguageStringbyIdStr("AccessoTableAntiSkidSys"), getLanguageStringbyIdStr("AccessoTablePowerWindows"), getLanguageStringbyIdStr("AccessoTableRadio"), getLanguageStringbyIdStr("AccessoTableMats"));
  var accessory_price = Array(360, 210, 1200, 900, 990, 490, 470, 26);
  var accessory_id = Array("S1", "S2", "S3", "S4", "S5", "S6", "S7", "S8");
  
   // }}}
   
   // {{{ vars that specificate the selected items
  
  // input data
  var selectedvehicle = 0, selectedspecial = 0, selectedaccesso = "", thecar_discount = 0;
  
   // }}}
   
   // {{{ price calcing function
   
  // calcint the cars price
  var price_vehicle, price_special, price_extra, extra_count, price_total, extra_discount;
  
  // calc the price of the car
  function calc_price_vars() {
    price_vehicle = (selectedvehicle - 1 == -1)?0:vehicle_price[selectedvehicle - 1];
    
    //BUG: In buggy mode any price greater 29000 will be ignored
    if (buggyVersion && price_vehicle >= 29000) {
        price_vehicle = 0;
    }
    
    price_special = vehicle_specialmodel_price[selectedspecial];
    price_extra = 0;
    extra_count = 0;
    // some accessory may be contained in the selected model
    for(var i = 0; i < accessory_price.length && document.getElementById('AddAccessoryPriceChkbox').checked == true; i++) {
      if(selectedaccesso.indexOf("xS" + i + "x") != -1) {
        if(vehicle_specialmodel_extra[selectedspecial].indexOf("xS" + i + "x") == -1) {
          if(buggyVersion && parseInt(accessory_id[i].substr(1)) % 3 == 0) {
               extra_count--;
          }
          extra_count++;
          price_extra += accessory_price[i];
        }
      }
    }
    extra_discount = 0;
    if(3 <= extra_count) {
      extra_discount = 10;            // if there are 3 ore more <u>extra</u> accessory there will be 10% of discount
    }
    if(5 <= extra_count) {
      extra_discount = 15;            // if there are 5 ore more <u>extra</u> accessory there will be 15% of discount
    }
    // calc total price
    price_total = (price_vehicle / 100 * (100 - thecar_discount)) + price_special + price_extra / 100 * ((100 - extra_discount));
  }
  
   // }}}
   
   // {{{ format outputing
   
  // for outputing in the dddd.dd -format
  function round(nr) {
    // if(nr == undefined) return "0.00";
    
    var dummy = nr.toFixed(2);
    // 
    var beforeDot = Math.floor(nr);
    var beforeDotString = beforeDot.toString();
    // 
    var dummyString = dummy.toString();
    var afterDot = dummyString.substr(dummyString.lastIndexOf(".") + dummyString.lastIndexOf(",") + 2);
    // 
    var beforeDotFormatString = "";
    while(beforeDotString.length != 0) {
        var dump = beforeDotString.substr((beforeDotString.length - 3 > 0)?beforeDotString.length - 3:0);
        beforeDotString = beforeDotString.substr(0, beforeDotString.length - 3);
        if(setlanguage == "EN") {
            beforeDotFormatString = "," + dump + beforeDotFormatString;
        } else {
            beforeDotFormatString = "." + dump + beforeDotFormatString;
        }
    }
    beforeDotFormatString = beforeDotFormatString.substr(1);
    //
    if(setlanguage == "EN") {
        return beforeDotFormatString + "." + afterDot;
    }
    return beforeDotFormatString + "," + afterDot;
  }
   
   // }}}
   
   // {{{ updating
   
  // this function updates all elements when something is changed by the user
  function updateCalculatorPane() {
    if(startup) return;
    calc_price_vars();
    document.getElementById('BasePrice_input').innerHTML = round(price_vehicle);
    document.getElementById('DiscountValue_input').value = thecar_discount;
    document.getElementById('SpecialPrice_input').innerHTML = round(price_special);
    document.getElementById('AccessoryPrice_input').innerHTML = round(price_extra);
    document.getElementById('AccessoryDiscountValue_input').innerHTML = extra_discount;
    document.getElementById('CalculatedPrice_input').innerHTML = round(price_total);
  }
  
  function updateAllPanels() {
    printVehicleTable();
    SpecialComboUpdate();
    document.getElementById('SpecialsCombo').selectedIndex = selectedspecial;
    change_special();
    AccessoryTableUpdate();
    updateCalculatorPane();
  }
  
   // }}}
   
   // {{{useful functions
  
  // is the number an Integer
  function isNr(nr) {
    return !isNaN(parseFloat(nr));
  }
  
  // useful array functions
  function deleteElement(arr, index) {              // arr is the array, index is the pointer to the element that is to be removed
    var dummy = Array();
    for(var i = 0; i < arr.length; i++) {
        if(i != index) {
            dummy.push(arr[i]);
        }
    }
    return dummy;
  }
  
  // useful delete accesso function
  function deleteAccessoIndex(str, index) {
    var dummy = "";
    var splitstr = str.split("x");
    for(var i = 0; i < splitstr.length; i++) {
        if(splitstr[i].charAt(0) == "S") {
            var nr = parseFloat(splitstr[i].substr(1));
            if(nr == index) continue;
            if(index < nr) {
                nr--;
            }
            dummy += "xS" + nr + "x";
        }
    }
    return dummy;
  }
  
  function cloneObj(obj) {
    return obj;
  }
  
  function cloneArray(arr) {
    var newArr = new Array();
    for(var i = 0; i < arr.length; i++) {
        newArr.push(cloneObj(arr[i]));
    }
    return newArr;
  }
  
   // }}}