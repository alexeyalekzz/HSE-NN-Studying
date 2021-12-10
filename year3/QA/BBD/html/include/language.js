/********************************************************************
 *
 * $Id: CarConfigurator, v 1.1 2012/03/20 16:01:28
 *
 * Copyright (C) 2012 Quality First Software
 * All rights reserved
 *
 * Author: QFS (qfs@qfs.de), Yann Spoeri
 *******************************************************************/
 
 // Language Control for CarConfig
 
 // {{{ language control as supported languages, selected language etc.
 
 // all supported languages are contained in this Array (uppercase)
 var supportedlanguages = Array("EN", "DE");
 
 // the language which is selected as default language
 var defaultlanguage = 0;                               // pointer to supportedlanguagesArray
 
 // Get the prefered language of the browser/user
   var languagestring = "", setlanguage = supportedlanguages[defaultlanguage], languagepointer = defaultlanguage, langByURIString = false;
   // look for URI param
   var uristring = decodeURI(window.location.search.toUpperCase());
   if(uristring.indexOf("&LANG=") != -1 || uristring.indexOf("?LANG=") != -1) {     // if the language is set by URI
     langByURIString = true;
     languagestring = uristring.substr(uristring.indexOf("&LANG=") + uristring.indexOf("?LANG=") + 7, 2);
   } else {
     if(navigator.appName.indexOf("Microsoft") != -1) {                // IE allways need a special case...
       languagestring = navigator.browserLanguage;                        // but hopefully he also supports the JS-Code navigator.language once
     } else {
       languagestring = navigator.language;
     }
     languagestring = languagestring.toUpperCase();
   }
   // now we have to look if we support the prefferd language
   for(var i = 0; i < supportedlanguages.length; i++) {
      if(languagestring.indexOf(supportedlanguages[i]) != -1) {
          setlanguage = supportedlanguages[i];
          languagepointer = i;
          break;
      }
   }
 
 // }}}
 
 // {{{ language Arrays in this Arrays there will be saved the names of each word in each language
 
 // The Language Arrays
  // Menus
   var language_Menu_File_Names = Array("File", "Datei");
   var language_Menu_File_Reset_Names = Array("Reset", "Zur&uuml;cksetzen");
   var language_Menu_File_Exit_Names = Array("Exit", "Beenden");
   
   var language_Menu_Option_Names = Array("Options", "Einstellungen");
   var language_Menu_Option_Vehicle_Names = Array("Vehicles...", "Fahrzeuge...");
   var language_Menu_Option_Specials_Names = Array("Specials...", "Sondermodelle...");
   var language_Menu_Option_Accessories_Names = Array("Accessories...", "Zubeh&ouml;r...");
   
   var language_Menu_Order_Names = Array("Purchase order", "Bestellung");
   var language_Menu_Order_Order_View = Array("View selected details", "Auswahldetails ansehen");
   var language_Menu_Order_Order_Names = Array("Send order", "Bestellen");
   
   var language_Menu_Help_Names = Array("Help", "Hilfe");
   var language_Menu_Help_Info_Names = Array("Info", "Info");
   var language_Menu_Help_Buggy_Names = Array("Buggy", "Fehlerhaft");
   var language_Menu_Help_NewVer_Names = Array("New version", "Neue Version");
   var language_Menu_Help_Loadtestmode_Name = Array("Load testing mode", "Lasttest Modus");
  
  // change settings
   var language_SettingsChange_Vehicle_Names = Array("Vehicles Dialog", "Fahrzeuge bearbeiten");
   var language_SettingsChange_Specials_Names = Array("Specials Dialog", "Sondermodelle bearbeiten");
   var language_SettingsChange_Accessory_Names = Array("Accessories Dialog", "Zubeh&ouml;r bearbeiten");
  
  // change settings contents
   var language_Vehicle_Dialog_Edit_Panel_Tabel_Description = Array("Vehicles", "Fahrzeuge");
   var language_Vehicle_Dialog_Edit_Panel_Table_Header_Name = Array("Name", "Name");
   var language_Vehicle_Dialog_Edit_Panel_Table_Header_ID = Array("ID", "ID");
   var language_Vehicle_Dialog_Edit_Panel_Table_Header_Prices = Array("Price", "Preis");
   var language_Vehicle_Dialog_Edit_Panel_Table_Header_Edit = Array("Edit", "Bearbeiten");
   var language_Vehicle_Dialog_Edit_Panel_Table_Edit_Del = Array("delete", "l\xF6schen");
   var language_Vehicle_Dialog_Edit_Panel_Table_Edit_Edit = Array("edit", "bearbeiten");
   var language_Vehicle_Dialog_Edit_Panel_NewDescription = Array("Create new vehicle:", "Erstelle neues Fahrzeug:");
   var language_Buttons_New = Array("New", "Neu");
   var language_Buttons_Edit = Array("Change", "\xC4ndern");
   var language_Buttons_Delete = Array("Delete", "L\xF6schen");
   var language_Buttons_Create_Vehicle = Array("Create vehicle", "Erstelle Fahrzeug");
   var language_Close_Button = Array("Cancel", "Abbrechen");
   var language_Save_Changes_Button = Array("OK", "OK"); //"\xC4nderungen speichern");
   var language_Order_Button = Array("Send purchase order", "Bestellung absenden");
   var language_Vehicle_Dialog_Edit_Panel_Del_Question = Array("Are you sure you want to delete the selected vehicle?", "Wollen Sie den selektierten Eintrag wirklich l\xF6schen?");
   var language_Accessory_Dialog_Edit_Panel_Del_Question = Array("Are you sure you want to delete the selected accessory?", "Wollen Sie den selektierten Eintrag wirklich l\xF6schen?");
   var language_Specials_Dialog_Edit_Panel_DelQuestion = Array("Are you sure you want to delete the selected model?", "Wollen Sie den selektierten Eintrag wirklich l\xF6schen?");
   var language_Vehicle_Dialog_Edit_Panel_Create_New_Car_Panel_Title = Array("Create New Vehicle", "Neues Fahrzeug erstellen");
   var language_Accessory_Dialog_Edit_Panel_Edit_Accessory_Panel_Title = Array("Edit Accessory", "Zubeh&ouml;r bearbeiten");
   var language_Vehicle_Dialog_Edit_Panel_Edit_Car_Panel_Title = Array("Edit Vehicle", "Fahrzeug bearbeiten");
   var language_Vehicle_Dialog_Edit_Panel_Edit_Car_Panel_Vehicle_Name = Array("Name", "Name");
   var language_Vehicle_Dialog_Edit_Panel_Edit_Car_Panel_Vehicle_ID = Array("ID", "ID");
   var language_Vehicle_Dialog_Edit_Panel_Edit_Car_Panel_Vehicle_Price = Array("Price", "Preis");
   var language_Vehicle_Dialog_Edit_Panel_Edit_Car_Panel_Vehicle_Button_OK = Array("OK", "OK");
   var language_Vehicle_Dialog_Edit_Panel_Edit_Car_Panel_Vehicle_Button_Cancel = Array("Cancel", "Abbrechen");
   
   var language_Accessory_Dialog_Edit_Panel_Create_New_Accessory_Button_Name = Array("Create accessory", "Erstelle Zubeh\xF6r");
   var language_Accessory_Dialog_Edit_Panel_Create_New_Accessory_Panel_Title = Array("Create New Accessory", "Erstelle neues Zubeh\xF6r");
   
   var language_Specials_Dialog_Edit_Panel_New_Model_Button = Array("Create new special model", "Neues Sondermodell erstellen");
   var language_Specials_Dialog_Edit_Panel_TableHeaderDescription = Array("Description", "Beschreibung");
   var language_Specials_Dialog_Edit_Panel_TableHeaderAccessories = Array("Accessories", "Zubeh\xF6r");
   var language_Specials_Dialog_Edit_Panel_TableViewDetails = Array("details", "details");
   
   var language_Specials_Model_Dialog_Edit_Panel_Tree_Dialog_Title = Array("Details", "Details");
   var language_Specials_Model_Dialog_Edit_Panel_Tree_Dialog_Info = Array("Information", "Information");
   var language_Specials_Model_Dialog_Edit_Panel_Tree_Dialog_Accessories = Array("Accessories", "Zubeh&ouml;r");
   var language_Specials_Model_Dialog_Edit_Panel_Tree_Dialog_Description = Array("Description", "Beschreibung");
   
   var language_Specials_Dialog_Edit_Panel_Edit_Special_Model_Title = Array("Edit Specials", "Sondermodelle bearbeiten");
   var language_Specials_Dialog_Edit_Panel_Edit_Special_Model_Name = Array("Model name", "Modellname");
   var language_Specials_Dialog_Edit_Panel_Edit_Special_Model_Price = Array("Price", "Preis");
   var language_Specials_Dialog_Edit_Panel_Edit_Special_Model_CPrice = Array("Separate price", "Einzelpreis");
   var language_Specials_Dialog_Edit_Panel_Edit_Special_Model_Desc = Array("Description", "Beschreibung");
   
  // Tab
   var language_Tab_Vehicle_Names = Array("Vehicles", "Fahrzeuge");
   var language_Tab_Accessory_Names = Array("Accessories", "Zubeh&ouml;r");
   var language_Tab_Specials_Names = Array("Specials", "Sondermodelle");
   
  // Tab1: Vehicle selection
   var language_Table_Header_Vehicle_Name = Array("Model", "Modell");
   var language_Table_Header_ID_Name = Array("ID", "ID");
   var language_Table_Header_Prices_Name = Array("Price", "Preis");
   
  // Tab2: Model Table Panel
   var language_Model_Table_Panel_No_Model_Names = Array("- no special model -", "- kein Sondermodell -");
   var language_Model_Table_Panel_Gomera_Names = Array("Gomera", "Gomera");
   var language_Model_Table_Panel_Jazz_Names = Array("Jazz", "Jazz");
   var language_Model_Table_Panel_Luxus_Names = Array("Luxus", "Luxus");
   var language_Model_Table_Panel_No_Model_Discription = Array("-", "-");
   var language_Model_Table_Panel_Gomera_Discription = Array(
     "Varnishing in two shades of brown\r\nRims ans bumpers in same color as car\r\nCentralized door locking\r\nAlloy rims\r\nPower windows in back\r\n\r\nPrice: 1413.00 EUR",
     "Lackierung in zwei Brauntönen\r\nFelgen und Stossstangen in Wagenfarbe\r\nZentralverriegelung\r\nSportfelgen\r\nFensterheber hinten\r\n\r\nPreis: 1413,00 EUR"
   );
   var language_Model_Table_Panel_Jazz_Discription = Array(
     "Varnishing bright and dark blue\r\nRims ans bumpers in same color as car\r\nRadio with CD\r\nAlloy rims\r\nMats\r\n\r\nPrice: 1049.00 EUR",
     "Lackierung in Himmelblau und Dunkelblau\r\nFelgen und Stossstangen in Wagenfarbe\r\nRadio mit CD\r\nSportfelgen\r\nFussmatten\r\n\r\nPreis: 1049,00 EUR"
   );
   var language_Model_Table_Panel_Luxus_Discription = Array(
     "Varnishing in two muted pastel colors\r\nRims ans bumpers in same color as car\r\nAnti-skid system\r\nRadio with CD\r\nCentralized door locking\r\nPower windows in back\r\n\r\nPrice: 2499.99 EUR",
     "Lackierung in zwei gedeckten Pastellfarben\r\nFelgen und Stossstangen in Wagenfarbe\r\nABS\r\nRadio mit CD\r\nZentralverriegelung\r\nFensterheber hinten\r\n\r\nPreis: 2499,99 EUR"
   );
   
  // Tab3: Accessory Table
   var language_Accessory_Table_Description = Array("Description", "Beschreibung");
   var language_Accessory_Table_ID = Array("ID", "ID");
   var language_Accessory_Table_Price = Array("Price", "Preis");
   var language_Accessory_wheel = Array("Steering wheel in leather", "Lederlenkrad");
   var language_Accessory_Mirror = Array("Heatable outside mirror", "Beheizbare Aussenspiegel");
   var language_Accessory_Centralized_doors = Array("Centralized door locking", "Zentralverriegelung");
   var language_Accessory_rims = Array("Alloy rims", "Sportfelgen");
   var language_Accessory_AntiSkidSystem = Array("Anti-skid system", "ABS");
   var language_Accessory_PowerWindows = Array("Power windows in back", "Fensterheber hinten");
   var language_Accessory_Radio = Array("Radio with CD", "Radio mit CD-Wechsler");
   var lanugage_Accessory_Mats = Array("Mats", "Fussmatten");
   
   var language_AddAccessoryPrice_Chkbox = Array("Add accessories price to final price", "Zubehörpreis zu Endpreis addieren");
   
  // Vehicle Names
   var language_Vehicle_Rolo_Names = Array("Rolo", "Rolo");
   var language_Vehicle_Golf_Names = Array("Minigolf", "Minigolf");
   var language_Vehicle_Rassant_Names = Array("Rassant", "Rassant");
   var language_Vehicle_RassantFamily_Names = Array("Rassant Family", "Rassant Family");
   var language_Vehicle_I5_Names = Array("I5", "I5");
   
  // Calculator Panel
   var language_Calculator_Panel_Base_Price_Names = Array("Base Price", "Preis Basismodell");
   var language_Calculator_Panel_Discount_Names = Array("Discount", "Rabatt");
   var language_Calculator_Panel_Price_Model_Names = Array("Specials Price", "Preis Sondermodell");
   var language_Calculator_Panel_Price_Accessory_Names = Array("Accessories Price", "Preis Zubeh&oumlr");           
   var language_Calculator_Panel_Total_Price_Names = Array("Final Price", "Endpreis");
  
  // View Order Dialog
   var language_View_Purchase_Order_Dialog_Title = Array("Selection details", "Auswahldetails");
   var language_View_Purchase_Order_Dialog_Vehicle_Details = Array("Vehicle details", "Fahrzeuginformationen");
   var language_View_Purchase_Order_Dialog_Special_Details = Array("Special details", "Sondermodellinformation");
   var language_View_Purchase_Order_Dialog_Accessory_Details = Array("Accessory details", "Zubeh&ouml;rinformationen");
   var language_View_Purchase_Order_Dialog_Price_Details = Array("Price details", "Preisinformationen");
   var language_View_Purchase_Order_Dialog_Selected_Vehicle = Array("Selected vehicle", "Ausgew&auml;hltes Fahrzeug");
   var language_View_Purchase_Order_Dialog_NO_Selected_Vehicle = Array("no vehicle selected", "kein Fahrzeug ausgewählt");
   var language_View_Purchase_Order_Dialog_Selected_Special = Array("Selected special", "Ausgew&auml;hltes Sondermodell");
   var language_View_Purchase_Order_Dialog_Selected_Accessory = Array("Selected accessory", "Ausgew&auml;hltes Zubeh&ouml;r");
   var language_View_Purchase_Order_Dialog_NO_Selected_Accessory = Array("No accessory selected!", "Kein Zubeh&ouml;r ausgew&auml;hlt!");
   var language_View_Purchase_Order_Dialog_Final_Price = Array("Final price", "Endpreis");
  
  // Order Dialog
   var language_Send_Purchase_Order_Dialog_Names = Array("Purchase order details", "Details zur Bestellung");
   var language_Send_Purchase_Order_Dialog_Accordion_Base_Price_Name = Array("Vehicle details", "Fahrzeuginformationen");
   var language_Send_Purchase_Order_Dialog_Accordion_Special_Model_Name = Array("Special details", "Sondermodellinformationen");
   var language_Send_Purchase_Order_Dialog_Accordion_Accessory_Name = Array("Accessory details", "Zubeh&ouml;rinformationen");
   var language_Send_Purchase_Order_Dialog_Accordion_Total_Prices_Name = Array("Price details", "Preisinformationen");
   var language_Send_Purchase_Order_Dialog_Accordion_Address_Name = Array("Contact details", "Kontaktinformationen");
   
   var language_Send_Purchase_Order_Dialog_Base_Price_Vehicle_Name = Array("Selected vehicle", "Ausgew\xE4hltes Fahrzeug");
   var language_Send_Purchase_Order_Dialog_Base_Price_Vehicle_Prices = Array("Price", "Preis");
   var language_Send_Purchase_Order_Dialog_Base_Price_Vehicle_Discount = Array("Discount", "Rabatt");
   var language_Send_Purchase_Order_Dialog_Base_Price_Vehicle_Total  = Array("Total", "Gesamt");
   
   var language_Send_Purchase_Order_Dialog_Special_Model_Special_Model_Name = Array("Selected special", "Ausgew\xE4hltes Sondermodell");
   var language_Send_Purchase_Order_Dialog_Special_Model_Special_Model_Price = Array("Price", "Preis");
   
   var language_Send_Purchase_Order_Dialog_Accessory_Table_Header_selected = Array("Selected accessory", "Ausgew\xE4hltes Zubeh\xF6r");
   var language_Send_Purchase_Order_Dialog_Accessory_Table_Header_price = Array("Price", "Preis");
   var language_Send_Purchase_Order_Dialog_Accessory_Table_discount = Array("Discount", "Rabatt");
   var language_Send_Purchase_Order_Dialog_Accessory_Table_total = Array("Result", "Gesamt");
   
   var language_Send_Purchase_Order_Dialog_Special_Total_Price_Desc_Total = Array("Final price", "Endpreis");
   var language_Send_Purchase_Order_Dialog_Missing_Input = Array("Mandatory contact details missing!\nPlease specify all contact details!", "Bitte f\xFCllen Sie die Kontaktinformationen aus!");
   
   var language_Send_Purchase_Order_Dialog_Contact_surename = Array("Surname", "Nachname");
   var language_Send_Purchase_Order_Dialog_Contact_firstname = Array("First name", "Vorname");
   var language_Send_Purchase_Order_Dialog_Contact_street = Array("Street address", "Adresse");
   var language_Send_Purchase_Order_Dialog_Contact_zip = Array("ZIP code", "PLZ");
   var language_Send_Purchase_Order_Dialog_Contact_city = Array("City", "Ort");
   var language_Send_Purchase_Order_Dialog_Contact_country = Array("Country", "Land");
   var language_Send_Purchase_Order_Dialog_Contact_number = Array("Phone number", "Telefonnummer");
   var language_Send_Purchase_Order_Dialog_Contact_mail = Array("E-mail address", "E-Mail Adresse");
   
   var language_Send_Purchase_Order_Dialog_Base_Price_Name_No_Vehicle_Selected = Array("- no vehicle selected -", "- kein Fahrzeug ausgew\xE4hlt -");
   var language_Send_Purchase_Order_Dialog_Base_Price_Price_No_Vehicle_Selected = Array("-----.-- &euro;", "-----.-- &euro;");
   var language_Send_Purchase_Order_Dialog_accessory_NoSelected_Accessory = Array("- no accessory selected -", "- kein Zubeh\xF6r ausgew\xE4hlt - ");
   
  // Error
   var language_Error_Close_Window = Array("CarConfigurator is unable to close the current window/tab!<br/><br/>Either your browser doesn't support this option<br/>or your security settings prevent the script from closing the window/tab!", "Der CarConfigurator kann das aktuelle Fenster/Tab nicht schlie&szlig;en!<br/><br/>Entweder unterst&uuml;tzt Ihr Browser diese Option nicht oder<br/>Ihre aktuellen Browsersicherheitseinstellungen verhindern dies!");
   var language_Error_Discount_Input = Array("Invalid Input! <br/><br/> Discount must be a number between 0 and 100 %.", "Ung\xFCltige Eingabe! <br/><br/> Der Rabatt muss eine Zahl zwischen 0 und 100 % sein.");
   var language_Error_by_Input = Array("Invalid Input!", "Ung\xFCltige Eingabe!");
   
 // }}}
 
 // {{{ getLanguageStringbyIdStr get Id returns string in the specific language uses the language-Arrays defined in this file
 
 // This function returns the strings in the preferd language
 function getLanguageStringbyIdStr(str) {
    // Menus
        if(str == "mFile") {
            return language_Menu_File_Names[languagepointer];
        }
        if(str == "miReset") {
            return language_Menu_File_Reset_Names[languagepointer];
        }
        if(str == "miExit") {
            return language_Menu_File_Exit_Names[languagepointer];
        }
        if(str == "mOptions") {
            return language_Menu_Option_Names[languagepointer];
        }
        if(str == "miVehicles") {
            return language_Menu_Option_Vehicle_Names[languagepointer];
        }
        if(str == "miSpecials") {
            return language_Menu_Option_Specials_Names[languagepointer];
        }
        if(str == "miAccessories") {
            return language_Menu_Option_Accessories_Names[languagepointer];
        }
        if(str == "mOrder") {
            return language_Menu_Order_Names[languagepointer];
        }
        if(str == "miViewOrder") {
            return language_Menu_Order_Order_View[languagepointer];
        }
        if(str == "miSendOrder") {
            return language_Menu_Order_Order_Names[languagepointer];
        }
        if(str == "mHelp") {
            return language_Menu_Help_Names[languagepointer];
        }
        if(str == "miInfo") {
            return language_Menu_Help_Info_Names[languagepointer];
        }
        if(str == "miBuggy") {
            return language_Menu_Help_Buggy_Names[languagepointer];
        }
        if(str == "miNewVersion") {
            return language_Menu_Help_NewVer_Names[languagepointer];
        }
        if(str == "miLoadtest") {
            return language_Menu_Help_Loadtestmode_Name[languagepointer];
        }
    // End Menu
    // settings change
        if(str == "SettingsChangeVehicle") {
            return language_SettingsChange_Vehicle_Names[languagepointer];
        }
        if(str == "SettingsChangeSpecials") {
            return language_SettingsChange_Specials_Names[languagepointer];
        }
        if(str == "SettingsChangeAccessory") {
            return language_SettingsChange_Accessory_Names[languagepointer];
        }
    // end settings change
    // change settings content
        if(str == "VehicleDialogEditPanelTabelDescription") {
            return language_Vehicle_Dialog_Edit_Panel_Tabel_Description[languagepointer];
        }
        if(str == "VehicleDialogEditPanelNewDescription") {
            return language_Vehicle_Dialog_Edit_Panel_NewDescription[languagepointer];
        }
        if(str == "VehicleDialogEditPanelTableHeaderName") {
            return language_Vehicle_Dialog_Edit_Panel_Table_Header_Name[languagepointer];
        }
        if(str == "VehicleDialogEditPanelTableHeaderID") {
            return language_Vehicle_Dialog_Edit_Panel_Table_Header_ID[languagepointer];
        }
        if(str == "VehicleDialogEditPanelTableHeaderPrices") {
            return language_Vehicle_Dialog_Edit_Panel_Table_Header_Prices[languagepointer];
        }
        if(str == "VehicleDialogEditPanelTableHeaderEdit") {
            return language_Vehicle_Dialog_Edit_Panel_Table_Header_Edit[languagepointer];
        }
        if(str == "VehicleDialogEditPanelTableDel") {
            return language_Vehicle_Dialog_Edit_Panel_Table_Edit_Del[languagepointer];
        }
        if(str == "VehicleDialogEditPanelTableEdit") {
            return language_Vehicle_Dialog_Edit_Panel_Table_Edit_Edit[languagepointer];
        }
        if(str == "ButtonNew") {
            return language_Buttons_New[languagepointer];
        }
        if(str == "ButtonEdit") {
            return language_Buttons_Edit[languagepointer];
        }
        if(str == "ButtonDelete") {
            return language_Buttons_Delete[languagepointer];
        }
        if(str == "VehicleDialogEditPanelCreateVehicle") {
            return language_Buttons_Create_Vehicle[languagepointer];
        }
        if(str == "CloseButton") {
            return language_Close_Button[languagepointer];
        }
        if(str == "SaveChangesButton") {
            return language_Save_Changes_Button[languagepointer];
        }
        if(str == "OrderButton") {
            return language_Order_Button[languagepointer];
        }
        if(str == "VehicleDialogEditPanelDelQuestion") {
            return language_Vehicle_Dialog_Edit_Panel_Del_Question[languagepointer];
        }
        if(str == "AccessoryDialogEditPanelDelQuestion") {
            return language_Accessory_Dialog_Edit_Panel_Del_Question[languagepointer];
        }
        if(str == "VehicleDialogEditPanelCreateNewCarPanelTitle") {
            return language_Vehicle_Dialog_Edit_Panel_Create_New_Car_Panel_Title[languagepointer];
        }
        if(str == "AccessoryDialogEditPanelEditAccessoryPanelTitle") {
            return language_Accessory_Dialog_Edit_Panel_Edit_Accessory_Panel_Title[languagepointer];
        }
        if(str == "VehicleDialogEditPanelEditCarPanelTitle") {
            return language_Vehicle_Dialog_Edit_Panel_Edit_Car_Panel_Title[languagepointer];
        }
        if(str == "VehicleDialogEditPanelEditCarPanelVehicleName") {
            return language_Vehicle_Dialog_Edit_Panel_Edit_Car_Panel_Vehicle_Name[languagepointer];
        }
        if(str == "VehicleDialogEditPanelEditCarPanelVehicleID") {
            return language_Vehicle_Dialog_Edit_Panel_Edit_Car_Panel_Vehicle_ID[languagepointer];
        }
        if(str == "VehicleDialogEditPanelEditCarPanelVehiclePrice") {
            return language_Vehicle_Dialog_Edit_Panel_Edit_Car_Panel_Vehicle_Price[languagepointer];
        }
        if(str == "VehicleDialogEditPanelEditCarPanelVehicleButtonOK") {
            return language_Vehicle_Dialog_Edit_Panel_Edit_Car_Panel_Vehicle_Button_OK[languagepointer];
        }
        if(str == "VehicleDialogEditPanelEditCarPanelVehicleButtonCancel") {
            return language_Vehicle_Dialog_Edit_Panel_Edit_Car_Panel_Vehicle_Button_Cancel[languagepointer];
        }
        if(str == "SettingsChangeAccessoryCreateNewAccessory") {
            return language_Accessory_Dialog_Edit_Panel_Create_New_Accessory_Button_Name[languagepointer];
        }
        if(str == "AccessoryDialogEditPanelCreateNewAccessory") {
            return language_Accessory_Dialog_Edit_Panel_Create_New_Accessory_Panel_Title[languagepointer];
        }
        if(str == "SpecialsDialogEditPanelNewModelButton") {
            return language_Specials_Dialog_Edit_Panel_New_Model_Button[languagepointer];
        }
        if(str == "SpecialModelEditPanelTableHeaderDescription") {
            return language_Specials_Dialog_Edit_Panel_TableHeaderDescription[languagepointer];
        }
        if(str == "SpecialModelEditPanelTableHeaderAccessories") {
            return language_Specials_Dialog_Edit_Panel_TableHeaderAccessories[languagepointer];
        }
        if(str == "SpecialModelEditPanelTableViewDetails") {
            return language_Specials_Dialog_Edit_Panel_TableViewDetails[languagepointer];
        }
        if(str == "SpecialModelDialogEditPanelDelQuestion") {
            return language_Specials_Dialog_Edit_Panel_DelQuestion[languagepointer];
        }
        if(str == "SpecialModelDialogEditPanelEditSpecialModel") {
            return language_Specials_Dialog_Edit_Panel_Edit_Special_Model_Title[languagepointer];
        }
        if(str == "SpecialModelDialogEditPanelEditSpecialModel_Name") {
            return language_Specials_Dialog_Edit_Panel_Edit_Special_Model_Name[languagepointer];
        }
        if(str == "SpecialModelDialogEditPanelEditSpecialModel_Price") {
            return language_Specials_Dialog_Edit_Panel_Edit_Special_Model_Price[languagepointer];
        }
        if(str == "SpecialModelDialogEditPanelEditSpecialModel_CPrice") {
            return language_Specials_Dialog_Edit_Panel_Edit_Special_Model_CPrice[languagepointer];
        }
        if(str == "SpecialModelDialogEditPanelEditSpecialModel_Desc") {
            return language_Specials_Dialog_Edit_Panel_Edit_Special_Model_Desc[languagepointer];
        }
        if(str == "SpecialModelDialogEditPanelTreeDialogTitle") {
            return language_Specials_Model_Dialog_Edit_Panel_Tree_Dialog_Title[languagepointer];
        }
        if(str == "SpecialModelDialogEditPanelTreeDialogInfo") {
            return language_Specials_Model_Dialog_Edit_Panel_Tree_Dialog_Info[languagepointer];
        }
        if(str == "SpecialModelDialogEditPanelTreeDialogAccessories") {
            return language_Specials_Model_Dialog_Edit_Panel_Tree_Dialog_Accessories[languagepointer];
        }
        if(str == "SpecialModelDialogEditPanelTreeDialogDescription") {
            return language_Specials_Model_Dialog_Edit_Panel_Tree_Dialog_Description[languagepointer];
        }
    // end change settings content
    // Tabs
        if(str == "TabVehicleName") {
            return language_Tab_Vehicle_Names[languagepointer];
        }
        if(str == "TabSpecialsName") {
            return language_Tab_Specials_Names[languagepointer];
        }
        if(str == "TabAccessoryName") {
            return language_Tab_Accessory_Names[languagepointer];
        }
    // End Tabs
    // Tab1: Vehicle selection
        if(str == "HeaderVehicle") {
            return language_Table_Header_Vehicle_Name[languagepointer];
        }
        if(str == "HeaderID") {
            return language_Table_Header_ID_Name[languagepointer];
        }
        if(str == "HeaderPrices") {
            return language_Table_Header_Prices_Name[languagepointer];
        }
    // End Tab1: Vehicle selection
    // Tab2: Model Table Panel
        if(str == "ModelNoModel") {
            return language_Model_Table_Panel_No_Model_Names[languagepointer];
        }
        if(str == "ModelGomera") {
            return language_Model_Table_Panel_Gomera_Names[languagepointer];
        }
        if(str == "ModelJazz") {
            return language_Model_Table_Panel_Jazz_Names[languagepointer];
        }
        if(str == "ModelLuxus") {
            return language_Model_Table_Panel_Luxus_Names[languagepointer];
        }
        if(str == "ModelNoModelDesc") {
            return language_Model_Table_Panel_No_Model_Discription[languagepointer];
        }
        if(str == "ModelGomeraDesc") {
            return language_Model_Table_Panel_Gomera_Discription[languagepointer];
        }
        if(str == "ModelJazzDesc") {
            return language_Model_Table_Panel_Jazz_Discription[languagepointer];
        }
        if(str == "ModelLuxusDesc") {
            return language_Model_Table_Panel_Luxus_Discription[languagepointer];
        }
    // End Model Table Panel
    // Tab3: Accessory Selection
        if(str == "AccessoTableDesc") {
            return language_Accessory_Table_Description[languagepointer];
        }
        if(str == "AccessoTableID") {
            return language_Accessory_Table_ID[languagepointer];
        }
        if(str == "AccessoTablePrice") {
            return language_Accessory_Table_Price[languagepointer];
        }
        if(str == "AccessoTableWheel") {
            return language_Accessory_wheel[languagepointer];
        }
        if(str == "AccessoTableMirror") {
            return language_Accessory_Mirror[languagepointer];
        }
        if(str == "AccessoTableDoors") {
            return language_Accessory_Centralized_doors[languagepointer];
        }
        if(str == "AccessoTableRims") {
            return language_Accessory_rims[languagepointer];
        }
        if(str == "AccessoTableAntiSkidSys") {
            return language_Accessory_AntiSkidSystem[languagepointer];
        }
        if(str == "AccessoTablePowerWindows") {
            return language_Accessory_PowerWindows[languagepointer];
        }
        if(str == "AccessoTableRadio") {
            return language_Accessory_Radio[languagepointer];
        }
        if(str == "AccessoTableMats") {
            return lanugage_Accessory_Mats[languagepointer];
        }
        if(str == "AddAccessoryPriceChkboxValue") {
            return language_AddAccessoryPrice_Chkbox[languagepointer];
        }
    // End Tab3 (Accessory Selection)
    // Vehicle Names
        if(str == "VehicleRolo") {
            return language_Vehicle_Rolo_Names[languagepointer];
        }
        if(str == "VehicleGolf") {
            return language_Vehicle_Golf_Names[languagepointer];
        }
        if(str == "VehicleRassant") {
            return language_Vehicle_Rassant_Names[languagepointer];
        }
        if(str == "VehicleRassantFamily") {
            return language_Vehicle_RassantFamily_Names[languagepointer];
        }
        if(str == "VehicleI5") {
            return language_Vehicle_I5_Names[languagepointer];
        }
    // End Vehicle Names
    // Calculator Panel
        if(str == "CalcPanelBasePrice") {
            return language_Calculator_Panel_Base_Price_Names[languagepointer];
        }
        if(str == "CalcPanelDiscount") {
            return language_Calculator_Panel_Discount_Names[languagepointer];
        }
        if(str == "CalcPanelPriceModel") {
            return language_Calculator_Panel_Price_Model_Names[languagepointer];
        }
        if(str == "CalcPanelPriceAccessory") {
            return language_Calculator_Panel_Price_Accessory_Names[languagepointer];
        }
        if(str == "CalcPanelTotalPrice") {
            return language_Calculator_Panel_Total_Price_Names[languagepointer];
        }
    // End Calculator Panel
    // ViewPurchaseOrderDialog
        if(str == "ViewPurchaseOrderDialogTitle") {
            return language_View_Purchase_Order_Dialog_Title[languagepointer];
        }
        if(str == "ViewPurchaseOrderDialogVehicleDetails") {
            return language_View_Purchase_Order_Dialog_Vehicle_Details[languagepointer];
        }
        if(str == "ViewPurchaseOrderDialogSpecialDetails") {
            return language_View_Purchase_Order_Dialog_Special_Details[languagepointer];
        }
        if(str == "ViewPurchaseOrderDialogAccessoryDetails") {
            return language_View_Purchase_Order_Dialog_Accessory_Details[languagepointer];
        }
        if(str == "ViewPurchaseOrderDialogPriceDetails") {
            return language_View_Purchase_Order_Dialog_Price_Details[languagepointer];
        }
        if(str == "ViewPurchaseOrderDialogSelectedVehicle") {
            return language_View_Purchase_Order_Dialog_Selected_Vehicle[languagepointer];
        }
        if(str == "ViewPurchaseOrderDialogNoSelectedVehicle") {
            return language_View_Purchase_Order_Dialog_NO_Selected_Vehicle[languagepointer];
        }
        if(str == "ViewPurchaseOrderDialogSelectedSpecial") {
            return language_View_Purchase_Order_Dialog_Selected_Special[languagepointer];
        }
        if(str == "ViewPurchaseOrderDialogSelectedAccessory") {
            return language_View_Purchase_Order_Dialog_Selected_Accessory[languagepointer];
        }
        if(str == "ViewPurchaseOrderDialogNoSelectedAccessory") {
            return language_View_Purchase_Order_Dialog_NO_Selected_Accessory[languagepointer];
        }
        if(str == "ViewPurchaseOrderDialogFinalPrice") {
            return language_View_Purchase_Order_Dialog_Final_Price[languagepointer];
        }
    // End ViewPurchaseOrderDialog
    // SendPurchaseOrderDialog
        if(str == "SendPurchaseOrderDialog") {
            return language_Send_Purchase_Order_Dialog_Names[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogBasePrice") {
            return language_Send_Purchase_Order_Dialog_Accordion_Base_Price_Name[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogModelName") {
            return language_Send_Purchase_Order_Dialog_Accordion_Special_Model_Name[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogAccessory") {
            return language_Send_Purchase_Order_Dialog_Accordion_Accessory_Name[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogTotalPrice") {
            return language_Send_Purchase_Order_Dialog_Accordion_Total_Prices_Name[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogAddressAc") {
            return language_Send_Purchase_Order_Dialog_Accordion_Address_Name[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogBasePriceSelecVehicle") {
            return language_Send_Purchase_Order_Dialog_Base_Price_Vehicle_Name[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogBasePriceVehiclePrices") {
            return language_Send_Purchase_Order_Dialog_Base_Price_Vehicle_Prices[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogBasePriceVehicleDisco") {
            return language_Send_Purchase_Order_Dialog_Base_Price_Vehicle_Discount[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogBasePriceTotalVePrice") {
            return language_Send_Purchase_Order_Dialog_Base_Price_Vehicle_Total[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogBasePriceErrorVehicle") {
            return language_Send_Purchase_Order_Dialog_Base_Price_Name_No_Vehicle_Selected[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogBasePriceErrorPrices") {
            return language_Send_Purchase_Order_Dialog_Base_Price_Price_No_Vehicle_Selected[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogSpecialModelModelName") {
            return language_Send_Purchase_Order_Dialog_Special_Model_Special_Model_Name[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogSpecialModelModelPrice") {
            return language_Send_Purchase_Order_Dialog_Special_Model_Special_Model_Price[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogContactSureName") {
            return language_Send_Purchase_Order_Dialog_Contact_surename[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogContactFirstName") {
            return language_Send_Purchase_Order_Dialog_Contact_firstname[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogContactStreet") {
            return language_Send_Purchase_Order_Dialog_Contact_street[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogContactZIP") {
            return language_Send_Purchase_Order_Dialog_Contact_zip[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogContactCity") {
            return language_Send_Purchase_Order_Dialog_Contact_city[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogContactCountry") {
            return language_Send_Purchase_Order_Dialog_Contact_country[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogContactNumber") {
            return language_Send_Purchase_Order_Dialog_Contact_number[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogContactMail") {
            return language_Send_Purchase_Order_Dialog_Contact_mail[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogAccordionTotalPriceDescTotal") {
            return language_Send_Purchase_Order_Dialog_Special_Total_Price_Desc_Total[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogMissingInput") {
            return language_Send_Purchase_Order_Dialog_Missing_Input[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogAccessoryHeaderSelected") {
            return language_Send_Purchase_Order_Dialog_Accessory_Table_Header_selected[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogAccessoryHeaderPrice") {
            return language_Send_Purchase_Order_Dialog_Accessory_Table_Header_price[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogAccessoryDiscount") {
            return language_Send_Purchase_Order_Dialog_Accessory_Table_discount[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogAccessoryTotal") {
            return language_Send_Purchase_Order_Dialog_Accessory_Table_total[languagepointer];
        }
        if(str == "SendPurchaseOrderDialogAccessoryNoSelectedAccesso") {
            return language_Send_Purchase_Order_Dialog_accessory_NoSelected_Accessory[languagepointer];
        }
    // End SendPurchaseOrderDialog
    // Error
        if(str == "CloseError") {
            return language_Error_Close_Window[languagepointer];
        }
        if(str == "DiscountInputError") {
            return language_Error_Discount_Input[languagepointer];
        }
        if(str == "ErrorByInput") {
            return language_Error_by_Input[languagepointer];
        }
    // End Error
    // not found
    return "!undefined!";
 }
 
 // }}}